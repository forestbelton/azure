/* Carved out of ovl_town; see docs/addressing-and-translation-units.md.
 * Replace this header with the evidence for the split. */
#include "common.h"

/* town.bin's overlay at 0x80088760, loaded when the player is in the town.
   Built at cc1psx -G0, not the -G8 the resident executable uses; the Makefile
   has the per-unit override and says why.

   This file is one splat unit and the original source was many. Unlike
   main.bin's card overlay there is no debug twin to name them: none of
   town.bin's 76 assert records is inside this block (they are all in the
   tier-2 low-RAM overlays at +0x2ab000 and beyond), so the unit boundaries
   have to come from the rodata partition the sweep anchors. See
   docs/overlays.md, "The sweep produces the anchors that pin the carve".

   Types for this overlay's own data are declared HERE rather than in
   include/structs.h while they are still guesses: 0x800CF720 and up is
   town-private and no engine unit references it, and every edit to
   include/structs.h rebuilds all 39 engine units under wine. Promote one
   once it is understood and shared. */

/* The overlay's own array of the engine's 8-byte Unk80082660 record -- same
   layout, witnessed by func_8008AC84 writing +0x00/+0x01/+0x02/+0x04 at a
   stride of 8. Declaring a second typedef for it would have duplicated an
   existing engine type, so include/structs.h's is used and its pad_02 was
   split into the halfword this function writes. */
extern Unk80082660 D_800CF720[];

/* A table of six pointers reached through an actor's +0x80 and a task's
   +0x44. Every family that touches it loads ONE of the six and hands it
   straight to the bind call, so the element type is unknown; only the fact
   that the six slots are word-sized has a witness. */
typedef struct UnkTownArg80
{
    void *unk_00;
    /* The actor reaches this one through +0x7C rather than +0x80. Whether
       that is the same table is not established; the field offsets do not
       overlap, so one typedef serves both. */
    void *unk_04;
    void *unk_08;
    void *unk_0C;
    void *unk_10;
    void *unk_14;
    void *unk_18;
    void *unk_1C;
    void *unk_20;
    void *unk_24;
} UnkTownArg80;

/* Passed as arg0 all over this overlay. Only the fields with a witness are
   named: a handler at +0x54, an index at +0x60 used as a stride-8 subscript
   into D_80082660, two s16s at +0x6C and +0x72 (+0x6C is a COUNTDOWN -- the
   handler installed at +0x54 decrements it and fires when it reaches zero),
   and the pointer table at +0x80. Everything else is unexplored space, not
   known filler. */
typedef struct UnkTownActor
{
    u8 pad_00[0x50];
    void (*unk_50)();
    void (*unk_54)();
    /* Two more pointers an install family fills from overlay rodata, beside
       the pair at +0x7C/+0x80 the same family fills. */
    void *unk_58;
    void *unk_5C;
    s32 unk_60;
    u8 pad_64[0x4];
    /* +0x6A is a small step counter a state machine walks 0,1,2 and resets,
       and +0x68 counts how many times it wrapped. */
    s16 unk_68;
    s16 unk_6A;
    s16 unk_6C;
    /* +0x6E is the FACING THIS FAMILY RESTORES: func_800CDCA8 walks +0x72
       forward by 0x400 a frame and, when its countdown expires, copies
       +0x6E back into +0x72 -- so +0x6E is the resting facing and +0x72 the
       animated one. */
    s16 unk_6E;
    /* +0x70 and +0x71 split out of pad_70: the func_800C5C70 pair writes a
       0/1 byte to +0x70 and ORs bit 0 into +0x71 beside it.  No existing
       field moved. */
    u8 unk_70;
    u8 unk_71;
    /* +0x72 is a FACING, in PSX angle units where 0x1000 is a full turn: the
       wander family writes 0, 0x400, 0x800 and 0xC00 into it, which are the
       four quarter turns, and the step family divides it by 0x400 to get a
       direction index back out. */
    s16 unk_72;
    u8 pad_74[0x8];
    UnkTownArg80 *unk_7C;
    UnkTownArg80 *unk_80;
    /* A position and a half-extent per axis: the families test
       +0x84 +/- +0x8C against the mover's x and +0x86 +/- +0x8E against its
       y, which is a centre-and-radius bounds check on both. */
    s16 unk_84;
    s16 unk_86;
    /* The target position, the actor's version of a task's +0x36/+0x38. */
    s16 unk_88;
    s16 unk_8A;
    s16 unk_8C;
    s16 unk_8E;
    /* A countdown a handler family walks down and fires on at zero, the
       actor's equivalent of +0x6C one layer out. */
    s16 unk_90;
} UnkTownActor;

/* The 0x8009xxxx families' arg0, and the same idea one layer down: handlers
   at +0x00 and +0x04, the countdown at +0x0A, a stride-8 index at +0x40 and
   the pointer table at +0x44. Whether the +0x00 and +0x04 families really
   share a type is NOT established -- the offsets do not conflict, so one
   struct covers both and the bytes are right either way, but two handler
   slots on one object is a hypothesis and not a finding. */
typedef struct UnkTownTask
{
    void (*unk_00)();
    void (*unk_04)();
    u8 pad_08[0x2];
    s16 unk_0A;
    u8 pad_0C[0x4];
    s16 unk_10;
    u8 pad_12[0x1A];
    /* The UnkTownColl the camera task is currently bound to: func_8009B9BC
       reads it in its prologue and compares it against its own second
       parameter to decide whether to suppress the +0x50 poll. */
    void *unk_2C;
    u8 pad_30[0x6];
    /* A target position: one pair computes the angle from here to the mover
       with func_8003BD84 and steers by it. The actor twin of that function
       uses +0x88/+0x8A for the same thing. */
    s16 unk_36;
    s16 unk_38;
    u8 pad_3A[0x6];
    s32 unk_40;
    UnkTownArg80 *unk_44;
} UnkTownTask;

/* The argument of the indirect-dispatch family, whose members load a function
   pointer from one offset and call it. The five offsets seen are not all on
   the same object: +0x00/+0x04 match UnkTownTask's handlers and +0x54 matches
   UnkTownActor's -- and +0x50 matches it too, now that a bind family has been
   seen installing a handler there, which leaves only +0x68 without a second
   witness. One struct with all five slots is a CONVENIENCE: it gets the bytes
   right and says nothing about which object each member is really handed. */
typedef struct UnkTownDispatch
{
    void (*unk_00)();
    void (*unk_04)();
    /* +0x08..+0x18: five more slots, witnessed by func_8009B9BC (+0x08,
       +0x0C, +0x10, +0x14) and func_8009B2C4 (+0x00, +0x18), both of which
       dispatch through a->unk_58 with all four of their own parameters. */
    void (*unk_08)();
    void (*unk_0C)();
    void (*unk_10)();
    void (*unk_14)();
    void (*unk_18)();
    u8 pad_1C[0x34];
    void (*unk_50)();
    void (*unk_54)();
    u8 pad_58[0x10];
    void (*unk_68)();
} UnkTownDispatch;

/* Named after the function that consumes it, the way include/structs.h names
   Unk80082660, because nothing here says what it IS. The countdown-and-
   advance family hands it to func_800C2AE8 or func_800C2B38, stores the
   halfword result at +0x0A, and on the other branch adds +/-0x20000 to the
   s32 at +0x00 or +0x04. Those two offsets collide with UnkTownTask's handler
   slots, which is how we know this is a different object and not a task. */
/* MERGED, and the merge is now forced rather than inferred. This and
   UnkTown2AE8 were kept apart through three commits because their witnessed
   field sets were disjoint and a word at +0x08 appeared to collide with a
   halfword at +0x0A. It never did collide: on little-endian MIPS the HIGH
   half of the word at +0x08 IS bytes +0x0A, so an `sh` there sets the integer
   part of a 16.16 value. What settles it is func_800CC070, which uses the
   WORD at +0x08 and the HALFWORD at +0x0A on one object in one function, and
   func_8009A1E8, which does the same for +0x14 and +0x16.

   So it is six 16.16 words: a position at +0x00/+0x04, a third at +0x08, a
   per-frame delta at +0x0C/+0x10 that the families clamp to +/-0x20000, and
   +0x14. Each one's integer half is the s16 two bytes in, and every access to
   one of those halves is spelled `*(s16 *)((u8 *)b + N)` -- a named s16 field
   cannot coexist with the word it lives inside, and the word is what the
   arithmetic uses. */
typedef struct UnkTown2AE8
{
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkTown2AE8;

/* The 2x5 grid family's two objects: an array of pointers to something with a
   pointer at +0x04, pointing at something with an s16 pair at +0x08/+0x0A.
   Those two halfwords are a SCREEN POSITION -- the family writes
   (i / 5) * 0x80 + x0 to one and (i % 5) * 0x10 + y0 to the other across ten
   items, which is a two-column, five-row layout with a 0x80 column pitch and
   a 0x10 row pitch.

   CANDIDATE MERGE, deliberately not taken: UnkTown2AE8 above could be this
   same object, since its pad_08 would hold the +0x08 halfword and nothing
   conflicts -- and +0x00/+0x04 taking +/-0x20000 would then read as a 16.16
   subpixel position beside this integer one, which is a coherent model. It is
   also only a model: the two families witness DISJOINT field sets, so they
   are kept apart until something uses both. */
typedef struct UnkTownPos
{
    u8 pad_00[0x8];
    s16 unk_08;
    s16 unk_0A;
} UnkTownPos;

typedef struct UnkTownGrid
{
    u8 pad_00[0x4];
    UnkTownPos *unk_04;
} UnkTownGrid;

/* 0x29 bytes of byte-sized entries, copied onto the stack and handed to the
   engine's func_800491F4 with a count of 0x28 -- one less than the copy, so
   the last byte is a terminator or spare. Alignment 1 is the point of the
   typedef: gcc cannot assume the source is 4-aligned, so the copy comes out
   as its runtime-checked pair of lwl/lwr and lw/sw loops. */
typedef struct UnkTownList
{
    u8 unk_00[0x29];
} UnkTownList;

/* An allocated object's +0x20 view and the renderable its +0x0C points at.
   Both are witnessed by the parked 279-word spawner
   (docs/near-misses/func_800BA00C.c) and by the bind family below, which is
   why they are here rather than only in the park. */
typedef struct UnkTownSpawn
{
    u8 pad_00[0x66];
    s16 unk_66;
    u8 pad_68[0x1C];
    /* Split out of pad_68 by func_8009CFE0, which subtracts this pair as a
       common origin from four D_80082D08 coordinate pairs. Size unchanged at
       0xA0 either way. */
    u16 unk_84;
    u16 unk_86;
    u8 pad_88[0xF];
    s8 unk_97;
    u32 unk_98;
    struct UnkTownSpawn *unk_9C;
} UnkTownSpawn;

typedef struct UnkTownRend
{
    /* +0x00/+0x04/+0x05 and +0x10/+0x12/+0x14 split out of two pads by the
       function that initialises a whole record rather than touching one
       field of one. No existing field moved. */
    u32 unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 pad_06[0x2];
    u32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    /* Split out of pad_0F: func_800A3B80 writes +0x0F from byte 1 of the
       +0x08 pointer.  No existing field moved. */
    u8 unk_0F;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    s16 unk_16;
    s16 unk_18;
    /* +0x1A is a rotation and +0x1C/+0x1E/+0x20 a 0x1000-based scale. */
    s16 unk_1A;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20;
} UnkTownRend;

/* A slot holding one handle, freed and zeroed in pairs of index ranges. */
typedef struct UnkTownSlot
{
    s32 unk_00;
} UnkTownSlot;

/* A 0x10-byte vector: one family adds two of these componentwise into a
   six-word UnkVec on the stack and hands that to a solver. Only the first
   three words have a witness; the fourth is the stride's remainder. */
typedef struct UnkTownVec16
{
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
} UnkTownVec16;

/* The card/prim pair this unit builds from the two resident templates
   D_8002E5D8 (4 words) and D_8002E5E8 (3 words). csrc/ovl_main_80020000.c
   has its own copies of these under similar names, but the layouts are NOT
   interchangeable: there +0x04 of the prim is one s32 and +0x08 of the
   12-byte prim is s32, whereas func_800AFF00 here stores s16 at +0x06 and
   s16 at +0x08 (measured -- an s32 at +0x08 gives sw where the target has
   sh). Hence the distinct names. */
typedef struct UnkTownCardPrim
{
    s32 unk_00;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkTownCardPrim;

typedef struct UnkTownCardPrim12
{
    s32 unk_00;
    s32 unk_04;
    s16 unk_08;
    s16 unk_0A;
} UnkTownCardPrim12;

typedef struct UnkTownCardPair
{
    s32 unk_00;
    UnkTownCardPrim *unk_04;
    UnkTownCardPrim12 *unk_08;
    s32 unk_0C;
} UnkTownCardPair;

/* The collision/claim table func_8008F55C's family writes, one slot per axis
   index. BOTH arrays are 6 elements, pinned by the caller func_8008F664's init
   loop, which runs `sb $zero, 0x3A($v0)` for idx 0..5 (slti $v0, $v0, 0x6):
   0x1C + 6*4 = 0x34, and 0x3A is where the byte array starts, so a shorter
   word array would have let the two overlap. */
typedef struct UnkTownColl
{
    u8 pad_00[0x1C];
    void *unk_1C[6];
    u8 pad_34[0x6];
    u8 unk_3A[6];
} UnkTownColl;

/* The scratchpad object at 0x1F800000 -- the caller func_8008F664 builds its
   base as `lui $s0, 0x1F80`. Witnessed fields only: a pointer at +0x04, signed
   distances at +0x10/+0x18/+0x20 (the caller negu/slt's them), one u8 flag per
   axis at +0x28/+0x29/+0x2A, and one index word per axis at +0x2C/+0x30/+0x34.
   Named here rather than in include/structs.h because only this unit uses it. */
typedef struct UnkTownSweep
{
    u8 pad_00[0x4];
    void *unk_04;
    u8 pad_08[0x8];
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    u8 unk_28;
    u8 unk_29;
    u8 unk_2A;
    u8 pad_2B[0x1];
    s32 unk_2C;
    s32 unk_30;
    s32 unk_34;
} UnkTownSweep;

/* The mover/state object the func_8008BA44..func_8008BBD4 handlers drive. Not
   UnkTownDispatch (whose +0x00 is a function pointer, where this one's is a
   data pointer to the UnkTown2AE8) and not UnkTownActor (which has s16 at
   +0x68 and +0x6C, colliding outright). Witnessed fields only: the mover at
   +0x00, an s16 pair at +0x64/+0x66 used as a table index, the handler slot at
   +0x68 that these four functions write, and an s32 at +0x6C.
   func_8008BC58, which they install at +0x68, agrees: it uses lhu/sh on +0x64
   and reads a word at +0x7C. */
typedef struct UnkTownMover
{
    UnkTown2AE8 *unk_00;
    u8 pad_04[0x60];
    s16 unk_64;
    s16 unk_66;
    void (*unk_68)();
    s32 unk_6C;
} UnkTownMover;

/* The +0x20 actor view of the UnkNode that func_800BD980 builds. Witnessed
   fields only: an ARRAY of two UnkTown2AE8 pointers at +0x00/+0x04 (the two
   child nodes' UnkNode.unk_08 movers, which func_800BD980 stores there as
   obj+0x24 then obj+0x20), an s32 at +0x08 (the first word of the six-word
   mover the same caller copies into +0x08..+0x1C), a state selector at +0x20
   and a mod-8 frame counter at +0x22.

   The array spelling at +0x00/+0x04 is load-bearing, not cosmetic: both loops
   below walk it with a decrementing index, and gcc's loop strength reduction
   puts the derived pointer's initialisation at the END of the loop preheader
   (`addiu $s1, $zero, 1` before `addiu $s0, $s2, 0x4`). Two named pointer
   fields walked with an explicit `UnkTown2AE8 **q` put the pointer init
   FIRST and cost a word. */
typedef struct UnkTownBD688
{
    UnkTown2AE8 *unk_00[2];
    s32 unk_08;
    u8 pad_0C[0x14];
    s16 unk_20;
    u16 unk_22;
} UnkTownBD688;

/* A NULL-terminated table of word-sized pointers at 0x8001029C, outside the
   loaded image. Each record's first two bytes are the id pair func_8009EBE0 /
   func_8009F830 match on; nothing else about the record has a witness. */
typedef struct UnkTownIdPair
{
    u8 unk_00;
    u8 unk_01;
} UnkTownIdPair;

/* func_80092A84 gates on this object's +0x10 word and hands its address to
   func_80095760 / func_8009567C. Only +0x10 has a witness. */
typedef struct UnkTownCFCB4
{
    u8 pad_00[0x10];
    s32 unk_10;
    u8 pad_14[0x4];
    /* +0x18 is an index into D_800CFDD8, and +0x1C/+0x3A have exactly
       UnkTownColl's layout -- func_8009B2C4 reads b->unk_3A[k] and
       D_800CFCB4.unk_3A[i] off the same two shapes, and tests
       b->unk_1C[1] == &D_800CFCB4 against D_800CFCB4.unk_1C[0] == b, so the
       two objects are linked both ways.  NOT merged with UnkTownColl: that
       would move unk_10, which six landed call sites read. */
    s32 unk_18;
    void *unk_1C[6];
    u8 pad_34[0x6];
    u8 unk_3A[6];
} UnkTownCFCB4;

/* The record D_800CFCC4 points at. func_800927EC reads the u8 at +0x14 and
   compares it against 4; func_8009E258 reads an s8 at +0x4E off the same
   pointer. Nothing else has a witness yet. About eleven more functions in
   asm/nonmatchings/ovl_town/ use this pointer, so expect it to grow. */
typedef struct UnkTownCFCC4
{
    u8 pad_00[0x14];
    u8 unk_14;
} UnkTownCFCC4;

/* The record the func_800ADD80 / func_800ADB5C grid-cursor handlers receive.
   The pointer points 0x20 bytes INTO its owning object: func_800AE090 stores
   the handler at -0x10 and the handlers hand -0x20 to func_800AE30C, so these
   are positive offsets from the handler's own view. The +0x20 table is the
   same (unk_04 << 2) + unk_20 4-byte-stride array func_800ADB30 indexes.
   Town-private, so it lives here rather than in include/structs.h. */
typedef struct UnkTownADD80
{
    u8 pad_00[0x4];
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    u8 pad_10[0x4];
    s32 unk_14;
    s32 unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x4];
    s32 unk_28;
} UnkTownADD80;

/* The record the func_800B260C grid-cursor handler receives -- the sibling of
   UnkTownADD80 above, and the SAME SOURCE with different callees. The pointer
   points 0x20 bytes INTO its owning object (func_800B2BA0 stores the handler
   at -0x10 and the handler hands -0x20 to func_800B2CE8, the func_800AE30C
   twin). Only +0x10, a repeat-delay counter, has a witness. */
typedef struct UnkTown260CE
{
    s32 unk_00;
    u8 pad_04[0x4];
    s32 unk_08;
    s32 unk_0C;
} UnkTown260CE;

typedef struct UnkTown260C
{
    s32 unk_00[3];
    s32 unk_0C;
    s32 unk_10;
    u8 pad_14[0x4];
    s32 unk_18;
    UnkTown260CE unk_1C[3];
} UnkTown260C;

/* The 0x1F800000 scratchpad object func_800A8CA8 projects through. Modelled on
   csrc/unk_72A8.c:40's Unk345B8SP rather than guessed from pads: the SVECTORs
   at +0x74/+0x7C/+0x84/+0x8C, unk_94/unk_98 as RotTransPers' p/flag, and
   unk_C4 as the otz accumulator are that unit's layout for the same object.
   func_80065420 IS RotTransPers (splat/slus_006.14_symbol_addrs.txt:112), but
   it is spelled func_80065420 here because that is the name
   splat/town.bin_undefined_funcs_auto.txt provides and so the one the town
   container definitely defines. */
typedef struct UnkTownSP
{
    u8 pad_00[0x74];
    s16 unk_74;
    s16 unk_76;
    s16 unk_78;
    u8 pad_7A[0x2];
    s16 unk_7C;
    s16 unk_7E;
    s16 unk_80;
    u8 pad_82[0x2];
    s16 unk_84;
    s16 unk_86;
    s16 unk_88;
    u8 pad_8A[0xA];
    s32 unk_94;
    s32 unk_98;
    u8 pad_9C[0x28];
    s32 unk_C4;
    u8 pad_C8[0x20];
    s32 unk_E8;
    s32 unk_EC;
    u8 pad_F0[0x24];
    u8 unk_114;
    u8 unk_115;
    u8 unk_116;
    u8 pad_117[0xD];
    s32 unk_124;
} UnkTownSP;

/* The 0x14-byte script record the town entity tables are built from. The
   tables live outside the loaded image -- the caller func_8009CEE8 walks a
   NULL-terminated table of pointers at D_801007F8 / D_80100900 -- and each
   list is terminated by (unk_01 & 0xC0) == 0x80. unk_00 is the only field
   func_8009DA50 writes: a "fired" flag. */
typedef struct UnkTownScrRec
{
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u32 unk_08;
    u32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkTownScrRec;

/* The two levels func_800A00E8 and func_8009FF8C reach the UnkTownScrRec
   table through: D_80100900 is a POINTER (func_8009CDB4 clears it with
   `sw $zero, %lo(D_80100900)`), its +0x0C points at a header whose +0x04 is
   the base of the stride-0x14 record array, and its own +0x00/+0x02 are a
   u16 origin pair added to each record's +0x10/+0x12. */
typedef struct UnkTownScrHdr
{
    u8 pad_00[0x4];
    UnkTownScrRec *unk_04;
} UnkTownScrHdr;

typedef struct UnkTownScrCtx
{
    u16 unk_00;
    u16 unk_02;
    u8 pad_04[0x4];
    /* Split out of pad_04 by func_8009CDCC, which writes this pair as s16
       next to +0x00/+0x02; func_8009D940 reads +0x00/+0x04/+0x08 and
       +0x02/+0x06/+0x0A as three s16 pairs.  No existing field moved. */
    s16 unk_08;
    s16 unk_0A;
    UnkTownScrHdr *unk_0C;
} UnkTownScrCtx;

/* The spinning actor at D_80100B70 and the 0x1F800000 scratchpad as
   func_800A1080 uses it. The scratchpad members are reached as &p->unk_2C etc
   rather than as literal (void *)0x1F80002C constants -- see the lever
   "address-of a MEMBER, not a literal constant" in docs/matching-levers.md;
   the literal form is 3 words short here, which is the opposite of what
   csrc/unk_1811C.c's note says for the main binary. */
typedef struct UnkTownA1080
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s32 unk_0C;
    s16 unk_10;
    s16 unk_12;
    MATRIX unk_14;
    MATRIX unk_34;
    /* Split out of pad_54 for func_800A1354, which walks six of these as an
       INDEXED subscript -- `s->unk_54[i]`, i stepping 3 -- and reads each
       one's UnkNode.unk_08 as the UnkTown2AE8 it hands to func_800A130C.
       The subscript is load-bearing: the same six loads spelled as a walked
       `UnkNode **q` with `q += 3` are 286 words and 26 differing, because
       loop.c's address giv is only reachable from the subscript form.  No
       existing field moved; +0x6C..+0x79 stays pad. */
    UnkNode *unk_54[6];
    u8 pad_6C[0xE];
    s16 unk_7A;
} UnkTownA1080;

/* The 0x1F800000 scratchpad as this function uses it: an SVECTOR of Euler
   angles at +0x2C, the uniform scale VECTOR at +0x34 and the translation
   VECTOR at +0x44. */
typedef struct UnkTownA1080SP
{
    u8 pad_00[0x2C];
    SVECTOR unk_2C;
    VECTOR unk_34;
    VECTOR unk_44;
} UnkTownA1080SP;

/* The view func_800A3B1C walks (node + 0x20, the interior-pointer convention
   this unit uses) as func_800A3B80 reads it: a facing-like halfword at +0x00
   scaled by 32 into the ring's sine argument, a byte at +0x06 that seeds the
   ring's start angle, and a halfword at +0x0C that picks which way the ring's
   vertical offset is applied.  +0x00 and +0x0C are `lh`, +0x06 is `lbu`. */
typedef struct UnkTownA3B80
{
    s16 unk_00;
    u8 pad_02[0x4];
    u8 unk_06;
    u8 pad_07[0x1];
    /* Split out of pad_07, and +0x16 added past unk_0C, for func_800A41D8,
       which reads both with `lhu`: +0x08 seeds the strip's two base .vz
       values (-0x6200 and -0x5400) and +0x16 is the brightness ramp's base,
       used as a plain u16 in an `addu` with no extension.  No existing field
       moved. */
    u16 unk_08;
    u8 pad_0A[0x2];
    s16 unk_0C;
    u8 pad_0E[0x8];
    u16 unk_16;
} UnkTownA3B80;

/* The 0x1F800000 scratchpad as func_800A3B80 lays it out, and the same
   placement include/structs.h's Unk463ECSP already has: the OT base at +0x20
   and the four RotAverageNclip4 input SVECTORs at +0x70, whose .vz
   func_800A41D8 steps.  The one field Unk463ECSP has no name for is the
   +0x118 pointer this function parks the render record's +0x08 in. */
typedef struct UnkTownA3B80SP
{
    u8 pad_00[0x8];
    /* Split out of pad_00 and pad_90 for func_800A41D8.  +0x08..+0x17 are
       the four UV scalars it builds each quad's texture window in, stored
       with `sw` and read back both with `lw` and through `(u16)` casts --
       the same word/halfword asymmetry Unk3DBD0W and Unk3CCB0SP's
       unk_08..unk_14 model for this region.  +0x90, +0x94 and +0xC0 are
       RotAverageNclip4's `p`, `flag` and `otz` out-parameters, which is
       Unk463ECSP's unk_90/unk_94/unk_C0 field-for-field, and +0xF0 is the
       four projected screen points.  unk_20 and unk_118 keep both their
       offsets and their `u32` types, so this is additive. */
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
} UnkTownA3B80SP;

/* The 0x1F800000 scratchpad as func_800A17CC and the still-stubbed
   func_800A1354 use it: the OT base at +0x24 (D_80083160.unk_00 + 0xB0, which
   func_800A1354 installs there too, at 0x800A13D8), the four
   RotAverageNclip4 input SVECTORs at +0x74, its `p` and `flag` outputs at
   +0x94/+0x98, the returned otz at +0xC4 and the four projected screen points
   at +0xE8.

   A SIXTH typedef for this region after Unk3DBD0SP, Unk463ECSP, Unk44D24SP,
   Unk3CCB0SP and Unk45CC4SP, and the second one in this file after
   UnkTown970SP.  It is NOT Unk463ECSP retyped: that one's `SVECTOR unk_70[4]`
   with its unk_90/unk_94 outputs and unk_C0 z runs four bytes LOWER than the
   quadruple here, so the two really are different placements of the same
   RotAverage4 argument block and no field-for-field merge exists.

   +0x24 is `u32 *` rather than the `u32` Unk463ECSP's +0x20 uses so that
   `sp->unk_24 + sp->unk_C4` scales the otz index; spelling it as a scalar
   with an explicit `* 4` and a `(void *)` cast measures 0 differing too.
   The two u16 halves of each +0xE8 point are read with `lhu`, but s16
   measures 0 differing as well, so their signedness has no witness here. */
typedef struct UnkTownA17CCSP
{
    u8 pad_00[0x24];
    u32 *unk_24;
    u8 pad_28[0x4C];
    SVECTOR unk_74[4];
    s32 unk_94;
    s32 unk_98;
    u8 pad_9C[0x28];
    s32 unk_C4;
    u8 pad_C8[0x20];
    DVECTOR unk_E8[4];
} UnkTownA17CCSP;

/* The static descriptor func_800A9E68 hands to func_800AA10C: two of them,
   &D_800D1178 and &D_800D119C, 0x24 apart, so the record is 0x24 bytes.
   +0x00 is the SVECTOR vertex table, +0x04 the index quads (four s16 each,
   terminated by -1 in the first), +0x20 the four colour masks the random
   gouraud words are ANDed with.

   +0x08 and +0x10 were pad until func_800A9E68 and func_800AA068 landed.
   They are an SVECTOR rotation and a VECTOR scale, and the witness is the
   pair of libgte calls in func_800AA068 -- `func_80065820(&a->unk_08, m)`
   (RotMatrix, SVECTOR *) and `func_80064BC0(m, &a->unk_10)` (ScaleMatrix,
   VECTOR *) -- against func_800A1080, which builds the same matrix from the
   same three calls in the same order over its own SVECTOR/VECTOR pair.
   func_800A9E68 writes only unk_08.vy on &D_800D1178 and only unk_08.vx on
   &D_800D119C, so unk_08.vz has no witness; the VECTOR's +0x1C pad is the
   record's last word before unk_20. */
typedef struct UnkTownAA10C
{
    SVECTOR *unk_00;
    s16 *unk_04;
    SVECTOR unk_08;
    VECTOR unk_10;
    u32 *unk_20;
} UnkTownAA10C;

/* The actor func_800A12B4 ticks and hands straight on to func_800A17CC.  The
   same object UnkTownA1080 describes -- +0x12 is the brightness func_800A12B4
   counts down by 4 and clamps at 4, and that function's `lhu`/`sh`/`sll 16`/
   `sra 16`/`slti 4` is what types it s16 -- but every field func_800A17CC
   uses (+0x6C..+0x7A) falls inside UnkTownA1080's pad_6C, so this is a second
   view rather than a retyping of a landed one.

   Signedness that IS witnessed: +0x6C and +0x6E must be s16, because their
   `% 0x80` / `% 0x100` is the bgez/addiu/sra/sll/subu dance and an unsigned
   operand collapses it to one `andi` (285 and 290 words against 304).  +0x7A
   must be s16 too: u16 flips its `lh` to `lhu`, 1 differing.  +0x70, +0x72,
   +0x78 and +0x12 have NO witness here -- every read of them is narrowed to
   the width of the u_char or short field it is stored into, so +0x12 reads
   `lbu` even though the caller proves it is a halfword. */
typedef struct UnkTownA17CC
{
    u8 pad_00[0x12];
    s16 unk_12;
    u8 pad_14[0x58];
    s16 unk_6C;
    s16 unk_6E;
    u8 unk_70;
    u8 pad_71[0x1];
    u8 unk_72;
    u8 pad_73[0x5];
    u16 unk_78;
    s16 unk_7A;
} UnkTownA17CC;

/* The plane/normal triple func_8008CF48 writes through its out-parameter.
   Witnessed by func_8008C3B8's lh 0/2 and func_8008C758's lh 0/2/4, and the
   only thing known about the shape is that +0x04 is a DIVISOR: func_8008C758
   computes (unk_00 * dx + unk_02 * dy) / unk_04, i.e. a dot-product numerator
   over it. The out-parameter really is T ** and not an inline struct --
   func_8008CF48 forwards it untouched as $a3 to func_8008D0B8, where one
   `sw $v0, 0x0($s6)` is the only write. */
typedef struct UnkTownPlane
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
} UnkTownPlane;

typedef struct UnkTownCastPoly
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u8 pad_08[0x8];
    u16 unk_10;
    u8 pad_12[0x4];
    u8 unk_16;
    u8 unk_17;
} UnkTownCastPoly;

/* The COMPACT polygon record the +Z sweep walks: six halfwords, four vertex
   indices then the normal index then the flag halfword, against
   UnkTownCastPoly's 0x18 bytes with its normal index at +0x10 and its flags at
   +0x16.  Same flag bits in the top byte: bit 0 (= 0x100 of the halfword) is
   the per-polygon skip and bit 7 (= 0x8000) is the end-of-run marker. */
typedef struct UnkTownCastPolyS
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
} UnkTownCastPolyS;

typedef struct UnkTownCastGeo
{
    u16 *unk_00;
    UnkTownCastPoly **unk_04;
    SVECTOR *unk_08;
    SVECTOR *unk_0C;
    u8 pad_10[0x4];
    s16 unk_14;
    u8 pad_16[0x2];
    u16 unk_18;
    u16 unk_1A;
} UnkTownCastGeo;

typedef struct UnkTownCastSP
{
    UnkTownPlane *unk_00;
    u16 unk_04;
    u8 pad_06[0x12];
    SVECTOR *unk_18;
    u16 unk_1C;
    u16 unk_1E;
    u16 unk_20;
    u8 pad_22[0x2];
    s32 unk_24;
    s32 unk_28;
    s32 unk_2C;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    s32 unk_3C;
    s32 unk_40;
    s32 unk_44;
    u16 unk_48;
    u16 unk_4A;
    u16 unk_4C;
    u16 unk_4E;
    u16 unk_50;
    u16 unk_52;
    s32 unk_54;
    s32 unk_58;
    s32 unk_5C;
    s32 unk_60;
    s32 unk_64;
    s32 unk_68;
    s16 unk_6C;
    s16 unk_6E;
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    s16 unk_76;
    s16 unk_78;
    s16 unk_7A;
} UnkTownCastSP;

/* The 0x54-byte item record array at the constant base 0x800102F0. The
   constant-base spelling is the project's own, already landed at
   csrc/unk_1C540.c:1080, so it is not a device.

   ALIGNMENT IS LOAD-BEARING, which is why this is NOT the same typedef as
   that unit's `Unk800102F0`: its copy is built entirely from u8, so alignment
   1, and a whole-struct assignment through it cannot assume either side is
   word-aligned -- gcc expands the 0x54-byte copy as the runtime-safe
   lwl/lwr + swl/swr pair of loops, 54 words where func_8009DFD8's target
   spends 26 (measured: 172 candidate words against 144). One u32 member
   raises the alignment to 4 and the copy comes out as the target's four-word
   lw/sw loop plus a single trailing word. Same object, same size, two copy
   expansions -- so a cross-unit typedef is only reusable if its alignment
   suits how THIS unit uses it. */
typedef struct UnkTownItemRec
{
    u32 unk_00;
    u8 pad_04[0x50];
} UnkTownItemRec;

/* The ten bytes func_8003C06C's record hands over, into +0x34 of the record
   just filled. ALIGNMENT 1 IS LOAD-BEARING: +0x34 is word-aligned, so a type
   with any word member makes gcc emit lw/sw here, where the target has the
   runtime-safe lwl/lwr pair plus two byte moves. Deliberately distinct from
   UnkTownItemRec above, whose leading u32 raises its alignment to 4 and is
   what makes the whole-record assignment come out as the four-word lw/sw loop
   plus one trailing word. */
typedef struct UnkTownName10
{
    u8 unk_00[0xA];
} UnkTownName10;

/* The INTEGER-HALF view of a UnkTown2AE8's six 16.16 words. This unit spells
   those halves `*(s16 *)((u8 *)b + N)` everywhere else, because a named s16
   field cannot coexist with the word it lives inside -- still true, which is
   why this is a SECOND typedef over the same bytes rather than fields added to
   UnkTown2AE8. Use it only in a function that touches the halves and never
   the words.

   The view is not cosmetic; on func_8008FFC8 it is worth 11 words.
   gcc 2.7.2's true_dependence / anti_dependence (sched.c:817) DROP a
   dependence between two MEMs when one is MEM_IN_STRUCT_P at a varying
   address and the other is not in a struct at a fixed address.
   `*(s16 *)((u8 *)b + 2)` is an INDIRECT_REF of scalar type, so
   MEM_IN_STRUCT_P is clear and the store floats freely past loads that ARE
   in-struct at a varying address -- the second load group gets hoisted above
   the first store and the schedule is wrong (36 differing). A COMPONENT_REF
   sets MEM_IN_STRUCT_P, the exclusion stops applying, and the dependence
   survives. So the `*(s16 *)` idiom is the WRONG one whenever a store to one
   of these halves must stay ordered against a load through a pointer. */
typedef struct UnkTown2AE8Int
{
    s16 pad_00;
    s16 unk_02;
    s16 pad_04;
    s16 unk_06;
    s16 pad_08;
    s16 unk_0A;
    s16 pad_0C;
    /* The integer halves of the SECOND triple (+0x0C/+0x10/+0x14), which is a
       velocity: func_800B9998 squares +0x0E and +0x12 and sums them, i.e. a
       squared planar speed, and uses it as a per-frame spin increment. */
    s16 unk_0E;
    s16 pad_10;
    s16 unk_12;
    s16 pad_14;
    s16 unk_16;
} UnkTown2AE8Int;

/* The func_800C1EA4 state machine's record: a 0x18-byte array element whose
   +0x00 is the handler slot four states ping-pong between.  The stride is
   witnessed, not guessed -- func_800C1DD8 and func_800C202C both walk the
   arrays at D_800D4668 and D_800D4710 with `addiu $s0, $s0, 0x18` and stop on
   a zero +0x00, so the record is 0x18 bytes and +0x00 is both the handler and
   the array terminator.

   +0x04/+0x06 are an integer target position (the same s16-at-+2 view of a
   32-bit coordinate that UnkTown2AE8Int takes of D_80083780), +0x08/+0x0A a
   per-axis half-extent used by func_800C1EA4/func_800C2124 and +0x0C/+0x0E a
   second, wider pair used by func_800C1F8C/func_800C21F8 -- the two states
   test the SAME distance against different boxes, which is what makes the
   machine hysteretic.  +0x10/+0x12/+0x14 are three ids whose roles differ per
   state: func_800C1EA4 passes +0x10 to func_80053DA8 and +0x12 to
   func_80033B2C, func_800C2124 passes +0x10 and +0x12 to func_800C15C0 and
   +0x14 to func_80033B2C.

   +0x10 is s16, NOT u16, although func_800C1EA4 and func_800C1F8C read it
   with `lhu`.  Those two pass it to func_80053DA8, whose parameter is `u16`
   (include/structs.h), and convert.c's trunc1: narrowing turns the s16 load
   into a halfword load because the value is truncated to 16 bits anyway.
   func_800C2124 passes the same field to an unprototyped callee and gets
   `lh`.  Spelling it u16 and casting at func_800C2124's call site measures
   identically; s16 is the spelling that needs no cast anywhere. */
typedef struct UnkTownC1F8C
{
    void (*unk_00)();
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    s16 unk_0E;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
} UnkTownC1F8C;

/* D_80082E80, cleared as 0xC words == 0x30 bytes. Almost certainly an
   extension of UnkTownRend -- +0x0C is an 0x808080 RGB triple and +0x1C/+0x1E
   the same 0x1000-based scale -- but UnkTownRend spells +0x0C as three u8 and
   has nothing at +0x28, and casting +0x0C to u32 would clear MEM_IN_STRUCT_P
   on that store (see UnkTown2AE8Int), so it keeps its own typedef. */
typedef struct UnkTownScene
{
    u8 pad_00[0xC];
    u32 unk_0C;
    u8 pad_10[0xC];
    s16 unk_1C;
    s16 unk_1E;
    u8 pad_20[0x8];
    u32 unk_28;
    u8 pad_2C[0x4];
} UnkTownScene;

/* The 0x1F800000 scratchpad as func_800970FC uses it: the position pair
   func_80044D24 reads back at +0x00/+0x02 (Unk44D24SP's unk_00/unk_02) and
   the word func_800453E0 reads at +0x13C (Unk453E0SP's unk_13C).  A fifth
   typedef for the region, as Unk3CCB0SP's comment anticipates -- it is here
   and not Unk44D24SP because that one stops at +0x8E, and the stores have to
   be COMPONENT_REFs: through `u16 *scratch = (u16 *)0x1F800000` they are
   INDIRECT_REFs of scalar type, MEM_IN_STRUCT_P is clear, and sched2 floats
   both halfword stores past the in-struct loads that follow them (4 words). */
typedef struct UnkTown970SP
{
    s16 unk_00;
    s16 unk_02;
    u8 pad_04[0x138];
    u32 unk_13C;
} UnkTown970SP;

/* The two 8-byte VRAM rects at D_800D04D0 (the window's source rect in VRAM,
   split into a tpage by `& ~0xFF` and texcoords by `% 0x100`) and D_800D04D8
   (its destination rect on screen).  u16 fields with (s16) casts at the signed
   uses: the texcoord modulus needs a signed operand (`lh` + the bgez/addiu
   0xFF/sra dance) while the coordinate sums are `lhu`. */
typedef struct UnkTownRect
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
} UnkTownRect;

/* D_8006ADBC, a resident `extern u8 []`. Witnessed fields only: a u16 at
   +0x0C, an s16 flag at +0x0E, a pointer at +0x10 to a u16 pair, two u16 at
   +0x14/+0x16 added to that pair, and an s32 at +0x18 compared to 0x37000C.
   D_8006ADD4 is the low half of that same word (0x8006ADBC + 0x18), but it
   gets its OWN %hi/%lo rather than an `lh 0x18($s1)`, so it is a separate
   symbol in the source. */
typedef struct Unk8006ADBCSub
{
    u16 unk_00;
    u16 unk_02;
} Unk8006ADBCSub;

typedef struct Unk8006ADBCRec
{
    u8 pad_00[0xC];
    u16 unk_0C;
    s16 unk_0E;
    Unk8006ADBCSub *unk_10;
    u16 unk_14;
    u16 unk_16;
    s32 unk_18;
} Unk8006ADBCRec;

/* MACROS and not local pointer variables on purpose: a local's INITIALISER
   materialises the %hi/%lo pair in the prologue, where the target has neither
   (60 differing for the D_8006ADBC one), while the cast expression lets cse
   place it at the first use. Compare D_80082E80 in func_8008FFC8, which must
   be a local because its last store is after an if/else merge -- but assigned
   as a statement, not an initialiser, for the same prologue reason. */
#define TOWN_PT ((UnkTown2AE8Int *)D_80083780)
#define TOWN_REC ((Unk8006ADBCRec *)D_8006ADBC)

/* A RESIDENT symbol (splat/town.bin_undefined_syms_auto.txt), declared
   nowhere in include/structs.h or csrc/. Only its +0x08 pointer has a
   witness, and that pointee's type is inferred from three s16 reads at
   +0x02/+0x06/+0x0A -- a convenience, not a finding. */
typedef struct UnkTownCam
{
    u8 pad_00[0x8];
    UnkTown2AE8 *unk_08;
    void *unk_0C;
    u8 pad_10[0x10];
    /* The task this object owns, embedded by VALUE. Witness: func_8009A674
       sets it up with one three-instruction address computation
       (lui/addiu/addiu -0x20). Two independent symbols would cost two
       %hi/%lo pairs at -G0, so D_800834B8 is this member, not its own
       symbol: %lo(D_80083498) + 0x20 == 0x34B8 with no carry. */
    UnkTownTask unk_20;
} UnkTownCam;

extern UnkTownCam D_80083498;

/* The third argument of the func_800C3050 wrapper family: an s16 at +0x12
   each wrapper sets to a small constant, and a u16 flag word at +0x14 that
   one of the families clears bit 8 of. */
typedef struct UnkTownArg2
{
    u8 pad_00[0x12];
    s16 unk_12;
    u16 unk_14;
} UnkTownArg2;

/* The 68-member func_800C3050 wrapper family. Every member is the same 18
   words and differs only in an id and four symbols, which sit at a fixed
   0 / +8 / +0x30 / +0x34 spacing from a per-member base -- but each gets its
   own `%hi`, so they are four separate symbols in the source and not one
   struct (docs/overlays.md lever 15, read the other way round). The spacing
   between members is irregular, so they are not an array either. Generated
   mechanically from the listings -- `families.py --show func_800C5368` prints
   exactly the table the bodies were written from -- and all 68 verify 0
   differing. */
extern u8 D_800D517C[];
extern u8 D_800D5184[];
extern u8 D_800D51AC[];
extern u8 D_800D51B0[];
extern u8 D_800D51C8[];
extern u8 D_800D51D0[];
extern u8 D_800D51F8[];
extern u8 D_800D51FC[];
extern u8 D_800D5240[];
extern u8 D_800D5248[];
extern u8 D_800D5270[];
extern u8 D_800D5274[];
extern u8 D_800D52B4[];
extern u8 D_800D52BC[];
extern u8 D_800D52E4[];
extern u8 D_800D52E8[];
extern u8 D_800D5328[];
extern u8 D_800D5330[];
extern u8 D_800D5358[];
extern u8 D_800D535C[];
extern u8 D_800D53C4[];
extern u8 D_800D53CC[];
extern u8 D_800D53F4[];
extern u8 D_800D53F8[];
extern u8 D_800D5448[];
extern u8 D_800D5450[];
extern u8 D_800D5478[];
extern u8 D_800D547C[];
extern u8 D_800D55A0[];
extern u8 D_800D55A8[];
extern u8 D_800D55D0[];
extern u8 D_800D55D4[];
extern u8 D_800D55E8[];
extern u8 D_800D55F0[];
extern u8 D_800D5618[];
extern u8 D_800D561C[];
extern u8 D_800D56FC[];
extern u8 D_800D5704[];
extern u8 D_800D572C[];
extern u8 D_800D5730[];
extern u8 D_800D5774[];
extern u8 D_800D577C[];
extern u8 D_800D57A4[];
extern u8 D_800D57A8[];
extern u8 D_800D57E8[];
extern u8 D_800D57F0[];
extern u8 D_800D5818[];
extern u8 D_800D581C[];
extern u8 D_800D5A50[];
extern u8 D_800D5A58[];
extern u8 D_800D5A80[];
extern u8 D_800D5A84[];
extern u8 D_800D5AC8[];
extern u8 D_800D5AD0[];
extern u8 D_800D5AF8[];
extern u8 D_800D5AFC[];
extern u8 D_800D5BD8[];
extern u8 D_800D5BE0[];
extern u8 D_800D5C08[];
extern u8 D_800D5C0C[];
extern u8 D_800D5D18[];
extern u8 D_800D5D20[];
extern u8 D_800D5D48[];
extern u8 D_800D5D4C[];
extern u8 D_800D5DD4[];
extern u8 D_800D5DDC[];
extern u8 D_800D5E04[];
extern u8 D_800D5E08[];
extern u8 D_800D5E0C[];
extern u8 D_800D5E14[];
extern u8 D_800D5E3C[];
extern u8 D_800D5E40[];
extern u8 D_800D5E80[];
extern u8 D_800D5E88[];
extern u8 D_800D5EB0[];
extern u8 D_800D5EB4[];
extern u8 D_800D5EF4[];
extern u8 D_800D5EFC[];
extern u8 D_800D5F24[];
extern u8 D_800D5F28[];
extern u8 D_800D5F68[];
extern u8 D_800D5F70[];
extern u8 D_800D5F98[];
extern u8 D_800D5F9C[];
extern u8 D_800D600C[];
extern u8 D_800D6014[];
extern u8 D_800D603C[];
extern u8 D_800D6048[];
extern u8 D_800D6090[];
extern u8 D_800D6098[];
extern u8 D_800D60C0[];
extern u8 D_800D60C4[];
extern u8 D_800D6330[];
extern u8 D_800D6338[];
extern u8 D_800D6360[];
extern u8 D_800D6364[];
extern s32 D_800D636C[2];
extern u8 D_800D63BC[];
extern u8 D_800D63C4[];
extern u8 D_800D63EC[];
extern u8 D_800D63F0[];
extern u8 D_800D6430[];
extern u8 D_800D6438[];
extern u8 D_800D6460[];
extern u8 D_800D6464[];
extern u8 D_800D6490[];
extern u8 D_800D6498[];
extern u8 D_800D64C0[];
extern u8 D_800D64C4[];
extern u8 D_800D6568[];
extern u8 D_800D6570[];
extern u8 D_800D6598[];
extern u8 D_800D65A0[];
extern u8 D_800D66E8[];
extern u8 D_800D66F0[];
extern u8 D_800D6718[];
extern u8 D_800D671C[];
extern u8 D_800D6770[];
extern u8 D_800D6778[];
extern u8 D_800D67A0[];
extern u8 D_800D67A4[];
extern u8 D_800D67AC[];
extern u8 D_800D67B4[];
extern u8 D_800D67DC[];
extern u8 D_800D67E0[];
extern u8 D_800D6824[];
extern u8 D_800D682C[];
extern u8 D_800D6854[];
extern u8 D_800D6858[];
extern u8 D_800D6C4C[];
extern u8 D_800D6C54[];
extern u8 D_800D6C7C[];
extern u8 D_800D6C80[];
extern u8 D_800D6D38[];
extern u8 D_800D6D40[];
extern u8 D_800D6D68[];
extern u8 D_800D6D6C[];
extern u8 D_800D6DF8[];
extern u8 D_800D6E00[];
extern u8 D_800D6E28[];
extern u8 D_800D6E2C[];
extern u8 D_800D6E90[];
extern u8 D_800D6E98[];
extern u8 D_800D6EC0[];
extern u8 D_800D6EC4[];
extern u8 D_800D6F74[];
extern u8 D_800D6F7C[];
extern u8 D_800D6FA4[];
extern u8 D_800D6FA8[];
extern u8 D_800D71C4[];
extern u8 D_800D71CC[];
extern u8 D_800D71F4[];
extern u8 D_800D71F8[];
extern u8 D_800D7238[];
extern u8 D_800D7240[];
extern u8 D_800D7268[];
extern u8 D_800D726C[];
extern u8 D_800D72FC[];
extern u8 D_800D7304[];
extern u8 D_800D732C[];
extern u8 D_800D7338[];
extern u8 D_800D78C8[];
extern u8 D_800D78D0[];
extern u8 D_800D78F8[];
extern u8 D_800D78FC[];
extern u8 D_800D793C[];
extern u8 D_800D7944[];
extern u8 D_800D796C[];
extern u8 D_800D7970[];
extern u8 D_800D79B0[];
extern u8 D_800D79B8[];
extern u8 D_800D79E0[];
extern u8 D_800D79E4[];
extern u8 D_800D7A24[];
extern u8 D_800D7A2C[];
extern u8 D_800D7A54[];
extern u8 D_800D7A58[];
extern u8 D_800D7A98[];
extern u8 D_800D7AA0[];
extern u8 D_800D7AC8[];
extern u8 D_800D7ACC[];
extern u8 D_800D7B0C[];
extern u8 D_800D7B14[];
extern u8 D_800D7B3C[];
extern u8 D_800D7B40[];
extern u8 D_800D7B80[];
extern u8 D_800D7B88[];
extern u8 D_800D7BB0[];
extern u8 D_800D7BB4[];
extern u8 D_800D7BF4[];
extern u8 D_800D7BFC[];
extern u8 D_800D7C24[];
extern u8 D_800D7C28[];
extern u8 D_800D7C7C[];
extern u8 D_800D7C84[];
extern u8 D_800D7CAC[];
extern u8 D_800D7CB0[];

/* The first parameter is a pointer and not the s32 the forwarding
   wrappers implied: one family below writes a byte through it at +0x93
   before passing it on. Same lesson as func_800C4174's second. */
void func_800C3050(void *a, s32 id, void *a2, void *a3, void *a4,
                   void *a5);

/* The 29-member func_800C2E84 wrapper family: bind an actor to a descriptor,
   clear its D_80082660 slot, install a handler at +0x54 and set +0x6C. Three
   values vary; the base of D_80082660 goes through a LOCAL assigned after the
   call, which is what puts its `lui` after the `jal` and ahead of the index
   (docs/overlays.md levers 1 and 2). Written from
   `families.py --show func_800C62E4`; all 29 verify 0 differing. */
extern u8 D_800D5670[];
extern u8 D_800D58A4[];
extern u8 D_800D58CC[];
extern u8 D_800D58E0[];
extern u8 D_800D5908[];
extern u8 D_800D591C[];
extern u8 D_800D5944[];
extern u8 D_800D59C4[];
extern u8 D_800D6110[];
extern u8 D_800D62A4[];
extern u8 D_800D62AC[];
extern u8 D_800D62BC[];
extern u8 D_800D68FC[];
extern u8 D_800D6910[];
extern u8 D_800D6938[];
extern u8 D_800D694C[];
extern u8 D_800D6974[];
extern u8 D_800D6A98[];
extern u8 D_800D6AA0[];
extern u8 D_800D6DA8[];
extern u8 D_800D6E7C[];
extern u8 D_800D6EFC[];
extern u8 D_800D6F10[];
extern u8 D_800D77B0[];
extern u8 D_800D77B8[];
extern u8 D_800D783C[];
extern u8 D_800D7844[];

void func_800C2E84(UnkTownActor *a, s32 b, void *c);
void func_800C3B7C();
extern u8 D_800D0158[];
extern u8 D_800D54C4[];
void func_800917EC();
void func_800C591C();
void func_800C6440();
UnkNode *func_800C6C10(UnkTownActor *a);
UnkNode *func_800CB8CC(UnkTownActor *a, s32 b);
void func_800CBB68();
void func_8009AFF4(void);
void func_800CB708(UnkTownActor *a);

/* Three more families, each written from its `families.py --show` table and
   each verified 0 differing on every member:

   * 16 more func_800C3050 wrappers that also set +0x12 on their third
     argument;
   * 17 func_800C2E84 wrappers that clear a D_80082660 entry at a CONSTANT
     index. Those four addresses -- +0x20, +0x28, +0x58, +0xB0 -- are entries
     4, 5, 11 and 22 of the engine's stride-8 array, not four scalars. Written
     as an index, `fold` reassociates the constant onto the symbol and the
     store comes out off `%lo(D_80082660)+N*8`, which is the idiom
     include/structs.h already records for this array. The masked verify
     cannot tell the two spellings apart -- only the container build can, and
     it does.
   * 20 eight-word wrappers that do nothing but call one function. The
     listing does not fix their arity -- the delay slot is a `nop` whether or
     not an argument is passed through -- so `(void)` is a choice and not a
     finding (the CALLEE ARITY caution in docs/matching-levers.md). Five of
     them had that choice MADE for them later: they call func_800C4174, and
     once a family below pinned that function at three parameters a
     zero-argument call stopped compiling. The callee's arity is the evidence
     the listing withholds, and it is worth looking for before guessing. */

/* Seven more families, 86 functions. Each shape was verified on one member
   first and then applied to the rest; all 86 verify 0 differing.

   What they are, together, is the overlay's STATE MACHINE, and reading them
   as a group is what named the fields above. A bind call installs a handler
   at +0x54 (actors) or +0x00/+0x04 (tasks) and sets a countdown at +0x6C or
   +0x0A; the handler families ARE the functions those wrappers install, and
   each one decrements that same countdown and fires when it reaches zero.
   The `lhu` there is not a mistake for a signed field: gcc loads the low
   half unsigned because only the low 16 bits enter the subtraction, then
   sign-extends the RESULT with `sll 16` for the `bgtz`. An s16 field is what
   produces that, and a u16 would not.

   One member of the countdown family calls func_800C4174, which is declared
   with three parameters, so that one passes its own through. Nothing in the
   listing forces that reading -- the delay slot is a `nop` either way -- but
   a zero-argument call to a three-parameter function is not writable, so the
   target's arity decides it. */
extern u8 D_800D0078[];
extern u8 D_800D00A0[];
extern u8 D_800D00C0[];
extern u8 D_800D00D0[];
extern u8 D_800D00D8[];
extern u8 D_800D00E0[];
extern u8 D_800D0100[];
extern u8 D_800D0118[];
extern u8 D_800D0120[];
extern u8 D_800D0138[];
extern u8 D_800D0180[];
extern u8 D_800D0178[];
extern u8 D_800D54BC[];
extern u8 D_800D54D4[];
extern u8 D_800D5B90[];
extern u8 D_800D5BB4[];
extern u8 D_800D5CAC[];
extern u8 D_800D5CF4[];
extern u8 D_800D6AE4[];
extern u8 D_800D6AEC[];
extern u8 D_800D6B14[];
extern u8 D_800D6B18[];
extern u8 D_800D6DBC[];
extern u8 D_800D6DD0[];
extern u8 D_800D6DE4[];
extern u8 D_800D6E68[];
extern u8 D_800D7C68[];
void func_80090C68();
void func_80090E78();
void func_80090F00();
void func_80091A08();
void func_80091CE0();
void func_80091D6C();
void func_80091EB4();
void func_80091F64();
void func_80093328();
void func_800935BC();
void func_8009803C();
void func_80098ABC();
void func_80098C40();
void func_80099790();
void func_80099894();
void func_80099920();
void func_800999AC();
void func_80099A38();
void func_80099AC4();
void func_800AA844();
void func_800C3A98();
void func_800C3BF4();
void func_800C3C28();
void func_800C3C7C();
void func_800C3D08();
void func_800C3D94();
void func_800C3E20();
void func_800C3FC8();
void func_800C41D4();
void func_800C4E30();
void func_800C5ADC();
void func_800C5C70(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c);
void func_800C5FDC(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c);
void func_800C641C();
void func_800C645C();
void func_800C6478();
void func_800C7138();
void func_800C71F8();
void func_800C8928();
void func_800C895C();
void func_800C9CDC();
void func_800CB028();
void func_800CB04C();
void func_800CB060();
/* The second parameter is an UnkTown2AE8 and not the s32 the
   pass-through wrappers implied; the countdown-and-advance family
   below is the witness. */
/* The third parameter is an UnkTownRend. Until now every caller only
   FORWARDED it, so `s32` was a placeholder; the scale family below writes
   +0x1C/+0x1E through it before passing it on, which is the first witness.
   func_800C2E84's second parameter is probably the same object by the same
   reasoning -- the bind families only forward it -- but nothing has used it
   yet, so it stays `s32` rather than become a guess with a type name on
   it. */
void func_800C4174(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c);
void func_8003F320(void);
void func_800C78A8();
void func_800C7A00();
void func_800C5864(UnkTownActor *a, s32 b, s32 c);
void func_800CA88C(UnkTownActor *a, s32 b, s32 c);
void func_800C56A8();
void func_800CA458();
void func_8009C7A8();
void func_80041094();
extern u8 D_800D64DC[];
extern u8 D_800D06F8[];
extern u8 D_800D071C[];
extern u8 D_800F8BCC[];
extern u8 D_800F8BD8[];
extern u8 D_800F8D10[];
extern u8 D_800F8D1C[];
extern u16 D_80082E76;
extern void (*D_800D1638[])();
extern void (*D_800D1640[])();
void func_8006733C(void *r, void *src);
extern s32 D_80113220;
extern u8 D_800D43B0[];
extern u8 D_800D43B8[];
extern u8 D_800D4740[];
extern u8 D_800D4748[];
extern u8 D_800D4750[];
extern u8 D_800D6A0C[];
extern u8 D_800D6A10[];
extern u8 D_800D69DC[];
extern u8 D_800D69E4[];
extern u8 D_800D69B8[];
extern u8 D_800D69BC[];
extern u8 D_800D6988[];
extern u8 D_800D6990[];
/* Called with TWO args by 46 sites here and with THREE by func_8009A674 /
   func_8009A8EC (the dead-otherwise addu $a2 in the jal delay slot). The
   callee only reads lh 0x16($a1), so the third argument is unused; this
   splat unit is many original files and they disagreed on the prototype. */
void func_80094984();
extern u8 D_800D5360[];
extern u8 D_800D5388[];
extern u8 D_800D539C[];
extern u8 D_800D53B0[];
extern u8 D_800D5FE8[];
extern u8 D_800D64C8[];
extern u8 D_800D6504[];
extern u8 D_800D652C[];
extern u8 D_800D6554[];
extern u8 D_800D72AC[];
extern u8 D_800D739C[];
extern u8 D_800D73A4[];
extern u8 D_800D73CC[];
extern u8 D_800D73D0[];
extern u8 D_800D7410[];
extern u8 D_800D7418[];
extern u8 D_800D7440[];
extern u8 D_800D7444[];
extern u8 D_800D7484[];
extern u8 D_800D748C[];
extern u8 D_800D74B4[];
extern u8 D_800D74B8[];
extern u8 D_800D74F8[];
extern u8 D_800D7500[];
extern u8 D_800D7528[];
extern u8 D_800D752C[];
extern u8 D_800D756C[];
extern u8 D_800D7574[];
extern u8 D_800D759C[];
extern u8 D_800D75A0[];
extern u8 D_800D75E0[];
extern u8 D_800D75E8[];
extern u8 D_800D7610[];
extern u8 D_800D7614[];
extern u8 D_800D7654[];
extern u8 D_800D765C[];
extern u8 D_800D7684[];
extern u8 D_800D7688[];
extern u8 D_800D76C8[];
extern u8 D_800D76D0[];
extern u8 D_800D76F8[];
extern u8 D_800D76FC[];
extern u8 D_800D773C[];
extern u8 D_800D7744[];
extern u8 D_800D776C[];
extern u8 D_800D7770[];
extern u8 D_800D77C0[];
extern u8 D_800D77C8[];
extern u8 D_800D77F0[];
extern u8 D_800D77F4[];
extern u8 D_800D784C[];
extern u8 D_800D7854[];
extern u8 D_800D787C[];
extern u8 D_800D7880[];
void func_8008FFC8();
void func_80097DE8();
void func_80098180();
void func_80098928();
void func_80098988();
void func_8009AE88();
void func_800B62A4(u8 *p, UnkTownCardPair **b);
void func_800B8248();
void func_800BB508();
void func_800C0C88();
/* Returns a value one family tests against zero, so the placeholder
   `void func_800C30E0();` an earlier family emitted for it is wrong. */
s32 func_800C30E0(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C4174();
void func_800C5620();
void func_800C565C();
void func_800C7CB4();
void func_800C7CF0();
void func_800C7DB8();
void func_800C8644();
void func_800C9F1C();
void func_800C9F58();
void func_800C9FBC();
void func_800CA40C();
void func_800CA744();
void func_800CE08C();
void func_800CE0C8();
void func_800CE558();
void func_800CE62C();
void func_800C3BB8();
void func_800C6680();
void func_800C6740();
void func_800C6814();
void func_800C68E0();
void func_800C69AC();
void func_800C6A78();
void func_800C6B44();
void func_800C8524();
void func_800C899C();
void func_800C8FF0();
void func_800CB21C();
void func_800CB2DC();
void func_800CB3A8();
void func_800CB4B0();
void func_800CB570();
void func_800CB63C();
void func_800CB7EC();
void func_800CC524();
void func_800CC644();
void func_800CC8AC();

/* THE REMAINING LARGE PAIRS: four families of two, 600 words, all eight
   members 0 differing. They finish the pair work in this overlay.

   One is a BLOCK COPY and reads as nothing else: gcc copies 0x29 bytes from
   an overlay rodata table onto the stack as a runtime-checked pair of loops,
   lwl/lwr when the source turns out unaligned and lw/sw when it does not,
   then a 9-byte tail. All 60 words of it are one struct assignment and one
   call.

   Three things these taught that generalise.

   A PARAMETER'S DECLARED WIDTH CAN DECIDE A REGISTER, with no instruction of
   its own. func_800CC070 came out 80 of 80 words with its third and fourth
   arguments in each other's registers, and stayed that way through every
   reordering; declaring the fourth `s16` rather than `s32` -- it is only ever
   stored as a halfword -- flipped the tie-break and the function went to 0.
   Worth trying before parking an ALLOC residue.

   WHERE A `return` BLOCK ENDS UP IS SOURCE. func_80092DA8 reaches 73 of 73
   words with two inverted branches fixed, and the last word of difference was
   that gcc put its early-return block inline and jumped over it where the
   target has it after the convergence. Writing it as a `goto` to a label at
   the end of the function is what places it there, and that is the third time
   in this overlay that block ORDER rather than block content was the residue.

   AND THE TWO MEMBERS OF A PAIR NEED NOT SHARE A TYPE. func_8009A1E8 and
   func_800C46A8 are the same 87 words over different structs -- the task's
   +0x04/+0x0A/+0x36/+0x38/+0x40 against the actor's
   +0x54/+0x6C/+0x88/+0x8A/+0x60 -- which is why that pair has sixteen varying
   words where the others have two. Reading the offsets as one struct would
   have invented a type that does not exist. */
/* THE LARGE PAIRS: three families of two, 710 words, the first functions in
   this overlay with real control flow rather than a call and a store. They
   are the town NPC movement AI, and each pair is one function emitted twice
   with a different callee -- a 0x800C3xxx/0x800C7xxx set and a 0x800C9xxx
   twin of it.

   The wander function tries up to sixteen times: take a direction from
   func_800374F4(4), test the actor's bounds against the mover's position on
   that axis, and on the first direction with room write the matching quarter
   turn into +0x72 and break. The step function then advances the mover by its
   own delta, divides +0x72 by 0x400 to recover the direction, clamps the
   position against the same bounds, and either hands off to a per-pair callee
   or drifts the delta off four 4-entry tables with a +/-0x20000 limit.

   Three things in these worth knowing.

   THE DISPATCH SHAPE TELLS YOU WHICH C IT WAS. The wander function tests its
   direction with `bnez`, then `bne 1`, then `bne 2` -- a sequential chain,
   which is an if/else-if. The step function tests `== 1`, then `< 2`, then
   `== 0`, then `== 2` -- a balanced tree, which is a `switch`. Same four
   cases, same value range, different source construct, and the tree is how
   you tell.

   TWO OF THE SIX DIFFER ONLY IN STATEMENT ORDER, and it costs four words.
   func_800C7674 computes its direction BEFORE the two calls and runs the
   func_800C30E0 guard after them, where func_800C355C does the opposite; so
   the direction has to survive the calls, takes a callee-saved register,
   and the function pays an extra save, restore and 8 bytes of frame. Nothing
   else about them differs. A reordering is worth a register.

   ARITHMETIC THAT IS NOT IN THE SOURCE. `bgez` then `addiu 0x3FF` then
   `sra 10` is one signed divide by 0x400, not a test and a bias; and `bgez`
   then `negu` then `slt` is abs(). Reading either as source control flow
   invents branches that are not there. */
/* Four families, 22 functions. Two of them pin things the earlier sweeps
   could only guess at, both the same lesson as func_800C4174's second
   parameter: an actor has handler slots at BOTH +0x50 and +0x54 (one bind
   family installs at each), and func_800C3050's first parameter is a POINTER,
   because one family writes a[0x93] = 1 through it before passing it on.

   That last family's two arguments are spelled as `u8 *` with explicit casts
   rather than structs. Its members touch +0x93 of one and +0x28 of the other
   and nothing else, so a struct would be two named fields and a lot of
   padding asserting a layout nothing here witnesses.

   A NOTE ON UnkTown2AE8 AND UnkTown2AE8, which look like they should be one
   type and are kept apart. One family here writes an s32 at +0x14 of its
   second argument, matching UnkTown2AE8, while the countdown-and-advance
   family writes an s16 at +0x0A of its second argument, matching UnkTown2AE8,
   and a word at +0x08 would overlap that halfword. The overlap is probably
   not a conflict at all: on little-endian MIPS the HIGH half of the word at
   +0x08 lives at bytes +0x0A, so `sh` to +0x0A is how you set the integer
   part of a 16.16 fixed-point value at +0x08 -- which fits +0x00/+0x04 taking
   +/-0x20000 in the same family.

   CONFIRMED by the large pairs below, which is why the accesses there are
   spelled `*(s16 *)((u8 *)b + 2)`. Those functions read and WRITE +0x02 and
   +0x06 as s16s while the same struct's +0x00 and +0x04 are whole words, so
   both views of one field are now witnessed. The eventual shape is a union
   of an s32 with an {u16 frac; s16 i;} pair; it is not taken here because it
   would mean respelling every landed member of the countdown-and-advance
   family for no change in bytes. */
extern u8 D_800D61C4[];
extern u8 D_800D61CC[];
extern u8 D_800D61F4[];
extern u8 D_800D6200[];
extern u8 D_800D6268[];
extern u8 D_800D627C[];
extern u8 D_800D6518[];
extern u8 D_800D6674[];
extern u8 D_800D667C[];
extern u8 D_800D66A4[];
extern u8 D_800D66A8[];
extern u8 D_800D72D4[];
extern u8 D_800D72E8[];
void func_800C382C();
void func_800C7AC4();
void func_800C8788();
void func_800C9610();
void func_800C9678();
void func_800C9B44();
void func_800CA020();
void func_800CDADC();
void func_800CDC2C();
void func_800CDCA8();
void func_800CE20C();
void func_800CE3FC();
extern u32 D_80081458;

/* The 2x5 grid layout family, 6 functions. The division is gcc's: one `mult`
   by 0x66666667 with `sra 1` is a signed divide by five, and the remainder
   comes back as i - (i / 5) * 5 rather than from any `div`. tools/decomp/magic.py
   is what turns such a constant back into its divisor. Only the two origins
   vary across the six, and one of them is negative.

   A FIFTH family in this neighbourhood is parked, not landed:
   func_800AEF5C and its four siblings (155 words) are at 3 DIFF with the
   shape settled and three instructions rotated among three slots. The source,
   what is settled and what is left are in docs/near-misses/func_800AEF5C.c --
   it is a sched1 tie-break, which is the permuter's job rather than a
   sweep's. */
/* Two families, 13 functions, that are the first to constrain a type ACROSS
   families. The countdown-and-advance family passes its second argument to
   func_800C4174, which the forwarding wrappers above had only ever passed
   through -- so its `s32 b` was never more than a placeholder, and this is
   what replaces it. A parameter that is only forwarded carries no type
   information at all; the first family that USES it decides. */
void func_8009A1A4();
s32 func_800C2AE8(UnkTown2AE8 *b);
s32 abs(s32 x);
/* Three more families, 12 functions, and the first of them came off the
   PERMUTER rather than off a reading.

   func_800AEF5C and its four siblings were parked at 3 DIFF in 7a3c2d8 with
   the shape settled and three instructions rotated among three slots. The
   park said the residue was a sched1 tie-break and so the permuter's job; it
   was, and the permuter found it in 687 candidates and about a minute. The
   answer is `ok = 1` AFTER the func_800491CC call rather than before it --
   one legal line, no device. The park had explicitly reasoned that moving it
   after the call could not work, on the grounds that the target has it before
   the `jal`; that reasoning was wrong, and the only reason it cost a minute
   instead of a day is that it was handed to CPU rather than argued about.
   Read that as the measurement the protocol says it is: a park whose residue
   is an ORDERING should go to the permuter immediately, not after one more
   round of hypotheses.

   The other two are ordinary. One is three BYTE-IDENTICAL functions -- two
   strength-reduced loops over adjacent index ranges that free a handle and
   zero it -- so one source lands all three. */
/* The CONSTRUCTOR family, four functions, and it ties three earlier families
   together: it allocates a node, hands the node's +0x20 view to a member of
   the alloc-and-copy family, and on success reads the RETURN VALUE of a
   member of the func_800491F4 family into one of its own fields. Those
   functions were all landed as `void`, because nothing had used their result
   yet -- they end in a call and gcc leaves the callee's value in $v0 by
   itself, so `void` and `u32` compile identically and only a caller can tell
   the difference. Nine of them are retyped here, the build is unchanged, and
   this is the same lesson as the three arity cascades: a signature is only as
   good as the callers seen so far.

   It also wants ONE return rather than early returns. Written with
   `if (o == 0) return 0;` the function is 49 words and materialises a literal
   zero; the target returns the VARIABLE on every path, with the failure arm
   assigning `o = 0` and falling through to a single `return o`. */
/* Two more families, 204 words, and one of them turned up a lever the index
   did not have.

   docs/overlays.md's lever 11 says `addu` operand order follows RTL
   canonicalisation and that pre-scaling the offset fixes it. That is not
   enough on its own: the 42-word family needs `addu $s0, $a2, $a1` with the
   SCALED value first, and `&q[i]`, `(u8 *)q + i * 0x10`, `i * 0x10 + (u8 *)q`
   and `(i << 4) + (u8 *)q` all produce the operands the other way round. The
   reason is that POINTER ARITHMETIC NORMALISES the order -- `int + pointer`
   and `pointer + int` are the same tree to the front end, so writing the
   scaled term first changes nothing. Making both operands the same type is
   what preserves source order: `(UnkTownVec16 *)(i * 0x10 + (s32)q)` matches.
   So the rule is narrower than lever 11 states -- to control the order of a
   commutative op you must take the pointer out of the expression. */
/* SIX TINY FAMILIES, thirty functions in one batch: two-call sequences, a
   leaf that installs a handler and a count with no stack frame at all, a
   +0x20 forwarder, a constant-argument forwarder, and a setter.

   The setter family is the one worth reading. Its five members write the
   second argument to +0x00, +0x04 or +0x08 and zero +0x0C, +0x10 or +0x14 in
   the matching pair -- so a position component and its delta are 0xC apart,
   which is the clearest confirmation yet of the six-word fixed-point struct
   b7d23d1 merged: +0x00/+0x04/+0x08 are a position and +0x0C/+0x10/+0x14 the
   per-frame delta for the same axis. The families that clamp the delta to
   +/-0x20000 and the families that add it into the position were both
   consistent with that; this one makes the pairing explicit. */
/* Three more small families, thirteen functions. One of them, the solver
   wrapper, reads the s16 at +0x02, +0x06 and +0x0A of its argument -- the
   integer halves of the three 16.16 words again -- which is the third
   independent witness for that layout and the reason its caller builds a
   six-word UnkVec on the stack to pass in.

   func_8008F104 is in the byte-setter family and already had a THREE
   parameter declaration from a bind family that calls it that way, while its
   own body reads only the first. It is defined with three here and two go
   unread, which costs nothing. The alternative -- a one-parameter definition
   with callers passing three -- needs an unprototyped declaration and hides
   the mismatch instead of recording it. */
/* EIGHTEEN functions in this batch, twelve of them from six parallel agents
   given one function each and no context beyond the docs -- and all twelve
   were independently re-measured here before landing, which is the only
   reason to trust them. Each agent's function came with its sibling's
   varying words, so the six landings carried six more for free.

   Four mechanisms came back that the lever list did not have, all of them
   measured rather than argued:

   * EMISSION ORDER IS SOURCE ORDER in a function that initialises a whole
     record. The 50-word initialiser reaches 25 differing words with its
     fields written in ascending offset order, every differing word a
     permutation and every opcode already right; writing the statements in
     the listing's own order -- the 0x1C/0x1E/0x20 group first, then
     +0x12, +0x14, +0x10 in that sequence -- takes it to zero. Ascending
     order demonstrably emits ascending, so the target's order is the
     source's and not a scheduling artefact.
   * A CONSTANT ON THE SUBTRAHEND SIDE GETS FOLDED ONTO THE RESULT.
     `(p - (x - 0x10)) / 2` is rewritten by fold to `(p - x) + 0x10`, which
     biases the wrong operand and costs a word and a nop (54 words, 26
     differing). An `s32` local for the biased operand is what keeps the
     `addiu` where the target has it. An `s16` local does not -- it adds a
     sign-extend round trip (16 differing) -- and nor does an inline
     `(s32)(s16)` cast of the same expression.
   * TWO INLINE memcpys with ALIGNMENT-1 SOURCES explain a whole 57-word
     function: 0x30 bytes exceeds twice MAX_MOVE_BYTES so mips.c emits the
     runtime `(dst|src) & 3` test with an lwl/lwr body and an lw/sw body,
     while the 0xC-byte tail is straight-line unaligned ops with no second
     test, which is why both paths converge.
   * A `bgtz` GUARD NEED NOT COVER THE WHOLE BODY. One function's branch
     target is thirteen words in, so only two of its five statements are
     inside the test; with all five inside, everything else was already
     exact at 1 differing -- the branch offset alone.

   Also worth recording as method: the agents cost about 70K tokens each for
   a function in the 50-57 word range, against a reading of my own that
   costs a turn of this conversation. Six at once is the right shape for
   functions that need an individual reading and have a sibling.

   The parked 279-word spawner paid for itself here too: three of its settled
   notes -- the interior pointer, the handler store before the call, and the
   `col` local plus a temp for the third colour byte -- were used verbatim by
   the initialiser agent and all three were needed. */
extern s32 D_800D0410;
extern s32 D_800D0418;
extern u8 D_800786DC[];
extern u8 D_800786E8[];
extern u8 D_800D0090[];
s32 func_8008CA20(UnkTown2AE8 *v, s32 b, s32 n);
s32 func_8008CB58(UnkTown2AE8 *v, s32 b, s32 n);
s16 func_80096368(UnkTown2AE8 *b);
s16 func_80096440(UnkTown2AE8 *b);
s16 func_80096518(UnkTown2AE8 *b);
s16 func_800965F0(UnkTown2AE8 *b);
void func_800966C8(UnkTown2AE8 *a, s32 b);
void func_800966F8(UnkTown2AE8 *a, s32 b);
void func_80096728(UnkTown2AE8 *a, s32 b);
void func_80096758(UnkTown2AE8 *a, s32 b);
void func_8009A724();
void func_8009A81C();
s32 func_8008CFE8(UnkVec *v);
s32 func_8008D050(UnkVec *v);
void func_80098B30(void *p, UnkTown2AE8 *q, void *r);
void func_80098B70(void *p, UnkTown2AE8 *q, void *r);
extern u8 D_800D64F0[];
extern u8 D_800D00E8[];
extern u8 D_800D0188[];
void func_80033D08(u8 *p);
void func_80092698();
void func_80093524();
void func_800CE2D4();
void func_800CE4C4();
extern u8 D_800D6AA8[];
extern u8 D_800D6F24[];
extern u8 D_800ED15C[];
extern u8 D_800EE344[];
extern u8 D_800D0678[];
extern u8 D_800D0690[];
void func_8003DB94(void *p, void *t, s32 i);
extern u8 D_800CFC70[];
extern s32 D_800CFC7C[];
extern u8 D_800D0098[];
extern u8 D_800D0170[];
void func_8008BC58();
void func_800BB57C(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
void func_800BB640();
void func_800BB894(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
void func_800BBA98(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
extern u32 D_800F162C[];
void func_8009503C(UnkTown2AE8 *b);
void func_80091528();
void func_80091990();
/* ONE parameter HERE, and DO NOT "FIX" IT. The same symbol is DEFINED with
   TWO in csrc/unk_1AF38.c:92 -- `void func_800478B8(UnkSlot *s, s32 b)` --
   landed and byte-identical in the resident image, so the function really does
   take two. But all four of this overlay's call sites set up only $a0 and
   leave $a1 holding whatever the preceding call left, so the original
   overlay's translation unit must have seen a one-parameter declaration too.
   C does not check across translation units, so NOTHING in the build catches
   the disagreement: declaring it with two parameters here would add an $a1
   setup to four landed call sites and break every one of them. Verified
   2026-11-03 on func_8009DD40's landing, where the two-parameter form was
   tried first and is wrong. */
void func_800478B8(s32);
s32 func_8009CFE0(UnkTownSpawn *p, UnkVec *q);
extern u8 D_800D5FA0[];
extern u8 D_800D72C0[];
void func_800B9260();
s16 func_8008F3CC(UnkTown2AE8 *b);
s16 func_8008F41C(UnkTown2AE8 *b);
s16 func_8008F46C(UnkTown2AE8 *b);
s16 func_8008F4BC(UnkTown2AE8 *b);
void func_8008F50C(UnkTownColl *a, UnkTown2AE8 *b, s32 c);
void func_8008F520(UnkTownColl *a, UnkTown2AE8 *b, s32 c);
void func_8008F534(UnkTownColl *a, UnkTown2AE8 *b, s32 c);
void func_8008F548(UnkTownColl *a, UnkTown2AE8 *b, s32 c);
UnkNode *func_8009C390(void *a, UnkVec *b, void (*c)(), void *d);
void func_8009CD18(void *a, UnkTownRend *r, s32 id);
void func_800A6B10();
void func_800A6BEC();
void func_800A09F4();
extern u8 D_800D0E04[];
extern u8 D_800D0704[];
extern u8 D_800D0710[];
extern u8 D_800D06AC[];
extern u8 D_800F8C44[];
extern u8 D_800F8C80[];
s32 func_800970FC(u8 *arg0, UnkVec *vec, UnkSlot *arg2);
void func_800AB408(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800AB50C(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800AB708(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_80033AA8(s32 id);
void func_8008F134(UnkTownActor *a);
void func_800C30A4(UnkTownActor *a);
s32 func_8009F88C(s32 a, s32 b);
u8 *func_80048E00(u8 *p, u8 *q);
extern u8 D_800D15B8[];
extern u8 D_800D161C[];
extern u8 D_800D1650[];
s32 func_800C2D0C(s32 angle);
s32 func_800C2D94(s32 angle);
extern u8 D_800D0110[];
extern s32 D_800D4F88[];
void func_800924EC();
void func_80093250(void *p, UnkTown2AE8 *q, void *r);
void func_800981D4();
void func_80098244();
void func_800C26C0(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C27DC(void);
s32 func_80096FF4(UnkVec *q);
void func_800970AC(s32 a, UnkTownActor *b);
s32 func_800C2E1C(s16 a, s16 b);
s16 func_800C2F14(s16 a, s16 b);
void func_800A6A94(UnkVec *p);
void func_800A6B70(UnkVec *p);
extern UnkTownCFCB4 D_800CFCB4;
extern s32 D_800CFDD8[];
extern void (*D_800FE5D8)();
void func_800945B8(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_800946A0(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80092320();
void func_80092FF0();
void func_800CD70C();
void func_800CD87C();
void func_800C283C();
void func_800C28E8();
extern u8 D_800D0108[];
extern u8 D_800717D0[];
extern u8 D_800D4284[];
extern u8 D_800D42CC[];
extern u8 D_800D42E4[];
extern u8 D_800D4398[];
extern void (*D_800D6B28[])();
extern void (*D_800D6FBC[])();
extern void (*D_800D70A0[])();
void func_8003AF58(u8 *p, u8 *q);
s32 func_80095760(UnkTownCFCB4 *p);
s32 func_8009567C(UnkTownCFCB4 *p);
s32 func_80094BC8(u32 a, s32 b);
extern s16 D_800D0464[];
extern s32 D_800D046C[];
s32 func_8009FF50(void);
void func_800988C8(UnkTownTask *t, UnkTown2AE8 *b, void *c);
s32 func_800A9D74(s32 a, s32 b);
void func_80035208(s32 a);
void func_8008B158(s32 a);
void func_80094944(s32 a, s32 b);
void func_80093B00();
void func_8009B828();
void func_8009B8E8();
void func_8009B9BC();
extern UnkTownItemRec D_80100AF8;
extern Unk42710 D_80100A10;
extern void *D_80100B0C;
extern u8 D_8006E240[];
extern u8 D_800D073C[];
extern u8 D_800D0754[];
extern u8 D_800D076C[];
extern u8 D_800D0788[];
UnkNode *func_8003FD64(s32 type, void *base);
u32 func_8004A658(s32 i, s32 j);
void func_8009C1B4();
void func_8009DC8C(UnkTownSpawn *p, UnkTownRend *r, s32 c, void *d);
void func_8009DEBC();
void func_8009DD40(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
void func_8009F148(UnkTownSpawn *p, UnkTownSpawn *p2, UnkVec *q, UnkTownRend *r);
void func_800B2244();
extern s32 D_800D0B14;
extern s16 D_80100D18;
extern s32 D_80100D1C;
extern UnkVec *D_80100D20;
extern s32 D_80100D60;
extern s16 D_80100D80;
extern s16 D_80100D82;
extern u16 D_80100D88;
extern u16 D_80100D8A;
void func_80094378(UnkTownTask *t, UnkTown2AE8 *b, s32 c);
void func_800AAA58(UnkTownTask *t, UnkTown2AE8 *b, s32 c);
void func_800A9F14(UnkTown2AE8 *b);
extern UnkTownScene D_80082E80;
extern s32 D_80100DB0;
extern s32 D_80100E18;
extern s16 D_8006ADD4;
extern u8 D_800D0420[];
extern u8 D_800FE490[];
extern u8 D_80100D98[];
void func_8008EF58(void);
void func_8008FCE0(void);
void func_8008FD48(u8 *a, UnkTown2AE8 *b, u8 *c);
void func_8008FF5C(void);
void func_800903FC(void);
void func_800C01DC(void);
UnkNode *func_8003FE78(s32, UnkNode *, s16);
void func_800A48B0(u8 *a, UnkTown2AE8 *b);
void func_8009550C(UnkTown2AE8 *b);
void func_80033AE8(s32 a);
void func_800AB084(void);
extern UnkTownCFCC4 *D_800CFCC4;
extern s32 D_800FE518;
void func_80094774(UnkTownTask *t, UnkTown2AE8 *b, void *c);
extern s16 D_800D4258[4];
extern s16 D_800D4260[4];
extern s16 D_800D4268[4];
s32 func_8008CC90(s16 a, s16 b, s16 c, s16 d, s16 e, s16 f, s16 g, s16 h);
void func_800AE30C(void *p);
s32 func_80049E1C(s32 a, s32 d, s32 m);
void func_800B1778(s32 a, s32 b, s32 c);
void func_800B17C0(s32 a, s32 b);
void func_800ADB30(UnkTownADD80 *s);
s32 func_80065420(SVECTOR *v0, s32 *sxy, s32 *p, s32 *flag);
void func_800A8C84(SVECTOR *dst, UnkTown2AE8 *b);
s32 func_8009D424();
void func_800AD8CC(UnkTownADD80 *s);
void func_800ADA1C(UnkTownADD80 *s);
void func_800ADB04(UnkTownADD80 *s);
void func_800B1DCC(UnkTown260C *s);
void func_800B1F80(UnkTown260C *s, s32 d);
void func_800B2068(UnkTown260C *s);
void func_800B2394(UnkTown260C *s, s32 a);
s32 func_800B2400(UnkTown260C *s);
void func_800B25D8(UnkTown260C *s);
void func_800B2CE8(void *p);
extern s32 D_800834A8;
s32 func_800352FC(void);
void func_800AAF5C(void);
void func_800AAFE0(UnkTown2AE8 *v, s32 b);
void func_800C2C80(UnkTownActor *a, UnkTownRend *b, s32 c, s32 d);
void func_800649A0(void);
void func_80064A40(void);
MATRIX *func_80064B90(MATRIX *m, VECTOR *v);
MATRIX *func_80064BC0(MATRIX *m, VECTOR *v);
MATRIX *func_80065820(SVECTOR *r, MATRIX *m);
void func_800A1354(UnkTownA1080 *s);
s32 func_8008C3B8(UnkTownVec16 *q, s32 i, UnkTownPlane *pl);
s32 func_8008CF48(UnkVec *v, UnkTownPlane **out);
void func_800B1F10(s32 a, s32 i);
void func_800B1F48(s32 a, s32 i);
void func_8004DD2C(u8 *p);
s32 func_800AD99C(UnkTownADD80 *s);
void func_800AD7F8(UnkTownADD80 *s);
void func_800B17E4(s32 a);
extern u8 *D_800D1558;
extern u8 *D_800D155C;
void func_80049490(u8 *a);
void func_800494B4(u8 *p);
s32 func_800AD828(u8 *a, s32 b);
void func_800AE2A4(UnkTownADD80 *s, void *b, void *c);
void func_800B2C6C(UnkTown260C *s, void *b, void *c);
void func_800B06C4();
void func_800B1664();
/* The result record. func_8008FAC0 is the only reference in the overlay's
   listings, so only the two words it writes have a witness: +0x00 takes the
   axis index 0/1/2 and +0x04 the signed penetration on that axis. The
   function returns its ADDRESS, which is why the %lo lands in $v0 as well as
   in the stores' displacement. */
typedef struct UnkTownCFE60
{
    s32 unk_00;
    s32 unk_04;
} UnkTownCFE60;

extern UnkTownCFE60 D_800CFE60;

extern u8 D_800788E0[];
extern u8 D_800792DC[];
extern u8 D_800D15F4[];
void func_8004CBFC(u32 *p, u8 *q, u32 *r);
void func_800B08D4();
void func_800B0C68();
void func_800B0D0C();
void func_800B0FD4();
void func_8004E5A0(s32 v, s32 n, u8 *out);
void func_8004E69C(u8 *q);
void func_80069E38(u8 *dst, u8 *src);
u32 func_8004AC18(s32 i);
u8 *func_800B544C(u8 *s, u8 *p);
u8 *func_800B557C(u8 *s, u8 *p);
s32 func_800B555C(s32 a, s32 b);
UnkTownCardPair **func_800B5174(u8 *p, UnkTownCardPair **b, s32 c);
void func_800B52F4(u8 *p, UnkTownCardPair **b);
void func_800B59A4(u8 *p, UnkTownCardPair **b);
void func_800B5C60(u8 *p, UnkTownCardPair **b, s32 c);
extern u8 D_800892D8[];
extern u8 D_800892F0[];
extern u8 D_800892F4[];
extern u8 D_80089304[];
s32 func_800B5E10(s32 a);
u8 *func_800A652C(u8 *p);
u8 *func_800B512C(u8 *q, s16 n);
void func_800B66E8(u8 *p);
s32 func_800B6E3C(u8 *p);
s32 func_800B6F54(u8 *p);
void func_800B5B20(void);
void func_800B7B8C(s16 *p, UnkVec *b, u32 c);
extern u8 D_801116C8[];
extern u8 D_801118C8[];
extern u8 D_80111AC8[];
extern u8 D_80111CC8[];
extern u8 D_80111EC8[];
extern u8 D_80111EE8[];
extern u8 D_80111F08[];
extern u8 D_80111F28[];
extern u8 D_80111F48[];
extern u8 D_80111F68[];
extern u8 D_80111F88[];
extern s16 D_80162004[];
extern s16 D_800D2644[][0x10];
/* Two 0x20-byte-row tables func_800C4AEC selects between on bit 23 of the
   renderable's +0x08 flag word, indexed by bits 24..29 of the same word, and
   read as four u16s at +0x00/+0x02/+0x04/+0x06.  Same row stride as
   D_800D2644 above (and D_800D2650 is D_800D2644 + 0xC), so the `[][0x10]`
   shape is what makes the index come out as one `sll ... 5`. */
extern u16 D_800D2650[][0x10];
extern u16 D_800D2FC0[][0x10];
/* A u16 lookup func_800C4A88 indexes with a 0..3 direction and stores into
   the actor's resting facing at +0x6E, so these are the four quarter turns. */
extern u16 D_800D5070[];
/* (0x400 - a) >> 10 & 3: a facing in PSX angle units turned into a 0..3
   direction index.  Its `sll $a0, 16 / sra $a0, 16` head is the narrow-
   parameter lever -- the parameter is declared s16, not s32 with a cast at
   the use. */
s32 func_800C2B6C(s16 a);
extern u8 D_800D2EA4[];
void func_800B8E08(u16 *p);
void func_8009CDB4(void);
void func_8009CDCC(s32 a, s32 b, s32 c, s32 d, u8 *e);
void func_8009CE34(void);
extern u8 D_800D43F0[];

/* The two compiler constant pools func_800B8C90 and func_800B8B00 share. They
   are LOCAL INITIALISED ARRAYS in the original -- `s32 a[5] = {4, 5, 7, 0x1C,
   0x1F}` and `s32 b[4] = {6, 8, 0xB, 0xC}` -- and that spelling also reaches 0
   differing, but it emits its own $LC labels and so would duplicate the two
   surviving INCLUDE_RODATA blocks while func_800B8B00 is still asm. Read as
   aggregates instead: identical text, zero rodata. Switch BOTH functions to
   the local-array spelling and delete BOTH INCLUDE_RODATA lines in the same
   commit when func_800B8B00 lands -- that is the only configuration in which
   the rodata comes out at the right addresses. */
typedef struct
{
    s32 v[5];
} UnkTownRo5;

typedef struct
{
    s32 v[4];
} UnkTownRo4;

extern const UnkTownRo5 D_800894D0;
extern const UnkTownRo4 D_800895F0;
extern u16 *D_800D17D8[];
extern u16 *D_800D1868[];
extern s16 D_80111FA8[];
extern u8 D_80110EC8[];
extern u8 D_801110C8[];
extern u8 D_801112C8[];
extern u8 D_801114C8[];
s16 func_80042900(s8 *p, s8 n);
u32 *func_80040574(s32 n);
void func_80067014(s32 n);
void func_800672D8(void *p, void *q);
void func_8003F2A4(Unk80083968 *p);
extern u32 D_8002E5D8[];
extern u32 D_8002E5E8[];
extern u32 D_800D17C8[];
extern u8 D_80089344[];
s32 func_80048DA0(s32 i, s32 n);
u32 func_80048DCC(s32 i);
s32 func_80048DE8(s32 i);
u8 *func_800B6030(s32 a, u8 *s);
s32 func_800439BC(s16 a);
u32 func_800498EC(s32 i);
u32 func_80049918(s32 i);
s32 func_80049944(u32 a);
s32 func_80049984(u8 *p);
s32 func_8004AC3C(u8 *p, s32 *out);
void func_80069EC8(void *p, s32 n);
extern u8 D_80077FBC[];
extern u8 D_800892CC[];
extern u8 D_800D1794[];
extern u8 *D_800D1790;
extern const u32 D_800892C0[];
u8 *func_800B5D30(u8 *p);
extern const u32 D_80089308[];
extern u8 D_80089310[];
extern u8 D_80089318[];
extern u8 D_8008931C[];
extern u8 D_80089324[];
u8 *func_8004E634(u32 n, u8 *s);
s32 func_80043868(u8 *p);
/* func_800B2834's 4-byte template.  It HAS its own
   asm/nonmatchings/ovl_town/D_8008925C.s (`.word 0x00030201`) and keeps its
   INCLUDE_RODATA, so it is `extern` and the function copies FROM it; writing
   the literal would emit a second copy of the bytes.  `u8` and not `u32`:
   move_by_pieces takes MIN(src_align, dst_align) and the target's copy is the
   unaligned lwl/lwr + swl/swr pair, which needs alignment 1 on both sides. */
extern u8 D_8008925C[];
extern u8 D_800892DC[];
extern u8 D_800892E8[];
extern u8 D_800892EC[];
/* The primitive func_800B1DCC fades: an RGB triple at +0x00 (g and b are
   copied from r) and a signed screen coordinate at +0x0C. */
typedef struct UnkTownB1DCCPrim
{
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 pad_03[0x9];
    s16 unk_0C;
} UnkTownB1DCCPrim;

/* What each UnkNode's unk_0C points at here; +0x04 is the primitive above. */
typedef struct UnkTownB1DCCObj
{
    u8 pad_00[0x4];
    UnkTownB1DCCPrim *unk_04;
} UnkTownB1DCCObj;

extern u8 D_800D162C[][2];
extern u8 D_800D1630[][4];
void func_80044AAC(UnkNode *, UnkNode *);
s32 func_80069EF8();
/* The particle template func_80097AD0/func_800A895C build on the stack and
   hand to func_8009792C/func_800A878C: a THIRD view over the same six 16.16
   words as UnkTown2AE8, and the only one that needs to exist, because this
   object's witnessed fields do not collide -- the INTEGER HALVES of words
   0/1/2/5 (at +0x02/+0x06/+0x0A/+0x16) and the WHOLE words 3/4 (at
   +0x0C/+0x10, the per-frame delta pair).  UnkTown2AE8Int cannot serve: it
   stops at +0x0A and has no word members.

   IT MUST BE A NAMED-FIELD STRUCT AND NOT THE `*(s16 *)((u8 *)&t + N)` IDIOM
   THIS UNIT USES ELSEWHERE.  The halves are stored to a FIXED stack address
   here while the addends are loaded through a pointer, so the cast spelling
   clears MEM_IN_STRUCT_P and sched.c:854's exclusion drops the dependence:
   the next group's argument load floats above the store, the store lands in
   the following jal's delay slot, and the function comes out 94 words instead
   of 96.  A COMPONENT_REF keeps the dependence.  Measured both ways. */
typedef struct UnkTownPuff
{
    s16 pad_00;
    s16 unk_02;
    s16 pad_04;
    s16 unk_06;
    s16 pad_08;
    s16 unk_0A;
    s32 unk_0C;
    s32 unk_10;
    s16 pad_14;
    s16 unk_16;
} UnkTownPuff;

/* The circular body list func_8008FE90 walks and func_80095910 drives.  The
   argument is the list's sentinel, so the walk ends when the cursor comes back
   to it.  +0x08 and +0x0C are the position and the extent record the
   func_8008FD9C overlap test takes (that pairing is witnessed: the head's pair
   goes in as arguments 1-2 and the cursor's as 3-4).  +0x10 is where
   func_80095910 stores the node that was hit, which is what types it as a
   pointer back into the same record.  +0x14 is a kind byte compared against 2
   and 3, +0x15 an enable flag, +0x16 a packed angle masked to 12 bits.

   Size is a floor and the layout is PARTLY A GUESS, so second-guess this
   before building on it.  Witnessed by a use: +0x04 (the list link),
   +0x08, +0x0C, +0x10, +0x14, +0x15, +0x16 -- seven fields.  +0x00 is NOT
   witnessed by anything; nothing in the five functions landed here reads or
   writes it, and `s32 unk_00` is a placeholder chosen to make the +0x04 link
   land at its measured offset.  Nothing beyond +0x18 is known at all, and
   the other ~950 unlanded listings in this unit were not searched for
   further uses. */
typedef struct UnkTownBody
{
    s32 unk_00;
    struct UnkTownBody *unk_04;
    UnkVec *unk_08;
    UnkVec *unk_0C;
    struct UnkTownBody *unk_10;
    u8 unk_14;
    u8 unk_15;
    s16 unk_16;
} UnkTownBody;

UnkNode *func_8009792C(UnkTownPuff *t, s16 ang);
UnkNode *func_800A878C(UnkTownPuff *t, s16 ang);
UnkNode *func_800ABC00(UnkTownPuff *t, s16 ang);
void func_80097AD0(UnkTown2AE8 *src, UnkTown2AE8 *spread, s32 count);
void func_800A895C(UnkTown2AE8 *src, UnkTown2AE8 *spread, s32 count);
extern UnkTownRect D_800D04D0;
extern UnkTownRect D_800D04D8;
extern UnkSlot D_800D04E0;
extern s32 D_800D0460;
extern u16 D_800D0504;
extern s16 D_800D45AA[];
/* D_80082E80 under its UnkSlot face.  The unit declares the same address as
   UnkTownScene for func_8008FFC8; a second DECL with the same asm name is what
   keeps cse from unifying the two `&D_80082E80` pointer locals func_800970FC
   needs (2 words, and the whole second half's register assignment). */
extern UnkSlot D_80082E80_b __asm__("D_80082E80");
void func_80044D24(u8 *, UnkSlot *, s16);
void func_800453E0(u8 *, UnkVec *, UnkSlot *, s16);
u16 func_80066460(s32 tp, s32 abr, s32 x, s32 y); /* GetTPage */
void func_8006658C(void *ot, void *p);            /* AddPrim */
void func_80066640(void *p, s32 abe);             /* SetSemiTrans */
void func_800666E0(POLY_F4 *p);                   /* SetPolyF4 */
void func_800666F4(POLY_FT4 *p);                  /* SetPolyFT4 */
void func_80067E2C(DR_AREA *p, RECT *r);          /* SetDrawArea */
void func_80067EF4(void *p, s32 a, s32 b);        /* the 0xE6 mask primitive */
u16 func_8008C180(s16 x, s16 y);
s32 func_800C1D44(s32 id);
extern u8 D_80045C34[];
extern void *D_800D1BCC[];
extern u8 D_800F8A1C[];
extern u8 D_800F8A28[];
void func_800B9CB8();
void func_800BA8C4();
void func_800B9D64();
extern u8 D_80078CDC[];
void func_800AFFA8(u8 *s);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800D1464[];
extern u8 D_800F15AC[];
void func_800ABB20();
void func_800A86BC();
void func_800AE630();
void func_800AEEF0(u8 *s, void *a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_800B13D4(u8 *s, u32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void func_8009B454();
s32 func_8008F170(UnkTown2AE8 *q, u8 *t);
void func_8008F27C(UnkTownColl *a, UnkTown2AE8 *b, s32 c);
void func_8008F294(UnkTownColl *a, UnkTown2AE8 *b);
void func_8008F664(UnkTownColl *a, UnkTown2AE8 *b);
void func_8009539C(UnkTown2AE8 *b);
/* D_8008267C: a RESIDENT pointer (splat/town.bin_undefined_syms_auto.txt),
   referenced in this unit only by func_800CC644/func_800CC8AC, both as
   `lui %hi / lw %lo` -- so it is a POINTER VARIABLE in memory, not an
   address-taken object (compare D_80083498, which is lui/addiu). Its +0x08 is
   the same UnkTown2AE8 * UnkTownCam's is, read here for its integer halves at
   +0x02/+0x06. Typed UnkTownCam * on that witness alone. */
extern UnkTownCam *D_8008267C;
/* The func_800C5C70/func_800C5FDC pair's descriptors: one bind descriptor
   shared by both, and a per-member +0x58/+0x5C/+0x7C/+0x80 quad at the same
   0 / +4 / -0x30 / -0x28 spacing from the pair's base that the func_800C3050
   wrapper family uses (so four separate `%hi` symbols, not one struct). */
extern u8 D_800D5518[];
extern u8 D_800D5624[];
extern u8 D_800D562C[];
extern u8 D_800D5654[];
extern u8 D_800D5658[];
extern u8 D_800D5590[];

/* Both are INCLUDE_ASM stubs in this unit; only their addresses are taken, by
   the func_800BF3F0/func_800BF654 pair, so they are declared unprototyped the
   way func_800B9998 and func_800ABB20 above are. func_800BF1C8 is the park in
   docs/near-misses/func_800BF1C8.c, which reads its three parameters off
   func_80040190's cast. */
void func_800BF1C8();
void func_800BF4CC();
/* Two descriptor records 0x1C apart, each a 0x18-byte block followed by one
   pointer word: func_800BF3F0 hands &D_800D232C to func_8008F074 and stores
   D_800D2344 at the actor view's +0x48, and func_800BF654 does the same with
   the next pair. The blocks are address-taken (`lui`/`addiu`) and the words
   are loaded (`lui`/`lw %lo`), which is what fixes the two shapes. All four
   are resident symbols from splat/town.bin_undefined_syms_auto.txt. */
extern u8 D_800D232C[];
extern u32 D_800D2344;
extern u8 D_800D2348[];
extern u32 D_800D2360;
void func_800A7308(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C8B5C(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
/* The b94130 batch: thirteen more members of the bind family, 356 words, all
   thirteen verified 0 differing in a merged copy of the whole unit.  Ten are
   the func_80094984 task form and three the func_800C2E84 actor form; the
   descriptor is a D_800Dxxxx object in eight of them and a LOADED pointer --
   `t->unk_44->unk_14` or `a->unk_80->unk_14`, `lui`/`lw` rather than
   `lui`/`addiu` -- in the other five.  Eight landed on the first compile from
   the landed relatives; three needed a statement order found by brute force
   (func_800947CC 10 -> 0, func_80099468 10 -> 0, func_80099680 2 -> 0, all at
   the right word count throughout, so never structural), and two needed the
   pinned-register DEVICE annotated at their sites.

   Two externs the unit had not needed yet, and five handler symbols whose
   INCLUDE_ASM or definition sits BELOW the wrapper that installs them -- an
   address-taken symbol needs a declaration where an implicitly-declared call
   does not. */
extern u8 D_800D01B8[];
extern u8 D_800D01E0[];
void func_80091BD8();
void func_80093458();
void func_800980B4();
void func_80098764();
void func_80099874(UnkTownTask *a, UnkTown2AE8 *b, s32 c);
s32 func_800C2B88();
s32 func_800C2BE8();
/* Arity 3 read off func_80094AA0's OWN listing: it reads $a0, $a1 and $a2
   with no prologue and returns in $v0 (asm/nonmatchings/ovl_town/
   func_80094AA0.s, 22 words).  An angle-slew helper: both operands are
   masked to 0xFFF and the third is the per-frame step. */
s32 func_80094AA0(s32 cur, s32 tgt, s32 step);
/* One parameter: the body reads only +0x8 (lw) and +0xA (lh) off $a0 and
   returns in $v0.  The pointer is the UnkTown2AE8 its two call sites pass. */
s32 func_800A5894(UnkTown2AE8 *q);
/* Three parameters: func_800A55CC sets neither $a1 nor $a2 before its first
   jal (func_800942B0), so both are incoming, and func_800A5DF8 fills all
   three registers at its call site.  Types taken from the func_800942B0
   call sites in this unit. */
void func_800A55CC(UnkTownTask *a, UnkTown2AE8 *b, void *c);
/* Address-taken only, as func_80093B00 is. */
void func_800A5A98();
void func_800A5FDC();

/* The func_80092320 cluster: the two task descriptors its bind pair installs,
   the two handlers those pair bodies store at +0x00, and the two stubs they
   call.  func_800948DC reads no incoming argument register at all (its own
   listing writes $a0 and never reads one), so it takes none; func_80095840
   returns 0, 1 or 2 and takes (task, &D_800CFCB4).

   The four task setups the func_80092320 pair dispatches to are declared here
   rather than left to an implicit declaration: both pair bodies PRECEDE all
   four definitions, so without these the unit gains four more
   `type mismatch with previous implicit declaration` warnings. */
extern u8 D_800D00B0[];
extern u8 D_800D0140[];
void func_8009204C();
void func_80092CD4();
void func_800948DC(void);
s32 func_80095840(UnkTownTask *t, UnkTownCFCB4 *p);
void func_80094088(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80094414(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80094474(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_800944BC(UnkTownTask *t, UnkTown2AE8 *b, void *c);

/* The two continuation handlers the func_800A7360 / func_800C8BB4 pair
   installs into +0x50.  Both are defined further down (:9247 and :18117) and
   both are referenced by ADDRESS, which no implicit declaration can cover. */
void func_800A73E0(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C8C3C(UnkTownActor *a, UnkTown2AE8 *b, s32 c);

/* The two UnkNode +0x10 dispatch handlers the func_800A71F0 / func_800C8A68
   pair installs, and the delta-vector pair func_800A71F0 copies into the
   mover's +0x0C/+0x10.  Both functions are defined further down (:9835 and
   :19157) and both are referenced by ADDRESS -- `%hi`/`%lo` on a function
   symbol, which no implicit declaration can cover and which ovlverify masks.
   D_800D0DF0 is read as two s32s; `extern u8 D_800D0DF0[]` with
   `*(s32 *)(D_800D0DF0 + 4)` is 71 words against 70 with 53 differing. */
void func_800A7338(UnkTownDispatch *d);
void func_800C8B8C(UnkTownDispatch *d);
extern s32 D_800D0DF0[];

/* Three near-sibling pairs of bind-and-install bodies, and the two resident
   allocator entry points the func_800B44F0 pair reaches past func_80049004 /
   func_800491CC.  func_800AAB20 and func_800A6EA4 are declared because their
   ADDRESSES are taken (`%hi`/`%lo` on a function symbol) by a definition that
   precedes them in the file. */
extern u8 D_800D6A58[];
extern u8 D_800D11D0[];
extern u8 D_800D11EC[];
extern u8 D_800D0DA8[];
extern u8 D_800D0DC0[];
void func_800AAB20();
void func_800A6EA4();
s32 func_80048FBC(u8 *p, s32 n);
u32 func_8004B404(s32 n);

/* Three forward references the bind-and-dispatch group needs: both handlers
   are DEFINED further down the file than the function that installs or calls
   them, and an ADDRESS-TAKEN name cannot be reached by an implicit
   declaration the way a call can.  Spelled unprototyped, the way the
   address-taken-only func_80093B00 above is, so the real definitions below
   stay the prototypes.

   D_800D0620 is a word func_8009AC0C clears; its store is the split
   `lui %hi` + `sw $zero, %lo(...)($v0)` form, which is what a declared-size
   scalar gives at -G0. */
void func_8009ACFC();
void func_80093D48();
void func_800CA60C();
extern s32 D_800D0620;

/* func_800C26C0's gate: still an INCLUDE_ASM stub in this unit, and its
   `beqz $v0` at 0x800C26E4 is the only witness that it returns a value. */
s32 func_800C2A60(UnkTownActor *a);

/* The func_800A07E8 pair's three stub callees and the handler it installs.
   func_800A0668 reads only its own $a0 (lw 0x98, lbu 0x95) and returns a 0/1
   flag; func_800A0F10 takes the actor and the second slot argument, which it
   forwards to func_8008FAC0 against D_800CFCB4, and returns 0/1;
   func_800A0B74 takes the actor and a POINTER TABLE whose index comes from
   func_800B2834/func_800B28A0.  func_800A0884 is only ever address-taken into
   the +0x50 slot, so it is declared unprototyped the way the slot is. */
s32 func_800A0668(UnkTownActor *a);
s32 func_800A0F10(UnkTownActor *a, void *b);
void func_800A0B74(UnkTownActor *a, void *tbl);
void func_800A0884();

/* The func_800A3248 pair's four installed handlers.  Both pair bodies PRECEDE
   func_800A32C8's and func_800A34E0's definitions, and the other two are
   still stubs, so all four need a declaration here: these are ADDRESS-TAKEN
   (`%hi`/`%lo` on a function symbol, which ovlverify masks and which has no
   `jal` to decode), not called. */
void func_800A32C8();
void func_800A32F0();
void func_800A34E0();
void func_800A3508();

/* The func_80091260 / func_80091528 pair's forward references.  Both bodies
   are at :4562/:4564 and every one of these is defined, or still stubbed,
   below them.  func_800951B4 is this pair's second mover update -- the
   position the rest of the D_80083160 family gives func_80095094 -- and is
   still an INCLUDE_ASM stub, as are func_80093ED8 and func_80094DA8. */
void func_800951B4();
void func_80093ED8();
void func_80094DA8();
void func_80093E18(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80093E58(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80093F48(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80093FC8(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_800943B8(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80097844(UnkTown2AE8 *src, s32 count);
void func_80098868(UnkTownTask *t, UnkTown2AE8 *b, s32 c);

/* The func_800CB8CC / func_800C6C10 effect pair and the two UnkNode +0x10
   handlers they install.  Each gate (func_800CB7D0 / func_800C648C) is called
   with the PARENT view -- `lw $a0, 0x9C($s1)` loads straight into $a0 -- and
   its result is tested with bnez, so it returns a value.  The handlers take
   func_80040190's three-parameter shape (view, mover, rend); neither reads
   its third parameter, which costs nothing in the callee and is witnessed by
   func_800CB8CC's own `n->unk_10(p, q, r)` call. */
s32 func_800CB7D0(UnkTownSpawn *p);
s32 func_800C648C(UnkTownSpawn *p);
void func_800CB9B8(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
void func_800C6CE4(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
/* The sprite-handle table func_800CB8CC indexes by its second argument and
   func_800CBAE0 (still a stub) indexes by the actor's +0x96.  Read as a word
   array: `lui/addiu` the whole address into one register, `sll idx, 2`,
   `addu`, then `lw 0x0($v1)`. */
extern u32 D_800D68B0[];
extern s32 D_800D0A48[];
extern RECT D_800D0A40;

void func_8009F1C0();
void func_8009F48C();
void func_800A22D0();
void func_800A24A8();
void func_800A33E8();

/* func_8009C148 and func_800C15C0 had no declaration anywhere in the unit.
   func_800C1F8C and func_800C21F8 are ADDRESS-TAKEN by func_800C1EA4 and
   func_800C2124 (`%hi`/`%lo` on a function symbol -- no `jal`, and ovlverify
   masks it), so they need one here even while they are stubs. */
void func_8009C148();
void func_800C15C0();
void func_800C170C();
void func_800C1F8C();
s32 func_800C21F8();
/* func_8009B2C4 and func_8009B9BC compare the camera task's +0x00 handler
   against these three.  Same case as the five above: the address is taken
   with no `jal`, so ovlverify masks the word and only the container build
   can see a wrong symbol.  func_800AA5F8 is LANDED further down, so this is
   a forward declaration of its real signature rather than an unprototyped
   placeholder -- an implicit declaration at the use site would not compile. */
void func_800920F4();
void func_800933FC();
void func_800AA5F8(UnkTownTask *a, UnkTown2AE8 *b, s32 c);
extern s32 D_80083788;
extern s32 D_800D0428;
/* func_800BC840 installs func_800BC764 at the UnkNode's +0x10.  It is an
   INCLUDE_ASM stub in this unit and only its address is taken, so it is
   declared unprototyped the way func_800BF1C8 and func_800BF4CC above are.
   D_800D20CC is address-taken (`lui`/`addiu`) and handed to func_8008F074;
   D_800D20E4 is LOADED (`lui`/`lw %lo`) and stored at the actor view's +0x48
   -- the same 0x18-byte-record-plus-pointer-word pair shape the
   D_800D232C/D_800D2344 comment above records.  D_800F15E4 is a bind
   descriptor handed to func_8003DB94 exactly as D_800F15AC is at the
   func_800ABB20 definition.  All four are read out of
   asm/nonmatchings/ovl_town/func_800BC840.s itself. */
void func_800BC764();
extern u8 D_800D20CC[];
extern u32 D_800D20E4;
extern u8 D_800F15E4[];

extern u8 D_800CFFF0[];
extern u8 D_8006CADE[];
void func_800A77D0();
void func_800A1D1C();
void func_800A1F00();
void func_800BFB8C();
extern u8 D_80077E84[];
extern u8 D_80077EF0[];
extern u8 D_80077EFC[];
s32 func_80049DE8(s32 a, s32 b, s32 c);
s32 func_80049E6C(s32 a);
s32 func_800AE4E4(s32 a);
void func_800AE854(u8 *s);
void func_8004E9E4(void);
void func_8004E994(u8 *p);
u32 func_8004DA74(u8 *a, void *b, s32 c);
void func_800BC1DC(u8 *rec, void (*fn)(), u32 payload);
void func_800BC390();
extern u8 D_8008961C[];
extern u8 D_80053A88[];
extern u8 D_800782EC[];
extern u8 D_80111FB0[];
extern u8 D_80100B50[];
extern s32 D_80100B68;
extern u8 D_80100B3B;
extern UnkTownScrCtx *D_80100900;
void func_8009ED7C();
s32 func_800A00E8(s32 i);
/* The 0x800BCxxx walker pair's animation tables, each handed straight to
   func_8003DB94 as its second argument, and the four-entry table of such
   pointers the direction state indexes.  Element type unknown; only the fact
   that D_800D210C's slots are word-sized has a witness (`lw` at a stride of
   4). */
extern u8 *D_800D210C[];
extern u8 D_800E9CD4[];
extern u8 D_800E9CFC[];
extern u8 D_800E9D0C[];
extern u8 D_800E9D34[];
extern u8 D_800E9D54[];
extern u8 D_800E9D8C[];
extern u8 D_800E9DAC[];
extern u8 D_800E9DD4[];
extern u8 D_800E9DEC[];
extern u8 D_800E9E14[];
extern u8 D_800E9E34[];
extern u8 D_800E9E54[];
extern u8 D_800E9E7C[];
extern u8 D_800E9ECC[];
/* The m29_ group (func_800BDFA0, func_800BEA88, func_800BEAD0, func_800BF4CC).
   func_800BEBA4 is an INCLUDE_ASM stub BELOW the func_800BEAD0 that installs
   it at the UnkNode's +0x10, and an address-taken symbol needs a declaration
   where an implicitly-declared call does not; it is declared unprototyped the
   way func_800BF1C8 and func_800BF4CC are.  func_800673A0 has no declaration
   anywhere in csrc/ or include/ -- the signature is the one the func_800BF1C8
   park (docs/near-misses/func_800BF1C8.c:82) reads off its own listing.
   D_800D231C/D_800D2324 are the func_800BEA88 pair's two descriptor blocks,
   D_80113138 its VRAM source and D_8007789C the texture record func_800BEAD0
   stores at the renderable's +0x08 -- all four address-taken (`lui`/`addiu`)
   and all four resident symbols from splat/town.bin_undefined_syms_auto.txt. */
void func_800BEBA4();
void func_800BF15C(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
void func_800673A0(s16 *r, s32 x, s32 y);
extern u8 D_800D231C[];
extern s16 D_800D2324[];
extern u8 D_80113138[];
extern u8 D_8007789C[];

/* p6: the 0x1F800000 scratchpad as func_800BB218 uses it. */
typedef struct UnkTownBB218SP
{
    DVECTOR unk_00;
    u32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    u32 *unk_10;
    SVECTOR unk_14;
} UnkTownBB218SP;
extern u8 D_80033D54[];
extern u8 D_80037714[];
extern u8 D_80072210[];
extern u8 D_80072214[];
extern s32 D_80082ABC;
extern u8 D_800794AC[];
extern u8 *D_800CFB74[];
extern u8 D_800CFC38[];
extern s32 D_800CFC44[];
extern u8 D_800FC418;
void func_8008B9D8(UnkTownDispatch *d);
void func_8008BA00();
void func_8008BED8();
extern UnkTownAA10C D_800D1178;
extern UnkTownAA10C D_800D119C;
extern VECTOR D_800D11C0;
extern s8 D_800D1074[];
extern s32 D_800D1080[];
extern u16 D_80100D40;
extern u16 D_80100D42;
void func_800AA10C(UnkTownAA10C *a, UnkTown2AE8 *b);
/* The two UnkTown2AE8 sentinels func_800917EC (:5617) rings puffs with:
   D_800FE4E0 is the latched position it builds from the mover, D_800D043C
   the per-axis spread func_800A895C randomises inside.  Both are
   out-of-image (splat/town.bin_undefined_syms_auto.txt:946 and :272) and
   both are reached symbolically, `lui %hi` + `addiu %lo`, so a plain
   complete-type extern is what produces the listing's addressing. */
extern UnkTown2AE8 D_800D043C;
extern UnkTown2AE8 D_800FE4E0;

/* func_800917EC (:5617) is func_80094048's ONLY caller anywhere -- no other
   csrc call site and no other `jal` under asm/nonmatchings/ovl_town/ -- and
   it sets all three argument registers, so the handler convention is the
   witness and func_80094048's one-parameter definition (:6696) is the
   narrower guess.  Without this declaration the call rides the implicit one
   and costs three warnings (`previously implicitly declared to return int`
   and its pair); declared `()` like func_80093ED8 (:2539) they go away and
   both functions stay at 0 differing.  Widening the DEFINITION instead does
   not help -- the implicit declaration still precedes it (measured). */
void func_80094048();

s32 func_8003BD84(s32 x, s32 y);
extern s16 D_80083228;
extern u8 D_80046398[];
extern u8 D_800D16D8[];
extern u8 D_800D16FC[];
extern u8 D_800D172C[];
extern s32 D_800D0414;
extern s32 D_800D041C;
extern s32 D_800FE5CC;
extern s32 D_800FE5D0;
s32 func_8008CABC(UnkTown2AE8 *v, s32 b, s32 n);
s32 func_8008CBF4(UnkTown2AE8 *v, s32 b, s32 n);
void func_800B9998();
extern u8 D_800D6220[];
extern u8 D_800D6244[];
extern u8 D_800D62C4[];
extern u8 D_800D62D8[];
extern u8 D_800D62E0[];
extern u8 D_800D6308[];
extern u8 D_800D630C[];
extern u8 D_800D00A8[];
extern u8 D_800D0198[];
extern u8 D_800D01C8[];
extern u8 D_800D01D0[];
extern u8 D_800D01D8[];
extern u8 D_800D6A70[];
extern u8 D_800D6EE8[];
extern u8 D_800D7284[];
void func_80098608();
void func_8009AA2C();
void func_8009AAB8();
void func_8009AB74();
void func_80094620();
void func_8009AD70();
void func_800CBFAC();
void func_800CCC34();
void func_800CCC70();
void func_800CD650();
void func_800CD9F4();
void func_800CDBB8();
extern u8 D_800F8B6C[];
extern u8 D_800F8F68[];
extern u8 D_800F8F78[];
extern u8 D_800F8F88[];
extern u8 D_800F8F98[];
extern u8 D_800F8FA8[];
extern u8 D_800F8FC0[];
extern u8 D_800F8FD8[];
extern u8 D_800F8FF0[];
void func_8009CB64(void *a, void *b, u8 *c, void *d);
void func_800A7DF8();
void func_800A9624();
void func_800A970C();
void func_800C2CB0(u8 *a, s32 c, void *s, s32 z);
void func_80033CD8(u8 *p, u32 tag);
extern u8 D_80045340[];
void func_80069EE8(u8 *p, s32 v, s32 n);
void func_800422DC(u8 *arg0, u8 *arg1);
void func_800423C0(u8 *arg0, s32 arg1, u8 *arg2);
u32 func_8003C06C(s32 i);
void func_8003E188(s16 arg0, s16 arg1);
void func_8009F374();
s32 func_8008CE08(void *p);
s32 func_8008D0B8(s32 x, s32 y, s32 z, UnkTownPlane **out);
s32 func_8008D5C4(s32 x, s32 y, s32 z);
s32 func_8008DA80(s32 x, s32 y, s32 z);
s32 func_8008DFB4(s32 x, s32 y, s32 z);
s32 func_8008E4EC(s32 x, s32 y, s32 z);
s32 func_8008EA20(s32 x, s32 y, s32 z);
void func_800CCF74(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
/* Called with ONE argument by func_8008F074/F0A4/F0D4 and with THREE by
   func_8008F104 and func_800C6180, which set $a0/$a1/$a2 before the jal. A
   1-param prototype cannot express the 3-arg sites and a 3-param one would
   make the 1-arg sites "too few arguments", so this is unprototyped. Measured:
   the four existing callers are unchanged by the relaxation. */
void func_8008F01C();
extern u8 D_800CFCE8;
extern u8 D_800CFCEA;
extern u8 D_800CFCEB;
extern u8 D_800CFCEC;
extern u8 D_800CFCED;
extern u8 D_800CFCE9;
extern u8 D_800CFCF0;
extern u8 D_800CFCF1;
extern u8 D_800CFCF2;
extern u8 D_800CFCF3;
void func_80020298();
void func_80020300();
void func_80020A20();
void func_800212B8();
void func_800224E0();
void func_80022C8C();
void func_80024100();
void func_800254A4();
/* Returns a value: func_8008FFC8 tests it with `if (func_80033B2C(...) == 0)`.
   The two callers at func_800A9C6C / func_800A9C8C discard it, which is why
   the placeholder was `void`; this is the same "only a caller can falsify a
   signature" case as the arity cascades. Measured: both of those unmoved. */
s32 func_80033B2C();
void func_8008B408();
void func_800930E4();
void func_800954C4(UnkTown2AE8 *b);
void func_800954DC(UnkTown2AE8 *b);
void func_8009CEE8();
void func_8009FDBC();
void func_8009FE84();
void func_800A496C();
void func_800AE56C();
void func_800B2E3C();
void func_800B3440(u8 *s);
void func_800B2D00(u32 *a, u8 *b, u32 *c);
void func_800B2FF8(u32 *a, u8 *b, u32 *c);
void func_800B3178(u32 *a, u8 *b, u32 *c);
void func_800B39E4(UnkTownSlot **a);
void func_800B5020(UnkTownSlot **a);
void func_800B4AB0(u8 *s);
void func_8004CB2C(u8 *p);
void func_800497F4(u8 *p, s32 a);
extern u8 D_80071244[];
void func_800B47BC();
void func_800B4B7C(u8 *s);
void func_800C22A4();
void func_800C23A8();
void func_800C2444();
void func_800C4D20();
void func_800CA134();
void func_800CA214();
void func_800CA308();
extern u8 D_800D5540[];
extern u8 D_800D5554[];
extern u8 D_800D5568[];
extern u8 D_800D557C[];
extern u8 D_800D5958[];
extern u8 D_800D5960[];
extern u8 D_800D5988[];
extern u8 D_800D598C[];
void func_800C7E70();
void func_800C7EB0();
void func_800CA094();
void func_800CA0D4();
void func_800C58F4(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
s32 func_8008CF80(UnkVec *v);
s32 func_8008CFB4(UnkVec *v);
s32 func_8008D01C(UnkVec *v);
s32 func_800644B8(s32);
void func_800940D0();
void func_800942B0();
void func_800C9C94();
void func_800CEA80();
void func_800CEB94();
void func_80095094(UnkTown2AE8 *b);
s32 func_800B0158();
s32 func_800B3868();
s32 func_800B44B0();
s32 func_800B44F0();
s32 func_800B4E98();
s32 func_800B4ED8();
UnkNode *func_8003FC64(s32);
void func_8004B248(u32 *);
extern u8 D_8004CAA0[];
extern u8 D_800D54DC[];
extern u8 D_800D6290[];
extern u8 D_800D6734[];
extern u8 D_800D675C[];
void func_800C86EC();
void func_800CE6E4();
void func_800CE800();
extern u8 D_800CFD00[];
extern UnkTownVec16 D_800CFD18[];
extern UnkTown2AE8 D_800FE5C0;
extern UnkTownVec16 *D_800D0408;
extern u8 D_800D0648[];
extern u8 D_800D0660[];
extern u8 D_800F8A40[];
extern u8 D_800F8CEC[];
extern u8 D_800F8D64[];
extern u8 D_800F8D70[];
u8 *func_80049004(u8 **p, s32 n, s32 m);
void func_800491CC(u32 **p, u32 v, s32 n);
void func_8008F104(UnkTownSpawn *p, UnkVec *q, void *t);
/* The 4th parameter is a POINTER, not s32: func_800C6180/func_800C8D34 pass
   %hi/%lo(D_800D0690) in $a3 where the four landed siblings pass 0. Widened
   here rather than cast at those two sites; measured, the siblings' 0 still
   emits addu $a3, $zero, $zero and none of them moved. */
void func_8009B218(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r, void *d);
u32 func_800491F4(s32, u8 *, s32);
extern UnkTownList D_800891F8;
extern UnkTownList D_8008926C;
extern u8 D_800CFCEE;
extern UnkTown2AE8 D_800D0608;
extern u8 D_800CFCEF;
extern UnkTownPlane D_800CFCAC;
extern s32 D_800FE480;
extern s32 D_800FE484;
extern u8 D_800FE488[];
extern u8 D_800D6ABC[];
extern u8 D_800D6AD0[];
extern u8 D_800D6F4C[];
extern u8 D_800D6F60[];
void func_80095C80(UnkTown2AE8 *q);
void func_80094C1C(void *p);
void func_80094C74(UnkTown2AE8 *q);
void func_80094714(void *p, UnkTown2AE8 *q, void *r);
s32 func_80095978(UnkTown2AE8 *q, u8 *t);
void func_80094910(void);
void func_80095A94(UnkTown2AE8 *q, s32 n, u8 *t);
void func_800ABD74(UnkTown2AE8 *q);
void func_800954F4(UnkTown2AE8 *q);
void func_8009451C(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_80099C18();
void func_800C3FFC();
void func_80098928(UnkTownTask *a, UnkTown2AE8 *b, s32 c);
void func_80098834(UnkTownTask *a, UnkTown2AE8 *b);
void func_8008BF0C(UnkTownMover *m, UnkTown2AE8 *b, void *c);
extern u8 D_800CFD58[];
extern u8 D_800CFD78[];
extern u8 D_800CFD98[];
extern u8 D_800CFDB8[];
u32 func_800374F4(u16 n);
extern s32 D_800D4FE8[];
extern s32 D_800D4FF8[];
extern s32 D_800D5008[];
extern s32 D_800D5018[];
void func_800C37C4(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C7A6C(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C95A8(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C9DB8(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C355C(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C9384(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
void func_800C9864(UnkTownActor *a, UnkTown2AE8 *b, s32 c);
s32 func_800C2B38(UnkTown2AE8 *b);
void func_800C9D40();
void func_800CDF54();
void func_800CEA40();
void func_800CEB54();
void func_80095388(UnkTown2AE8 *b);

/* Five more families, 40 functions, all 0 differing. Two notes.

   The four-zero-argument family's `addu $a1, $a0, $zero` copies are gcc
   reusing a value it already has in a register, not four source zeros being
   materialised separately -- the same thing the card overlay's message
   wrappers did. The source is one call with four zeros.

   And one family hands its argument to func_800491F4, which is a RESIDENT
   ENGINE function already decompiled in csrc/unk_1C540.c. Its prototype is
   not a guess here, it is copied, and csrc/unk_1C540.c has a call with the
   same shape. Where an overlay family calls into the engine, the arity
   question the listing cannot answer is already answered in csrc/. */
extern u8 D_800D0080[];
extern u8 D_800D0088[];
extern u8 D_800D00B8[];
extern u8 D_800D0128[];
extern u8 D_800D0148[];
extern u8 D_800D0150[];
extern u8 D_800D0160[];
extern u8 D_800D0168[];
extern u8 D_800D01A0[];
extern u8 D_800D01A8[];
extern u8 D_800D01E8[];
extern u8 D_800D01F0[];
extern u8 D_800D1594[];
extern u8 D_800D15CC[];
extern u8 D_800D1624[];
extern u8 D_800D16BC[];
extern u8 D_800D1760[];
extern u8 D_800D17AC[];
extern u8 D_800D17C4[];
void func_80090D44();
void func_80090DF0();
void func_80090F84();
void func_80091114();
void func_800911D4();
void func_8009221C(void *p, UnkTown2AE8 *q, void *r);
void func_800927EC();
void func_80092DA8();
void func_80092ECC();
void func_80093638();
void func_80093C40();
void func_80093C58();
void func_8009E218();
void func_8009E258();
void func_8009E2C0();
void func_8009E348();
void func_8009E370();
void func_8009E3D4();

/* Reset one entry. The index is written inline, not through a base local:
   that is what puts the `sll` ahead of the `lui`/`addiu` the way the target
   has it (docs/overlays.md lever 1, read the other way round). */

void func_800BEBA4(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    s16 buf[16];
    UnkNode *o;
    UnkTownRend *rr;
    s32 i;
    s32 red;
    s32 green;
    s32 blue;
    s16 v;
    u16 w;

    *(s16 *)((u8 *)p + 0x2) -= 1;
    q->unk_00 += q->unk_0C;
    q->unk_08 += q->unk_14;
    q->unk_14 += 0x38000;
    switch (*(s16 *)p)
    {
    case 0:
        if (*(s16 *)((u8 *)p + 0x2) > 0)
        {
            break;
        }
        func_80053DA8(0x519);
        for (i = 0xF; i >= 0; i--)
        {
            o = func_8003FC64(0x136);
            if (o != NULL)
            {
                o->unk_10 = (u32)func_800BF15C;
                func_8004491C(o, (u32)D_80045340);
                rr = (UnkTownRend *)o->unk_0C;
                o->unk_08->unk_00 = q->unk_00 + (((func_80069EF8() & 0x1FF) - 0x100) << 13);
                o->unk_08->unk_04 = q->unk_04 + (((func_80069EF8() & 0x1FF) - 0x100) << 13);
                o->unk_08->unk_08 = q->unk_08;
                rr->unk_10 = 0x60;
                rr->unk_1E = 0x1000;
                rr->unk_1C = 0x1000;
                rr->unk_14 |= 0xC;
                func_8003DB94(rr, D_800F15AC, 0);
                *(u32 *)&rr->unk_0C = 0x606060;
            }
        }
        *(s16 *)((u8 *)p + 0x2) = 0x8;
        (*(s16 *)p)++;
        break;
    case 1:
        if (*(s16 *)((u8 *)p + 0x2) > 0)
        {
            break;
        }
        r->unk_14 |= 0x80;
        *(s16 *)((u8 *)p + 0x2) = 0x40;
        (*(s16 *)p)++;
        break;
    case 2:
        func_8006733C(D_800D2324, buf);
        func_80067014(0);
        for (i = 0xF; i >= 0; i--)
        {
            v = buf[i];
            red = 0x1F - (((0x1F - (v & 0x1F)) * *(s16 *)((u8 *)p + 0x2)) >> 6);
            green = 0x1F - (((0x1F - ((v >> 5) & 0x1F)) * *(s16 *)((u8 *)p + 0x2)) >> 6);
            blue = 0x1F - (((0x1F - ((v >> 10) & 0x1F)) * *(s16 *)((u8 *)p + 0x2)) >> 6);
            buf[i] = (blue << 10) + ((green << 5) - 0x8000) + red;
        }
        if (*(s16 *)((u8 *)p + 0x2) < 0x30)
        {
            for (i = 0; i < 0x10 - (*(s16 *)((u8 *)p + 0x2) >> 2); i++)
            {
                red = (func_80069EF8() % 3) * 8 + 0xF;
                green = (func_80069EF8() % 3) * 8 + 0xF;
                blue = (func_80069EF8() % 3) * 8 + 0xF;
                buf[func_80069EF8() & 0xF] = (blue << 10) + ((green << 5) - 0x8000) + red;
            }
        }
        func_800672D8(D_800D231C, buf);
        func_80067014(0);
        if (*(s16 *)((u8 *)p + 0x2) > 0)
        {
            break;
        }
        *(s16 *)((u8 *)p + 0x2) = 0x1E;
        (*(s16 *)p)++;
        break;
    case 3:
        for (i = 0xF; i >= 0; i--)
        {
            red = (func_80069EF8() % 3) * 8 + 0xF;
            green = (func_80069EF8() % 3) * 8 + 0xF;
            blue = (func_80069EF8() % 3) * 8 + 0xF;
            buf[i] = (blue << 10) + ((green << 5) - 0x8000) + red;
        }
        func_800672D8(D_800D231C, buf);
        func_80067014(0);
        if (*(s16 *)((u8 *)p + 0x2) > 0)
        {
            break;
        }
        *(s16 *)((u8 *)p + 0x2) = 0x20;
        (*(s16 *)p)++;
        break;
    case 4:
        for (i = 0xF; i >= 0; i--)
        {
            w = ((u16 *)D_80113138)[i];
            red = w & 0x1F;
            green = (w >> 5) & 0x1F;
            blue = (w >> 10) & 0x1F;
            red = red + (((0x1F - red) * *(s16 *)((u8 *)p + 0x2)) >> 5);
            green = green + (((0x1F - green) * *(s16 *)((u8 *)p + 0x2)) >> 5);
            blue = blue + (((0x1F - blue) * *(s16 *)((u8 *)p + 0x2)) >> 5);
            buf[i] = (blue << 10) + ((green << 5) - 0x8000) + red;
        }
        func_800672D8(D_800D231C, buf);
        func_80067014(0);
        if (*(s16 *)((u8 *)p + 0x2) > 0)
        {
            break;
        }
        (*(s16 *)p)++;
        break;
    case 5:
        func_800672D8(D_800D231C, D_80113138);
        func_80067014(0);
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* func_800BC6CC's bare form: the same +0x10 handler shape installed by
   func_800BEBA4, with no mover advance at all -- run the renderable, then
   retire the node on the REND +0x14 gate.

   THE UNREAD SECOND PARAMETER IS WITNESSED HERE, which the "an unread
   parameter costs no words in the callee" lever does not predict: dropping it
   and taking the renderable second is 27 words / 1 differing, and the one
   word is `move $s0, $a1` for the target's `move $s0, $a2`.  An unread
   parameter is free only when nothing after it is read.  Typing the third
   parameter `s32` and casting at the use is 27 / 0, so the pointer spelling
   is the house convention's and not this body's witness.

   The gate must follow the call (before it, 21 words / 27 differing) and the
   +0x1E OR must precede the D_800814A0 one (swapped, 27 / 6).  That swap is a
   pure emission-order difference -- both read-modify-writes stay the same six
   words and the D_800814A0 `lui` stays in the `beqz`'s delay slot either way,
   so it is the ordinary "neither arm leaves, source order is emission order"
   fact and NOT the `lui`-sharing mechanism func_800C6CE4's comment records
   for its own swap.

   27 words -- its header reads `nonmatching func_800BF15C, 0x6C`, and
   0x6C/4 = 27.  0 differing, first compile. */
void func_800BF15C(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    func_800478B8((s32)r);
    if ((r->unk_14 & 0x6000) != 0)
    {
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Installed in an UnkNode's +0x10 handler slot by func_800BF3F0, so the three
   parameters are the dispatcher's: func_80040190 calls it as
   `fn((u8 *) node + 0x20, node->unk_08, node->unk_0C)`.

   TWO LOW-RAM FLAGS OFF ONE BASE POINTER, and that is the whole shape of
   case 0. 0x80011F81 and 0x80012D5C are both outside the loaded image, so
   they are constants rather than symbols; spelled as two independent
   constant addresses the function is 7 DIFF, because the second one's `lui`
   is then expanded in the fall-through block, where $v0 is free and the high
   shares the loaded value's register. Read BOTH off one `(u8 *) 0x80010000`
   -- a base whose low half is zero, so it costs a single `lui` and every
   displacement folds into its load -- and that pointer's pseudo lives from
   the first test into the second block, so it takes $v1 while the byte test
   re-lowers its own `lui` into $v0, and sched1 leaves the `lui` last in the
   first block where reorg takes it into the `bnez`'s delay slot. The `i = 0`
   then stays ahead of the test instead of filling that slot. MATCH against
   7 DIFF, and the unit already spells 0x80010000 this way in func_8009DFD8.

   The 16-byte local is a 2x4 s16 table, so the copy from rodata is an
   alignment-2 block move (`lwl`/`lwr`), and `abs()` is one insn whose own
   delay slot is the real `nop` after the `bgez`. Cases 1 and 4 both end in
   the `+/- 0x20` plus the shared countdown test, which cross-jumping merges
   into one tail; case 3's trailing `a->unk_68++` merges into the same tail
   from the other side, which is the `j` into the middle of it. */
void func_800BF1C8(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *r)
{
    s16 box[2][4] = {{0x198, 0x1C0, 0x8, 0x20}, {0x190, 0x1C0, 0x8, 0x20}};
    s32 i;
    s32 d;
    u8 *q;

    a->unk_6C--;
    switch (a->unk_68)
    {
    case 0:
        i = 0;
        q = (u8 *)0x80010000;
        if (q[0x1F81] != 0 || *(u32 *)(q + 0x2D5C) >= 0x1388)
        {
            i = 1;
        }
        func_800673A0(box[i], 0x188, 0x180);
        /* fallthrough */
    case 3:
        d = ((UnkTown2AE8 *)D_80083780)->unk_00 - b->unk_00;
        d = abs(d);
        if (d <= 0x3FFFFF && func_800352FC() != 0 && func_800C2AB4(a) != 0)
        {
            func_80053DA8(0x50B);
            a->unk_6C = 0x20;
            a->unk_68++;
        }
        break;
    case 1:
        if (((UnkTown2AE8 *)D_80083780)->unk_04 < b->unk_04 + 0x800000)
        {
            ((UnkTown2AE8 *)D_80083780)->unk_04 += 0x40000;
        }
        r->unk_1A -= 0x20;
        if (a->unk_6C <= 0)
        {
            a->unk_68++;
        }
        break;
    case 2:
    case 5:
        if (func_800352FC() == 0 || func_800C2AB4(a) == 0)
        {
            a->unk_68 = (a->unk_68 + 1) % 6;
        }
        break;
    case 4:
        r->unk_1A += 0x20;
        if (a->unk_6C <= 0)
        {
            a->unk_68++;
        }
        break;
    }
}

/* The boundary condition for this carve, and the whole reason the unit
   exists: this unit's .rodata must END at 0x80089900, so the TAIL unit
   begins with func_800BF4CC's jump table at tail-offset 0 and every
   later `.align 3` in it lands 0 mod 8.  Carved out of
   ovl_town_800BF3F0, where it sat below func_800BF3F0.  If it moves back,
   jtbl_80089900 lands at tail-offset 0x14 -- 4 mod 8 -- and gets padded:
   the same bug one symbol further on.  See
   docs/near-misses/func_800BEBA4.c. */
INCLUDE_RODATA("asm/nonmatchings/ovl_town_800BEBA4", D_800898EC);
