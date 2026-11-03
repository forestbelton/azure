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
    u8 pad_6E[0x4];
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
    u8 pad_12[0x24];
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
    u8 pad_08[0x48];
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
    u8 pad_0F[0x1];
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
    u8 pad_54[0x26];
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
} UnkTown2AE8Int;

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
void func_800C5C70();
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
extern u8 D_800D64DC[];
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
void func_800CB21C();
void func_800CB2DC();
void func_800CB3A8();
void func_800CB4B0();
void func_800CB570();
void func_800CB63C();
void func_800CB7EC();
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
void func_8009503C(UnkTown2AE8 *b);
void func_80091528();
void func_80091990();
void func_800478B8(s32);
s32 func_8009CFE0(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r);
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
void func_800970FC();
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
void func_80093250();
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
extern void (*D_800FE5D8)();
void func_800945B8(UnkTownTask *t, UnkTown2AE8 *b, void *c);
void func_800946A0(UnkTownTask *t, UnkTown2AE8 *b, void *c);
s32 func_80095760(UnkTownCFCB4 *p);
void func_8009567C(UnkTownCFCB4 *p);
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
void func_8009DD40();
void func_8009F148(UnkTownSpawn *p, UnkTownSpawn *p2, UnkVec *q, UnkTownRend *r);
void func_800B2244();
extern s32 D_800D0B14;
extern s16 D_80100D18;
extern s32 D_80100D1C;
extern s32 D_80100D60;
extern s16 D_80100D80;
extern s16 D_80100D82;
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
s32 func_8003BD84(s32 x, s32 y);
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
void func_800C2CB0(u8 *a, s32 c, void *s, s32 z);
void func_80033CD8(u8 *p, u32 tag);
extern u8 D_80045340[];
void func_80069EE8(u8 *p, s32 v, s32 n);
void func_800422DC(u8 *arg0, u8 *arg1);
void func_800423C0(u8 *arg0, s32 arg1, u8 *arg2);
u32 func_8003C06C(s32 i);
void func_8003E188(s16 arg0, s16 arg1);
void func_8009F374();
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
void func_800954C4();
void func_800954DC();
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
extern u8 D_800D6988[];
extern u8 D_800D6990[];
extern u8 D_800D69B8[];
extern u8 D_800D69BC[];
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
extern u8 D_800CFCEF;
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
void func_8009221C();
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
u32 func_800491F4(s32, u8 *, s32);

/* Reset one entry. The index is written inline, not through a base local:
   that is what puts the `sll` ahead of the `lui`/`addiu` the way the target
   has it (docs/overlays.md lever 1, read the other way round). */
void func_8008AC84(s32 idx)
{
    Unk80082660 *p = &D_800CF720[idx];

    p->unk_00 = 0;
    p->unk_01 = -1;
    p->unk_02 = 0;
    p->unk_04 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008ACAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008ACE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008AD1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008AD90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008AE4C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008AF2C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008AFB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B0E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B158);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B23C);

void func_8008B2BC(void) {
}

void func_8008B2C4(void) {
}

void func_8008B2CC(void) {
}

void func_8008B2D4(void) {
}

void func_8008B2DC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B2E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B328);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B3AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B408);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B4B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B550);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B55C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B5D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B620);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B654);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B7F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B808);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008B818);

void func_8008B9D8(UnkTownDispatch *d)
{
    d->unk_68();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BA00);

void func_8008BA44(UnkTownMover *m, UnkTown2AE8 *b, void *c)
{
    *(s16 *)((u8 *)m->unk_00 + 0xA) -= 0x100;
    if (*(s16 *)((u8 *)m->unk_00 + 0xA) < 0)
    {
        *(s16 *)((u8 *)m->unk_00 + 0xA) = 0;
        m->unk_68 = func_8008BC58;
        m->unk_64 = (s8)D_800CFC70[m->unk_66];
        m->unk_6C = D_800CFC7C[m->unk_66];
        m->unk_64 = 0x16;
    }
}

void func_8008BACC(UnkTownMover *m, UnkTown2AE8 *b, void *c)
{
    *(s16 *)((u8 *)m->unk_00 + 0xA) -= 0x200;
    if (*(s16 *)((u8 *)m->unk_00 + 0xA) < 0)
    {
        *(s16 *)((u8 *)m->unk_00 + 0xA) = 0;
        m->unk_68 = func_8008BC58;
        m->unk_64 = (s8)D_800CFC70[m->unk_66];
        m->unk_6C = D_800CFC7C[m->unk_66];
    }
}

void func_8008BB50(UnkTownMover *m, UnkTown2AE8 *b, void *c)
{
    *(s16 *)((u8 *)m->unk_00 + 0x8) += 0x200;
    if (*(s16 *)((u8 *)m->unk_00 + 0x8) > 0)
    {
        *(s16 *)((u8 *)m->unk_00 + 0x8) = 0;
        m->unk_68 = func_8008BC58;
        m->unk_64 = (s8)D_800CFC70[m->unk_66];
        m->unk_6C = D_800CFC7C[m->unk_66];
    }
}

void func_8008BBD4(UnkTownMover *m, UnkTown2AE8 *b, void *c)
{
    *(s16 *)((u8 *)m->unk_00 + 0x8) -= 0x200;
    if (*(s16 *)((u8 *)m->unk_00 + 0x8) < 0)
    {
        *(s16 *)((u8 *)m->unk_00 + 0x8) = 0;
        m->unk_68 = func_8008BC58;
        m->unk_64 = (s8)D_800CFC70[m->unk_66];
        m->unk_6C = D_800CFC7C[m->unk_66];
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BC58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BED8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BF0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BF5C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BFB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C004);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C058);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C0AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C0F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C134);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C180);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C1C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C214);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C28C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C2B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C304);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C374);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C3B8);

s32 func_8008C420(UnkTownVec16 *p, UnkTownVec16 *q, s32 i)
{
    UnkVec buf;
    UnkTownVec16 *e;
    s32 r;

    e = (UnkTownVec16 *)(i * 0x10 + (s32)q);
    buf.unk_00 = p->unk_00 + e->unk_00;
    buf.unk_04 = p->unk_04 + e->unk_04;
    buf.unk_08 = p->unk_08 + e->unk_08;
    r = func_8008CF80(&buf);
    if ((s16)r == -0x7FFF)
    {
        return -0x7FFF;
    }
    return (s16)(r - e->unk_08 / 0x10000);
}

s32 func_8008C4C8(UnkTownVec16 *p, UnkTownVec16 *q, s32 i)
{
    UnkVec buf;
    UnkTownVec16 *e;
    s32 r;

    e = (UnkTownVec16 *)(i * 0x10 + (s32)q);
    buf.unk_00 = p->unk_00 + e->unk_00;
    buf.unk_04 = p->unk_04 + e->unk_04;
    buf.unk_08 = p->unk_08 + e->unk_08;
    r = func_8008CFB4(&buf);
    if ((s16)r == 0x7FFF)
    {
        return 0x7FFF;
    }
    return (s16)(r - e->unk_00 / 0x10000);
}

s32 func_8008C570(UnkTownVec16 *p, UnkTownVec16 *q, s32 i)
{
    UnkVec buf;
    UnkTownVec16 *e;
    s32 r;

    e = (UnkTownVec16 *)(i * 0x10 + (s32)q);
    buf.unk_00 = p->unk_00 + e->unk_00;
    buf.unk_04 = p->unk_04 + e->unk_04;
    buf.unk_08 = p->unk_08 + e->unk_08;
    r = func_8008CFE8(&buf);
    if ((s16)r == 0)
    {
        return 0;
    }
    return (s16)(r - e->unk_00 / 0x10000);
}

s32 func_8008C610(UnkTownVec16 *p, UnkTownVec16 *q, s32 i)
{
    UnkVec buf;
    UnkTownVec16 *e;
    s32 r;

    e = (UnkTownVec16 *)(i * 0x10 + (s32)q);
    buf.unk_00 = p->unk_00 + e->unk_00;
    buf.unk_04 = p->unk_04 + e->unk_04;
    buf.unk_08 = p->unk_08 + e->unk_08;
    r = func_8008D01C(&buf);
    if ((s16)r == 0x7FFF)
    {
        return 0x7FFF;
    }
    return (s16)(r - e->unk_04 / 0x10000);
}

s32 func_8008C6B8(UnkTownVec16 *p, UnkTownVec16 *q, s32 i)
{
    UnkVec buf;
    UnkTownVec16 *e;
    s32 r;

    e = (UnkTownVec16 *)(i * 0x10 + (s32)q);
    buf.unk_00 = p->unk_00 + e->unk_00;
    buf.unk_04 = p->unk_04 + e->unk_04;
    buf.unk_08 = p->unk_08 + e->unk_08;
    r = func_8008D050(&buf);
    if ((s16)r == 0)
    {
        return 0;
    }
    return (s16)(r - e->unk_04 / 0x10000);
}

s32 func_8008C758(UnkTownVec16 *p, UnkTownVec16 *q, s32 n, UnkTownPlane **out, s16 best, UnkTownPlane *cur)
{
    UnkVec buf;
    UnkTownPlane *hit;
    s32 i;
    s32 x;
    s32 y;
    s32 m;
    s32 d;
    s32 r;

    *out = cur;
    for (i = 0; i < n; i++)
    {
        if (func_8008C3B8(q, i, cur) < 0)
        {
            continue;
        }
        buf.unk_00 = p->unk_00 + q[i].unk_00;
        buf.unk_04 = p->unk_04 + q[i].unk_04;
        x = q[i].unk_00;
        y = abs(q[i].unk_04);
        m = abs(x);
        if (m < y)
        {
            m = y;
        }
        buf.unk_08 = p->unk_08 - (m << 1);
        func_8008CF48(&buf, &hit);
        if (func_8008C3B8(q, i, hit) < 0)
        {
            d = (hit->unk_00 * ((-q[i].unk_00) / 0x10000) + hit->unk_02 * ((-q[i].unk_04) / 0x10000)) / hit->unk_04;
        }
        else
        {
            d = 0;
        }
        buf.unk_08 = p->unk_08 + q[i].unk_08 + (d << 16);
        r = func_8008CF48(&buf, &hit);
        if ((s16)r != 0x7FFF)
        {
            r = r - (q[i].unk_08 / 0x10000 + d);
        }
        if ((s16)r < best)
        {
            best = r;
            *out = hit;
        }
    }
    return best;
}

s32 func_8008C984(UnkTown2AE8 *v, s32 b, s32 n)
{
    s32 best;
    s32 i;
    s32 r;

    best = -0x7FFF;
    for (i = 0; i < n; i++)
    {
        r = func_8008C420((UnkTownVec16 *)v, (UnkTownVec16 *)b, i);
        if ((s16)r > (s16)best)
        {
            best = r;
        }
    }
    return (s16)best;
}

s32 func_8008CA20(UnkTown2AE8 *v, s32 b, s32 n)
{
    s32 best;
    s32 i;
    s32 r;

    best = 0x7FFF;
    for (i = 0; i < n; i++)
    {
        r = func_8008C4C8((UnkTownVec16 *)v, (UnkTownVec16 *)b, i);
        if ((s16)r < (s16)best)
        {
            best = r;
        }
    }
    return (s16)best;
}

s32 func_8008CABC(UnkTown2AE8 *v, s32 b, s32 n)
{
    s16 best;
    s32 i;
    s32 r;

    best = 0;
    for (i = 0; i < n; i++)
    {
        r = func_8008C570((UnkTownVec16 *)v, (UnkTownVec16 *)b, i);
        if ((s16)r > best)
        {
            best = r;
        }
    }
    return best;
}

s32 func_8008CB58(UnkTown2AE8 *v, s32 b, s32 n)
{
    s32 best;
    s32 i;
    s32 r;

    best = 0x7FFF;
    for (i = 0; i < n; i++)
    {
        r = func_8008C610((UnkTownVec16 *)v, (UnkTownVec16 *)b, i);
        if ((s16)r < (s16)best)
        {
            best = r;
        }
    }
    return (s16)best;
}

s32 func_8008CBF4(UnkTown2AE8 *v, s32 b, s32 n)
{
    s16 best;
    s32 i;
    s32 r;

    best = 0;
    for (i = 0; i < n; i++)
    {
        r = func_8008C6B8((UnkTownVec16 *)v, (UnkTownVec16 *)b, i);
        if ((s16)r > best)
        {
            best = r;
        }
    }
    return best;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CC90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CE08);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CF48);

s32 func_8008CF80(UnkVec *v)
{
    return (s16)func_8008D5C4(*(s16 *)((u8 *)v + 2), *(s16 *)((u8 *)v + 6),
                      *(s16 *)((u8 *)v + 0xA));
}

s32 func_8008CFB4(UnkVec *v)
{
    return (s16)func_8008DA80(*(s16 *)((u8 *)v + 2), *(s16 *)((u8 *)v + 6),
                      *(s16 *)((u8 *)v + 0xA));
}

s32 func_8008CFE8(UnkVec *v)
{
    return (s16)func_8008DFB4(*(s16 *)((u8 *)v + 2), *(s16 *)((u8 *)v + 6),
                      *(s16 *)((u8 *)v + 0xA));
}

s32 func_8008D01C(UnkVec *v)
{
    return (s16)func_8008E4EC(*(s16 *)((u8 *)v + 2), *(s16 *)((u8 *)v + 6),
                      *(s16 *)((u8 *)v + 0xA));
}

s32 func_8008D050(UnkVec *v)
{
    return (s16)func_8008EA20(*(s16 *)((u8 *)v + 2), *(s16 *)((u8 *)v + 6),
                      *(s16 *)((u8 *)v + 0xA));
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008D084);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008D0B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008D5C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008DA80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008DFB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008E4EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008EA20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008EF58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008EFA0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008EFE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F01C);

void func_8008F074(u8 *a)
{
    func_8008F01C(a);
    a[0x14] = 0x1;
}

void func_8008F0A4(u8 *a)
{
    func_8008F01C(a);
    a[0x14] = 0x2;
}

/* Widened from the one-parameter `(u8 *a)` it was landed with: func_8009DFD8
   calls it with three (target words 76-80 are the $a0/$a1/$a2 setup), and it
   has no other caller in the unit. Same shape as its sibling func_8008F104 one
   definition below. The extra two go unread and it re-measures 12 words,
   0 differing. */
void func_8008F0D4(UnkTownSpawn *p, UnkVec *q, void *t)
{
    func_8008F01C(p);
    *((u8 *)p + 0x14) = 0x4;
}

void func_8008F104(UnkTownSpawn *p, UnkVec *q, void *t)
{
    func_8008F01C(p);
    *((u8 *)p + 0x14) = 0x3;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F134);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F170);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F27C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F294);

void func_8008F2D4(UnkTownColl *a, UnkTown2AE8 *b)
{
    s32 lim;

    lim = func_8008F3CC(b) << 16;
    if (b->unk_00 >= lim)
    {
        func_8008F50C(a, b, lim);
    }

    lim = (func_8008F41C(b) << 16) - 1;
    if (b->unk_00 <= lim)
    {
        func_8008F520(a, b, lim);
    }
}

void func_8008F350(UnkTownColl *a, UnkTown2AE8 *b)
{
    s32 lim;

    lim = func_8008F46C(b) << 16;
    if (b->unk_04 >= lim)
    {
        func_8008F534(a, b, lim);
    }

    lim = (func_8008F4BC(b) << 16) - 1;
    if (b->unk_04 <= lim)
    {
        func_8008F548(a, b, lim);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F3CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F41C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F46C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F4BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F50C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F520);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F534);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F548);

void func_8008F55C(UnkTownColl *a, UnkTown2AE8 *b, UnkTownSweep *c)
{
    if (c->unk_28 == 0)
    {
        b->unk_00 -= c->unk_10;
        b->unk_0C = 0;
    }
    a->unk_3A[c->unk_2C]++;
    a->unk_1C[c->unk_2C] = c->unk_04;
}

void func_8008F5B4(UnkTownColl *a, UnkTown2AE8 *b, UnkTownSweep *c)
{
    if (c->unk_29 == 0)
    {
        b->unk_04 -= c->unk_18;
        b->unk_10 = 0;
    }
    a->unk_3A[c->unk_30]++;
    a->unk_1C[c->unk_30] = c->unk_04;
}

void func_8008F60C(UnkTownColl *a, UnkTown2AE8 *b, UnkTownSweep *c)
{
    if (c->unk_2A == 0)
    {
        b->unk_08 -= c->unk_20;
        b->unk_14 = 0;
    }
    a->unk_3A[c->unk_34]++;
    a->unk_1C[c->unk_34] = c->unk_04;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F664);

/* AABB overlap of two nodes, each carrying a position at +0x08 and a box at
   +0x0C (minimum at +0x00/+0x04/+0x08, extent at +0x0C/+0x10/+0x14).  Returns
   NULL when the boxes miss on any axis or when two of the three penetrations
   are zero, and otherwise records the axis of least penetration. */
UnkTownCFE60 *func_8008FAC0(UnkNode *a, UnkNode *b)
{
    UnkVec *pa = a->unk_08;
    UnkVec *ba = (UnkVec *)a->unk_0C;
    UnkVec *pb = b->unk_08;
    UnkVec *bb = (UnkVec *)b->unk_0C;
    s32 ax0;
    s32 ay0;
    s32 az0;
    s32 bx0;
    s32 by0;
    s32 bz0;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 ex;
    s32 ey;
    s32 ez;

    ax0 = pa->unk_00 + ba->unk_00;
    bx0 = pb->unk_00 + bb->unk_00;
    dx = ax0 - (bx0 + bb->unk_0C);
    /* DEVICE: the `do { } while (0)` is a reference weight, not control flow.
       flow.c counts `reg_n_refs[regno] += loop_depth`, so wrapping this one
       test lifts `dx` from 7 references to 8; floor_log2 steps 2 -> 3 and its
       global-alloc priority goes 0.212 -> 0.364, which is the window
       (0.308, 0.375) between pseudo 140 and `ba` that hands `dx` $t0 and
       rotates {ay0,ey}/{pa,az0}/pb/ex onto $t1/$t2/$t3/$t4. Worth 32 words of
       pure register names; without it the function is 136 words / 32
       differing with every opcode, immediate, displacement and branch arm
       already exact. It emits nothing and warns under nothing.
       Legal spellings ruled out, all at 136 words: declaration order, all six
       permutations tried -- 32 (no tie to break, the priorities are distinct);
       `abs(ex) < abs(dx)` for `abs(dx) > abs(ex)` -- 43;
       `dx = (abs(dx) > abs(ex)) ? ex : dx;` -- 56;
       an `else { dx = dx; }` arm -- 32 (gcc drops the self-copy before flow);
       `dx = bx0 + bb->unk_0C; dx = ax0 - dx;` -- 35 (the split puts the
       intermediate in dx's own register);
       `ex = dx + bb->unk_0C + ba->unk_0C;` -- 135 words / 129;
       `if (dz == 0 && dx == 0)` for the middle zero test -- 138 words / 55.
       Free-reference PAIRS reach the same place but overshoot, because a pair
       is +4 references: `dx = dx + ax0; dx = dx - ax0;` puts dx at 0.485 and
       in $a3 -- 19. The wrapper is the only lift measured that is +1.
       Other wrapper placements, for the record: around `dx = ex;` -- 9;
       around the whole x-axis reduction -- 42; around dx's definition --
       137 words; around the `unk_04 = dx` store -- 137 words.
       `__asm__ volatile("" ::"r"(dx));` here or before the zero tests also
       MATCHES; the wrapper is preferred per matching-levers:717. */
    do
    {
        if (dx > 0)
        {
            return NULL;
        }
    } while (0);
    ex = ax0 + ba->unk_0C - bx0;
    if (ex < 0)
    {
        return NULL;
    }

    ay0 = pa->unk_04 + ba->unk_04;
    by0 = pb->unk_04 + bb->unk_04;
    dy = ay0 - (by0 + bb->unk_10);
    if (dy > 0)
    {
        return NULL;
    }
    ey = ay0 + ba->unk_10 - by0;
    if (ey < 0)
    {
        return NULL;
    }

    az0 = pa->unk_08 + ba->unk_08;
    bz0 = pb->unk_08 + bb->unk_08;
    dz = az0 - (bz0 + bb->unk_14);
    if (dz > 0)
    {
        return NULL;
    }
    ez = az0 + ba->unk_14 - bz0;
    if (ez < 0)
    {
        return NULL;
    }

    if (abs(dx) > abs(ex))
    {
        dx = ex;
    }
    if (abs(dy) > abs(ey))
    {
        dy = ey;
    }
    if (abs(dz) > abs(ez))
    {
        dz = ez;
    }

    /* "at least two of the three are zero". jump.c threads the second test's
       `dx == 0`, which is why the listing tests dx once and then shares the
       dy/dz pair between both paths. */
    if (dx == 0 && dy == 0)
    {
        return NULL;
    }
    if (dx == 0 && dz == 0)
    {
        return NULL;
    }
    if (dy == 0 && dz == 0)
    {
        return NULL;
    }

    /* The axis-2 arm is written ONCE, after the chain, not as an `else` in
       each branch: the target reaches it by falling out of the first arm and
       by the else-if's false edge, and reorg steals its `addiu` into the
       delay slot at 8008FCB0. Spelled as two nested if/else pairs the two
       copies cross-jump into the middle of the function instead and the arms
       come out swapped -- 38 differing. */
    if (abs(dx) < abs(dy))
    {
        if (abs(dy) < abs(dz) || abs(dx) < abs(dz))
        {
            D_800CFE60.unk_00 = 0;
            D_800CFE60.unk_04 = dx;
            return &D_800CFE60;
        }
    }
    else if (abs(dy) < abs(dz))
    {
        D_800CFE60.unk_00 = 1;
        D_800CFE60.unk_04 = dy;
        return &D_800CFE60;
    }
    D_800CFE60.unk_00 = 2;
    D_800CFE60.unk_04 = dz;
    return &D_800CFE60;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FCE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FCF4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FD20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FD48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FD9C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FE78);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FE90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FF5C);

void func_8008FFC0(void) {
}

void func_8008FFC8(void)
{
    UnkNode *n;
    UnkTownScene *sc;
    UnkTownRend *r;
    u16 t;
    UnkTown2AE8 *b = (UnkTown2AE8 *)D_80083780;

    D_80100E18 = 0;
    D_80100DB0 = 0;
    func_8003DB4C((int *)b, 6);
    /* assigned HERE and not in the initialiser: the target materialises
       %lo(D_80082E80) at word 16, after the first func_8003DB4C call, and an
       initialiser would put it in the prologue. It has to be a variable
       rather than plain `D_80082E80.unk_NN` references, because the last
       store to it is after the if/else merge, where a fresh cse block would
       re-materialise the address instead of reusing $s3 (3 words). */
    sc = &D_80082E80;
    func_8003DB4C((int *)sc, 0xC);
    sc->unk_28 = D_80080A80;
    func_8008EF58();
    /* &D_800CFCB4, not D_800CFCB4: this unit declares it as the struct
       UnkTownCFCB4, so the bare name passes it BY VALUE. The agent's
       standalone file had it as `extern u8 D_800CFCB4[]`, which decays to a
       pointer, which is why that file measured 146 words and this one measured
       152 until the address was taken. */
    func_8008F01C(&D_800CFCB4, b, D_800D0420);
    func_8008FCE0();
    func_8008FD48(D_800FE490, b, D_800D0420);
    if (TOWN_REC->unk_18 == 0x37000C)
    {
        func_800C01DC();
        return;
    }
    n = func_8003FE78(0, (UnkNode *)&D_80083498, 0x22);
    n->unk_08 = (UnkVec *)b;
    n->unk_0C = (u32)sc;
    if (TOWN_REC->unk_0E != 0)
    {
        n->unk_10 = (u32)func_8008FFC0;
        TOWN_PT->unk_02 = TOWN_REC->unk_14 + TOWN_REC->unk_10->unk_00;
        TOWN_PT->unk_06 = TOWN_REC->unk_16 + TOWN_REC->unk_10->unk_02;
        TOWN_PT->unk_0A = 0;
        func_800A48B0(D_80100D98, b);
        return;
    }
    n->unk_10 = (u32)func_800903FC;
    func_8004491C(n, (u32)func_800970FC);
    sc->unk_1E = 0x1000;
    sc->unk_1C = 0x1000;
    TOWN_PT->unk_02 = TOWN_REC->unk_14 + TOWN_REC->unk_10->unk_00;
    /* this store sits BETWEEN the +0x02 and +0x06 stores: it is the filler
       the scheduler puts in the load-delay slot of `lhu 0x0($a1)`, and no
       other position in this statement group reaches 0 (grid of all 60
       order-preserving interleavings: here 1, next best 8). */
    sc->unk_0C = 0x808080;
    TOWN_PT->unk_06 = TOWN_REC->unk_16 + TOWN_REC->unk_10->unk_02;
    TOWN_PT->unk_0A = -0x80;
    TOWN_PT->unk_0A = func_800C2AE8(b);
    func_8008FF5C();
    func_8009550C(b);
    r = (UnkTownRend *)((u8 *)n + 0x20);
    r->unk_12 = -1;
    /* one u16 temp, not two reads: the store to r->unk_10 in between stops
       cse re-using the load, and a second `lhu 0xC($s1)` is 2 words. */
    t = TOWN_REC->unk_0C;
    r->unk_10 = t;
    r->unk_18 = t;
    func_80033AE8(0xA4);
    if (func_80033B2C(0x1202) == 0)
    {
        r->unk_16 = 0;
    }
    else
    {
        r->unk_16 = 1;
    }
    sc->unk_28 = D_80080A80;
    if (D_8006ADD4 != 0xC)
    {
        if (func_8009F830(2, 0x13) != 0)
        {
            func_800AB084();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090210);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090378);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800903A8);

void func_800903BC(void)
{
    func_8008FFC8();
}

void func_800903DC(void)
{
    func_8008FFC8();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800903FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009065C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800909A0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090A04);

void func_80090A64(void) {
}

void func_80090A6C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090A74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090C68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090D44);

void func_80090DF0(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    void (*f)();

    f = a->unk_00;
    func_80090A74(a, b, c);
    if ((f == a->unk_00) && (b->unk_0C == 0) && (b->unk_10 == 0))
    {
        func_80093D48(a, b, c);
    }
}

void func_80090E78(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    void (*f)();

    f = a->unk_00;
    func_80090A74(a, b, c);
    if ((f == a->unk_00) && (b->unk_0C == 0) && (b->unk_10 == 0))
    {
        func_80093D48(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090F00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090F84);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091000);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091114);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800911D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091260);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091528);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800917EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091990);

void func_80091A08(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    s32 n;

    func_80095C80(b);
    func_80095094(b);
    n = (s16)func_80095978(b, D_800FE488);
    if (n - *(s16 *)((u8 *)b + 0xA) >= 4)
    {
        if (D_800CFCEF == 0)
        {
            func_80094378(a, b, c);
            return;
        }
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            func_80095A94(b, n, D_800FE488);
        }
    }
    if (--a->unk_0A < 0)
    {
        func_800940D0(a, b, c);
    }
}

void func_80091AF0(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    s32 n;

    func_80095C80(b);
    func_80095094(b);
    n = (s16)func_80095978(b, D_800FE488);
    if (n - *(s16 *)((u8 *)b + 0xA) >= 4)
    {
        if (D_800CFCEF == 0)
        {
            func_80094378(a, b, c);
            return;
        }
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            func_80095A94(b, n, D_800FE488);
        }
    }
    if (--a->unk_0A < 0)
    {
        func_80093D48(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091BD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091CE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091D6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091EB4);

void func_80091F64(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    s32 n;

    func_80095C80(b);
    func_80095094(b);
    n = (s16)func_80095978(b, D_800FE488);
    if (n - *(s16 *)((u8 *)b + 0xA) >= 4)
    {
        if (D_800CFCEF == 0)
        {
            func_80094378(a, b, c);
            return;
        }
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            func_80095A94(b, n, D_800FE488);
        }
    }
    if (--a->unk_0A < 0)
    {
        func_800942B0(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009204C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800920F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009221C);

void func_80092318(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092320);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800924EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092698);

void func_800927EC(UnkTownTask *a, UnkTown2AE8 *b, void *c)
{
    Unk80083160 *g;
    s32 n;
    s32 r;

    g = &D_80083160;
    func_80095C80(b);
    func_80094C1C(a);
    func_80095094(b);
    n = (s16)func_80095978(b, D_800FE488);
    if (n - *(s16 *)((u8 *)b + 0xA) >= 4)
    {
        if (D_800CFCEF == 0)
        {
            func_80094660(a, b, c);
            return;
        }
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            func_80095A94(b, n, D_800FE488);
        }
    }
    if (g->unk_10 & 0x10)
    {
        func_800945B8(a, b, c);
        return;
    }
    if (g->unk_10 & 0x80)
    {
        if (D_800CFCC4 != 0 && D_800CFCC4->unk_14 == 4)
        {
            func_8009550C(b);
            /* Three arguments: the dump sets $a0/$a1/$a2 before the jal. The
               unit defines func_80094774 with one parameter, which an
               implicit declaration here tolerates; widening it to three the
               way func_80094660 was widened is equivalent (both measured). */
            func_80094774(a, b, c);
            D_800FE518 = 1;
            return;
        }
        func_80099754(b);
        func_8009550C(b);
        if (func_80094B0C((u8 *)a - 0x20) != 0)
        {
            func_80093D38(a, b, c);
            D_800FE5D8 = func_80093B00;
        }
        return;
    }
    if (g->unk_10 & 0x40)
    {
        func_800946A0(a, b, c);
        return;
    }
    if (func_80095760(&D_800CFCB4) == -1 && func_8009FF50() == 0)
    {
        func_8009567C(&D_800CFCB4);
        func_80094C1C(a);
        func_800988C8(a, b, c);
        if ((*(UnkTownDispatch **)((u8 *)a + 0x2C))->unk_50 != func_8009B828 &&
            (*(UnkTownDispatch **)((u8 *)a + 0x2C))->unk_50 != func_8009B8E8 &&
            (*(UnkTownDispatch **)((u8 *)a + 0x2C))->unk_50 != func_8009B9BC)
        {
            *(s32 *)((u8 *)a + 0x2C) = 0;
        }
        r = func_800A9D74(D_800CFCB4.unk_10, *(s32 *)((u8 *)a + 0x2C));
        if (r != 0)
        {
            func_80035208(r);
        }
        else
        {
            func_8008B158(D_800CFCB4.unk_10);
        }
        D_800FE5D8 = func_80093B00;
        return;
    }
    if (g->unk_08 & 0xF000)
    {
        func_8009455C(a, b, c);
    }
}

void func_80092A84(UnkTownTask *a, UnkTown2AE8 *b, void *c)
{
    Unk80083160 *g;
    s32 n;
    s32 r;

    g = &D_80083160;
    func_80095C80(b);
    func_80095094(b);
    n = (s16)func_80095978(b, D_800FE488);
    if (n - *(s16 *)((u8 *)b + 0xA) >= 4)
    {
        if (D_800CFCEF == 0)
        {
            func_80094660(a, b, c);
            return;
        }
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            func_80095A94(b, n, D_800FE488);
        }
    }
    if (g->unk_10 & 0x10)
    {
        func_800945B8(a, b, c);
        return;
    }
    if (g->unk_10 & 0x40)
    {
        func_800946A0(a, b, c);
        return;
    }
    if (func_80095760(&D_800CFCB4) == -1 && func_8009FF50() == 0)
    {
        func_8009567C(&D_800CFCB4);
        func_80094C1C(a);
        func_80094984(D_800D0128, a, c);
        func_800988C8(a, b, c);
        if ((*(UnkTownDispatch **)((u8 *)a + 0x2C))->unk_50 != func_8009B828 &&
            (*(UnkTownDispatch **)((u8 *)a + 0x2C))->unk_50 != func_8009B8E8 &&
            (*(UnkTownDispatch **)((u8 *)a + 0x2C))->unk_50 != func_8009B9BC)
        {
            *(s32 *)((u8 *)a + 0x2C) = 0;
        }
        r = func_800A9D74(D_800CFCB4.unk_10, *(s32 *)((u8 *)a + 0x2C));
        if (r != 0)
        {
            func_80035208(r);
        }
        else
        {
            func_8008B158(D_800CFCB4.unk_10);
        }
        D_800FE5D8 = func_80093B00;
        return;
    }
    if (g->unk_08 & 0xF000)
    {
        /* Spelled with a local and a separate decrement rather than
           `(s16)--*(u16 *)(...)`: clang-format rewrites `--*` to `-- *`, which
           would make this the only line in the file that format-on-save
           changes. Measured identical. */
        u16 *n = (u16 *)((u8 *)a + 0x3E);

        *n -= 1;
        func_80094944((s16)*n, 0xC);
        func_80094C1C(a);
        func_80094C74(b);
    }
    else
    {
        func_8009451C(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092CD4);

void func_80092DA8(void *p, UnkTown2AE8 *q, void *r)
{
    Unk80083160 *g;
    s32 n;

    g = &D_80083160;
    func_80095C80(q);
    func_80094C1C(p);
    func_80094C74(q);
    if (g->unk_10 & 0x40)
    {
        func_80094714(p, q, r);
    }
    n = func_80095978(q, D_800FE488);
    if (*(s16 *)((u8 *)q + 0xA) >= (s16)n)
    {
        func_80094910();
        func_80095A94(q, (s16)n, D_800FE488);
        func_800ABD74(q);
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            goto idle;
        }
        func_80094910();
        q->unk_14 = 0;
        func_800954F4(q);
    }
    func_8009451C(p, q, r);
    return;

idle:
    func_80095388(q);
}

void func_80092ECC(void *p, UnkTown2AE8 *q, void *r)
{
    Unk80083160 *g;
    s32 n;

    g = &D_80083160;
    func_80095C80(q);
    func_80094C1C(p);
    func_80094C74(q);
    if (g->unk_10 & 0x40)
    {
        func_80094714(p, q, r);
    }
    n = func_80095978(q, D_800FE488);
    if (*(s16 *)((u8 *)q + 0xA) >= (s16)n)
    {
        func_80094910();
        func_80095A94(q, (s16)n, D_800FE488);
        func_800ABD74(q);
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            goto idle2;
        }
        func_80094910();
        q->unk_14 = 0;
        func_800954F4(q);
    }
    func_8009451C(p, q, r);
    return;

idle2:
    func_80095388(q);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092FF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800930E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093250);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093328);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800933FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093458);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093524);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800935BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093638);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009368C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800936D0);

void func_80093704(UnkTownTask *t)
{
    func_80094984(D_800D0078, t);
    t->unk_00 = func_80093638;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093744);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800937F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093864);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093894);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093B00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093BAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093C40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093C58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093C70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093CEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093D18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093D38);

void func_80093D48(UnkTownTask *t)
{
    func_80094984(D_800D0078, t);
    t->unk_00 = func_80090C68;
    t->unk_0A = 0x54;
}

void func_80093D90(UnkTownTask *t)
{
    func_80094984(D_800D0180, t);
    t->unk_00 = func_80090C68;
    t->unk_0A = 0x15;
}

void func_80093DD8(UnkTownTask *t)
{
    func_80094984(D_800D0080, t);
    t->unk_00 = func_80090D44;
}

void func_80093E18(UnkTownTask *t)
{
    func_80094984(D_800D0088, t);
    t->unk_00 = func_80090DF0;
}

void func_80093E58(UnkTownTask *t)
{
    func_80094984(D_800D0088, t);
    t->unk_00 = func_80091114;
}

void func_80093E98(UnkTownTask *t)
{
    func_80094984(D_800D0168, t);
    t->unk_00 = func_800911D4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093ED8);

void func_80093F48(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    *(s16 *)((u8 *)t + 0x3E) = 6;
    func_80094C1C(t);
    func_8009503C(b);
    func_80094984(D_800D0098, t, c);
    t->unk_00 = func_80091528;
    t->unk_0A = func_800374F4(2) + 0xC;
}

void func_80093FC8(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    *(s16 *)((u8 *)t + 0x3E) = 6;
    func_80094C1C(t);
    func_8009503C(b);
    func_80094984(D_800D0150, t, c);
    t->unk_00 = func_80091990;
    t->unk_0A = func_800374F4(2) + 0xC;
}

void func_80094048(UnkTownTask *t)
{
    func_80094984(D_800D0160, t);
    t->unk_00 = func_80090F84;
}

void func_80094088(UnkTownTask *t)
{
    func_80094984(D_800D00D0, t);
    t->unk_00 = func_80091A08;
    t->unk_0A = 0x6;
}

void func_800940D0(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    func_80053DA8(0x510);
    func_80094984(D_800D00E8, t, c);
    t->unk_00 = func_80091AF0;
    t->unk_0A = 0xC;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094130);

void func_80094190(UnkTownTask *t)
{
    func_80094984(D_800D00D8, t);
    t->unk_00 = func_80091CE0;
    t->unk_0A = 0x1;
}

void func_800941D8(UnkTownTask *t)
{
    func_80094984(D_800D00E0, t);
    t->unk_00 = func_80091D6C;
    t->unk_0A = 0x15;
}

void func_80094220(UnkTownTask *t)
{
    func_80094984(D_800D0100, t);
    t->unk_00 = func_80091EB4;
    t->unk_0A = 0x4;
}

void func_80094268(UnkTownTask *t)
{
    func_80094984(D_800D00A0, t);
    t->unk_00 = func_80091F64;
    t->unk_0A = 0x3;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800942B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009431C);

void func_80094330(UnkTownTask *t)
{
    func_80094984(D_800D00C0, t);
    t->unk_00 = func_80090F00;
    t->unk_0A = 0x6;
}

/* Widened from the one-parameter `(UnkTownTask *t)` it was landed with. Four
   callers pass THREE arguments -- the three at lines ~2164/2192/2228 and
   func_800AA5F8 -- and those three only compiled because they PRECEDE this
   definition and so saw an implicit declaration; func_800AA5F8 follows it and
   would not compile at all. Widening makes all four legal and explicit. The
   extra two go unread; measured 10 words, 0 differing, and all three existing
   callers unmoved. */
void func_80094378(UnkTownTask *t, UnkTown2AE8 *b, s32 c)
{
    func_80094984(D_800D00B8, t);
    t->unk_00 = func_8009221C;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800943B8);

void func_80094414(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    func_80053DA8(0x50B);
    func_80094C1C(t);
    func_80094984(D_800D0110, t, c);
    t->unk_00 = func_800924EC;
}

void func_80094474(UnkTownTask *t)
{
    func_80094984(D_800D0118, t);
    t->unk_00 = func_80090E78;
    t->unk_0A = 0x6;
}

void func_800944BC(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    func_80053DA8(0x511);
    func_80094984(D_800D0120, t, c);
    t->unk_00 = func_80092698;
    t->unk_0A = 0x6;
}

/* Three parameters, not the one its own body uses: the 73-word pair below
   calls it with (actor, mover, c) like every other handler here, and a
   one-parameter prototype will not compile against that. The extra two are
   unread, which costs nothing -- they arrive in registers and stay there. */
void func_8009451C(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    func_80094984(D_800D0128, t);
    t->unk_00 = func_800927EC;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009455C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800945B8);

void func_80094620(UnkTownTask *t)
{
    func_80094984(D_800D0148, t);
    t->unk_00 = func_80092DA8;
}

/* Called with THREE arguments by func_80092A84 (dump words 31-34 set
   $a0/$a1/$a2 before the jal) and with one by nothing else in the unit, so
   widened here rather than left at one. The extra two go unread; measured 16
   words, 0 differing both before and after. */
void func_80094660(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    func_80094984(D_800D00B8, t);
    t->unk_00 = func_80092ECC;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800946A0);

void func_800946FC(UnkTownTask *t)
{
    t->unk_00 = func_800930E4;
    t->unk_0A = 0x13;
}

void func_80094714(void *p, UnkTown2AE8 *q, void *r)
{
    func_80053DA8(0x512);
    func_80094C1C(p);
    func_80094984(D_800D00B8, p, r);
    ((UnkTownTask *)p)->unk_00 = func_80093250;
}

/* Widened from one parameter: func_800927EC calls it with three. That call
   would have compiled anyway, because this definition FOLLOWS it and there is
   no forward prototype, so the call site sees an implicit declaration -- the
   same accident func_80094660 and func_80094378 were relying on. Widening it
   makes the call explicit. The extra two go unread; measured 16 words,
   0 differing. */
void func_80094774(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    func_80094984(D_800D0120, t);
    t->unk_00 = func_80093328;
    t->unk_0A = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800947BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800947CC);

void func_80094834(UnkTownTask *t, UnkTown2AE8 *b, void *c)
{
    func_80053DA8(0x500);
    func_80094984(D_800D0188, t, c);
    t->unk_00 = func_80093524;
    t->unk_0A = 0x1E;
}

void func_80094894(UnkTownTask *t)
{
    func_80094984(D_800D00A0, t);
    t->unk_00 = func_800935BC;
    t->unk_0A = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800948DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094910);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094944);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094984);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800949C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094A38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094A60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094AA0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094AF8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094B0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094B58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094BC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094C1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094C74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094DA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094F58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009503C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095094);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800951B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095360);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095388);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009539C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800953D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095460);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800954C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800954DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800954F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009550C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095544);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009567C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095760);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095840);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095910);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009593C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095978);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095A94);

s16 func_80095BC0(UnkTown2AE8 *v, s32 i)
{
    return (s16)func_8008C4C8((UnkTownVec16 *)v, (UnkTownVec16 *)D_800D0410, i);
}

s16 func_80095BF0(UnkTown2AE8 *v, s32 i)
{
    return (s16)func_8008C570((UnkTownVec16 *)v, (UnkTownVec16 *)D_800D0414, i);
}

s16 func_80095C20(UnkTown2AE8 *v, s32 i)
{
    return (s16)func_8008C610((UnkTownVec16 *)v, (UnkTownVec16 *)D_800D0418, i);
}

s16 func_80095C50(UnkTown2AE8 *v, s32 i)
{
    return (s16)func_8008C6B8((UnkTownVec16 *)v, (UnkTownVec16 *)D_800D041C, i);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095C80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800961A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800961D8);

void func_80096208(UnkTown2AE8 *b)
{
    s32 d;
    s32 hi;
    s32 lo;

    d = D_800FE5CC;
    if (d > 0)
    {
        hi = func_80096368(b) << 16;
        if (b->unk_00 >= hi)
        {
            func_800966C8(b, hi);
        }
    }
    else if (d < 0)
    {
        lo = (func_80096440(b) << 16) - 1;
        if (b->unk_00 <= lo)
        {
            func_800966F8(b, lo);
        }
    }
    else
    {
        hi = func_80096368(b) << 16;
        if (b->unk_00 >= hi)
        {
            func_800966C8(b, hi);
        }
        lo = (func_80096440(b) << 16) - 1;
        if (b->unk_00 <= lo)
        {
            func_800966F8(b, lo);
        }
    }
}

void func_800962B8(UnkTown2AE8 *b)
{
    s32 d;
    s32 hi;
    s32 lo;

    d = D_800FE5D0;
    if (d > 0)
    {
        hi = func_80096518(b) << 16;
        if (b->unk_04 >= hi)
        {
            func_80096728(b, hi);
        }
    }
    else if (d < 0)
    {
        lo = (func_800965F0(b) << 16) - 1;
        if (b->unk_04 <= lo)
        {
            func_80096758(b, lo);
        }
    }
    else
    {
        hi = func_80096518(b) << 16;
        if (b->unk_04 >= hi)
        {
            func_80096728(b, hi);
        }
        lo = (func_800965F0(b) << 16) - 1;
        if (b->unk_04 <= lo)
        {
            func_80096758(b, lo);
        }
    }
}

s16 func_80096368(UnkTown2AE8 *b)
{
    UnkTown2AE8 v;
    s32 r1;
    s32 r2;
    s32 d;

    v.unk_00 = b->unk_00;
    v.unk_04 = b->unk_04;
    v.unk_08 = b->unk_08;
    r1 = func_8008CA20(&v, D_800D0410, 6);
    d = D_800FE5CC;
    if ((d < 0 ? -d : d) <= 0x140000)
    {
        return (s16)r1;
    }
    v.unk_00 = b->unk_00 - d / 2;
    r2 = func_8008CA20(&v, D_800D0410, 6);
    if ((s16)r2 < (s16)r1)
    {
        return (s16)r2;
    }
    return (s16)r1;
}

s16 func_80096440(UnkTown2AE8 *b)
{
    UnkTown2AE8 v;
    s32 r1;
    s32 r2;
    s32 d;

    v.unk_00 = b->unk_00;
    v.unk_04 = b->unk_04;
    v.unk_08 = b->unk_08;
    r1 = func_8008CABC(&v, D_800D0414, 6);
    d = D_800FE5CC;
    if ((d < 0 ? -d : d) <= 0x140000)
    {
        return (s16)r1;
    }
    v.unk_00 = b->unk_00 - d / 2;
    r2 = func_8008CABC(&v, D_800D0414, 6);
    if ((s16)r1 < (s16)r2)
    {
        return (s16)r2;
    }
    return (s16)r1;
}

s16 func_80096518(UnkTown2AE8 *b)
{
    UnkTown2AE8 v;
    s32 r1;
    s32 r2;
    s32 d;

    v.unk_00 = b->unk_00;
    v.unk_04 = b->unk_04;
    v.unk_08 = b->unk_08;
    r1 = func_8008CB58(&v, D_800D0418, 6);
    d = D_800FE5D0;
    if ((d < 0 ? -d : d) <= 0x140000)
    {
        return (s16)r1;
    }
    v.unk_04 = b->unk_04 - d / 2;
    r2 = func_8008CB58(&v, D_800D0418, 6);
    if ((s16)r2 < (s16)r1)
    {
        return (s16)r2;
    }
    return (s16)r1;
}

s16 func_800965F0(UnkTown2AE8 *b)
{
    UnkTown2AE8 v;
    s32 r1;
    s32 r2;
    s32 d;

    v.unk_00 = b->unk_00;
    v.unk_04 = b->unk_04;
    v.unk_08 = b->unk_08;
    r1 = func_8008CBF4(&v, D_800D041C, 6);
    d = D_800FE5D0;
    if ((d < 0 ? -d : d) <= 0x140000)
    {
        return (s16)r1;
    }
    v.unk_04 = b->unk_04 - d / 2;
    r2 = func_8008CBF4(&v, D_800D041C, 6);
    if ((s16)r1 < (s16)r2)
    {
        return (s16)r2;
    }
    return (s16)r1;
}

void func_800966C8(UnkTown2AE8 *a, s32 b)
{
    D_800CFCEB = 1;
    a->unk_00 = b;
    a->unk_0C = 0;
    func_800954C4(a);
}

void func_800966F8(UnkTown2AE8 *a, s32 b)
{
    D_800CFCEA = 1;
    a->unk_00 = b;
    a->unk_0C = 0;
    func_800954C4(a);
}

void func_80096728(UnkTown2AE8 *a, s32 b)
{
    D_800CFCED = 1;
    a->unk_04 = b;
    a->unk_10 = 0;
    func_800954DC(a);
}

void func_80096758(UnkTown2AE8 *a, s32 b)
{
    D_800CFCEC = 1;
    a->unk_04 = b;
    a->unk_10 = 0;
    func_800954DC(a);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096788);

void func_800967E0(UnkTown2AE8 *a, s32 b)
{
    D_800CFCE8 = 1;
    a->unk_08 = b;
    a->unk_14 = 0;
    func_800954F4(a);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096810);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096868);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096918);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096924);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096A30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096A5C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096A90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096B50);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096D58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096DC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096E08);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096E9C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096FF4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009706C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800970AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800970FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097844);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009792C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097AD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097C50);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097D08);

void func_80097D2C(UnkTownDispatch *d)
{
    d->unk_04();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097D54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097DE8);

void func_80097EA0(void)
{
    func_80097DE8();
}

void func_80097EC0(UnkTownTask *t)
{
    if (--t->unk_0A <= 0)
    {
        func_80098ABC();
    }
}

void func_80097EFC(void *p, UnkTown2AE8 *q, void *r)
{
    s32 n;

    func_80095C80(q);
    n = func_80095978(q, D_800FE488);
    if (*(s16 *)((u8 *)q + 0xA) >= (s16)n)
    {
        func_80095A94(q, (s16)n, D_800FE488);
        func_80098B30(p, q, r);
    }
    else
    {
        func_80095388(q);
    }
}

void func_80097F9C(void *p, UnkTown2AE8 *q, void *r)
{
    s32 n;

    func_80095C80(q);
    n = func_80095978(q, D_800FE488);
    if (*(s16 *)((u8 *)q + 0xA) >= (s16)n)
    {
        func_80095A94(q, (s16)n, D_800FE488);
        func_80098B70(p, q, r);
    }
    else
    {
        func_80095388(q);
    }
}

void func_8009803C(UnkTownTask *t, UnkTown2AE8 *b, s32 c)
{
    if (--t->unk_0A <= 0)
    {
        func_80098928(t, b, c);
    }
}

void func_80098078(UnkTownTask *t)
{
    if (--t->unk_0A <= 0)
    {
        func_80098988();
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800980B4);

void func_80098104(UnkTownTask *t)
{
    if (--t->unk_0A <= 0)
    {
        func_80098C40();
    }
}

void func_80098140(void)
{
    func_80098988();
}

void func_80098160(void)
{
    func_80098988();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098180);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800981D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098244);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800982EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800983BC);

void func_800984AC(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    s32 n;

    n = func_80095978(b, D_800FE488);
    if (*(s16 *)((u8 *)b + 0xA) >= (s16)n)
    {
        func_80095A94(b, (s16)n, D_800FE488);
        func_80098928(a, b, c);
    }
    else
    {
        func_80095388(b);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098544);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098608);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098690);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098764);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098834);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098868);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800988C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098928);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098988);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800989C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098A30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098A98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098ABC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098B30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098B70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098B9C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098C04);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098C40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098C9C);

void func_80098CC0(UnkTownTask *t)
{
    t->unk_04 = func_80098140;
    t->unk_10 = 0x800;
}

void func_80098CD8(UnkTownTask *t)
{
    t->unk_04 = func_80098160;
    t->unk_10 = 0x400;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098CF0);

void func_80098D60(UnkTownTask *t, s32 b, s32 c)
{
    func_80099754(b);
    func_80094984(D_800D0090, t, c);
    t->unk_04 = func_800981D4;
    t->unk_10 = 0x800;
}

void func_80098DC0(UnkTownTask *t, s32 b, s32 c)
{
    func_80099754(b);
    func_80094984(D_800D0090, t, c);
    t->unk_04 = func_80098244;
    t->unk_10 = 0x400;
}

void func_80098E20(UnkTownTask *t)
{
    func_80094984(D_800D00A0, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_80098078;
    t->unk_0A = 0x8;
}

void func_80098E70(UnkTownTask *t)
{
    func_80094984(D_800D0170, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_80098078;
    t->unk_0A = 0x15;
}

void func_80098EC0(UnkTownTask *t)
{
    func_80094984(D_800D0180, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_8009803C;
    t->unk_0A = 0x15;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098F10);

void func_80098F48(UnkTownTask *t, s32 b)
{
    Unk80082660 *p;

    func_80094984(D_800D00A8, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_0A = 0xA;
    func_80099754(b);
    t->unk_04 = func_8009AA2C;
}

void func_80098FBC(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D01A0, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_8009803C;
    t->unk_0A = 0x15;
    func_80099754(b);
}

void func_80099020(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D01E8, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_8009803C;
    t->unk_0A = 0x7;
    func_80099754(b);
}

void func_80099084(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D01F0, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_8009803C;
    t->unk_0A = 0x10;
    func_80099754(b);
}

void func_800990E8(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D01A8, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_80098078;
    t->unk_0A = 0x3C;
    func_80099754(b);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009914C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800991C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099238);

void func_80099294(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D0150, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_80098078;
    t->unk_0A = 0x4;
    func_80099754(b);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800992F8);

void func_80099390(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D0150, t);
    D_80082660[1].unk_00 = 0;
    t->unk_04 = func_8009803C;
    t->unk_0A = 0x4;
    func_80099754(b);
}

void func_800993F4(UnkTownTask *t, s32 b)
{
    Unk80082660 *p;

    func_80094984(D_800D00A8, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_0A = 0xA;
    func_80099754(b);
    t->unk_04 = func_80098608;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099468);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800994DC);

void func_8009953C(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D01C8, t);
    D_80082660[1].unk_00 = 0;
    D_80082660[1].unk_01 = 0;
    t->unk_04 = func_80098078;
    t->unk_0A = 0x4;
    func_80099754(b);
}

void func_800995A8(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D01D0, t);
    D_80082660[1].unk_00 = 0;
    D_80082660[1].unk_01 = 0;
    t->unk_04 = func_80098078;
    t->unk_0A = 0x4;
    func_80099754(b);
}

void func_80099614(UnkTownTask *t, s32 b)
{
    func_80094984(D_800D01D8, t);
    D_80082660[1].unk_00 = 0;
    D_80082660[1].unk_01 = 0;
    t->unk_04 = func_80098078;
    t->unk_0A = 0x15;
    func_80099754(b);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099680);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800996E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099754);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099764);

void func_80099790(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
    if (--a->unk_0A <= 0)
    {
        *(s16 *)((u8 *)b + 0x2) = *(s16 *)((u8 *)a + 0x30);
        *(s16 *)((u8 *)b + 0x6) = *(s16 *)((u8 *)a + 0x32);
        a->unk_10 = *(s16 *)((u8 *)a + 0xE);
        func_80098928(a, b, c);
    }
    else
    {
        *(s16 *)((u8 *)b + 0x2) += (*(s16 *)((u8 *)a + 0x30) - *(s16 *)((u8 *)b + 0x2)) / a->unk_0A;
        *(s16 *)((u8 *)b + 0x6) += (*(s16 *)((u8 *)a + 0x32) - *(s16 *)((u8 *)b + 0x6)) / a->unk_0A;
    }
}

void func_80099874(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    func_80098928(a, b, c);
}

void func_80099894(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
    if (--a->unk_0A < 0)
    {
        func_80098928(a, b, c);
    }
    else
    {
        b->unk_04 += -0x20000;
    }
}

void func_80099920(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
    if (--a->unk_0A < 0)
    {
        func_80098928(a, b, c);
    }
    else
    {
        b->unk_04 += 0x20000;
    }
}

void func_800999AC(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
    if (--a->unk_0A < 0)
    {
        func_80098928(a, b, c);
    }
    else
    {
        b->unk_00 += -0x20000;
    }
}

void func_80099A38(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
    if (--a->unk_0A < 0)
    {
        func_80098928(a, b, c);
    }
    else
    {
        b->unk_00 += 0x20000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099AC4);

void func_80099B00(void *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_8009A1A4(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099B70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099C18);

void func_80099CE0(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_08, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099790;
    t->unk_0A = 0x20;
}

void func_80099D44(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_0C, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_8009803C;
    t->unk_0A = 0x15;
}

void func_80099DA8(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_10, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_8009803C;
    t->unk_0A = 0x15;
}

void func_80099E0C(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_14, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099874;
    t->unk_10 = 0x800;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099E70);

void func_80099ED0(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_14, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099874;
    t->unk_10 = 0xC00;
}

void func_80099F34(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_14, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099874;
    t->unk_10 = 0x400;
}

void func_80099F98(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_18, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099894;
    t->unk_0A = 0x20;
}

void func_80099FFC(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_18, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099920;
    t->unk_0A = 0x20;
}

void func_8009A060(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_18, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_800999AC;
    t->unk_0A = 0x20;
}

void func_8009A0C4(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_18, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099A38;
    t->unk_0A = 0x20;
}

void func_8009A128(UnkTownTask *t)
{
    Unk80082660 *p;

    func_80094984(t->unk_44->unk_1C, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_04 = func_80099AC4;
    t->unk_0A = 0x20;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A18C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A1A4);

void func_8009A1E8(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    Unk80082660 *e;

    D_80082660[a->unk_40].unk_00 = 0;
    a->unk_04 = func_80099C18;
    a->unk_0A = func_8003BD84((a->unk_36 << 16) - b->unk_00,
                              (a->unk_38 << 16) - b->unk_04) / 0x20000;
    e = &D_80082660[a->unk_40];
    if (*(s8 *)((u8 *)e + 2) == -0x80)
    {
        b->unk_00 = a->unk_36 << 16;
        b->unk_04 = a->unk_38 << 16;
        *(s16 *)((u8 *)b + 0xA) += -0x80;
        *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
        func_80098928(a, b, c);
    }
    else if (*(s8 *)((u8 *)e + 2) > 0)
    {
        a->unk_0A = (a->unk_0A << 4) / (*(s8 *)((u8 *)e + 2) + 0x10);
    }
    else if (*(s8 *)((u8 *)e + 2) < 0)
    {
        a->unk_0A = a->unk_0A * (abs(*(s8 *)((u8 *)e + 2)) + 0x10) / 16;
    }
}

void func_8009A344(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    func_80094984(a->unk_44->unk_24, a, c);
    func_8009A1E8(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A3A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A41C);

void func_8009A494(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    Unk80082660 *p;

    func_80033D08((u8 *)a);
    p = D_80082660;
    p[a->unk_40].unk_00 = 0;
    func_80098928(a, b, c);
}

void func_8009A4FC(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    Unk80082660 *p;

    func_80033CD8((u8 *)a, (u32)func_800970FC);
    p = D_80082660;
    p[a->unk_40].unk_00 = 0;
    func_80098928(a, b, c);
}

void func_8009A56C(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    func_80098928(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A58C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A5C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A5F4);

void func_8009A654(void)
{
    func_800C4D20(0x1);
}

void func_8009A674(s32 a, s32 b, s32 c)
{
    UnkTownTask *t = &D_80083498.unk_20;
    UnkTown2AE8 *p = D_80083498.unk_08;
    void *q = D_80083498.unk_0C;

    func_80099754(p);
    func_80094984(D_800D0090, t, q);
    t->unk_00 = func_80097D2C;
    t->unk_04 = func_8009A724;
    t->unk_10 = a & 0xFFF;
    t->unk_36 = b;
    t->unk_38 = c;
    t->unk_0A = 8;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A724);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A81C);

void func_8009A8A8(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A8B0);

void func_8009A8EC(s32 a, s32 b, s32 c)
{
    UnkTownTask *t = &D_80083498.unk_20;
    UnkTown2AE8 *p = D_80083498.unk_08;
    void *q = D_80083498.unk_0C;

    func_80099754(p);
    func_80094984(D_800D0090, t, q);
    t->unk_00 = func_80097D2C;
    t->unk_04 = func_8009A81C;
    t->unk_10 = a & 0xFFF;
    t->unk_36 = b;
    t->unk_38 = c;
    t->unk_0A = 0x10;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A99C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AA2C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AAB8);

void func_8009AB74(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    s32 n;

    n = func_80095978(b, D_800FE488);
    if (*(s16 *)((u8 *)b + 0xA) >= (s16)n)
    {
        func_80095A94(b, (s16)n, D_800FE488);
        func_80098928(a, b, c);
    }
    else
    {
        func_80095388(b);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AC0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AC8C);

void func_8009ACFC(UnkTownTask *t, s32 b)
{
    Unk80082660 *p;

    func_80094984(D_800D0198, t);
    p = D_80082660;
    p[t->unk_40].unk_00 = 0;
    t->unk_0A = 0x14;
    func_80099754(b);
    t->unk_04 = func_8009AD70;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AD70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AE88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AF74);

void func_8009AFF4(void)
{
    func_8009AE88();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B014);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B0EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B120);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B148);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B218);

void func_8009B2BC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B2C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B454);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B594);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B6B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B828);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B8E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009B9BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009BC44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009BDC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009BFC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009BFD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C0C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C0D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C120);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C148);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C1B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C20C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C2C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C340);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C390);

void func_8009C46C(UnkTownDispatch *d)
{
    d->unk_50();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C494);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C4E8);

void func_8009C5D8(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    s32 t;

    t = r->unk_0E - 6;
    if (t < 0)
    {
        t = 0;
    }
    r->unk_0E = t;
    r->unk_0D = t;
    r->unk_0C = t;
    if (t == 0)
    {
        func_80033D08((u8 *)p);
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C648);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C7A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C83C);

void func_8009C884(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    r->unk_08 = (u32)D_800F8C44;
    *(u8 **)((u8 *)p + 0x80) = D_800D0704;
    func_8008F01C(p, q, D_800CFD00);
    func_8009B218(p, q, r, D_800D06AC);
}

void func_8009C8FC(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    r->unk_08 = (u32)D_800F8C80;
    *(u8 **)((u8 *)p + 0x7C) = D_800D0710;
    func_8008F01C(p, q, D_800CFD00);
    func_8009B218(p, q, r, D_800D0690);
}

void func_8009C974(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    r->unk_08 = (u32)D_800F8CEC;
    func_8008F104(p, q, D_800CFD00);
    func_8009B218(p, q, r, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C9DC);

void func_8009CA24(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    r->unk_08 = (u32)D_800F8D64;
    func_8008F104(p, q, D_800D0648);
    func_8009B218(p, q, r, 0);
}

void func_8009CA8C(void) {
}

void func_8009CA94(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    r->unk_08 = (u32)D_800F8D70;
    func_8008F104(p, q, D_800D0648);
    func_8009B218(p, q, r, 0);
}

void func_8009CAFC(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    r->unk_08 = (u32)D_800F8A40;
    func_8008F104(p, q, D_800D0660);
    func_8009B218(p, q, r, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CB64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CB90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CBB4);

void func_8009CBD8(void *a, void *b, u8 *c)
{
    func_8009CB64(a, b, c, D_800F8B6C);
    *(s16 *)(c + 0x10) = 0;
    *(u16 *)(c + 0x14) |= 0x1C;
}

void func_8009CC18(void *a, void *b, u8 *c)
{
    func_8009CB64(a, b, c, D_800F8FA8);
    *(s16 *)(c + 0x10) = 0;
    *(u16 *)(c + 0x14) |= 0x1C;
}

void func_8009CC58(void *a, void *b, u8 *c)
{
    func_8009CB64(a, b, c, D_800F8FC0);
    *(s16 *)(c + 0x10) = 0;
    *(u16 *)(c + 0x14) |= 0x1C;
}

void func_8009CC98(void *a, void *b, u8 *c)
{
    func_8009CB64(a, b, c, D_800F8FD8);
    *(s16 *)(c + 0x10) = 0;
    *(u16 *)(c + 0x14) |= 0x1C;
}

void func_8009CCD8(void *a, void *b, u8 *c)
{
    func_8009CB64(a, b, c, D_800F8FF0);
    *(s16 *)(c + 0x10) = 0;
    *(u16 *)(c + 0x14) |= 0x1C;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CD18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CDB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CDCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CE34);

void func_8009CE80(void)
{
    func_8009CEE8();
    func_800A496C();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CEA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CEE8);

s32 func_8009CFE0(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    u8 *s;

    s = (u8 *)p->unk_98;
    if (s != NULL)
    {
        if ((s[1] & 1) == 0)
        {
            if (func_80033B2C(*(s16 *)(s + 2)) == 0)
            {
                return 1;
            }
        }
        else
        {
            if (func_80033B2C(*(s16 *)(s + 2)) == 1)
            {
                return 1;
            }
        }
        if (s[1] & 0x10)
        {
            return 0;
        }
        if (D_8006ADD4 == 0xC)
        {
            return 0;
        }
    }
    if (func_8008CC90(*(u16 *)(D_80082D08 + 0x0) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0x2) - *(u16 *)((u8 *)q + 6),
                      *(u16 *)(D_80082D08 + 0x8) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0xA) - *(u16 *)((u8 *)q + 6),
                      *(u16 *)(D_80082D08 + 0x10) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0x12) - *(u16 *)((u8 *)q + 6),
                      *(u16 *)(D_80082D08 + 0x18) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0x1A) - *(u16 *)((u8 *)q + 6)) == 0)
    {
        if (func_8008CC90(*(u16 *)(D_80082D08 + 0x0) - p->unk_84, *(u16 *)(D_80082D08 + 0x2) - p->unk_86,
                          *(u16 *)(D_80082D08 + 0x8) - p->unk_84, *(u16 *)(D_80082D08 + 0xA) - p->unk_86,
                          *(u16 *)(D_80082D08 + 0x10) - p->unk_84, *(u16 *)(D_80082D08 + 0x12) - p->unk_86,
                          *(u16 *)(D_80082D08 + 0x18) - p->unk_84, *(u16 *)(D_80082D08 + 0x1A) - p->unk_86) == 0)
        {
            return 2;
        }
    }
    return 0;
}

/* func_8009CFE0 above, minus its leading `D_8006ADD4 == 0xC` guard: same two
   func_8008CC90 probes against the same four D_80082D08 coordinate pairs, same
   two return values, 139 - 5 = 134 words.

   Arity is 2, not 3, even though the sibling takes three. The witness is the
   unlanded caller func_8009E714, which calls this twice back to back
   (0x8009E730, 0x8009E744): it saves its own $a2 into $s2 in the first call's
   delay slot precisely because $a2 is call-clobbered, then sets up only
   $a0/$a1 before the second jal. A third parameter would force an
   `addu $a2, $s2, $zero` there that the target does not contain. The sibling's
   three params are untestable -- both its landed callers forward a0/a1/a2 in
   place -- so they are not evidence against this. */
s32 func_8009D20C(UnkTownSpawn *p, UnkVec *q)
{
    u8 *s;

    s = (u8 *)p->unk_98;
    if (s != NULL)
    {
        if ((s[1] & 1) == 0)
        {
            if (func_80033B2C(*(s16 *)(s + 2)) == 0)
            {
                return 1;
            }
        }
        else
        {
            if (func_80033B2C(*(s16 *)(s + 2)) == 1)
            {
                return 1;
            }
        }
        if (s[1] & 0x10)
        {
            return 0;
        }
    }
    if (func_8008CC90(*(u16 *)(D_80082D08 + 0x0) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0x2) - *(u16 *)((u8 *)q + 6),
                      *(u16 *)(D_80082D08 + 0x8) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0xA) - *(u16 *)((u8 *)q + 6),
                      *(u16 *)(D_80082D08 + 0x10) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0x12) - *(u16 *)((u8 *)q + 6),
                      *(u16 *)(D_80082D08 + 0x18) - *(u16 *)((u8 *)q + 2), *(u16 *)(D_80082D08 + 0x1A) - *(u16 *)((u8 *)q + 6)) == 0)
    {
        if (func_8008CC90(*(u16 *)(D_80082D08 + 0x0) - p->unk_84, *(u16 *)(D_80082D08 + 0x2) - p->unk_86,
                          *(u16 *)(D_80082D08 + 0x8) - p->unk_84, *(u16 *)(D_80082D08 + 0xA) - p->unk_86,
                          *(u16 *)(D_80082D08 + 0x10) - p->unk_84, *(u16 *)(D_80082D08 + 0x12) - p->unk_86,
                          *(u16 *)(D_80082D08 + 0x18) - p->unk_84, *(u16 *)(D_80082D08 + 0x1A) - p->unk_86) == 0)
        {
            return 2;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D424);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D5C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D73C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D7D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D808);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D940);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D9E4);

void func_8009DA50(UnkTownScrRec *p, u8 *b, s32 a2, s32 a3)
{
    /* THE LOCAL COPIES ARE LOAD-BEARING, and only a3's is: they are worth the
       last 6 words (4 register-allocation words plus the two addu operand
       orders).  ox and oz tie exactly in global.c's priority
       (floor_log2(n_refs)*n_refs*size/live_length -- both 3 refs, both live
       all 214 insns, so both score 140), and allocno_compare breaks the tie by
       allocno number, which for parameters is declaration order: a2 always got
       $s5 and a3 $s6, where the target has them the other way round.  Copying
       a3 into a local moves its birth out of the prologue, shortens its live
       range, and flips the tie -- and the copy costs no word, because it
       replaces the prologue's own parameter move.  Measured: with both
       parameters used directly, 143 words / 6 differing; copying a3 only, or
       both (either as a statement or as an initialiser), 143 / 0. */
    s32 ox = a2;
    s32 oz = a3;
    /* tx/tz are the int sums; x/z are the s16 values func_8009D424 is passed.
       Both pairs are needed.  The int sum is what the subtractions use, and it
       must be an int: assigning the sum straight to an s16 makes gcc shorten
       the addition into HImode (convert.c's trunc1 path), and the shortened
       add comes out with the operands the other way round --
       `addu $t0,$t0,$s5` for the target's `addu $t0,$s6,$t0` (143 / 6).  The
       s16 copy is what produces the target's `addu $s2,$t0,$zero`: it is the
       HImode truncation.  Spelling the last two arguments `(s16)tx, (s16)tz`
       off the int sums instead drops both copies and the function is 141. */
    s32 tx;
    s32 tz;
    s16 x;
    s16 z;

    for (; (p->unk_01 & 0xC0) != 0x80; p++)
    {
        if (p->unk_00 != 0)
        {
            continue;
        }
        if ((p->unk_01 & 1) == 0)
        {
            if (func_80033B2C(p->unk_02) == 0)
            {
                continue;
            }
        }
        else
        {
            if (func_80033B2C(p->unk_02) == 1)
            {
                continue;
            }
        }
        tx = ox + p->unk_10;
        tz = oz + p->unk_12;
        x = tx;
        z = tz;
        if (func_8008CC90((s16)(*(u16 *)(b + 0x00) - tx), (s16)(*(u16 *)(b + 0x02) - tz),
                          (s16)(*(u16 *)(b + 0x08) - tx), (s16)(*(u16 *)(b + 0x0A) - tz),
                          (s16)(*(u16 *)(b + 0x10) - tx), (s16)(*(u16 *)(b + 0x12) - tz),
                          (s16)(*(u16 *)(b + 0x18) - tx), (s16)(*(u16 *)(b + 0x1A) - tz)) == 0)
        {
            if (D_8006ADD4 != 0xC)
            {
                if ((p->unk_01 & 0x10) == 0)
                {
                    continue;
                }
            }
        }
        if (func_8009D424(p->unk_0C, p->unk_04, p->unk_05, p->unk_06, p->unk_07,
                          p->unk_01 & 0x20, p->unk_08, x, z, p) != 0)
        {
            p->unk_00 = 1;
        }
        while ((p->unk_01 & 0xC0) == 0)
        {
            p++;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009DC8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009DD40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009DE28);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009DEBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009DF84);

UnkNode *func_8009DFD8(u8 *a, UnkTown2AE8 *b, s32 c)
{
    UnkNode *n;
    UnkTownSpawn *p;
    UnkVec *q;
    UnkTownRend *r;

    n = func_8003FD64(0x136, &D_80083498);
    if (n == NULL)
    {
        return NULL;
    }
    q = n->unk_08;
    r = (UnkTownRend *)n->unk_0C;
    *(u16 *)((u8 *)q + 0x2) = *(u16 *)((u8 *)b + 0x2);
    *(u16 *)((u8 *)q + 0x6) = *(u16 *)((u8 *)b + 0x6);
    *(u16 *)((u8 *)q + 0xA) = *(u16 *)((u8 *)b + 0xA);
    p = (UnkTownSpawn *)((u8 *)n + 0x20);
    r->unk_1E = 0x1000;
    r->unk_1C = 0x1000;
    *(s32 *)((u8 *)r + 0xC) = 0x808080;
    *(s32 *)((u8 *)p + 0x48) = 0;
    p->unk_98 = 0;
    *((u8 *)p + 0x93) = 0;
    *((u8 *)p + 0x4C) = a[0];
    *((u8 *)p + 0x4D) = a[1];
    *((u8 *)p + 0x4E) = a[2];
    *((u8 *)p + 0x4F) = a[3];
    if (*((u8 *)p + 0x4D) == 0x13)
    {
        /* The subscript MUST be read back out of the field just stored, not
           taken from `a[3]`. Reading `a[3]` again puts a second `lbu
           0x3($s4)` in the block (the intervening byte stores through `p`
           invalidate cse's memory table, so the first load cannot be
           reused), and the shift chain then feeds off $v1 instead of the $a1
           the store left behind. Reading p+0x4F forwards the stored value,
           because nothing has been written since. Measured: `a[3] & 0x1F`
           costs one extra word and misaligns the whole block. */
        D_80100AF8 = ((UnkTownItemRec *)0x800102F0)[*((u8 *)p + 0x4F) & 0x1F];
    }
    func_8008F0D4(p, q, D_800D073C);
    if (c)
    {
        func_8009B218(p, q, r, D_800D0788);
    }
    else
    {
        func_8009B218(p, q, r, D_800D076C);
    }
    if (*((u8 *)p + 0x4D) == 0x12 || *((u8 *)p + 0x4D) == 0x13)
    {
        *(u8 **)((u8 *)p + 0xC) = D_800D0754;
    }
    if (*((u8 *)p + 0x4D) == 0x12)
    {
        r->unk_08 = (u32)D_8006E240;
    }
    else if (*((u8 *)p + 0x4D) == 0x13)
    {
        func_8009DC8C(p, r, *((u8 *)p + 0x4C), D_80100B0C);
    }
    else
    {
        r->unk_08 = func_8004A658(*((u8 *)p + 0x4D), *((u8 *)p + 0x4C));
    }
    n->unk_10 = (u32)func_8009DEBC;
    func_8009C1B4(p, p, q, r);
    func_800B2244(a);
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E218);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E258);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E2C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E348);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E370);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E3D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E3EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E424);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E4BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E4EC);

/* The tail from func_8009DFD8 (the landed entity constructor) almost verbatim:
   the same D_800D073C / D_800D076C / D_800D0788 tables, the same
   `0x12 || 0x13` range test writing D_800D0754 to p+0xC, and the same
   0x12 -> D_8006E240 / 0x13 -> func_8009DC8C / else -> func_8004A658 chain
   into r->unk_08. p-0x10 is the UnkNode update hook that func_8009DFD8
   spells `n->unk_10`, re-pointed at func_8009DD40 or func_8009DEBC.

   THE TWO TABLES AT 0x80010980 AND 0x80010A94 ARE ONE CONSTANT BASE, AND THE
   BASE LOCAL MUST BE PER-BLOCK. Both are outside the loaded image, so they
   are the constant 0x80010000 with the whole offset inside the index -- see
   docs/matching-levers.md, "A %lo in the displacement with a REGISTER index
   means a constant base". mips.h's LEGITIMIZE_ADDRESS only splits a constant
   address into a register high plus a displacement low when the address is
   plus(REG, CONST_INT); an array or struct base makes it plus(ashift, ...),
   code0 != REG, and force_operand materialises the whole address, one word
   too many every time.

   What is new here: ONE base local per basic block, declared and assigned
   INSIDE the arm and AFTER the calls. A single function-scope base shared by
   the else-arm and the 0x13-arm is 132 words / 5 differing on register names
   alone; assigning it at the top of the block instead hoists the `lui` above
   the calls, which buys it a callee-saved register (`lui $s3`, an extra
   `sw $s3`, frame 0x28 not 0x20) and costs 135 words / 127 differing.

   Arity 3 is what the body witnesses ($s0/$s1/$s2 all used) but is NOT
   falsifiable: this function has no caller anywhere in the repo and its
   address is never taken, so a fourth unused pointer parameter would be
   invisible. */
void func_8009E504(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    if (*((u8 *)p + 0x94) == 0)
    {
        func_8008F0D4(p, q, D_800D073C);
        func_8009B218(p, q, r, D_800D076C);
        *((u8 *)p + 0x4C) = *((u8 *)p + 0x95);
        *((u8 *)p + 0x4D) = *((u8 *)p + 0x96);
    }
    else if (*((s8 *)p + 0x94) == -1)
    {
        func_8008F104(p, q, D_800D073C);
        func_8009B218(p, q, r, 0);
        *((u8 *)p + 0x4C) = *((u8 *)p + 0x95);
        *((u8 *)p + 0x4D) = *((u8 *)p + 0x96);
    }
    else
    {
        u8 *b;

        func_8008F0D4(p, q, D_800D073C);
        func_8009B218(p, q, r, D_800D0788);
        b = (u8 *)0x80010000;
        *((u8 *)p + 0x4C) = b[*((u8 *)p + 0x95) * 4 + 0x980];
        *((u8 *)p + 0x4D) = b[*((u8 *)p + 0x95) * 4 + 0x981];
        *((u8 *)p + 0x4E) = b[*((u8 *)p + 0x95) * 4 + 0x982];
        *((u8 *)p + 0x4F) = b[*((u8 *)p + 0x95) * 4 + 0x983];
        func_8009F148(p, p, q, r);
    }
    if (func_8009FF50() == 0)
    {
        *(u32 *)((u8 *)p - 0x10) = (u32)func_8009DD40;
    }
    else
    {
        *(u32 *)((u8 *)p - 0x10) = (u32)func_8009DEBC;
    }
    if (*((u8 *)p + 0x4D) == 0x12 || *((u8 *)p + 0x4D) == 0x13)
    {
        *(u8 **)((u8 *)p + 0xC) = D_800D0754;
    }
    if (*((u8 *)p + 0x4D) == 0x12)
    {
        r->unk_08 = (u32)D_8006E240;
    }
    else if (*((u8 *)p + 0x4D) == 0x13)
    {
        u8 *c;

        c = (u8 *)0x80010000;
        func_8009DC8C(p, r, *((u8 *)p + 0x4C),
                      *(void **)(c + (c[*((u8 *)p + 0x95) * 4 + 0x983] & 0x3F) * 0x54 + 0xA94));
    }
    else
    {
        r->unk_08 = func_8004A658(*((u8 *)p + 0x4D), *((u8 *)p + 0x4C));
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E714);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E85C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E994);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EAB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EB20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EB3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EB9C);

UnkTownIdPair *func_8009EBE0(s32 a, s32 b)
{
    UnkTownIdPair **tbl = (UnkTownIdPair **)0x80010000;
    s32 i;

    for (i = 0; tbl[0xA7 + i] != NULL; i++)
    {
        if (tbl[0xA7 + i]->unk_00 == a && tbl[0xA7 + i]->unk_01 == b)
        {
            return tbl[0xA7 + i];
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EC3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EC70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009ECF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009ED7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EE9C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EF88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F148);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F1C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F270);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F2A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F374);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F48C);

/* Installs an item (type 0x13) into the actor's slot. Two tables live at the
   out-of-image constant base 0x80010000, which is why the base is a one-set
   pointer local and the offsets stay in the load/store displacements: a
   four-byte record array at +0x980 (+0x01 a type, +0x03 two flag bits over a
   six-bit record index) and the 0x54-byte record array at +0xA80. The pair is
   cross-linked here -- the slot's +0x03 gets the record index and the record's
   +0x43 gets the slot.

   Two bases, b and c, not one: with a single pointer local its `lui` is held
   in a callee-saved register across all five calls, where the target
   materialises 0x80010000 twice, once either side of the record copy's loop
   (which is also cse's block boundary). 112 differing with one base.

   `m = i` where the two are equal is not cosmetic. The record index and the
   slot index are separate quantities in the target: the X-table index comes
   from the `lbu` ($s5) and the 0x54-record index from a copy of it ($s3), and
   the copy is what cse canonicalises on. Which one wins is decided by
   make_regs_eqv (cse.c): the copy's register becomes the quantity's canonical
   register only if its LAST mention is later than the original's, which is why
   `m` must also be the variable the +0x3F index is read back into at the end
   -- spelled as a separate `j`, the canonical register flips and 15 words
   differ, every one of them a register. The copy's own position is measured
   too: before func_80042640 it is 2 words out (reorg puts the `$a0` setup in
   that call's delay slot instead), before the 0x981 store 6, and only here
   does it land in func_80042640's delay slot as the target has it.

   The record's destination addresses are CONSTANTS (`lui`/`ori`), not offsets
   off b or c: emit_block_move forces a block move's address into a register,
   so 0x80010A80 and 0x80010AB4 come out whole, while the scalar accesses keep
   their %lo in the displacement off the base. Both spellings are in this one
   function because that is what the listing shows. */
void func_8009F4C0(UnkTownActor *a, s32 arg1, s32 arg2, UnkTownRend *r)
{
    u8 *b;
    u8 *c;
    s32 i;
    s32 m;
    void *d;
    UnkTownName10 *p;

    i = *((u8 *)a + 0x95);
    b = (u8 *)0x80010000;
    func_80033CD8((u8 *)a, (u32)D_80045340);
    *(u8 *)(b + i * 4 + 0x981) = 0x13;
    func_80069EE8((u8 *)&D_80100A10, 0, 0x54);
    func_80042640(&D_80100A10, *((u8 *)a + 0x4C));
    func_800423C0((u8 *)&D_80100A10, *((u8 *)a + 0x4C), NULL);
    func_800422DC((u8 *)&D_80100AF8, (u8 *)&D_80100A10);
    m = i;
    ((UnkTownItemRec *)0x80010A80)[m] = D_80100AF8;
    p = (UnkTownName10 *)((u8 *)0x80010AB4 + m * 0x54);
    *p = *(UnkTownName10 *)func_8003C06C(*((u8 *)a + 0x4C));
    c = (u8 *)0x80010000;
    *(u8 *)(c + i * 4 + 0x983) = (*(u8 *)(c + i * 4 + 0x983) & 0xC0) | m;
    *(u8 *)(c + m * 0x54 + 0xAC3) = i;
    *((u8 *)a + 0x4D) = *(u8 *)(c + i * 4 + 0x981);
    *((u8 *)a + 0x4F) = *(u8 *)(c + i * 4 + 0x983);
    m = *(u8 *)(c + *((u8 *)a + 0x95) * 4 + 0x983) & 0x3F;
    d = *(void **)(c + m * 0x54 + 0xA94);
    func_8003E188(*((u8 *)a + 0x4C), 1);
    func_8009DC8C((UnkTownSpawn *)a, r, *((u8 *)a + 0x4C), d);
    a->unk_50 = func_8009F374;
    a->unk_6C = 0xA;
}

void func_8009F6D4(void) {
}

void func_8009F6DC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F6E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F71C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F750);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F77C);

UnkTownIdPair *func_8009F830(s32 a, s32 b)
{
    UnkTownIdPair **tbl = (UnkTownIdPair **)0x80010000;
    s32 i;

    for (i = 0; tbl[0xA7 + i] != NULL; i++)
    {
        if (tbl[0xA7 + i]->unk_00 == a && tbl[0xA7 + i]->unk_01 == b)
        {
            return tbl[0xA7 + i];
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F88C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F8EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F938);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F970);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F9A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F9D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FA14);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FB2C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FCF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FD68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FDBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FE84);

void func_8009FF28(void)
{
    func_8009FDBC();
    func_8009FE84();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FF50);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FF8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A00E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0150);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A022C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A027C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A03DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0404);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A04A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A04B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0504);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0560);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0608);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0668);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A06A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0708);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A07A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A07E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0884);

void func_800A08E8(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A08F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A092C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0958);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0988);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A09F4);

void func_800A0A34(void *a)
{
    func_800A09F4(a, 8);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0A54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0A88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0AC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0B74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0BE4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0C30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0CC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0CF8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0DAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0E3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0EB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0F10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0F9C);

s32 func_800A1080(UnkTownA1080 *s, UnkTown2AE8 *b, s32 arg2)
{
    UnkTownA1080SP *p = (UnkTownA1080SP *)0x1F800000;

    s->unk_02 += 0x16;
    s->unk_10 += 5;
    s->unk_0C += 5;
    if (s->unk_0C >= 0xFF)
    {
        s->unk_0C = 0xFF;
    }
    if (s->unk_0C < 0xC1)
    {
        s->unk_10 = func_800644B8(s->unk_0C * 5) / 0x10;
    }
    else
    {
        s->unk_10 = func_800644B8(s->unk_0C * 0x10 - 0x400) / 0x10;
    }
    if (s->unk_10 >= 0xFF)
    {
        s->unk_10 = 0xFF;
    }
    s->unk_08 -= 6;
    if (s->unk_08 < 0)
    {
        s->unk_08 = 0;
    }
    func_800649A0();
    p->unk_44.vx = *(s16 *)((u8 *)b + 0x2);
    p->unk_44.vy = *(s16 *)((u8 *)b + 0x6);
    p->unk_44.vz = *(s16 *)((u8 *)b + 0xA) - s->unk_7A / 2;
    p->unk_34.vx = s->unk_08;
    p->unk_34.vy = s->unk_08;
    p->unk_34.vz = s->unk_08;
    p->unk_2C.vx = s->unk_00;
    p->unk_2C.vy = s->unk_02;
    p->unk_2C.vz = s->unk_04;
    func_80064B90(&s->unk_14, &p->unk_44);
    func_80065820(&p->unk_2C, &s->unk_14);
    func_80064BC0(&s->unk_14, &p->unk_34);
    p->unk_2C.vx = -s->unk_00;
    p->unk_2C.vy = -s->unk_02;
    p->unk_2C.vz = -s->unk_04;
    func_80064B90(&s->unk_34, &p->unk_44);
    func_80065820(&p->unk_2C, &s->unk_34);
    func_80064BC0(&s->unk_34, &p->unk_34);
    func_80064A40();
    func_800A1354(s);
    if (s->unk_08 == 0)
    {
        s->unk_0A = 1;
        return 1;
    }
    return 0;
}

void func_800A12AC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A12B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A130C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A1330);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A1354);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A17CC);

void func_800A1C8C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A1C94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A1D1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A1F00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A1FEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2000);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A20D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2180);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A21EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A22D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2304);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2338);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A23D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A24A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A24C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A24FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A25CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2690);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2A18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2AF8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2E30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A2FCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A3248);

void func_800A32C8(UnkTownDispatch *d)
{
    d->unk_00();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A32F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A3320);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A33E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A3450);

void func_800A34E0(UnkTownDispatch *d)
{
    d->unk_00();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A3508);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A378C);

void func_800A38A8(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    s32 t;

    t = r->unk_0E - 6;
    if (t < 0)
    {
        t = 0;
    }
    r->unk_0E = t;
    r->unk_0D = t;
    r->unk_0C = t;
    if (t == 0)
    {
        func_80033D08((u8 *)p);
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A3918);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A3B1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A3B80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A41D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A47A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A488C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4898);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A48A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A48B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A48F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4928);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A496C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4A4C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088760);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088C0C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088C48);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088C8C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088CCC);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088D04);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088D44);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088D60);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80088D78);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008910C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4A58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4B40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4B94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4C10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4CE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4D4C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4E1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4F4C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A4F84);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5080);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5094);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A50B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A50E0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A510C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5130);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5160);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A516C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5178);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A51CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A526C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5340);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5440);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A549C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A54FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A553C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5598);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A55CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5638);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A573C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5894);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A58CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5A98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5DF8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A5FDC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A62E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A630C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6328);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A639C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A63C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A643C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A647C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A64C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A652C);

void func_800A6684(UnkTownActor *a, UnkVec *q, UnkTownRend *r)
{
    UnkVec v;
    s32 idx;

    a->unk_50(a, a, q, r);
    if (*(u16 *)((u8 *)a - 0x2) & 0x8000)
    {
        return;
    }
    func_800970AC(func_80096FF4(q), a);
    idx = func_800C2E1C(a->unk_72, *(s16 *)((u8 *)a + 0x64));
    if (*(s16 *)((u8 *)a + 0x74) != idx)
    {
        func_8003DB94(r, (*(void ***)((u8 *)a + 0x78))[idx], *(s8 *)((u8 *)r + 0x4));
        *(s16 *)((u8 *)a + 0x74) = idx;
    }
    if (func_800C2F14(a->unk_72, *(s16 *)((u8 *)a + 0x64)))
    {
        r->unk_14 |= 1;
    }
    else
    {
        r->unk_14 &= ~1;
    }
    func_800478B8((s32)r);
    if (q->unk_04 <= 0x6500000 && *(s32 *)((u8 *)a + 0xA0) > 0x6500000)
    {
        r->unk_0E = 0xFF;
        r->unk_0D = 0xFF;
        r->unk_0C = 0xFF;
        v.unk_00 = q->unk_00;
        v.unk_04 = 0x6400000;
        v.unk_08 = q->unk_08 + 0xFFD80000;
        func_800A6A94(&v);
    }
    else if (q->unk_04 > 0x6500000 && *(s32 *)((u8 *)a + 0xA0) <= 0x6500000 && *(s32 *)((u8 *)a + 0xA4) <= 0x6500000)
    {
        r->unk_0E = 0xFF;
        r->unk_0D = 0xFF;
        r->unk_0C = 0xFF;
        v.unk_00 = q->unk_00;
        v.unk_04 = 0x6600000;
        v.unk_08 = q->unk_08 + 0xFFD80000;
        func_800A6B70(&v);
    }
    else
    {
        r->unk_0C -= 8;
        if (r->unk_0C < 0x80)
        {
            r->unk_0C = 0x80;
        }
        r->unk_0D -= 0x10;
        if (r->unk_0D < 0x80)
        {
            r->unk_0D = 0x80;
        }
        r->unk_0E = r->unk_0D;
    }
    *(s32 *)((u8 *)a + 0xA4) = *(s32 *)((u8 *)a + 0xA0);
    *(s32 *)((u8 *)a + 0xA0) = q->unk_04;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A68C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6994);

void func_800A6A94(UnkVec *p)
{
    UnkNode *o;
    UnkTownRend *r;

    o = func_8009C390(0, p, func_800A6B10, 0);
    if (o != 0)
    {
        r = (UnkTownRend *)o->unk_0C;
        func_8009CD18((u8 *)o + 0x20, r, 0x3A1);
        r->unk_20 = 0xC00;
        r->unk_1E = 0xC00;
        r->unk_1C = 0xC00;
        r->unk_0E = 0xFF;
        r->unk_0D = 0xFF;
        r->unk_0C = 0xFF;
        r->unk_14 |= 0x1C;
    }
}

void func_800A6B10(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    s32 t;

    t = r->unk_0E;
    t -= 0x10;
    if (t < 0)
    {
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        s32 s = (u16)r->unk_20 + 0x100;

        r->unk_0E = t;
        r->unk_0D = t;
        r->unk_0C = t;
        r->unk_20 = s;
        r->unk_1E = s;
        r->unk_1C = s;
    }
}

void func_800A6B70(UnkVec *p)
{
    UnkNode *o;
    UnkTownRend *r;

    o = func_8009C390(0, p, func_800A6BEC, 0);
    if (o != 0)
    {
        r = (UnkTownRend *)o->unk_0C;
        func_8009CD18((u8 *)o + 0x20, r, 0x3A1);
        r->unk_20 = 0x1800;
        r->unk_1E = 0x1800;
        r->unk_1C = 0x1800;
        r->unk_0E = 0xFF;
        r->unk_0D = 0xFF;
        r->unk_0C = 0xFF;
        r->unk_14 |= 0x1C;
    }
}

void func_800A6BEC(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    s32 t;

    t = r->unk_0E;
    t -= 0x10;
    if (t < 0)
    {
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        s32 s = (u16)r->unk_20 - 0x100;

        r->unk_0E = t;
        r->unk_0D = t;
        r->unk_0C = t;
        r->unk_20 = s;
        r->unk_1E = s;
        r->unk_1C = s;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6C4C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6DA0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6E1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6EA4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6EE4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6F48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A70EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A71F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7308);

void func_800A7338(UnkTownDispatch *d)
{
    d->unk_50();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7360);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A73E0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A74D8);

void func_800A7554(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    func_8003DB94(r, D_800D0E04, 0);
    func_8009B218(p, q, r, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A75B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A75E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A76F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7774);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A77D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A782C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A78AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A790C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7950);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A79C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A79F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7AC0);

void func_800A7B14(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    if (func_8009CFE0(p, q, r))
    {
        if (p->unk_98 != 0)
        {
            *(u8 *)p->unk_98 = 0;
        }
        func_80033D08((u8 *)p);
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        func_800478B8((s32)r);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7B98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7C0C);

void func_800A7C98(u8 *a, s32 b, s32 c)
{
    *(u32 *)(a - 0x10) = (u32)func_800A7DF8;
    *(s32 *)(a + 0x50) = 0;
    func_800C2CB0(a, c, D_800F8F68, 0);
    func_80033CD8(a, (u32)D_80045340);
}

void func_800A7CF0(u8 *a, s32 b, s32 c)
{
    *(u32 *)(a - 0x10) = (u32)func_800A7DF8;
    *(s32 *)(a + 0x50) = 0;
    func_800C2CB0(a, c, D_800F8F78, 0);
    func_80033CD8(a, (u32)D_80045340);
}

void func_800A7D48(u8 *a, s32 b, s32 c)
{
    *(u32 *)(a - 0x10) = (u32)func_800A7DF8;
    *(s32 *)(a + 0x50) = 0;
    func_800C2CB0(a, c, D_800F8F88, 0);
    func_80033CD8(a, (u32)D_80045340);
}

void func_800A7DA0(u8 *a, s32 b, s32 c)
{
    *(u32 *)(a - 0x10) = (u32)func_800A7DF8;
    *(s32 *)(a + 0x50) = 0;
    func_800C2CB0(a, c, D_800F8F98, 0);
    func_80033CD8(a, (u32)D_80045340);
}

void func_800A7DF8(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    if (func_8009CFE0(p, q, r))
    {
        if (p->unk_98 != 0)
        {
            *(u8 *)p->unk_98 = 0;
        }
        func_80033D08((u8 *)p);
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        func_800478B8((s32)r);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7E7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7EC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7FD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8070);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8184);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8248);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A82F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A833C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8408);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8418);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8440);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A84D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A85C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A86BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A878C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A895C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8ADC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8B98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8BBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8C84);

void func_800A8CA8(UnkTownSP *p, s16 ang, UnkTown2AE8 *mv, UnkTownRend *r)
{
    s32 a = (s16)(ang + 0x800);
    s32 a2 = a << 1;
    s32 dx;
    s32 dy;

    p->unk_114 = (func_800644B8(*(s16 *)((u8 *)mv + 0xE) + a2) + 0x1000) / 0x80;
    p->unk_115 = (func_800644B8(*(s16 *)((u8 *)mv + 0x12) + a2) + 0x1000) / 0x80;
    p->unk_116 = (func_800644B8(*(s16 *)((u8 *)mv + 0x16) + a2) + 0x1000) / 0x80;
    func_800A8C84((SVECTOR *)&p->unk_74, mv);
    func_800A8C84((SVECTOR *)&p->unk_7C, mv);
    func_800A8C84((SVECTOR *)&p->unk_84, mv);
    dx = func_800644B8(a) * (u16)r->unk_1C / 0x1000;
    dy = func_80064584(a) * (u16)r->unk_1E / 0x1000;
    p->unk_74 += dx;
    p->unk_76 += dy;
    p->unk_C4 = func_80065420((SVECTOR *)&p->unk_74, &p->unk_E8, &p->unk_94, &p->unk_98);
    p->unk_7C += dx;
    p->unk_7E += dy;
    p->unk_80 += r->unk_20 / 2;
    p->unk_C4 = (p->unk_C4 + func_80065420((SVECTOR *)&p->unk_7C, &p->unk_EC, &p->unk_94, &p->unk_98)) / 2 - 8;
    p->unk_84 += dx;
    p->unk_86 += dy;
    p->unk_88 += r->unk_20;
    func_80065420((SVECTOR *)&p->unk_84, &p->unk_124, &p->unk_94, &p->unk_98);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8EE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9358);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9528);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A957C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9624);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A970C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9734);

void func_800A9778(u8 *p)
{
    if (func_8009F88C(0xC, 0xF))
    {
        func_80033AA8(0xAB);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A97D8);

void func_800A97F8(u8 *p)
{
    if (func_8009F88C(0x1, 0x12))
    {
        func_80033AA8(0xAD);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9858);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9878);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A98C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A98F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9970);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A99D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9A38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9ADC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9B2C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9C18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9C50);

void func_800A9C6C(void)
{
    func_80033B2C(0x11FA);
}

void func_800A9C8C(void)
{
    func_80033B2C(0x11FB);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9CAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9D00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9D74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9E68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9F14);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA044);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA068);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA10C);

void func_800AA5F8(UnkTownTask *a, UnkTown2AE8 *b, s32 c)
{
    Unk80083160 *g = &D_80083160;
    s32 n;
    s32 ang;
    s32 base;
    u8 *p;

    func_80095C80(b);
    func_80095094(b);
    n = (s16)func_80095978(b, D_800FE488);
    if (n - *(s16 *)((u8 *)b + 0xA) >= 4)
    {
        if (D_800CFCEF == 0)
        {
            /* func_80094378 is widened to three parameters in csrc/ovl_town.c, so this
                 is a plain call rather than the function-pointer cast the
                 agent originally needed. */
            func_80094378(a, b, c);
            D_800D0B14 = 0;
            return;
        }
    }
    else
    {
        if (D_800CFCEF == 0)
        {
            func_80095A94(b, n, D_800FE488);
        }
    }
    if ((g->unk_10 & 0x40) && (D_80100D1C >= 0x800))
    {
        func_800AAA58(a, b, c);
        D_800D0B14 = 0;
        return;
    }
    if (g->unk_08 & 0x2000)
    {
        if (D_80100D80 < 0x100)
        {
            D_80100D80 += 8;
        }
        else
        {
            D_80100D80 = 0x100;
        }
    }
    if (g->unk_08 & 0x8000)
    {
        if (D_80100D80 > -0x100)
        {
            D_80100D80 -= 8;
        }
        else
        {
            D_80100D80 = -0x100;
        }
    }
    if (g->unk_08 & 0x4000)
    {
        if (D_80100D82 < 0x100)
        {
            D_80100D82 += 8;
        }
        else
        {
            D_80100D82 = 0x100;
        }
    }
    if (g->unk_08 & 0x1000)
    {
        if (D_80100D82 > -0x100)
        {
            D_80100D82 -= 8;
        }
        else
        {
            D_80100D82 = -0x100;
        }
    }
    p = (u8 *)&D_80100D60;
    /* Split across three statements on purpose.  Written as the one
       expression `D_80100D60 = D_80100D60 + 0x500000 - (func_80064584(...)
       << 10);` gcc's fold reassociates it to `A - (B - 0x500000)` and emits
       `lui -0x500000; addu` into the SUBTRAHEND: 6 differing words (113-118).
       Hoisting only the shifted term into a temp leaves 2 differing, the
       `sll` and the `lui 0x500000` swapped in the schedule.  Keeping the
       `+ 0x500000` in its own statement (so the final tree holds no literal
       for fold to move) and the `<< 10` at its USE site is 0 differing. */
    ang = func_80064584(*(s16 *)(p + 2) - 0x400);
    base = D_80100D60 + 0x500000;
    D_80100D60 = base - (ang << 10);
    a->unk_10 = *(s16 *)(p + 2);
    if (D_80100D1C < 0x800)
    {
        D_80100D1C += 0x80;
    }
    else
    {
        D_80100D1C = 0x800;
    }
    if (D_80100D18 < 0x1000)
    {
        D_80100D18 += 0x100;
    }
    else
    {
        D_80100D18 = 0x1000;
    }
    func_800A9F14(b);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA844);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA998);

/* Widened from one parameter: func_800AA5F8 calls it with three. That call
   compiled before only because this definition FOLLOWS it, so the call site
   saw an implicit declaration -- widening makes it explicit instead, and lets
   the declaration block carry a real prototype rather than an unprototyped
   one. The extra two go unread; measured 18 words, 0 differing. */
void func_800AAA58(UnkTownTask *t, UnkTown2AE8 *b, s32 c)
{
    func_80094984(D_800D0138, t);
    t->unk_00 = func_800AA844;
    t->unk_0A = 0x15;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAAA0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAB20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAC64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAD3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AADB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAE00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAE20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAE30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAE98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAEFC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAF5C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AAFE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB014);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB030);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB084);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB158);

void func_800AB1E0(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    Unk80083160 *g = &D_80083160;
    UnkTown2AE8 v;
    s32 idx;

    if (D_800834A8 == 0)
    {
        func_80033D08((u8 *)a);
        *(u16 *)((u8 *)a - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    func_800AAFE0(&v, 0);
    if (*(s16 *)((u8 *)&v + 0x2) != TOWN_PT->unk_02 || *(s16 *)((u8 *)&v + 0x6) != TOWN_PT->unk_06 ||
        *(s16 *)((u8 *)&v + 0xA) != TOWN_PT->unk_0A || func_800352FC() != 0 || (g->unk_08 & 0xF000) != 0)
    {
        func_800AAF5C();
        a->unk_90 = 0;
    }
    else
    {
        a->unk_90++;
        if (a->unk_90 >= 0x65)
        {
            a->unk_90 = 0x64;
        }
    }
    a->unk_50(a, b, c);
    if (*(u16 *)((u8 *)a - 0x2) & 0x8000)
    {
        return;
    }
    idx = func_800C2E1C(a->unk_72, *(s16 *)((u8 *)a + 0x64));
    if (*(s16 *)((u8 *)a + 0x74) != idx)
    {
        func_800C2CB0((u8 *)a, (s32)c, (*(void ***)((u8 *)a + 0x78))[idx], *(s8 *)((u8 *)c + 0x4));
        *(s16 *)((u8 *)a + 0x74) = idx;
    }
    if ((*(u8 *)((u8 *)a + 0x71) & 1) == 0)
    {
        if (func_800C2F14(a->unk_72, *(s16 *)((u8 *)a + 0x64)))
        {
            c->unk_14 |= 1;
        }
        else
        {
            c->unk_14 &= ~1;
        }
    }
    else
    {
        if (func_800C2F14(a->unk_72, *(s16 *)((u8 *)a + 0x64)))
        {
            c->unk_14 &= ~1;
        }
        else
        {
            c->unk_14 |= 1;
        }
    }
    func_800C2C80(a, c, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB408);

void func_800AB4A8(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    if (a->unk_90 < 100)
    {
        a->unk_50 = func_800AB708;
    }
    if (--a->unk_6C < 0)
    {
        a->unk_50 = func_800AB50C;
        a->unk_6C = 0x14;
    }
    else
    {
        a->unk_72 += 0x200;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB50C);

void func_800AB55C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    if (a->unk_90 < 100)
    {
        a->unk_50 = func_800AB708;
    }
    if (--a->unk_6C < 0)
    {
        a->unk_50 = func_800AB408;
        a->unk_6C = 0x96;
    }
    else
    {
        a->unk_72 += -0x400;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB5C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB708);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB7FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB9A0);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089128);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ABA1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ABB20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ABC00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ABD74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ABE60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ABEEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AC1B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AC37C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AC3EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AC454);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AC488);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AC4C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD138);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD7F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD828);

void func_800AD8CC(UnkTownADD80 *s)
{
    if (func_800AD828(s->unk_20, *(s32 *)((u8 *)s + 0x18)) != 0)
    {
        D_80082AB8[0] = 1;
    }
    else
    {
        D_80082AB8[0] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD914);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD968);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD99C);

void func_800ADA1C(UnkTownADD80 *s)
{
    if (s->unk_20[s->unk_04 * 4 + 1] != 0x16)
    {
        if ((s->unk_20[s->unk_04 * 4 + 3] & 0x80) == 0)
        {
            if (func_800AD99C(s))
            {
                func_80053DA8(0x503);
                func_800AD7F8(s);
                func_800B17E4(s->unk_28);
            }
            else
            {
                func_80053DA8(0x506);
                func_8004DD2C(D_800D1558);
            }
        }
        else
        {
            func_80053DA8(0x506);
            if (s->unk_18 == 1)
            {
                func_8004DD2C(D_800D155C);
            }
        }
    }
    else
    {
        func_800AE30C((u8 *)s - 0x20);
        func_800AD8CC(s);
    }
}

void func_800ADB04(UnkTownADD80 *s)
{
    func_80049490(&s->unk_20[s->unk_04 * 4]);
}

void func_800ADB30(UnkTownADD80 *s)
{
    func_800494B4(&s->unk_20[s->unk_04 * 4]);
}

void func_800ADB5C(UnkTownADD80 *s, void *b, void *c)
{
    Unk80083160 *g = &D_80083160;
    s32 d = 0;
    s32 n;
    s32 m;

    if (g->unk_08 == 0)
    {
        return;
    }
    if (g->unk_10 & 0x10)
    {
        func_80053DA8(0x514);
        func_800AD8CC(s);
        func_800AE30C((u8 *)s - 0x20);
        return;
    }
    if (g->unk_10 & 0x20)
    {
        func_80053DA8(0x515);
        D_80082AB8[0] = 0;
        func_800AE30C((u8 *)s - 0x20);
        return;
    }
    if (g->unk_10 & 0x40)
    {
        func_800ADA1C(s);
        return;
    }
    if ((g->unk_08 & 0xF000) == 0)
    {
        return;
    }
    if (g->unk_10 & 0xF000)
    {
        s->unk_14 = 0;
        if (g->unk_10 & 0x8000)
        {
            d = -5;
        }
        else if (g->unk_10 & 0x2000)
        {
            d = 5;
        }
        else if (g->unk_10 & 0x1000)
        {
            d = -1;
        }
        else if (g->unk_10 & 0x4000)
        {
            d = 1;
        }
    }
    else if (s->unk_14 >= 9)
    {
        if (g->unk_08 & 0x8000)
        {
            d = -5;
        }
        else if (g->unk_08 & 0x2000)
        {
            d = 5;
        }
        else if (g->unk_08 & 0x1000)
        {
            d = -1;
        }
        else if (g->unk_08 & 0x4000)
        {
            d = 1;
        }
        s->unk_14 = s->unk_14 - 1;
    }
    else
    {
        s->unk_14 = s->unk_14 + 1;
        return;
    }
    n = func_80049E1C(s->unk_04, d, s->unk_08);
    m = n / 5;
    if (n == s->unk_04)
    {
        return;
    }
    func_80053DA8(0x502);
    s->unk_04 = n;
    if (m != s->unk_0C)
    {
        s->unk_0C = m;
        func_800B1778(s->unk_28, m, n);
    }
    func_800B17C0(s->unk_28, n);
    if (g->unk_08 & 0x40)
    {
        func_800ADA1C(s);
    }
    func_800ADB04(s);
}

void func_800ADD80(UnkTownADD80 *s, void *b, void *c)
{
    Unk80083160 *g = &D_80083160;
    s32 d = 0;
    s32 n;
    s32 m;

    if (g->unk_08 == 0)
    {
        return;
    }
    /* The 0x515 arm is written twice; jump.c cross-jumps the two copies and
       the first becomes the `j .L800ADDF0` the listing has. */
    if (s->unk_08 == 0)
    {
        if (g->unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            D_80082AB8[0] = 0;
            func_800AE30C((u8 *)s - 0x20);
        }
        return;
    }
    if (g->unk_10 & 0x20)
    {
        func_80053DA8(0x515);
        D_80082AB8[0] = 0;
        func_800AE30C((u8 *)s - 0x20);
        return;
    }
    if (g->unk_10 & 0x40)
    {
        func_80053DA8(0x514);
        D_80082AB8[0] = 1;
        func_800AE30C((u8 *)s - 0x20);
        s->unk_20[s->unk_04 * 4 + 3] |= 0x20;
        return;
    }
    if ((g->unk_08 & 0xF000) == 0)
    {
        return;
    }
    if (g->unk_10 & 0xF000)
    {
        s->unk_14 = 0;
        if (g->unk_10 & 0x8000)
        {
            d = -5;
        }
        else if (g->unk_10 & 0x2000)
        {
            d = 5;
        }
        else if (g->unk_10 & 0x1000)
        {
            d = -1;
        }
        else if (g->unk_10 & 0x4000)
        {
            d = 1;
        }
    }
    /* `>= 9` with the increment in the ELSE arm, not `< 9` with an early
       return: the early return has to be laid out AFTER the decrement block
       so that block ends in `j .L800ADF28` instead of falling through.
       `if (s->unk_14 < 9) { s->unk_14 = s->unk_14 + 1; return; }` ahead of
       the chain measured 142 words / 65 differing; this is 143 / 0. */
    else if (s->unk_14 >= 9)
    {
        if (g->unk_08 & 0x8000)
        {
            d = -5;
        }
        else if (g->unk_08 & 0x2000)
        {
            d = 5;
        }
        else if (g->unk_08 & 0x1000)
        {
            d = -1;
        }
        else if (g->unk_08 & 0x4000)
        {
            d = 1;
        }
        s->unk_14 = s->unk_14 - 1;
    }
    else
    {
        s->unk_14 = s->unk_14 + 1;
        return;
    }
    n = func_80049E1C(s->unk_04, d, s->unk_08);
    m = n / 5;
    if (n == s->unk_04)
    {
        return;
    }
    func_80053DA8(0x502);
    s->unk_04 = n;
    if (m != s->unk_0C)
    {
        s->unk_0C = m;
        func_800B1778(s->unk_28, m, n);
    }
    func_800B17C0(s->unk_28, n);
    func_800ADB30(s);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADFBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADFE8);

void func_800AE090(UnkTownADD80 *s, void *b, void *c)
{
    s->unk_14 = s->unk_14 - 1;
    if (s->unk_14 == 0)
    {
        if (*(s32 *)((u8 *)s + 0x1C) != 0)
        {
            *(void **)((u8 *)s - 0x10) = func_800ADB5C;
        }
        else
        {
            func_800ADB30(s);
            *(void **)((u8 *)s - 0x10) = func_800ADD80;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE0F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE148);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE1AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE2A4);

void func_800AE30C(void *p)
{
    if (p != NULL)
    {
        *(void **)((u8 *)p + 0x10) = func_800AE2A4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE324);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE414);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE484);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE494);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE4D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE4E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE500);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE510);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE518);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE534);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE550);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE56C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE5B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE600);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE630);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE734);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE854);

u32 func_800AEB9C(s32 a)
{
    return func_800491F4(a, D_800D1594, 0x1A);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEBC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AECA4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEE98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEEBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEEF0);

s32 func_800AEF5C(u8 *s, s32 n)
{
    s32 ok;

    ok = 0;
    *(u8 **)(s + 0xD8) = func_80049004((u8 **)(s + 0xDC), n, 0x8);
    if (*(u8 **)(s + 0xD8) != 0)
    {
        *(u32 *)(s + 0xBC) = D_8002E5E8[0];
        *(u32 *)(s + 0xC0) = D_8002E5E8[1];
        *(u32 *)(s + 0xC4) = D_8002E5E8[2];
        func_800491CC((u32 **)*(u8 **)(s + 0xD8), (u32)(s + 0xBC), 0x1A);
        ok = 1;
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEFD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF0E0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF148);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF1B4);

void func_800AF234(u8 *a)
{
    func_800AE56C(a + 0x20);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF254);

void func_800AF2F8(UnkTownGrid **p)
{
    s32 i;

    for (i = 0x1A; i < 0x24; i++)
    {
        p[i]->unk_04->unk_08 = (i - 0x1A) / 5 * 0x80 + 0x58;
        p[i]->unk_04->unk_0A = (i - 0x1A) % 5 * 0x10 + 0x88;
    }
}

void func_800AF36C(u8 *s)
{
    s32 i;

    i = *(s32 *)(*(u8 **)s + 0x18) % 10;
    (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_08 = i / 5 * 0x80 - 0x38;
    (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_0A = i % 5 * 0x10 + 0x8;
    (*(UnkTownGrid ***)(s + 0xA8))[5]->unk_04->unk_08 =
        (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_08 + 0x38;
    (*(UnkTownGrid ***)(s + 0xA8))[5]->unk_04->unk_0A =
        (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_0A - 0x9;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF448);

void func_800AF4AC(UnkTownGrid **p)
{
    s32 i;

    for (i = 0x6; i < 0x10; i++)
    {
        p[i]->unk_04->unk_08 = (i - 0x6) / 5 * 0x80 + -0x30;
        p[i]->unk_04->unk_0A = (i - 0x6) % 5 * 0x10 + 0x16;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF520);

void func_800AF600(UnkTownGrid **p)
{
    s32 i;

    for (i = 0x10; i < 0x1A; i++)
    {
        p[i]->unk_04->unk_08 = (i - 0x10) / 5 * 0x80 + 0x60;
        p[i]->unk_04->unk_0A = (i - 0x10) % 5 * 0x10 + 0x88;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF674);

void func_800AF734(UnkTownGrid **p)
{
    p[0x26]->unk_04->unk_08 = -0x40;
    p[0x26]->unk_04->unk_0A = 0x30;
    p[0x27]->unk_04->unk_08 = 0xC4;
    p[0x27]->unk_04->unk_0A = 0x30;
}

void func_800AF784(u8 *s)
{
    if (*(s32 *)(*(u8 **)s + 0x10) > 0)
    {
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x98) = D_800786DC;
        *(s32 *)(s + 0x14) &= ~2;
    }
    else
    {
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x98) = NULL;
        *(s32 *)(s + 0x14) |= 2;
    }
    if (*(s32 *)(*(u8 **)s + 0x10) < *(s32 *)(*(u8 **)s + 0x14))
    {
        *(s32 *)(s + 0x14) &= ~1;
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x9C) = D_800786E8;
    }
    else
    {
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x9C) = NULL;
        *(s32 *)(s + 0x14) |= 1;
    }
}

void func_800AF858(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF860);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF8B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF96C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF9C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFD00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFD5C);

void func_800AFDD0(u8 *p)
{
    func_80048E00(p, D_800D15B8);
    p[0x3C] |= 0x80;
}

u32 func_800AFE10(s32 a)
{
    UnkTownList buf;

    buf = D_800891F8;
    return func_800491F4(a, buf.unk_00, 0x28);
}

void func_800AFF00(UnkTownCardPair *p, u32 *a, u32 *b)
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
    p->unk_04 = (UnkTownCardPrim *)a;
    p->unk_08 = (UnkTownCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -60;
    p->unk_04->unk_0A = -60;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_06 = 0;
    p->unk_08->unk_08 = 0x30;
    p->unk_04->unk_0F = 4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFFA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0024);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0134);

s32 func_800B0158(u8 *s, void *p)
{
    func_800B0134(s);
    func_800AFF00((UnkTownCardPair *)(s + 0x98), (u32 *)(s + 0x70), (u32 *)(s + 0x80));
    return func_800B0024(s);
}

s32 func_800B0198(u8 *s, s32 n)
{
    s32 ok;

    ok = 0;
    *(u8 **)(s + 0xA8) = func_80049004((u8 **)(s + 0xAC), n, 0xC);
    if (*(u8 **)(s + 0xA8) != 0)
    {
        *(u32 *)(s + 0x8C) = D_8002E5E8[0];
        *(u32 *)(s + 0x90) = D_8002E5E8[1];
        *(u32 *)(s + 0x94) = D_8002E5E8[2];
        func_800491CC((u32 **)*(u8 **)(s + 0xA8), (u32)(s + 0x8C), 0x28);
        ok = 1;
    }
    return ok;
}

UnkNode *func_800B0214(void *p)
{
    UnkNode *o;
    u8 *v;

    o = func_8003FC64(0);
    if (o != 0)
    {
        v = (u8 *)o + 0x20;
        if (func_800B0198(v, 0x28) != 0)
        {
            o->unk_0C = (u32)((u8 *)o + 0xB8);
            *(u32 *)(v + 0xA4) = func_800AFE10(*(s32 *)(v + 0xA8));
            func_800B0158(v, p);
            func_8004491C(o, (u32)D_8004CAA0);
        }
        else
        {
            func_8004B248((u32 *)((u8 *)o + 0xCC));
            o->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            o = 0;
        }
    }
    return o;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B02D4);

void func_800B0318(UnkTownSlot **a)
{
    s32 i;

    for (i = 0x10; i < 0x1A; i++)
    {
        func_8004E99C(a[i]->unk_00);
        a[i]->unk_00 = 0;
    }
    for (i = 0x1A; i < 0x24; i++)
    {
        func_8004E99C(a[i]->unk_00);
        a[i]->unk_00 = 0;
    }
}

void func_800B03B4(UnkNode *o)
{
    u8 *v;

    if (o == 0)
    {
        return;
    }
    v = (u8 *)o + 0x20;
    func_8004B248((u32 *)((u8 *)o + 0xCC));
    func_800B0318(*(UnkTownSlot ***)(v + 0xA8));
    o->unk_1E |= 0x8000;
    D_800814A0 |= 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0418);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0424);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0490);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B05DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B06C4);

void func_800B0700(void *p)
{
    if (p != NULL)
    {
        *(void **)((u8 *)p + 0x10) = func_800B06C4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0718);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B074C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B07D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0824);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0870);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B08D4);

u32 func_800B091C(s32 a)
{
    return func_800491F4(a, D_800D15CC, 0x1F);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0944);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B09EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0BD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0C68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0D0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0D7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0E18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0F30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0F94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0FD4);

/* The 0x800B1xxx scene's layout pass. The object's grid-pointer array lives at
   +0xCC here, where the 0x800AFxxx and 0x800B2xxx scenes keep theirs at +0xA8
   (func_800AF784) and +0x5C (func_800B474C); otherwise this is the same
   vocabulary -- UnkTownGrid **, element +0x00 a pointer written through the
   u8 *** idiom and element->unk_04 the UnkTownPos whose unk_08/unk_0A are the
   on-screen x/y.

   Three things about the spelling are measured rather than chosen:

   * The array base is re-read as (*(UnkTownGrid ***)(s + 0xCC)) at EVERY
     access, 24 times, because that is what the target does. Binding it to a
     local would let gcc keep it in a register and drop 23 of the
     lw 0xCC($s0)s; the stores through the loaded pointers do not invalidate a
     register copy.

   * The loop counts DOWN from 4 and the three y expressions are written
     i * 0x10 + k. gcc 2.7.2 strength-reduces each of those into its own
     induction variable -- $a3 from 0x58, $t0 from 0x55 and $a1 from 0xC8, each
     -= 0x10 at the bottom -- while leaving the subscript as a plain
     sll $a0, $a2, 2 at the top, since the array base is reloaded each
     iteration and only i * 4 is loop-invariant. The two groups that share
     0x88 share $a1 by CSE.

   * func_800B08D4 takes the argument. Its own listing reads $a0 (into $s1,
     then $s1 + 0x48), and the call site has no argument move only because
     s + 0x34 is already in $a0 from the store two statements earlier. Writing
     the call with no argument measures 3 differing, so the argument is
     witnessed after all -- this is the inverse of the "a missing argument move
     is not evidence of a zero-arity call" trap.

   Arity: the sole caller is func_800B13D4 (still a stub), whose
   jal func_800B1188 has addu $a0, $s0, $zero in the delay slot and no other
   argument setup -- $a1/$a2 still hold the leftovers of its preceding
   func_800B0944 call. One parameter; the return value is discarded. */
void func_800B1188(u8 *s)
{
    s32 i;

    **(u8 ***)(*(u8 **)(s + 0xCC)) = D_800788E0;
    **(u8 ***)(*(u8 **)(s + 0xCC) + 0x4) = s + 0x34;
    (*(UnkTownGrid ***)(s + 0xCC))[1]->unk_04->unk_0A = 0x2F;
    func_800B08D4(s + 0x34);
    func_800B0C68(s);
    func_8004CBFC((u32 *)(s + 0x28), D_800792DC, (u32 *)(*(u8 ***)(s + 0xCC))[2]);
    **(u8 ***)(*(u8 **)(s + 0xCC) + 0xC) = D_800D15F4;
    for (i = 4; i >= 0; i--)
    {
        (*(UnkTownGrid ***)(s + 0xCC))[i + 4]->unk_04->unk_08 = -0x50;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 4]->unk_04->unk_0A = i * 0x10 + 0x18;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 9]->unk_04->unk_08 = 0x56;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 9]->unk_04->unk_0A = i * 0x10 + 0x15;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 14]->unk_04->unk_08 = 0x3C;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 14]->unk_04->unk_0A = i * 0x10 + 0x88;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 19]->unk_04->unk_08 = 0x8C;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 19]->unk_04->unk_0A = i * 0x10 + 0x88;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 24]->unk_04->unk_08 = 0x34;
        (*(UnkTownGrid ***)(s + 0xCC))[i + 24]->unk_04->unk_0A = i * 0x10 + 0x88;
    }
    func_800B0D0C(s);
    func_800B0FD4(s);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1390);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B13D4);

s32 func_800B1434(u8 *s, s32 n)
{
    s32 ok;

    ok = 0;
    *(u8 **)(s + 0xCC) = func_80049004((u8 **)(s + 0xD0), n, 0xC);
    if (*(u8 **)(s + 0xCC) != 0)
    {
        *(u32 *)(s + 0xB0) = D_8002E5E8[0];
        *(u32 *)(s + 0xB4) = D_8002E5E8[1];
        *(u32 *)(s + 0xB8) = D_8002E5E8[2];
        func_800491CC((u32 **)*(u8 **)(s + 0xCC), (u32)(s + 0xB0), 0x1F);
        ok = 1;
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B14B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B15B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1664);

void func_800B1718(void *p)
{
    if (p != NULL)
    {
        *(void **)((u8 *)p + 0x10) = func_800B1664;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1730);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B173C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1778);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B17C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B17E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B180C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1890);

void func_800B18F8(u8 *p)
{
    func_80048E00(p, D_800D161C);
    p[0x3C] |= 0x80;
}

u32 func_800B1938(s32 a)
{
    return func_800491F4(a, D_800D1624, 0x4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1960);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B19F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1B04);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1B10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1B70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1BEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1D00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1DBC);

void func_800B1DCC(UnkTown260C *s)
{
    s32 v[2];
    s32 i;
    s32 j;
    u8(*t)[2];
    u8(*u)[4];
    UnkTownB1DCCObj *o;
    UnkTownB1DCCPrim *p1;
    UnkTownB1DCCPrim *p2;
    u8 n1;
    u8 n2;
    s32 c1;
    u8 c2;
    s32 d;

    /* Three spellings here are load-bearing, all measured against the 81-word
       listing; each is worth the words named.
       1. The two table addresses must be held in LOCALS assigned before the
          loop (4 words). Left to loop.c they are hoisted as movables, which
          lands them AFTER the loop's own preamble statements, so `j = 0` comes
          out before the two lui/addiu pairs instead of after them: 5 differing.
       2. `j` must be an explicit byte offset, not `v[i]` (4 words). `v[i]`
          leaves loop.c's `4 * i` giv at benefit 0 ("not worth while", loop.c
          line 3823: 2 for the shift minus add_cost * biv_count), so it is
          recomputed with an in-loop `sll`: 4 differing with `v[i]`.
       3. `n1`, `n2` and `c2` must be `u8`, not `s32` (the whole function).
          loop runs before combine, so each zero-extension is still a separate
          RTL insn and the loop measures 59 real insns rather than 56. That is
          what keeps move_movables from hoisting the frame address `$fp + 16`
          out of the loop (threshold * savings * lifetime >= insn_count, with
          savings and lifetime both 1 and the threshold measured between 58 and
          63): hoisted, the array reference becomes a reduced DEST_ADDR giv and
          the store is `sw 0(t2)` off an `addiu t2, sp, 16` instead of
          `sw 0x10(v0)` off an in-loop `addu v0, sp, t2`. 82 words with any two
          of the three as `s32`, 84 if `c1` is narrowed too. */
    i = 0;
    t = D_800D162C;
    u = D_800D1630;
    j = 0;
    for (; i < 2; j += 4, i++)
    {
        n1 = t[s->unk_0C][i];
        o = (UnkTownB1DCCObj *)((UnkNode *)s->unk_00[i])->unk_0C;
        c1 = o->unk_04->unk_0C;
        /* Two statements, not one: as `n1 * 40 + 0x200 - c1` fold
           reassociates it to `n1 * 40 - (c1 - 0x200)` and the `addiu 0x200`
           becomes an `addiu -512` on c1 instead. */
        d = n1 * 40 + 0x200;
        d -= c1;
        p1 = o->unk_04;
        if (n1 != 0)
        {
            d++;
        }
        p1->unk_0C = c1 + (d >> 1);
        *(s32 *)((u8 *)v + j) = o->unk_04->unk_0C;

        n2 = t[s->unk_0C][i];
        p2 = o->unk_04;
        c2 = p2->unk_00;
        d = u[n2][0] - c2;
        if (n2 == 0)
        {
            d++;
        }
        p2->unk_00 = c2 + (d >> 1);
        o->unk_04->unk_01 = o->unk_04->unk_00;
        o->unk_04->unk_02 = o->unk_04->unk_00;
    }
    /* `v[0] > v[1]`, not `v[1] < v[0]`: the loads come out in source order. */
    if (v[0] > v[1])
    {
        func_80044AAC((UnkNode *)s->unk_00[1], (UnkNode *)s->unk_00[0]);
    }
    else
    {
        func_80044AAC((UnkNode *)s->unk_00[0], (UnkNode *)s->unk_00[1]);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1F10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1F48);

void func_800B1F80(UnkTown260C *s, s32 d)
{
    UnkTown260CE *e = &s->unk_1C[s->unk_0C];
    s32 n;
    s32 q;

    n = func_80049E1C(e->unk_08, d, e->unk_0C);
    if (n >= e->unk_0C)
    {
        e->unk_08 = 0;
        return;
    }
    if (n == e->unk_08)
    {
        return;
    }
    q = n / 10;
    e->unk_08 = n;
    func_800B1F48(s->unk_00[s->unk_0C], s->unk_0C);
    if (q != e->unk_00)
    {
        e->unk_00 = q;
        func_800B1F10(s->unk_00[s->unk_0C], s->unk_0C);
    }
}

void func_800B2068(UnkTown260C *s)
{
    UnkTown260CE *p = &s->unk_1C[s->unk_0C];
    s32 n;

    n = func_80049E1C(p->unk_08 - 1, 1, p->unk_0C);
    p->unk_08 = n;
    p->unk_00 = n / 10;
    func_800B1F48(s->unk_00[s->unk_0C], s->unk_0C);
    func_800B1F10(s->unk_00[s->unk_0C], s->unk_0C);
    func_800B1F10(s->unk_00[s->unk_18], s->unk_18);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2130);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2164);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2190);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2214);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2244);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2280);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2344);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2370);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2394);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B23C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2400);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B25D8);

void func_800B260C(UnkTown260C *s, void *b, void *c)
{
    Unk80083160 *g = &D_80083160;
    s32 d = 0;
    s32 r;

    if (g->unk_08 != 0)
    {
        if (g->unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            func_800B2CE8((u8 *)s - 0x20);
        }
        else if (g->unk_10 & 0x40)
        {
            r = func_800B2400(s);
            if (r == 0)
            {
                func_80053DA8(0x503);
                func_800B2068(s);
            }
            else if (r == 3)
            {
                func_80053DA8(0x506);
            }
        }
        else if (g->unk_10 & 0x80)
        {
            func_80053DA8(0x503);
        }
        else if (g->unk_10 & 0x10)
        {
            func_80053DA8(0x503);
            func_800B25D8(s);
            func_800B2068(s);
        }
        else if (g->unk_10 & 0x4)
        {
            func_800B2394(s, 0);
        }
        else if (g->unk_10 & 0x1)
        {
            func_800B2394(s, 1);
        }
        else if (g->unk_08 & 0xF000)
        {
            if (g->unk_10 & 0xF000)
            {
                s->unk_10 = 0;
                if (g->unk_10 & 0x8000)
                {
                    d = -5;
                }
                else if (g->unk_10 & 0x2000)
                {
                    d = 5;
                }
                else if (g->unk_10 & 0x1000)
                {
                    d = -1;
                }
                else if (g->unk_10 & 0x4000)
                {
                    d = 1;
                }
            }
            else if (s->unk_10 >= 5)
            {
                if (g->unk_08 & 0x8000)
                {
                    d = -5;
                }
                else if (g->unk_08 & 0x2000)
                {
                    d = 5;
                }
                else if (g->unk_08 & 0x1000)
                {
                    d = -1;
                }
                else if (g->unk_08 & 0x4000)
                {
                    d = 1;
                }
                s->unk_10 = s->unk_10 - 1;
            }
            else
            {
                s->unk_10 = s->unk_10 + 1;
            }
            if (d != 0)
            {
                func_80053DA8(0x502);
                func_800B1F80(s, d);
            }
        }
    }
    func_800B1DCC(s);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2834);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008917C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800891F4);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800891F8);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008925C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B28A0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2918);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2964);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B29A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B29D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2A38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2B60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2BA0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2C6C);

void func_800B2CE8(void *p)
{
    if (p != NULL)
    {
        *(void **)((u8 *)p + 0x10) = func_800B2C6C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2D00);

/* Two copies of one source. 0x800B2D00..0x800B35xx and
   0x800B4720..0x800B4C38 are the same translation unit compiled twice over a
   different object layout -- the record pointer sits at +0xA8 in the first and
   +0x5C in the second, and the inner record's fields are at +0x1C/+0x3C
   against +0x2C/+0x40. Every function below has a twin in the other half, and
   the twin is the same C with those four offsets substituted; func_800B2D00,
   func_800B2FF8 and func_800B3178 are SHARED by both halves rather than
   duplicated, which is what makes the two halves one unit and not two.

   families.py groups five of these seven pairs. It does not group the other
   two -- func_800B2DA0/func_800B4720 and func_800B30D8/func_800B4978, 11 and
   12 words -- only because they fall below worklist.py's reporting cut, not
   because their shapes differ. All fourteen measured 0 differing on the first
   compile, from one reading of each pair. */

/* A pure forward: the inner record is dereferenced once, its +0x3C word read,
   and the record itself advanced to +0x1C. $a0 is untouched and passed
   straight through, which is why the 11 words hold no argument move for it. */
void func_800B2DA0(u32 *a, u8 **b)
{
    func_800B2D00(a, *b + 0x1C, *(u32 **)(*b + 0x3C));
}

void func_800B2DCC(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + 0x58;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x88;
    }
}

void func_800B2E3C(u8 *s)
{
    s32 i;

    i = *(s32 *)(*(u8 **)s + 0x24) % 10;
    (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_08 = i / 5 * 0x80 - 0x38;
    (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_0A = i % 5 * 0x10 + 0x8;
    (*(UnkTownGrid ***)(s + 0xA8))[5]->unk_04->unk_08 =
        (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_08 + 0x38;
    (*(UnkTownGrid ***)(s + 0xA8))[5]->unk_04->unk_0A =
        (*(UnkTownGrid ***)(s + 0xA8))[4]->unk_04->unk_0A - 0x9;
}

void func_800B2F18(u8 *a)
{
    func_800B2E3C(a + 0x20);
}

/* Installs the row's primitive block and then plants a resident callback
   through a DOUBLE indirection: +0x14 of the record holds a pointer to the
   slot that receives D_80071244's address. The record pointer is loaded twice,
   once per statement. */
void func_800B2F38(u8 *s)
{
    func_8004CBFC((u32 *)(s + 0x1C), D_800792DC, *(u32 **)(*(u8 **)(s + 0xA8) + 0x10));
    **(u32 **)(*(u8 **)(s + 0xA8) + 0x14) = (u32)D_80071244;
}

void func_800B2F88(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + -0x30;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x16;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2FF8);

/* func_800B2DA0 one level in: the same three arguments, but the first comes
   from the outer object's record pointer at +0xA8 rather than from the
   caller. */
void func_800B30D8(u8 *s)
{
    func_800B2FF8((u32 *)(*(u8 **)(s + 0xA8) + 0x18), *(u8 **)s + 0x1C,
                  *(u32 **)(*(u8 **)s + 0x3C));
}

void func_800B3108(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + 0x60;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x88;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3178);

/* Releases the slot handles and then rebinds the row. The record pointer at
   +0xA8 is RE-DEREFERENCED for the second call rather than cached across the
   first: the target loads 0xA8($s0) twice, which is the unit's standing
   "re-dereference rather than cache" shape and not a missed optimisation. */
void func_800B32D4(u8 *s)
{
    func_800B39E4(*(UnkTownSlot ***)(s + 0xA8));
    func_800B3178((u32 *)(*(u8 **)(s + 0xA8) + 0x40), *(u8 **)s + 0x1C,
                  *(u32 **)(*(u8 **)s + 0x3C));
}

void func_800B331C(UnkTownGrid **p)
{
    p[0x26]->unk_04->unk_08 = -0x40;
    p[0x26]->unk_04->unk_0A = 0x30;
    p[0x27]->unk_04->unk_08 = 0xC4;
    p[0x27]->unk_04->unk_0A = 0x30;
}

void func_800B336C(u8 *s)
{
    if (*(s32 *)(*(u8 **)s + 0x1C) > 0)
    {
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x98) = D_800786DC;
        *(s32 *)(s + 0x14) &= ~2;
    }
    else
    {
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x98) = NULL;
        *(s32 *)(s + 0x14) |= 2;
    }
    if (*(s32 *)(*(u8 **)s + 0x1C) < *(s32 *)(*(u8 **)s + 0x20))
    {
        *(s32 *)(s + 0x14) &= ~1;
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x9C) = D_800786E8;
    }
    else
    {
        **(u8 ***)(*(u8 **)(s + 0xA8) + 0x9C) = NULL;
        *(s32 *)(s + 0x14) |= 1;
    }
}

/* The half's per-frame driver: three one-argument calls on the object and
   then the forward through func_800B2DA0 with the record's +0x68 subobject.
   func_800B3488 calls it as func_800B3440(a + 0x20), the actor-view idiom. */
void func_800B3440(u8 *s)
{
    func_800B30D8(s);
    func_800B336C(s);
    func_800B32D4(s);
    func_800B2DA0((u32 *)(*(u8 **)(s + 0xA8) + 0x68), (u8 **)s);
}

void func_800B3488(u8 *a)
{
    func_800B3440(a + 0x20);
}

/* The teardown handler func_800B34FC installs: drops the primitive block and
   releases the two sprite handles at +0x98 and +0x9C of the record. The second
   argument to both func_800497F4 calls is a literal 0, which is why `addu
   $a1, $zero, $zero` fills each jal's delay slot. */
void func_800B34A8(u8 *s)
{
    func_8004CB2C(s + 0x1C);
    func_800497F4(*(u8 **)(*(u8 **)(s + 0xA8) + 0x98), 0);
    func_800497F4(*(u8 **)(*(u8 **)(s + 0xA8) + 0x9C), 0);
}

/* A six-frame open animation that hands over to the teardown handler on the
   last frame. The counter at +0x08 drives a width: (i << 12) / 5 is written as
   an s16 at +0x06 of the record at +0x9C, which is the 0x66666667 magic with
   post_shift 1 (tools/decomp/magic.py). The divide is SIGNED -- the
   `sra $v0, $v0, 31` before the subtract is the sign correction -- even though
   the counter only ever runs 0..5, so the source says `/ 5` on an s32 and not
   a shift.

   The counter is read a SECOND time for the increment rather than reused from
   the divide: the target has `lw $v0, 0x8($a1)` twice with a nop between, so
   the store is its own statement and the incremented value goes through a
   local, which is also what gives the `slti` its register.

   The handler slot is at s - 0x10, a NEGATIVE offset: the parameter points
   into the middle of its object (func_800B3488 passes a + 0x20), so the slot
   being written is +0x10 of the object proper. */
void func_800B34FC(u8 *s)
{
    s32 i;

    *(s16 *)(*(u8 **)(s + 0x9C) + 6) = (*(s32 *)(s + 8) << 12) / 5;
    i = *(s32 *)(s + 8) + 1;
    *(s32 *)(s + 8) = i;
    if (i >= 6)
    {
        *(u32 *)(s - 0x10) = (u32)func_800B34A8;
    }
}

void func_800B3558(u8 *p)
{
    func_80048E00(p, D_800D1650);
    p[0x3C] |= 0x80;
}

u32 func_800B3598(s32 a)
{
    UnkTownList buf;

    buf = D_8008926C;
    return func_800491F4(a, buf.unk_00, 0x28);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3688);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3734);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B384C);

s32 func_800B3868(u8 *s, void *p)
{
    func_800B384C(s);
    func_800B3688((UnkTownCardPair *)(s + 0x98), (u32 *)(s + 0x70), (u32 *)(s + 0x80));
    return func_800B3734(s);
}

s32 func_800B38A8(u8 *s, s32 n)
{
    s32 ok;

    ok = 0;
    *(u8 **)(s + 0xA8) = func_80049004((u8 **)(s + 0xAC), n, 0xC);
    if (*(u8 **)(s + 0xA8) != 0)
    {
        *(u32 *)(s + 0x8C) = D_8002E5E8[0];
        *(u32 *)(s + 0x90) = D_8002E5E8[1];
        *(u32 *)(s + 0x94) = D_8002E5E8[2];
        func_800491CC((u32 **)*(u8 **)(s + 0xA8), (u32)(s + 0x8C), 0x28);
        ok = 1;
    }
    return ok;
}

UnkNode *func_800B3924(void *p)
{
    UnkNode *o;
    u8 *v;

    o = func_8003FC64(0);
    if (o != 0)
    {
        v = (u8 *)o + 0x20;
        if (func_800B38A8(v, 0x28) != 0)
        {
            o->unk_0C = (u32)((u8 *)o + 0xB8);
            *(u32 *)(v + 0xA4) = func_800B3598(*(s32 *)(v + 0xA8));
            func_800B3868(v, p);
            func_8004491C(o, (u32)D_8004CAA0);
        }
        else
        {
            func_8004B248((u32 *)((u8 *)o + 0xCC));
            o->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            o = 0;
        }
    }
    return o;
}

void func_800B39E4(UnkTownSlot **a)
{
    s32 i;

    for (i = 0x10; i < 0x1A; i++)
    {
        func_8004E99C(a[i]->unk_00);
        a[i]->unk_00 = 0;
    }
    for (i = 0x1A; i < 0x24; i++)
    {
        func_8004E99C(a[i]->unk_00);
        a[i]->unk_00 = 0;
    }
}

void func_800B3A80(UnkNode *o)
{
    u8 *v;

    if (o == 0)
    {
        return;
    }
    v = (u8 *)o + 0x20;
    func_8004B248((u32 *)((u8 *)o + 0xCC));
    func_800B39E4(*(UnkTownSlot ***)(v + 0xA8));
    o->unk_1E |= 0x8000;
    D_800814A0 |= 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3AE4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3B18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3C04);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3C74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3D04);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3D84);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3DC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3E30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3E68);

u32 func_800B3EC4(s32 a)
{
    return func_800491F4(a, D_800D16BC, 0x21);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3EEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3FB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B40E0);

void func_800B4188(u8 *p)
{
    memcpy(p, D_800D16FC, 0x30);
    memcpy(p + 0x30, D_800D16D8, 0xC);
}

void func_800B426C(u8 *p)
{
    memcpy(p, D_800D172C, 0x30);
    memcpy(p + 0x30, D_800D16D8, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4350);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B43E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4494);

s32 func_800B44B0(u8 *s, void *p)
{
    func_800B4494(s);
    func_800B3EEC((UnkTownCardPair *)(s + 0x4C), (u32 *)(s + 0x24), (u32 *)(s + 0x34));
    return func_800B43E8(s);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B44F0);

UnkNode *func_800B4588(void *p)
{
    UnkNode *o;
    u8 *v;

    o = func_8003FC64(0);
    if (o != 0)
    {
        v = (u8 *)o + 0x20;
        if (func_800B44F0(v, 0x21) != 0)
        {
            o->unk_0C = (u32)((u8 *)o + 0x6C);
            *(u32 *)(v + 0x58) = func_800B3EC4(*(s32 *)(v + 0x5C));
            func_800B44B0(v, p);
            func_8004491C(o, (u32)D_8004CAA0);
        }
        else
        {
            func_8004B248((u32 *)((u8 *)o + 0x80));
            o->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            o = 0;
        }
    }
    return o;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4648);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B46CC);

/* func_800B2DA0's twin; see the cluster note on that function. */
void func_800B4720(u32 *a, u8 **b)
{
    func_800B2D00(a, *b + 0x2C, *(u32 **)(*b + 0x40));
}

void func_800B474C(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + 0x58;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x88;
    }
}

void func_800B47BC(u8 *s)
{
    s32 i;

    i = *(s32 *)(*(u8 **)s + 0x34) % 10;
    (*(UnkTownGrid ***)(s + 0x5C))[4]->unk_04->unk_08 = i / 5 * 0x80 - 0x38;
    (*(UnkTownGrid ***)(s + 0x5C))[4]->unk_04->unk_0A = i % 5 * 0x10 + 0x8;
    (*(UnkTownGrid ***)(s + 0x5C))[5]->unk_04->unk_08 =
        (*(UnkTownGrid ***)(s + 0x5C))[4]->unk_04->unk_08 + 0x38;
    (*(UnkTownGrid ***)(s + 0x5C))[5]->unk_04->unk_0A =
        (*(UnkTownGrid ***)(s + 0x5C))[4]->unk_04->unk_0A - 0x9;
}

void func_800B4898(u8 *a)
{
    func_800B47BC(a + 0x20);
}

/* func_800B2F38's twin; see the cluster note on func_800B2DA0. Note the
   first argument is s + 0x14 here against s + 0x1C there, so the +0x1C/+0x2C
   substitution that governs the inner record does NOT govern this offset --
   the two objects differ in more than one field. */
void func_800B48B8(u8 *s)
{
    func_8004CBFC((u32 *)(s + 0x14), D_800792DC, *(u32 **)(*(u8 **)(s + 0x5C) + 0x10));
    **(u32 **)(*(u8 **)(s + 0x5C) + 0x14) = (u32)D_80071244;
}

void func_800B4908(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + -0x30;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x16;
    }
}

/* func_800B30D8's twin; see the cluster note on func_800B2DA0. */
void func_800B4978(u8 *s)
{
    func_800B2FF8((u32 *)(*(u8 **)(s + 0x5C) + 0x18), *(u8 **)s + 0x2C,
                  *(u32 **)(*(u8 **)s + 0x40));
}

void func_800B49A8(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + 0x60;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x88;
    }
}

/* func_800B32D4's twin; see the cluster note on func_800B2DA0. The releaser
   differs -- func_800B5020 for func_800B39E4 -- because each half has its own
   copy of that one, while func_800B3178 is shared. */
void func_800B4A18(u8 *s)
{
    func_800B5020(*(UnkTownSlot ***)(s + 0x5C));
    func_800B3178((u32 *)(*(u8 **)(s + 0x5C) + 0x40), *(u8 **)s + 0x2C,
                  *(u32 **)(*(u8 **)s + 0x40));
}

void func_800B4A60(UnkTownGrid **p)
{
    p[0x26]->unk_04->unk_08 = -0x40;
    p[0x26]->unk_04->unk_0A = 0x30;
    p[0x27]->unk_04->unk_08 = 0xC4;
    p[0x27]->unk_04->unk_0A = 0x30;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4AB0);

/* func_800B3440's twin; see the cluster note on func_800B2DA0. Its second
   call is func_800B4AB0, the twin of func_800B336C -- which is the one
   position where the two halves' call lists are not parallel in NAME, because
   func_800B336C is shared by nothing and func_800B4AB0 is its copy. */
void func_800B4B7C(u8 *s)
{
    func_800B4978(s);
    func_800B4A18(s);
    func_800B4AB0(s);
    func_800B4720((u32 *)(*(u8 **)(s + 0x5C) + 0x68), (u8 **)s);
}

void func_800B4BC4(u8 *a)
{
    func_800B4B7C(a + 0x20);
}

/* func_800B34A8's twin; see the cluster note on func_800B2DA0. */
void func_800B4BE4(u8 *s)
{
    func_8004CB2C(s + 0x14);
    func_800497F4(*(u8 **)(*(u8 **)(s + 0x5C) + 0x98), 0);
    func_800497F4(*(u8 **)(*(u8 **)(s + 0x5C) + 0x9C), 0);
}

/* func_800B34FC's twin; see the cluster note on func_800B2DA0. The record
   pointer it writes through is at +0x50 here against +0x9C there -- a third
   offset that does not follow the +0xA8/+0x5C substitution. */
void func_800B4C38(u8 *s)
{
    s32 i;

    *(s16 *)(*(u8 **)(s + 0x50) + 6) = (*(s32 *)(s + 8) << 12) / 5;
    i = *(s32 *)(s + 8) + 1;
    *(s32 *)(s + 8) = i;
    if (i >= 6)
    {
        *(u32 *)(s - 0x10) = (u32)func_800B4BE4;
    }
}

u32 func_800B4C94(s32 a)
{
    return func_800491F4(a, D_800D1760, 0x29);
}

void func_800B4CBC(UnkTownCardPair *p, u32 *a, u32 *b)
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
    p->unk_04 = (UnkTownCardPrim *)a;
    p->unk_08 = (UnkTownCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -60;
    p->unk_04->unk_0A = -60;
    p->unk_04->unk_0C = 0x228;
    p->unk_04->unk_06 = 0;
    p->unk_08->unk_08 = 0x30;
    p->unk_04->unk_0F = 4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4D64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4E7C);

s32 func_800B4E98(u8 *s, void *p)
{
    func_800B4E7C(s);
    func_800B4CBC((UnkTownCardPair *)(s + 0x4C), (u32 *)(s + 0x24), (u32 *)(s + 0x34));
    return func_800B4D64(s);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4ED8);

UnkNode *func_800B4F60(void *p)
{
    UnkNode *o;
    u8 *v;

    o = func_8003FC64(0);
    if (o != 0)
    {
        v = (u8 *)o + 0x20;
        if (func_800B4ED8(v, 0x29) != 0)
        {
            o->unk_0C = (u32)((u8 *)o + 0x6C);
            *(u32 *)(v + 0x58) = func_800B4C94(*(s32 *)(v + 0x5C));
            func_800B4E98(v, p);
            func_8004491C(o, (u32)D_8004CAA0);
        }
        else
        {
            func_8004B248((u32 *)((u8 *)o + 0x80));
            o->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            o = 0;
        }
    }
    return o;
}

void func_800B5020(UnkTownSlot **a)
{
    s32 i;

    for (i = 0x10; i < 0x1A; i++)
    {
        func_8004E99C(a[i]->unk_00);
        a[i]->unk_00 = 0;
    }
    for (i = 0x1A; i < 0x24; i++)
    {
        func_8004E99C(a[i]->unk_00);
        a[i]->unk_00 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B50BC);

/* The gauge-width helper func_800B5174 initialises each of its two 0xC-byte
   D_800D1794 records with: a value below 300 scales to v = n / 4 + 1 and
   anything at or above it clamps to 0x40, then +0x02 gets 0x40 - v and +0x0A
   gets v. The record pointer comes back in $v0.

   The parameter is `s16`: the `sll`/`sra 16` pair at words 0-1 is its
   sign-extension, sitting at entry because the `slti` is its first int use.
   Both `s16` and `s32` measure 0 differing at func_800B5174's call site, which
   passes an `lbu` result, so the callee's own entry shifts are the only
   witness -- see the lever on a narrow parameter witnessed at the call site,
   which is this case running the other way. The divide is signed, which is
   where the `bgez`/`addiu 3` bias pair comes from. */
u8 *func_800B512C(u8 *q, s16 n)
{
    s32 v;

    if (n < 300)
    {
        v = n / 4 + 1;
    }
    else
    {
        v = 0x40;
    }
    q[2] = 0x40 - v;
    q[0xA] = v;
    return q;
}

/* The first of func_800B5AC8's four panel builders, and the only one that
   configures the panel rather than filling it with text: a size pair and an
   owner word into each of b[0], b[1], b[2], then two 0xC-byte records of
   D_800D1794 initialised from two byte fields of the record, then `b` back.

   THE THIRD PARAMETER IS REAL AND IT COULD NOT BE SEEN FROM THE CALL SITE.
   `sw $a2, 0x0($v0)` stores a register this function never sets, and
   func_800B5AC8 forwards its own third argument into it -- this is its FIRST
   call, so $a0/$a1/$a2 are already in place and no argument-setup insn is
   emitted. func_800B5AC8 therefore measures 22 words / 0 differing with two
   parameters and with three, identically, and cannot witness the arity in
   either direction. The witness is the GRANDCALLER: func_800B6508, still a
   stub, does `lw $a0, 0x4($s0) / lw $a1, 0x98($s0) / lw $a2, 0x14($s0)`
   before `jal func_800B5AC8` and the same three before `jal func_800B640C`.
   See docs/matching-levers.md, "A STORED ARGUMENT REGISTER THE PROTOTYPE DOES
   NOT HAVE IS A FORWARDED PARAMETER".

   Two spellings reach 0 differing WITHOUT the parameter and are both wrong:
   `register s32 c __asm__("$6")` (a device with nothing left to justify it)
   and a bare uninitialised `s32 c;` (undefined behaviour that happens to
   allocate the pseudo to $a2 here). Reject both on the func_800B6508
   evidence.

   `return b` is worth FOUR WORDS: as void with the return dropped this is 56
   words, 57 differing. D_800D1794's address is used three times, so the
   lui/addiu pair lands in $s0 across both calls with no pointer local. */
UnkTownCardPair **func_800B5174(u8 *p, UnkTownCardPair **b, s32 c)
{
    b[0]->unk_04->unk_08 = -0x38;
    b[0]->unk_04->unk_0A = 8;
    b[0]->unk_00 = (s32)D_80077FBC;
    b[1]->unk_04->unk_08 = -0x38;
    b[1]->unk_04->unk_0A = 8;
    b[1]->unk_00 = c;
    b[2]->unk_04->unk_08 = -0x18;
    b[2]->unk_04->unk_0A = 0x30;
    b[2]->unk_00 = (s32)D_800D1794;
    func_800B512C(D_800D1794, p[0x28]);
    func_800B512C(D_800D1794 + 0xC, p[0x29]);
    return b;
}

void func_800B5264(UnkTownCardPair *a, u8 *name, s32 b, s16 x, s32 y)
{
    if (a->unk_00 != 0)
    {
        func_8004E99C(a->unk_00);
        a->unk_00 = 0;
    }
    a->unk_04->unk_08 = x;
    a->unk_04->unk_0A = y;
    a->unk_00 = func_8004DC14(name, b);
    a->unk_04->unk_0E = 2;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089268);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008926C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089298);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892C0);

/* The item detail panel, and func_800B5D70's near-sibling: one 0x40-byte text
   buffer at sp+0x18 seeded by an inline 5-byte block move, four escape/name
   strings appended, one text sprite through func_800B5264. The mechanisms are
   the family's and are explained on func_800B5E28; each was re-measured here.

   - The seed is gcc's builtin 5-byte expansion (lw/lb then sw/sb), not a call.
     D_800892C0's INCLUDE_RODATA survives this landing, so the literal
     `memcpy(buf, "\t8\t8", 5)` is 0 differing in .text and then emits a
     SECOND padded copy: measured at the object level, .rodata grows
     0x2524 -> 0x252c and 392 of 597 records shift. `strcpy` on the symbol
     gives a real jal, 45 words / 41 differing.

   - D_800892C0 needs its 4-byte element type: through
     `extern const u8 D_800892C0[]` move_by_pieces takes MIN(src, dst)
     alignment and the pair becomes lwl/lwr + swl/swr, 52 words / 42 differing.

   - D_800814A8 HOLDS an address; it is not an array. `(u8 *)(D_800814A8 +
     0x34)` gives the listing's lui / lw / addiu. `(u8 *)&D_800814A8 + 0x34`
     takes the symbol's own address and emits lui / addiu only: 49 words, 36
     differing. See docs/matching-levers.md, "A GLOBAL THAT HOLDS AN ADDRESS IS
     NOT A GLOBAL THAT IS AN ARRAY".

   - D_800892C8 has no .s listing of its own, so splat migrated it into this
     listing and replacing the INCLUDE_ASM loses it. Declared [4] for the
     listing's four bytes (0A093800) and trailing `.align 2`. `[]` happens to
     be harmless here -- the follower D_800892CC.s is three `.word` directives
     and GAS re-aligns those -- but [4] does not depend on a neighbour.

   - func_800A652C never reads its incoming $a0, but this call site's
     `addu $a0, $s1, $zero` sits in the jal's delay slot and the very next
     instruction reloads $a0 from $s0, so the argument is real. That is why
     the unit declares it with a parameter and func_800B5D30 forwards one. */
const u8 D_800892C8[4] = "\n\t8";

void func_800B52F4(u8 *p, UnkTownCardPair **b)
{
    u8 buf[0x40];

    memcpy(buf, D_800892C0, 5);
    func_80069E38(buf, (u8 *)(D_800814A8 + 0x34));
    func_80069E38(buf, (u8 *)D_800892C8);
    func_80069E38(buf, D_800892CC);
    func_80069E38(buf, D_800892D8);
    func_80069E38(buf, func_800A652C(p));
    func_800B5264(b[3], buf, 0, 0x47, 0x88);
}

void func_800B53BC(UnkTownCardPair *a, u8 *name, s32 b, s16 x, s32 y)
{
    if (a->unk_00 != 0)
    {
        func_8004B1A4(a->unk_00);
        a->unk_00 = 0;
    }
    a->unk_04->unk_08 = x;
    a->unk_04->unk_0A = y;
    a->unk_00 = func_8004E928(name, b);
    a->unk_04->unk_0E = 1;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892CC);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892D8);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892DC);

/* The experience line of the monster panel, and func_800B557C's sibling:
   memcpy the literal "EXP\x08" (D_800892DC) to the head of the caller's
   buffer, append a decimal through the font renderer, then draw
   func_80043868(p)'s 0..0x14 progress value as n/2 filled bar cells with
   D_800892E8 for the odd half.

   `n` MUST be s16 and the bound MUST be written `i < n / 2` rather than
   hoisted into a local. The target computes `sra $v1,$v1,1`, `blez $v1`, then
   a SEPARATE `addu $s1,$v1,$zero`; a named `half` local folds the `sra`
   straight into $s1 and loses that copy -- 67 words, one short. Writing `n` as
   s32 with a `(s16)n / 2` cast is 68 words but swaps $s3/$s4 (5 differing), so
   the declared width is load-bearing, not cosmetic.

   func_8004E634 is `void` in csrc/unk_1C540.c:4129, but this TU uses its $v0
   as a pointer and no header declares it, so it is declared `u8 *` here --
   separate translation units, no conflict.

   D_800892E4 ("m") is referenced only from this listing, so splat migrated it
   here and replacing the INCLUDE_ASM loses it; declared [4] for the trailing
   pad. The three other D_8008 records keep their own INCLUDE_RODATA lines. */
const u8 D_800892E4[4] = "m";

u8 *func_800B544C(u8 *s, u8 *p)
{
    u8 buf[0x40];
    s16 n;
    s32 i;

    memcpy(s, D_800892DC, 5);
    func_80069E38(s, func_8004E634(1, buf));
    n = func_80043868(p);
    for (i = 0; i < n / 2; i++)
    {
        func_80069E38(s, (u8 *)D_800892E4);
    }
    if (n & 1)
    {
        func_80069E38(s, D_800892E8);
    }
    func_80069E38(s, D_800892EC);
    func_80069E38(s, func_8004E634(0, buf));
    return s;
}

/* The font-colour selector func_800B557C uses twice: a three-way compare of
   a current value against its maximum. Equal gives 0 (the default colour),
   current below maximum gives 4 and current above it gives 3, and the
   comparison is SIGNED (`slt`, not `sltu`).

   The shape is load-bearing: one variable seeded 0 with a nested if/else and
   a single `return d`, not three early returns. Three `return`s give nine
   words with a `j` to a shared tail (8 differing); the accumulator form puts
   0 in the first branch's delay slot, 3 in the second's, and leaves the one
   `addu $v0, $v1, $zero` in the jr's slot. */
s32 func_800B555C(s32 a, s32 b)
{
    s32 d = 0;

    if (a != b)
    {
        if (a < b)
        {
            d = 4;
        }
        else
        {
            d = 3;
        }
    }
    return d;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892E8);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892EC);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892F0);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892F4);

/* A two-row stat panel written into the caller's buffer: "  ", then the two
   current values p[0x26]/p[0x27] colourised against their maxima
   p[0x68]/p[0x69] and separated by D_800892F8 (" ") and D_800892FC ("    "),
   then D_800892D8 ("\n") and D_80089304 ("   "), then the two maxima plain.
   D_800892F4 is the single byte 0x08, the font's colour-escape prefix, and
   func_8004E634(n, out) renders the colour index after it, so "\b"+d sets a
   colour and "\b"+"0" resets it. func_800B555C(max, cur) picks it: 0 when
   they match, 3 when cur < max, 4 when cur > max. The caller's D_80089320
   header reads "ATK    DEF\nMAX    MAX", the same two-row geometry.

   THIS LISTING OWNS TWO SYMBOLS AND BORROWS FOUR. D_800892F8 and D_800892FC
   have no .s file of their own, so they live inside this function's
   `.section .rodata` block and are lost when the INCLUDE_ASM goes -- they are
   defined below at the listing's declared sizes, 4 and 8 (0x800892FC + 8 ==
   0x80089304, the next record). D_800892D8, D_800892F0, D_800892F4 and
   D_80089304 all keep their own surviving INCLUDE_RODATA lines and are
   externs. Note D_800892F0 and D_800892F4 are referenced ONLY from here and
   splat still gave them their own listings, so "referenced once" is not the
   test -- whether the symbol has its own .s file is.

   `num` must be u8[0x10]: u8[0x40] or u8[0x8] gives frame 0x68 or too small
   against the target's 0x38, moving every sw/lw displacement (14 differing).
   The seed copy must go through D_800892F0, not a literal -- `strcpy(s, "  ")`
   is 0 differing in .text and emits a second copy of those bytes beside the
   surviving INCLUDE_RODATA, growing .rodata by 8. Unlike func_800B5E28 the
   source element type is irrelevant here: the destination is a u8 * parameter,
   so move_by_pieces' MIN(src,dst) alignment is 1 either way. */
const u8 D_800892F8[4] = " ";
const u8 D_800892FC[8] = "    ";

u8 *func_800B557C(u8 *s, u8 *p)
{
    u8 num[0x10];
    s32 d;

    memcpy(s, D_800892F0, 3);
    d = func_800B555C(p[0x68], p[0x26]);
    if (d != 0)
    {
        func_80069E38(s, D_800892F4);
        func_80069E38(s, func_8004E634(d, num));
    }
    func_80069E38(s, (u8 *)D_800892F8);
    func_8004E5A0(p[0x26], 3, num);
    func_80069E38(s, num);
    func_80069E38(s, D_800892F4);
    func_80069E38(s, func_8004E634(0, num));
    func_80069E38(s, (u8 *)D_800892FC);
    d = func_800B555C(p[0x69], p[0x27]);
    if (d != 0)
    {
        func_80069E38(s, D_800892F4);
        func_80069E38(s, func_8004E634(d, num));
    }
    func_8004E5A0(p[0x27], 3, num);
    func_80069E38(s, num);
    func_80069E38(s, D_800892F4);
    func_80069E38(s, func_8004E634(0, num));
    func_80069E38(s, D_800892D8);
    func_80069E38(s, D_80089304);
    func_8004E5A0(p[0x68], 3, num);
    func_80069E38(s, num);
    func_80069E38(s, (u8 *)D_800892FC);
    func_8004E5A0(p[0x69], 3, num);
    func_80069E38(s, num);
    return s;
}

/* The equipment detail panel, and func_800B5E28's near-sibling: one 0x40-byte
   text buffer assembled from byte fields of the record plus the font
   renderer's escape strings, then text sprites placed through func_800B53BC.
   Three sprites here against that one's five, and no func_800B5264 call, so
   every text sprite goes through func_800B53BC.

   Everything load-bearing is already explained in the comment on
   func_800B5E28; it holds here, and each piece was re-measured on this
   function:

   - The first write into `buf` is gcc's INLINE 5-byte block move (lw/lb then
     sw/sb) of the string D_80089308 holds, not a call. The original source
     was `strcpy(buf, "\t7\f4")` and that spelling also measures 0 differing
     in .text -- but D_80089308 only exists as a standalone rodata symbol
     because splat could not attribute the literal that this function and
     func_800B5E28 share, so the INCLUDE_RODATA survives and the literal emits
     a SECOND copy of those bytes. Measured at the object level on the merged
     unit: .rodata grows 0x2524 -> 0x252c and every record from 0xBA0 on
     shifts, which no .text sweep can see. Hence `memcpy(buf, D_80089308, 5)`
     against the existing symbol.

   - D_80089308 must be declared with a 4-byte element type. move_by_pieces
     takes MIN(src_align, dst_align), so `extern u8 D_80089308[]` turns that
     lw/sw pair into lwl/lwr + swl/swr: 83 words, 73 differing.

   - `num` must be declared BEFORE `buf`. Locals are assigned upward in
     declaration order, so num takes the sp+0x18 slot and buf the sp+0x58 one;
     swapping the declarations gives 84 words, 81 differing.

   Unlike the sibling this function owns no rodata of its own -- no
   `.section .rodata` block and no jump table in its listing -- so none of
   that family's declared-size or `.align 3` traps apply. 0x98, 0xB8 and 0xC0
   are each used once as the fifth argument, which is why all three are
   materialised in $v0 rather than parked in a saved register.  */
void func_800B5774(u8 *p, UnkTownCardPair **b)
{
    u8 num[0x40];
    u8 buf[0x40];

    memcpy(buf, D_80089308, 5);
    func_8004E5A0(p[0x11], 2, num);
    func_8004E69C(num);
    func_80069E38(buf, num);
    func_80069E38(buf, D_80089310);
    func_80069E38(buf, D_80089318);
    func_8004E5A0(p[0x28], 3, num);
    func_80069E38(buf, num);
    func_80069E38(buf, D_8008931C);
    func_8004E5A0(p[0x29], 3, num);
    func_80069E38(buf, num);
    func_800B53BC(b[4], buf, 0, 0x6C, 0x98);
    func_800B53BC(b[19], D_80089324, 0, 0x48, 0xB8);
    func_800B53BC(b[5], func_800B544C(buf, p), 0, 0x48, 0xC0);
}

/* Collects up to three live display-list pointers out of a record into a
   caller-supplied array, skipping the null ones. func_800B59A4 and
   func_800B62A4 both call it; it is the cluster's "which primitives does this
   record have" gatherer.

   +0x4C and +0x50 are tested and stored unconditionally-in-order, each bump of
   the cursor guarded by its own value being non-null. The third slot is
   conditional on the record being the ACTIVE one: D_800814A8 is the engine's
   current-record pointer, so +0xD8 is only harvested for it.

   The cursor is walked with `*q = v; q++;` rather than `*q++ = v`, and the
   third store reuses `p` as the temporary because the target loads +0xD8 back
   into $a1, the register p arrived in. Writing the third value into a fresh
   local gives a `move` the target does not have. The parameter types are
   provisional: nothing yet witnesses the element type of the array beyond its
   word-sized stores. */
void func_800B58B8(u32 *q, u8 *p)
{
    u32 v;

    v = *(u32 *)(p + 0x4C);
    if (v != 0)
    {
        *q = v;
        q++;
    }
    v = *(u32 *)(p + 0x50);
    if (v != 0)
    {
        *q = v;
        q++;
    }
    if ((u32)p == D_800814A8)
    {
        p = *(u8 **)(p + 0xD8);
        if (p != NULL)
        {
            *q = (u32)p;
        }
    }
}

/* The signed-delta formatter the two equipment panels share: a 4-byte field
   seeded from D_80089304 ("   ") and, when the delta at +0x02 of the record is
   nonzero, a sign glyph in byte 0 followed by the magnitude rendered two
   digits wide at s+1. 0x6B/0x6C are the font's "+"/"-". Returns the buffer it
   was handed, which both call sites feed straight into func_800B53BC.

   The field is read with `lb`, so it is SIGNED, and it is read TWICE -- once
   for the sign, once for the magnitude -- so no local holds it. The parameter
   stays `u8 *` for the cluster's convention with the sign in the cast; `s8 *p`
   and `p[2]` measures identically.

   TWO SPELLINGS ARE LOAD-BEARING, each measured over a 28-point grid:

   - The sign must be an explicit if/else with a store in each arm, NOT a
     ternary. `s[0] = (x > 0) ? 0x6B : 0x6C` takes the opposite branch polarity
     (blez with 0x6C in the delay slot) and lands the glyph in $v1 rather than
     $v0: 6 differing. `(x <= 0) ? 0x6C : 0x6B` is 5, `(x < 0) ? 0x6C : 0x6B`
     is 6, and the accumulator form that func_800B555C needs 40 lines up is 6
     and 8. Only the two-arm store emits bgtz with 0x6B in its delay slot and
     one shared `sb $v0`. SO THE func_800B555C LEVER IS NOT GENERAL: the
     accumulator wins for a value that is RETURNED, the two-arm store for a
     value that is STORED.

   - The magnitude must be the ternary with the POSITIVE arm first. Writing it
     `(x < 0) ? -x : x`, hoisting it into `v = x; if (v < 0) v = -v;`, or
     splitting the call across an if/else all give the same 35 words but pull
     `addiu $a1, 2` down into the bgez's delay slot and leave the nop before
     it: 2 differing, and the three losers are indistinguishable.

   D_80089304 keeps its own INCLUDE_RODATA and is `extern u8`, which is what
   gives the seed copy lwl/lwr + swl/swr here -- the destination is a plain
   pointer parameter, so alignment 1 wins on one side whatever the symbol's
   element type. That is the func_800B544C mechanism, not the func_800B5E28
   one. */
u8 *func_800B5918(u8 *q, u8 *s)
{
    memcpy(s, D_80089304, 4);
    if (*(s8 *)(q + 2) != 0)
    {
        if (*(s8 *)(q + 2) > 0)
        {
            s[0] = 0x6B;
        }
        else
        {
            s[0] = 0x6C;
        }
        func_8004E5A0(*(s8 *)(q + 2) >= 0 ? *(s8 *)(q + 2) : -*(s8 *)(q + 2), 2,
                      s + 1);
        func_8004E69C(s);
    }
    return s;
}

/* The last of func_800B5AC8's four panel builders, and the only one that is a
   LOOP: func_800B58B8 gathers the record's up-to-three sub-primitives into a
   local array, and each one present gets a name sprite, a signed-modifier
   sprite and a sprite id, each 0x10 further down the panel.

   Four things are load-bearing, each measured:

   - `list` must be the FIRST declaration. The two arrays are assigned upward
     in declaration order, so list takes sp+0x18 and buf sp+0x28; declaring buf
     first gives 75 words, 73 differing.

   - `n` is an address-taken SCALAR and its slot is assigned when the address is
     taken during RTL expansion, not at its declaration, so it lands ABOVE both
     arrays (sp+0x38) wherever it is written. Moving it carries no information;
     only the array/array order does. `s16 n` is 4 differing, which with
     func_8004AC3C's `s32 *out` pins it at a word.

   - buf's declared size is witnessed only to within an 8-byte rounding bucket:
     0xC and 0x10 are both 0 differing (n at sp+0x38, frame 0x58), 8 is 16
     differing (frame 0x50), 0x14 and 0x18 are 16 (frame 0x60), 0x40 is 16
     (frame 0x88). Scan the size; do not reason about it.

   - `b[i + 6]` / `b[i + 9]` / `b[i + 12]` is right even though the target walks
     ONE pointer with `addiu $s1, $s1, 0x4` and displacements 0x18/0x24/0x30.
     loop.c's combine_givs merges address givs that share a mult_val into the
     base induction variable plus constants, so that single bump is NOT evidence
     of a source pointer walk: writing it as `b++` with b[6]/b[9]/b[12] gives 74
     words, 58 differing. The same goes for the two `addiu $sN, $sN, 0x10` in
     the loop -- they are `i * 0x10 + K`, not source accumulators.

   Insensitive, all 0 differing: `break` or `return` on the null slot, `while`
   or `for`, hoisting `i * 0x10 + 0xD8` into a local, `sizeof(list)` for the
   0xC, and a `u8 *e = list[i]` temp. Dropping func_80069EC8's clear is 71
   words, 70 differing. */
void func_800B59A4(u8 *p, UnkTownCardPair **b)
{
    u8 *list[3];
    u8 buf[0x10];
    s32 n;
    s32 i;

    func_80069EC8(list, 0xC);
    func_800B58B8((u32 *)list, p);
    for (i = 0; i < 3; i++)
    {
        if (list[i] == NULL)
        {
            break;
        }
        func_800B5264(b[i + 6], (u8 *)func_8004AC3C(list[i], &n), n, 0x58,
                      i * 0x10 + 0xD8);
        func_800B53BC(b[i + 9], func_800B5918(list[i], buf), 0, 0xA0,
                      i * 0x10 + 0xD8);
        b[i + 12]->unk_00 = func_8004A658(list[i][1], list[i][0]);
        b[i + 12]->unk_04->unk_08 = -0x30;
        b[i + 12]->unk_04->unk_0A = i * 0x10 + 0x66;
    }
}

/* One of the two panel drivers for this cluster, and func_800B640C's twin:
   four calls of the same two arguments in source order, nothing else. The
   incoming $a0/$a1 serve the FIRST call directly -- gcc keeps them live and
   copies them to $s0/$s1 in the jal's delay slot -- which is why only three
   of the four call sites have an argument-setup pair.

   It pins func_800B52F4 and func_800B59A4 at two parameters each and is their
   only witness, but NOT func_800B5174, which takes three: this function
   forwards its own third argument into that first call, and a pure forward
   costs no instruction, so its 22 words are the same with two parameters and
   with three. CORRECTED -- an earlier version of this comment claimed it
   witnessed func_800B5174 at two, which is exactly the inference the forward
   makes unavailable. The witness is func_800B6508, one level further out. */
void func_800B5AC8(u8 *p, UnkTownCardPair **b, s32 c)
{
    func_800B5174(p, b, c);
    func_800B52F4(p, b);
    func_800B5774(p, b);
    func_800B59A4(p, b);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5B20);

/* The first of func_800B640C's four panel builders, and func_800B5174's twin
   under the other driver: no text buffer and no font renderer, just the three
   card slots' primitives set up directly.

   It carries the same arity correction as func_800B5174 and for the same
   reason -- `sw $a2, 0x0($v0)` stores a register nothing in the body writes,
   and func_800B640C forwards it for free because this is its FIRST call.
   func_800B6508 is the witness for both drivers; see the comment on
   func_800B5174.

   Unlike func_800B5174 this one is VOID. There is no trailing
   `addu $v0, $sN, $zero`; $v0 holds func_800498EC's value at exit only because
   it was just stored. `return b` costs a word: 53 words, 6 differing.

   The alternatives for the b[1]->unk_00 store, all at 52 words, are worth
   keeping because they price the parameter against the devices:
   - uninitialised `s32 x;`: 4 differing, and only in the $a2/$a3 roles. An
     uninitialised pseudo is never set, so local-alloc gives it no quantity;
     global-alloc then assigns it after the three-reference -0x38 constant has
     taken $a2. QTY_CMP_PRI's floor_log2(n_refs) factor is 0 for any
     ONE-reference quantity, so a single-use value can never outrank it.
   - `register s32 x __asm__("$6");` uninitialised: 0 differing, because a hard
     register used and never set is live-in and local-alloc must route the
     constant to $a3. Right bytes, wrong reason.
   - the actual third parameter: 0 differing with no device. */
void func_800B5C60(u8 *p, UnkTownCardPair **b, s32 c)
{
    b[0]->unk_04->unk_08 = -0x38;
    b[0]->unk_04->unk_0A = 8;
    b[0]->unk_00 = (s32)D_80077FBC;
    b[1]->unk_04->unk_08 = -0x38;
    b[1]->unk_04->unk_0A = 8;
    b[1]->unk_00 = c;
    b[2]->unk_04->unk_08 = -0x10;
    b[2]->unk_04->unk_0A = 0x16;
    b[2]->unk_00 = func_800498EC(func_80049984(p));
}

/* Picks the string func_800B5D70 appends last: a town-private pointer when
   the game-mode byte D_80082E6A is 1, and func_800A652C's result otherwise.

   TWO THINGS ARE LOAD-BEARING HERE.

   The parameter is REAL AND UNUSED, and it has to stay. This body reads
   neither it nor anything derived from it -- func_800A652C takes no argument,
   its own listing never reads an incoming $a0 -- but func_800B5D70's call site
   sets $a0 up in the jal's delay slot and reloads it immediately after, so the
   argument is passed. An unused parameter costs no words; dropping it makes
   the container build fail on func_800B5D70 with "too many arguments".

   The ARMS ARE INVERTED relative to the obvious reading. The target branches
   on `bne` to the call and falls through to the pointer load, so the `!= 1`
   arm is the one written first: `if (D_80082E6A == 1) return D_800D1790;`
   with the call second lays the pointer load out of line and gives 16 words,
   5 differing -- the same instructions in the other order. */
u8 *func_800B5D30(u8 *p)
{
    if (D_80082E6A != 1)
    {
        return func_800A652C(p);
    }
    return D_800D1790;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089304);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089308);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089310);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089318);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008931C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089324);

/* The smallest member of func_800B5E28's family: one 0x40-byte text buffer
   seeded by an inline 5-byte block move, then the record's own name appended
   and a single text sprite placed through func_800B5264.

   - The seed is gcc's builtin expansion of a 5-byte copy (lw/lb then sw/sb),
     not a call. It must go through the existing D_800892C0 symbol, whose
     INCLUDE_RODATA survives: `memcpy(buf, "\t8\t8", 5)` also measures 0
     differing but emits a SECOND padded copy of those bytes and grows the
     object's .rodata by 8, shifting every later record. D_800892C0 also needs
     a 4-byte element type -- through `extern const u8` move_by_pieces takes
     MIN(src, dst) alignment and the copy comes out lwl/lwr + swl/swr, 42
     words and 32 differing.

   - func_800B5D30 reads no parameter of its own, but this call site's
     `addu $a0, $s1, $zero` sits in the jal's delay slot and the next
     instruction already reloads $a0, so the argument is real. This is the
     inverse of the "a missing argument move is just CSE" trap: here a PRESENT
     move forces the arity. LANDING func_800B5D30 MUST KEEP AN UNUSED FIRST
     PARAMETER or this function stops matching.

   - D_8008933C ("\n\n") is referenced only from here, so splat migrated it
     into this function's listing and replacing the INCLUDE_ASM loses it.
     Declared [4] to match the listing's four bytes and trailing `.align 2`.
     Unlike func_800B5E28's D_80089340, `[]` happens to be harmless here --
     the gcc-emitted constant that follows carries its own `.align 2` and
     absorbs the lost pad byte -- but [4] does not depend on that. */
const u8 D_8008933C[4] = "\n\n";

void func_800B5D70(u8 *p, UnkTownCardPair **b)
{
    u8 buf[0x40];

    memcpy(buf, D_800892C0, 5);
    func_80069E38(buf, p + 0x34);
    func_80069E38(buf, (u8 *)D_8008933C);
    func_80069E38(buf, func_800B5D30(p));
    func_800B5264(b[4], buf, 0, 0x47, 0x88);
}

/* A one-entry remap on the index func_800B5E28 hands func_8004AC18: 0x39
   becomes 2 and everything else passes through. The ternary is the spelling --
   the target keeps the value in $v1 and moves it to $v0 once in the jr's delay
   slot, which is the accumulator form, not two returns. */
s32 func_800B5E10(s32 a)
{
    return a == 0x39 ? 2 : a;
}

/* Owned by func_800B5E28; see the comment on that function for why the
   declared size is 4 and not [].  */
const u8 D_80089340[4] = "\nw";

/* The monster detail panel: one 0x40-byte text buffer assembled from five
   byte fields of the record and the font renderer's escape strings, then five
   text sprites placed through func_800B53BC / func_800B5264. func_800B5774 is
   the near-sibling -- the same recipe with three sprites and the same five
   shared rodata strings, so landing this reads most of that one too.

   Four things are load-bearing, three of them measured against alternatives:

   - The first write into `buf` is an INLINE 5-byte block move (lw/lb then
     sw/sb), not a call. The original is gcc's builtin expansion of
     `strcpy(buf, "\t7\f4")`; D_80089308 only became a standalone rodata
     symbol because func_800B5774 uses the identical literal and splat could
     not attribute a merged string constant to one function. Spelling it as a
     literal here would emit a SECOND copy of those bytes beside the surviving
     INCLUDE_RODATA, so it is written as `memcpy(buf, D_80089308, 5)` against
     the existing symbol instead. Calling strcpy on the symbol instead of
     memcpy gives a real `jal` and 128 words, 124 differing.

   - D_80089308 HAS to be declared with a 4-byte element type. move_by_pieces
     uses MIN(src, dst) alignment, and through `extern u8 D_80089308[]` the
     copy comes out lwl/lwr + swl/swr: 132 words, 120 differing. That is the
     same mechanism that makes func_800B544C's copy unaligned on both sides --
     there the DESTINATION is a plain pointer parameter, so alignment 1 wins.

   - `num` must be declared BEFORE `buf`: locals are assigned upward in
     declaration order, so num is the sp+0x18 slot and buf the sp+0x58 one.
     Swapping the two declarations gives 133 words, 129 differing.

   - D_80089340 ("\nw") is referenced only from here, so splat migrated it
     into this function's listing and replacing the INCLUDE_ASM loses it.
     Writing it as a bare string literal reaches 0 differing in .text but
     silently LOSES a byte of .rodata: gcc emits `.rdata / .align 2 / .ascii
     "\nw\000"` with no TRAILING align, and the next item in the stream
     (D_80089344, whose splat listing carries no `.align`) then starts one
     byte early -- measured as .rodata differing at 0xBE3 with the section
     size unchanged, which is exactly the kind of regression ovlverify cannot
     see. Declaring the array `[4]` restores the pad and makes the whole
     object's .rodata byte-identical to the stub build's. `[]` would not.

   0x98 and 0xB8 each appear twice as the fifth argument and 0xC8 once; that,
   and nothing in the source, is why two of them live in $s0 across calls and
   the third in $v0. The third argument to func_800B5264 reads the same either
   way -- `p[0x43] >= 0x41 ? 3 : 0` also measures 0 differing. */
void func_800B5E28(u8 *p, UnkTownCardPair **b)
{
    u8 num[0x40];
    u8 buf[0x40];

    memcpy(buf, D_80089308, 5);
    func_8004E5A0(p[0x11], 2, num);
    func_8004E69C(num);
    func_80069E38(buf, num);
    func_80069E38(buf, D_80089310);
    func_80069E38(buf, D_80089318);
    func_8004E5A0(p[0x28], 3, num);
    func_80069E38(buf, num);
    func_80069E38(buf, D_8008931C);
    func_8004E5A0(p[0x29], 3, num);
    func_80069E38(buf, num);
    func_80069E38(buf, (u8 *)D_80089340);
    func_8004E5A0(p[0x25], 3, num);
    func_80069E38(buf, num);
    func_80069E38(buf, D_8008931C);
    func_8004E5A0(p[0x66], 3, num);
    func_80069E38(buf, num);
    func_800B53BC(b[5], buf, 0, 0x6C, 0x98);
    func_800B5264(b[6], (u8 *)func_8004AC18(func_800B5E10(p[0x13])),
                  p[0x43] < 0x41 ? 0 : 3, 0x68, 0x98);
    func_800B53BC(b[19], D_80089324, 0, 0x48, 0xB8);
    func_800B53BC(b[7], func_800B544C(buf, p), 0, 0x48, 0xC8);
    func_800B53BC(b[18], func_800B557C(buf, p), 0, 0x48, 0xB8);
}

/* func_800B5918's counterpart on the other panel, and NOT the same source:
   the first argument is a VALUE, not a record pointer -- its caller does
   `lbu $a0, 0x1($s3)` -- so there is no sign test and no negate, and the four
   constant bytes are written around the two calls rather than conditionally.
   Two independent readings of the listing agreed on the signature before it
   was compiled.

   The one trap is the LAST call's argument: $s1 is s + 1 and serves as both
   func_8004E5A0's third argument and func_8004E69C's only one, so the
   post-process takes `s + 1` and not `s`, unlike every other member of the
   family. The four constant stores are scheduled into the two jal delay slots,
   so their emitted order is not their source order. */
u8 *func_800B6030(s32 a, u8 *s)
{
    s[0] = 0x20;
    func_8004E5A0(a, 2, s + 1);
    s[3] = 0xA;
    s[4] = 0x6F;
    s[5] = 0;
    func_8004E69C(s + 1);
    return s;
}

/* The three-slot card row builder, driven by func_800B62A4's stride-3 loop:
   `c` is the destination slot index 0..2 and the three sprite slots it touches
   are b[c+8], b[c+11] and b[c+14] -- the same 8/11/14 the caller walks $s3
   through. The record is D_8006DE24[p[0]]; its +0x00 word is the name string
   and its +0x10 byte feeds func_80049944. p[1] goes through func_800B6030 into
   a 6-byte string in the 0x10-byte stack buffer.

   Three things are load-bearing, each measured:

   - The record must be CACHED IN A LOCAL POINTER. `&D_8006DE24[p[0]]` spelled
     out at both use sites gives 81 words / 63 differing: p[0] is a load that
     the intervening jal invalidates, so cse cannot carry the record address and
     gcc parks only the SYMBOL's base in a saved register, recomputing
     p[0] * 20 + base twice. The diagnostic is a saved register holding a bare
     symbol address -- that means you re-indexed where the original cached.

   - The store must be ONE assignment with a ternary RHS, not an if/else with a
     store in each arm. gcc evaluates the destination address before the RHS,
     which is why `lw $s1, 0x38($s1)` sits above `jal func_800439BC`, inside the
     RHS. if/else is 76 words / 57 differing; caching b[c+14] in a local as well
     -- which would let all three stores share it -- is 69 words / 47 differing,
     because the later reads of that lvalue have to RELOAD (the intervening
     `sh` may alias).

   - `buf` must be u8[0x10]: 0x40 and 0x8 both give 75 words / 16 differing,
     from the frame size moving every displacement off 0x48.

   c * 16 + 0xD8 is used twice as the fifth argument, so it is parked in $s2;
   c * 16 + 0x67 is used once and materialised in $v0. `(c << 4) + 0xD8` is also
   0 differing, and so is declaring buf before r -- r and v are register locals,
   so only buf claims a stack slot and declaration order cannot move it. That is
   UNLIKE the num/buf pair in func_800B5E28, where both are arrays. */
void func_800B6094(u8 *p, UnkTownCardPair **b, s32 c)
{
    Unk8006DE24 *r;
    u8 buf[0x10];
    s32 v;

    r = &D_8006DE24[p[0]];
    func_800B5264(b[c + 8], *(u8 **)r, 0, 0x58, c * 16 + 0xD8);
    func_800B53BC(b[c + 11], func_800B6030(p[1], buf), 0, 0xA0, c * 16 + 0xD8);
    v = func_80049944(r->unk_10);
    b[c + 14]->unk_00 = func_800439BC(p[0]) ? func_800498EC(v) : func_80049918(v);
    b[c + 14]->unk_04->unk_08 = -0x30;
    b[c + 14]->unk_04->unk_0A = c * 16 + 0x67;
}

/* func_800B59A4's loop body unrolled to one iteration with the index as a
   third parameter, three pair-slots higher. It landed from that function's
   body with no further search, which is the whole value of the cluster: the
   five callees, their order and the two `i * 0x10 + K` coordinates all carry
   over, and only the slot bases move 6/9/12 -> 8/11/14.

   Arity 3 is witnessed by its own listing -- $a0, $a1 and $a2 are all read --
   and its only caller func_800B62A4 is still a stub, so re-check when that
   lands. The frame confirms the two stack-slot rules recorded on
   func_800B59A4: with one array instead of two, buf sits at sp+0x18 and the
   address-taken scalar `n` at sp+0x28, above it, with a 0x48 frame. */
void func_800B61C0(u8 *p, UnkTownCardPair **b, s32 i)
{
    u8 buf[0x10];
    s32 n;

    func_800B5264(b[i + 8], (u8 *)func_8004AC3C(p, &n), n, 0x58,
                  i * 0x10 + 0xD8);
    func_800B53BC(b[i + 11], func_800B5918(p, buf), 0, 0xA0, i * 0x10 + 0xD8);
    b[i + 14]->unk_00 = func_8004A658(p[1], p[0]);
    b[i + 14]->unk_04->unk_08 = -0x30;
    b[i + 14]->unk_04->unk_0A = i * 0x10 + 0x66;
}

/* The card-panel slot filler: it fills the three destination slots 0..2 from
   three sources in order and then blanks whatever is left. `c` is the
   destination cursor every callee is handed, and it advances ONLY for a source
   that was present, so the three phases compact into the slots. Phase 1 walks
   the three 3-byte records at p+8/p+11/p+14 -- the stride-3 loop
   func_800B6094's comment describes, and its 8/11/14 is this function's $s3.
   Phase 2 is func_800B59A4's recipe. Phase 3 is the single record at p+0x48,
   guarded by p[0x49], so the record is at least 0x4A bytes. Phase 4 zeroes
   b[i + 14]->unk_00 for every slot the first three did not fill.

   Four things are load-bearing, each measured over about forty spellings:

   - The first argument must be `&p[i * 3 + 8]`, an ARRAY REF whose address is
     taken, NOT `p + i * 3 + 8`. The subscript is its own giv (8, 11, 14 in
     $s3) added to the parameter at the use; the left-associated pointer form
     folds that giv into the LOAD's address giv and emits `addiu $a0, $s2, 8`
     instead: 80 words, 80 differing. The two forms differ only in whether the
     +8 lands in the giv's initial value or in the MEM's displacement.

   - Phase 2's bound is `i + n < 3` with the COUNTER FIRST. `n + i < 3` emits
     the addu's operands the other way round, 1 differing; `i < 3 - n` hoists
     the subtraction into a register, 16 differing.

   - list must be 3 or 4 pointers -- both 0 differing, the same 8-byte rounding
     bucket as func_800B59A4's buf -- and `u8 *list[2]` is 16 differing, from
     the frame dropping to 0x38.

   - THE SECOND `n = c;` IS NOT REDUNDANT DECORATION. Without it the function is
     82 words and 1 differing, the phase-3 guard's `beqz` landing at +0xF4 where
     the target has +0xCC, with every other word already exact and immovable:
     twenty spellings of the loop and eleven of the following `if` all stay at
     that displacement. The target's label sits BETWEEN the compare and the
     branch that uses it, which is a rotated loop's entry guard that was only
     HALF threaded, and getting there needs thread_jumps to fail before it
     reaches the label. One insn between the loop-end label and the test's
     compare does that -- and it must then vanish, or it costs a word. A dead
     re-snapshot does both, since flow deletes it long after thread_jumps has
     run. `n = c;`, `n = 0;`, `n = i;` and `n++;` are all 0 differing; `n = n;`
     is 1, because it emits no insn at all. See docs/matching-levers.md, "A
     HALF-THREADED GUARD EDGE NEEDS AN INSN THAT BREAKS thread_jumps' WALK AND
     THEN DISAPPEARS", which also records why an empty `__asm__ volatile` is
     NOT a substitute here and why a LIVE copy is worse.

   Insensitive, all 0 differing: `continue` for the null slot, nested ifs for
   phase 3's `&&`, `p + 0x48` for `&p[0x48]`, phase 4 as a `while`, phase 2
   hand-rotated, and any declaration order of c/n/i, since only `list` claims a
   stack slot. Phase 4 must index a SEPARATE variable: running it on `c` itself
   is 81 words, 42 differing. */
void func_800B62A4(u8 *p, UnkTownCardPair **b)
{
    u8 *list[3];
    s32 c;
    s32 n;
    s32 i;

    c = 0;
    for (i = 0; i < 3; i++)
    {
        if (p[i * 3 + 8] != 0)
        {
            func_800B6094(&p[i * 3 + 8], b, c);
            c++;
        }
    }
    func_80069EC8(list, 0xC);
    func_800B58B8((u32 *)list, p);
    n = c;
    for (i = 0; i + n < 3; i++)
    {
        if (list[i] != NULL)
        {
            func_800B61C0(list[i], b, c);
            c++;
        }
    }
    n = c;
    if (n < 3 && p[0x49] != 0)
    {
        func_800B61C0(&p[0x48], b, c);
        c++;
    }
    for (i = c; i < 3; i++)
    {
        b[i + 14]->unk_00 = 0;
    }
}

void func_800B63EC(u8 *p, UnkTownCardPair **b)
{
    func_800B62A4(p, b);
}

/* func_800B5AC8's twin over the other four panel builders, with the same
   forwarded third parameter into the same first-call position.

   It is the arity witness for func_800B63EC, which forwards both parameters
   into func_800B62A4 -- and that forward costs no instruction, so the two
   arguments were already in place when func_800B63EC was landed calling
   func_800B62A4 with none. The container build did not catch it either,
   because a K&R `void func_800B62A4();` declaration accepts any call. It is NOT the witness for func_800B5C60's third
   parameter -- that forward is free and invisible here; func_800B6508 is. */
void func_800B640C(u8 *p, UnkTownCardPair **b, s32 c)
{
    func_800B5C60(p, b, c);
    func_800B5D70(p, b);
    func_800B5E28(p, b);
    func_800B63EC(p, b);
}

u32 func_800B6464(s32 a)
{
    return func_800491F4(a, D_800D17AC, 0x18);
}

/* Appends a card record to the tail of a +0x0C-linked list and parks the
   head at +0x0C of its first argument. func_800B6508 is the arity witness:
   `lw $a1, 0x98($s0)` sits before the jal and `addiu $a0, $s0, 0x88` in its
   delay slot, so both arguments are set explicitly and neither is inherited.

   The guard test is hoisted out of the loop -- `lw $v0, 0xC($s1)` / `beqz`
   before the body, then the body re-tests at the bottom with `bnez` -- which
   is a plain rotated `while`, not two loops. The second parameter is only
   forwarded to func_800B6464 and func_800B677C, so it carries no type
   information; `s32` would compile identically. */
void func_800B648C(u8 *q, UnkTownCardPair **b)
{
    UnkTownCardPair *t;

    t = (UnkTownCardPair *)func_800B6464((s32)b);
    *(u32 *)(q + 0xC) = (u32)t;
    while (t->unk_0C != 0)
    {
        t = (UnkTownCardPair *)t->unk_0C;
    }
    t->unk_0C = func_800B677C((s32)b);
}

/* The two-way dispatcher at the top of the 0x800B5xxx panel cluster: it
   refreshes the +0x88 sub-record and then runs ONE of the two panel drivers
   over the same (record, card array, index) triple.

   THIS IS THE ARITY WITNESS FOR BOTH DRIVERS' THIRD PARAMETER, and the only
   one there is. func_800B5AC8 and func_800B640C each forward their third
   argument into their own FIRST call, so no argument-setup insn is emitted
   there and neither driver's word count changes between two parameters and
   three. Here the forward is visible: `lw $a2, 0x14($s0)` ahead of each jal,
   in both arms. See the comment on func_800B5174.

   It also pins func_800B648C at two parameters.

   THE ARMS ARE AS WRITTEN, NOT INVERTED, and that is not luck. The target
   `beqz`-branches to func_800B5AC8 and falls through to func_800B640C, so the
   non-zero test comes first. func_800B5D30 needed the opposite -- there the
   obvious order gave the same 16 instructions reordered, 5 differing -- and
   the discriminator is that ONE OF ITS ARMS RETURNS. gcc lays an early
   `return` out inline, which forces the other arm out of line; when both arms
   are plain calls falling into a shared tail, the arms stay in source order.
   Inverting this one gives 28 words and reads as 1 differing under ovlverify,
   which is a trap: the two jal targets swap as well and the mask hides them,
   so it is really 3. */
void func_800B6508(u8 *p)
{
    func_800B648C(p + 0x88, *(UnkTownCardPair ***)(p + 0x98));
    if (*(s16 *)(p + 8) != 0)
    {
        func_800B640C(*(u8 **)(p + 4), *(UnkTownCardPair ***)(p + 0x98),
                      *(s32 *)(p + 0x14));
    }
    else
    {
        func_800B5AC8(*(u8 **)(p + 4), *(UnkTownCardPair ***)(p + 0x98),
                      *(s32 *)(p + 0x14));
    }
}

void func_800B6578(void) {
}

/* Binds a 0xC6/6/0x76 descriptor plus one caller-supplied word to the object
   and sets the +0x3C ready bit. The four halfwords are a stack struct handed
   to func_80048E00 by address, which is the same call the landed
   func_800B3558 makes against the static D_800D1650 -- here the descriptor is
   built per call because its last field varies.

   `n` is `s32`, not `s16`, although it is stored with `sh` and both widths are
   0 differing here. func_800B65D8 passes an `s32` accumulator straight in, and
   an `s16` prototype would put a `sll`/`sra 16` pair at that call site.
   func_800B683C's call passes a literal 0 and witnesses nothing. */
void func_800B6580(u8 *s, s32 n)
{
    s16 buf[4];

    buf[0] = 0xC6;
    buf[1] = 6;
    buf[2] = 0x76;
    buf[3] = n;
    func_80048E00(s, (u8 *)buf);
    s[0x3C] |= 0x80;
}

/* Lays the two card/prim pairs at p+0x50 and p+0x54 out left to right from
   the engine's width table. func_80048DA0 returns D_80080B18[n] and sets two
   global colour bytes as a side effect; func_80048DCC and func_80048DE8 then
   look the chosen index up for the pair's +0x00 word and its width. A width
   of the full 0x10 means "no offset"; otherwise the pair goes at the running
   total minus 8 per slot and the total advances.

   Five things are measured:

   - func_80048DA0 and func_80048DE8 must be declared returning `s32`, not the
     `u8` their definitions in csrc/unk_1C540.c carry. The `u8` prototypes make
     gcc truncate each result with `andi $s0, $v0, 0xff`: 67 words, 24
     differing. Nothing else in this unit calls them, so the local prototypes
     cost nothing.

   - `q` must be cached BEFORE the func_80048DA0 call. Swapped, the
     `lw $s1, 0x50($s4)` lands after the jal instead of in front of it: 68
     words, 3 differing. Dropping the local and writing the double indirection
     at all three uses is 69 words, 65 differing -- the func_800B6094 rule.

   - The pair array is indexed off `p` with 0x50 left in the MEMORY
     DISPLACEMENT. Hoisting the base (`b = (UnkTownCardPair **)(p + 0x50)`,
     then `b[i]`) makes loop.c's reduced giv start at p+0x50 with a zero MEM
     offset and costs an `addiu $s4, $s7, 0x50`: 68 words, 2 differing.

   - `w` IS REUSED for the tail's delta, and that rename is the whole last
     three words. With a separate local the two disjoint live ranges get two
     pseudos, global.c hands the width $a0 -- the first free call-clobbered
     register, since the CDK tree's config/mips/mips.h defines no
     REG_ALLOC_ORDER and the order is plain 0..31 -- and the delta $a1 as the
     outgoing argument: 68 words, 3 differing, with every other word exact.
     One local puts both ranges in $a1. Nothing else moved it off that floor:
     `u32`, `u16` and `u8` for `w` are all 3, as are moving its declaration
     and writing the delta as an if/else instead of `?:`. `s16 w` is 69 words,
     36 differing, and inverting the width test is 69 / 34.

   - D_80089344 keeps its own INCLUDE_RODATA, so it is `extern`, and its
     declared size carries no information here: the address is loop-invariant
     and hoisted into $s6 whether it is `[]` or `[4]`, both 0 differing. */
void func_800B65D8(u8 *p, s32 n)
{
    UnkTownCardPair *q;
    s32 total;
    s32 idx;
    s32 w;
    s32 i;

    total = 0;

    for (i = 0; i < 2; i++)
    {
        q = *(UnkTownCardPair **)(p + 0x50 + i * 4);
        idx = func_80048DA0(n, D_80089344[i]);
        if (i == 0 && n == 4)
        {
            idx--;
        }
        q->unk_00 = func_80048DCC(idx);
        w = func_80048DE8(idx);
        if (w == 0x10)
        {
            q->unk_04->unk_0A = 0;
        }
        else
        {
            q->unk_04->unk_0A = total - i * 8;
            total += w;
        }
    }

    w = total != 0 ? total - 0x14 : 0;
    func_800B6580(*(u8 **)*(u8 ***)(p + 0x5C), w);
}

/* The task's per-frame tick while it is opening: five frames of func_800B65D8
   driven by the counter at +0x0E, then one func_800B6508 and a hand-over to
   func_800B6578, which is a two-word `jr $ra` stub -- the idle handler.

   The counter is read SIGNED for the guard (`lh`) and UNSIGNED for the
   increment (`lhu`), which is two reads of one field and not one value reused;
   the signed read is also what func_800B65D8 receives, without a reload.

   Both arms are calls falling into a shared tail, so they are emitted in
   SOURCE order and no inversion lever applies -- see the comment on
   func_800B6508 for the discriminator. */
void func_800B66E8(u8 *s)
{
    if (*(s16 *)(s + 0xE) < 5)
    {
        func_800B65D8(*(u8 **)(s + 0x98), *(s16 *)(s + 0xE));
        *(s16 *)(s + 0xE) = *(u16 *)(s + 0xE) + 1;
    }
    else
    {
        func_800B6508(s);
        *(u32 *)s = (u32)func_800B6578;
    }
}

void func_800B6754(UnkTownDispatch *d)
{
    d->unk_00();
}

u32 func_800B677C(s32 a)
{
    return func_800491F4(a, D_800D17C4, 0x18);
}

/* Same template-copy shape as func_800AFF00 and func_800B4CBC above, with the
   three prim coordinates passed in instead of written as literals: the two
   resident templates D_8002E5D8 (4 words) and D_8002E5E8 (3 words) are copied
   into the caller's two buffers, the pair's two prim pointers are aimed at
   them, and then the coordinates are written THROUGH p->unk_04 rather than
   through the local.

   Four spellings are load-bearing, each measured:

   - The templates need POINTER LOCALS. Copying as `a[i] = D_8002E5D8[i]` emits
     a fresh %hi per element: 40 words, 36 differing. With the locals the
     lui/lw %lo/addiu appears once per template.

   - The four trailing stores must RE-READ `p->unk_04`. Carrying them on a
     named `UnkTownCardPrim *q = (UnkTownCardPrim *)a` gives 32 words, 13
     differing; spelling only the first through `a` gives 37 words, 14. The
     intervening `sh` may alias `p->unk_04`, so cse cannot keep the
     equivalence, and only the FIRST store gets $v0 from the `sw` that just
     executed.

   - `p->unk_00 = 0` goes AFTER both pointer stores: before them is 38 words,
     2 differing, purely the `sw $zero` moving up.

   - The 5th and 6th parameters are `s32`, NOT `s16`. Both widths are 0
     differing HERE -- the stores are `sh` either way -- so the callee cannot
     witness it; the caller can. An `s16` prototype makes cc1psx emit a
     `sll`/`sra 16` pair per argument at every call site, and func_800B691C's
     listing has the bare `lw 0x38($sp)` / `sw 0x10($sp)` instead. The trailing
     `addiu $v0, $zero, 0x4` is the byte store's own constant and not a return
     value: `s32` with `return 4` is 39 words, 3 differing. */
void func_800B67A4(UnkTownCardPair *p, u32 *a, u32 *b, s32 x, s32 y, s32 z)
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
    p->unk_04 = (UnkTownCardPrim *)a;
    p->unk_08 = (UnkTownCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = x;
    p->unk_04->unk_0A = y;
    p->unk_04->unk_0C = z;
    p->unk_04->unk_0F = 4;
}

/* Clears two slot pointers through the +0x98 record, points its +0x5C target
   back at the object's +0x18 view, and binds that target through
   func_800B6580.

   The last argument needs THREE dereference levels, and getting it wrong
   produces a 26-word body at 9 differing that looks like a cse problem and is
   not. The target reads `0x98`, then `0x5C`, then `0x00` -- and that third
   load is of the very address the preceding statement STORED to, with nothing
   between. That is the documented signature of a volatile re-read, and reaching
   for `volatile` here is the wrong move: writing the chain out at its true
   depth is 0 differing with no qualifier, and the 9-differing version was
   simply one `*` short. CHECK THE DEREFERENCE DEPTH BEFORE DIAGNOSING A
   FORWARDED STORE.

   The loop re-loads +0x98 on every iteration -- the branch target is the `lw`,
   not the body after it -- so the base is NOT hoisted and the source reads it
   inside the loop. */
void func_800B683C(u8 *p)
{
    s32 i;

    for (i = 0x14; i < 0x16; i++)
    {
        *((u32 **)*(u8 **)(p + 0x98))[i] = 0;
    }
    **(u32 **)(*(u8 **)(p + 0x98) + 0x5C) = (u32)(p + 0x18);
    func_800B6580(**(u8 ***)(*(u8 **)(p + 0x98) + 0x5C), 0);
}

/* A word lookup in the town-private table D_800D17C8. The symbol's address is
   materialised FIRST and the scaled index added to it, which is the shape a
   subscript on a declared array gives at -G0. */
u32 func_800B68AC(s32 i)
{
    return D_800D17C8[i];
}

/* Initialises the task header: handler func_800B66E8 at +0x00, the caller's
   word at +0x04, the mode at +0x08, two zeroed halfwords at +0x0C/+0x0E, and
   the D_800D17C8 lookup for that mode at +0x14, then runs func_800B6E3C.

   The third parameter is `s32`, not `s16`, even though it is stored with `sh`
   and both widths are 0 differing here. func_800B691C forwards its own third
   argument straight in, and with `s16` anywhere in that chain cc1psx emits a
   `sll`/`sra 16` pair at the call site: func_800B691C then comes out 30 words,
   15 differing. The callee cannot witness this; only the caller can.

   +0x08 is written and then read BACK with `lh` to feed func_800B68AC, rather
   than the parameter being reused -- the `sh` is in the way, so cse does not
   forward it, and the source reads the field. */
void func_800B68C8(u8 *p, u32 a, s32 m)
{
    *(s16 *)(p + 8) = m;
    *(u32 *)(p + 4) = a;
    *(s16 *)(p + 0xC) = 0;
    *(s16 *)(p + 0xE) = 0;
    *(u32 *)p = (u32)func_800B66E8;
    *(u32 *)(p + 0x14) = func_800B68AC(*(s16 *)(p + 8));
    func_800B6E3C(p);
}

/* The task's whole construction in three calls: build the prim pair at +0x88
   over the buffers at +0x60 and +0x70, wire the slots, then fill the header.
   func_800AFF00's call site at the +0x98/+0x70/+0x80 offsets is the same shape
   one object out.

   SIX parameters, and the last four are all `s32`. The fourth arrives in $a3
   and is forwarded to func_800B67A4 with no insn at all -- the register is
   already right -- so nothing here witnesses it; func_800B67A4's own body
   does. The THIRD must be `s32` as well: declared `s16` it is sign-extended
   at its first int use, which is the forward into func_800B68C8, giving 30
   words and 15 differing with a `sll`/`sra 16` pair and $s1/$s2 exchanged.
   That is one narrow declaration costing two extra words and an allocation
   swap three statements away. */
void func_800B691C(u8 *s, u32 b, s32 c, s32 x, s32 y, s32 z)
{
    func_800B67A4((UnkTownCardPair *)(s + 0x88), (u32 *)(s + 0x60),
                  (u32 *)(s + 0x70), x, y, z);
    func_800B683C(s);
    func_800B68C8(s, b, c);
}

s32 func_800B6990(u8 *s, s32 n)
{
    s32 ok;

    ok = 0;
    *(u8 **)(s + 0x98) = func_80049004((u8 **)(s + 0x9C), n, 0xC);
    if (*(u8 **)(s + 0x98) != 0)
    {
        *(u32 *)(s + 0x7C) = D_8002E5E8[0];
        *(u32 *)(s + 0x80) = D_8002E5E8[1];
        *(u32 *)(s + 0x84) = D_8002E5E8[2];
        func_800491CC((u32 **)*(u8 **)(s + 0x98), (u32)(s + 0x7C), 0x18);
        ok = 1;
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6A0C);

/* The generic releaser for the 0x18-entry card/prim table at +0xB8 of the
   func_800B6BC4/func_800B6C30 node: each entry's kind byte at +0x0E of its
   prim selects the freer -- 2 is the func_800B5264 family, 1 the
   func_800B53BC family -- then the handle and the kind byte are zeroed.

   THE ZEROING TAIL IS WRITTEN TWICE, ONCE IN EACH ARM, and jump.c cross-jumps
   the copies; the `j` out of the first arm is the merge. Factoring it out
   below the if/else-if with a `continue` in the default path is the reader's
   spelling and it is wrong by three words: 48 words, 39 differing. Fourteen
   other spellings of that shape were measured and every one with a factored
   tail is 39 to 46 differing.

   The mechanism was read off the `.loop` dumps and is worth keeping as a
   number. Duplicating the tail takes the loop from 25 real insns to 28.
   move_movables tests `threshold * savings * lifetime >= insn_count` and does
   `threshold -= 3` after each move, so with savings and lifetime both 1 the
   test is just `threshold >= insn_count`: at 25 BOTH equality constants are
   hoisted and the `1` eats a fourth call-saved register, while at 28 the
   second hoist is refused ("Insn 91: regno 111 (life 1), move-insn savings 1
   not desirable") and reorg steals the rematerialised `1` into the first
   bne's delay slot. That brackets this -G0 -O2 build's threshold at 28..30.

   The same insn-count change leaves biv 81 un-eliminated, so the exit test
   stays `slti` on the counter rather than a pointer compare against a hoisted
   `a + 96`. Hence the diagnostic: a loop with BOTH an `slti` counter test and
   an `addiu $sN, $sN, 4` has two live induction variables because the body was
   too big for biv elimination -- it is NOT evidence of a source pointer walk.
   Spelling the walk as `a++` makes it the parameter copy, which assign_parms
   emits before move_movables' hoists: 45 words, 6 differing, all six in that
   prologue ordering. */
void func_800B6B10(UnkTownCardPair **a)
{
    s32 i;

    for (i = 0; i < 0x18; i++)
    {
        if (a[i]->unk_04->unk_0E == 2)
        {
            func_8004E99C(a[i]->unk_00);
            a[i]->unk_00 = 0;
            a[i]->unk_04->unk_0E = 0;
        }
        else if (a[i]->unk_04->unk_0E == 1)
        {
            func_8004B1A4(a[i]->unk_00);
            a[i]->unk_00 = 0;
            a[i]->unk_04->unk_0E = 0;
        }
    }
}

/* Tears the task down and marks the node dead. func_800B3A80 is the template
   for the shape -- the null guard, the `v = (u8 *)o + 0x20` actor view, the
   func_8004B248 on a +0xBC/+0xCC field, then the releaser, then the two
   0x8000 bits. */
void func_800B6BC4(UnkNode *o)
{
    u8 *v;

    if (o == 0)
    {
        return;
    }
    v = (u8 *)o + 0x20;
    func_8004B248((u32 *)((u8 *)o + 0xBC));
    func_800B6B10(*(UnkTownCardPair ***)(v + 0x98));
    func_800B6F54(v);
    o->unk_1E |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* Re-enters the task on an existing node: release what is there, re-seed
   +0x04 and +0x08 from the two arguments, then run the dispatcher and the
   rebuild. Same null guard and +0x20 actor view as func_800B6BC4.

   NOTHING IN THE OVERLAY CALLS THIS FUNCTION, so its arity is consistent
   rather than witnessed: $a1 and $a2 are both saved into callee-saved
   registers in the prologue before any call, which makes them real incoming
   arguments, but nothing fixes their WIDTHS. The third is declared `s32`
   although it is stored with `sh`, on the rule that a narrow declaration is
   invisible here and costs a `sll`/`sra` pair in every caller -- see the
   comment on func_800B68C8. Re-check when a caller lands. */
void func_800B6C30(UnkNode *o, u32 b, s32 c)
{
    u8 *v;

    if (o == 0)
    {
        return;
    }
    v = (u8 *)o + 0x20;
    func_800B6B10(*(UnkTownCardPair ***)(v + 0x98));
    *(s16 *)(v + 8) = c;
    *(u32 *)(v + 4) = b;
    func_800B6508(v);
    func_800B6E3C(v);
}

/* Writes a coordinate pair into the prim hanging off +0x0C, if there is an
   object at all. +0x0C is re-dereferenced for the second store -- the target
   has `lw 0xC` / `lw 0x4` twice -- which is this unit's standing shape. */
void func_800B6C94(u8 *p, s16 x, s16 y)
{
    if (p != NULL)
    {
        (*(UnkTownCardPair **)(p + 0xC))->unk_04->unk_08 = x;
        (*(UnkTownCardPair **)(p + 0xC))->unk_04->unk_0A = y;
    }
}

/* Fills a two-word GPU primitive header: +0x04 gets a code selected by the
   first argument (0x42E6 or 0x42DB off a base) and +0x00 gets the fourth
   argument plus 0x80800000.

   The ternary is the spelling. The `bnez` branches over the FALSE value with
   the TRUE value in its delay slot, which is cc1psx's `dst = cond ? A : B`
   shape; the two-arm if/else form that func_800B5918 needs would put the
   store in each arm. 0x80800000 is a constant materialised with lui -- not a
   symbol, which would be lui/addiu. */
void func_800B6CCC(s32 a, s32 b, u32 *c, u32 d)
{
    c[1] = b + (a != 0 ? 0x42E6 : 0x42DB);
    c[0] = d + 0x80800000;
}

/* Picks the glyph index for the object's +0x13 slot: nothing there gives
   func_80049E88's answer, otherwise a nonzero low halfword from
   func_80042900(p, 0xA) forces 0x2F and anything else goes through
   func_800B5E10's one-entry remap.

   func_80049E88 IS DELIBERATELY NOT DECLARED IN THIS UNIT. Its real definition
   returns `u8`, and declaring it so makes gcc zero-extend the tail call's
   result: 27 words, 6 differing, the extra word being `andi $v0, $v0, 0xff`.
   The target has no such mask, so the original translation unit had no
   prototype for it and the implicit `int` is what matches. Several other
   callees in this file are implicit for the same reason.

   The `sll $v0, $v0, 16` with no `sra` before the `beqz` is a low-halfword
   test, not a shift in the source: the cast is `(s16)`.

   The arms are the way round they are written. The `p[0x13] == 0` arm is the
   branch TARGET and so laid out last, which is why the `!= 0` test comes
   first even though the zero case reads like the early return. */
s32 func_800B6CEC(u8 *p)
{
    s32 r;

    r = p[0x13];
    if (r != 0)
    {
        if ((s16)func_80042900((s8 *)p, 0xA) != 0)
        {
            r = 0x2F;
        }
        else
        {
            r = func_800B5E10(r);
        }
        return r;
    }
    return func_80049E88((s8 *)p);
}

/* Seeds the object's trailing primitive: +0xE0 takes the constant 0x8002B800
   and the pair at +0xD8 is built by func_800B6CCC from the +0x08/+0x0A
   coordinates and that same constant.

   0x8002B800 is materialised TWICE, once per occurrence, with lui/ori each --
   a constant address, not a symbol, which would be lui/addiu. Writing it as
   one local would give one materialisation. */
void func_800B6D54(u8 *p)
{
    *(u32 *)(p + 0xE0) = 0x8002B800;
    func_800B6CCC(*(s16 *)(p + 8), *(s16 *)(p + 0xA), (u32 *)(p + 0xD8),
                  0x8002B800);
}

/* Allocates a 0x200-byte scratch, clears it, and hands func_800672D8 a
   four-halfword descriptor packed out of the object's +0x04..+0x0B, bracketed
   by two func_80067014(0) calls.

   The two packings are the interesting part and both are measured: +0x10 is
   `(p[8] >> 2) + ((*(u16 *)(p + 4) << 6) & 0x3C0)` and +0x12 is
   `p[9] + ((*(u16 *)(p + 4) << 4) & 0x100)`. The +0x04 halfword is read twice
   rather than held, which is why there are two `lhu 0x4`; the shifts are
   unsigned (`srl`, not `sra`) because the byte fields are `u8`. */
void func_800B6D94(u8 *p)
{
    s16 buf[4];
    u32 *q;

    q = func_80040574(0x200);
    func_80069EC8(q, 0x200);
    func_80067014(0);
    buf[0] = (p[8] >> 2) + ((*(u16 *)(p + 4) << 6) & 0x3C0);
    buf[1] = p[9] + ((*(u16 *)(p + 4) << 4) & 0x100);
    buf[2] = p[0xA] >> 2;
    buf[3] = p[0xB];
    func_800672D8(buf, q);
    func_80067014(0);
}

/* The task's rebuild, and the function that decides whether a rebuild is
   needed at all: it refreshes the glyph at +0x0A and the value at +0x10, then
   compares both against the two-halfword cache record D_80083D78[slot] and
   returns 0 if they still agree. Otherwise it invalidates the record, tears
   the primitives down and builds them again, and returns 1.

   The `||` is the shape: the first test branches TO the rebuild and the second
   branches past it, with -1 preloaded in BOTH branch delay slots. func_800B6F54
   returning nonzero invalidates the record's first half BEFORE the comparison,
   which is a separate statement from the invalidation inside the rebuild.

   THE SECOND INVALIDATION NEEDS ITS OWN POINTER LOCAL, and that is the only
   word this function argued about. The target recomputes `slot * 4 + base` for
   the +0x02 store with the INDEX as the addu's first operand, the same way the
   two earlier occurrences do; written as a plain subscript off the symbol,
   `D_80083D78[slot * 2 + 1] = -1`, the base is already in a pseudo by then and
   gcc emits `addu $v1, $a1, $v1` instead -- 70 words, 1 differing, one operand
   order. Seven spellings of the index expression all stayed at 1 (the `+ 1`
   inside the subscript, a cast to u8 *, pointer-plus-one, integer arithmetic
   with the symbol cast to s32, the index in a local). What fixes it is giving
   the second store its own pointer variable, at which point the PLUS is built
   index-first again. A base-pointer local (`t = D_80083D78; t[slot * 2 + 1]`)
   is also 0 differing, so which of the two the original used is not pinned --
   only that one of them is needed.

   func_8003E4FC is left implicitly declared, as it is at every other call site
   in the project; it has no prototype anywhere. func_800B5B20's address is
   taken here and nothing else in the unit uses it, so its `(void)` declaration
   is a placeholder -- it will have to change when that stub (80 words) lands,
   and the build will say so. */
s32 func_800B6E3C(u8 *s)
{
    s16 *q;
    s16 *r;
    s32 n;

    *(s16 *)(s + 0xA) = func_800B6CEC(*(u8 **)(s + 4));
    n = func_80049984(*(u8 **)(s + 4));
    *(u32 *)(s + 0x10) = n;
    if (n == 3)
    {
        *(u32 *)(s + 0x10) = 0;
    }
    if (func_800B6F54(s) != 0)
    {
        D_80083D78[*(s16 *)(s + 8) * 2] = -1;
    }
    q = &D_80083D78[*(s16 *)(s + 8) * 2];
    if (q[0] != *(s16 *)(s + 0xA) || q[1] != *(u32 *)(s + 0x10))
    {
        q[0] = -1;
        r = &D_80083D78[*(s16 *)(s + 8) * 2];
        r[1] = -1;
        func_800B6D94(*(u8 **)(s + 0x14));
        *(u32 *)(s + 0xDC) = 0;
        func_800B6D54(s);
        *(u32 *)(s + 0xD0) = func_8003E4FC(6, s + 0xD8, 0);
        *(u32 *)(s + 0xD4) = func_8003E4FC(0xFF, func_800B5B20, s);
        return 1;
    }
    return 0;
}

/* Releases the two handles at +0xD0 and +0xD4 and reports whether anything
   was released. The return value is ONE accumulator seeded 0 in the first
   branch's delay slot and set to 1 in each arm, with a single exit -- the
   func_800B555C shape, which is what a RETURNED flag wants; a stored flag
   would want the two-arm form func_800B5918 needs. */
s32 func_800B6F54(u8 *p)
{
    s32 r = 0;

    if (*(u32 *)(p + 0xD0) != 0)
    {
        func_8003F2A4(*(Unk80083968 **)(p + 0xD0));
        *(u32 *)(p + 0xD0) = 0;
        r = 1;
    }
    if (*(u32 *)(p + 0xD4) != 0)
    {
        func_8003F2A4(*(Unk80083968 **)(p + 0xD4));
        *(u32 *)(p + 0xD4) = 0;
        r = 1;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6FB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B70EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B72B8);

/* Clears the 0x80x0x80 halfword surface D_80162004 holds, addressed as a
   two-dimensional index rather than a walked pointer: the target recomputes
   `(i << 7) + j` scaled by 2 inside the inner loop, which is what a plain
   `p[i * 0x80 + j]` subscript gives. `sll 16 / sra 9` is gcc's fused
   sign-extend-then-scale-by-128 on the `s16` outer counter.

   Both counters are `s16` -- each test is a `sll`/`sra 16` pair before the
   `slti` -- and `i = 0` must be written BEFORE the base pointer is taken, not
   in the `for`'s init clause: the other way round the lui/addiu pair comes out
   ahead of the counter's zeroing, 3 differing. That is the same statement
   order the landed func_80043568 uses in csrc/unk_154BC.c. */
void func_800B7314(void)
{
    s16 i;
    s16 j;
    s16 *p;

    i = 0;
    p = D_80162004;
    for (; i < 0x80; i++)
    {
        for (j = 0; j < 0x80; j++)
        {
            p[i * 0x80 + j] = 0;
        }
    }
}

/* The same 0x80x0x80 clear as func_800B7314 over a caller-supplied surface,
   and the contrast is instructive: with the base a PARAMETER rather than a
   symbol the pointer is walked (`*p++`, with the `addiu` in the inner loop's
   branch delay slot) instead of re-indexed, and the inner counter is reset at
   the top of the outer loop body. Both counters are `s16`. */
void func_800B7384(s16 *p)
{
    s16 i;
    s16 j;

    for (i = 0; i < 0x80; i++)
    {
        for (j = 0; j < 0x80; j++)
        {
            *p++ = 0;
        }
    }
}

void func_800B73D0(void)
{
    func_800B8248();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B73F0);

void func_800B7420(void) {
}

/* Stamps a tile pattern onto the 0x80x0x80 halfword surface D_80162004 at
   (x0, y0), autotiling six groups of tiles as it goes: a tile in one of those
   groups is replaced by the member of its own group that matches the
   CHECKERBOARD PARITY of the destination cell, so a stamp of alternating
   floor tiles stays in phase with the surface it lands on. A 0 in the source
   is transparent and every tile outside the six groups is copied through
   unchanged.

   `src` is a u16 run: width, then height, then width*height tiles. Width and
   height are read with `*src++` twice (two separate `addiu $a2, $a2, 2`, not
   one `addiu 4` with displacements), and the tiles with a third `src++` at
   the END of the loop body -- see the spelling table below, the increment's
   placement is worth 3 words.

   Both parameters are `s16`: func_800B783C, func_800B7934 and func_800B7B48
   all sign-extend their own arguments with a `sll`/`sra 16` pair before the
   `jal`, which is what an `s16` prototype costs a caller. func_800B7934 and
   func_800B7B48 also pass a third argument, `D_800D17D8[(u8) c]`, loaded with
   `lw`; func_800B783C forwards its own $a2 untouched, so all three agree on
   arity 3. No caller reads $v0. The width is witnessed HERE too, not only
   in the callers: `s32 x0, s32 y0` is 255 words / 256 differing, because the
   index is narrow and the parity is not, so both widths are load-bearing
   inside the body.

   `src` is `u16 *` and not `s16 *`: the switch value is a bare `lhu` with no
   extension, which is the zero-extending load of an `unsigned short` promoted
   to int. An `s16 *` gives `lh` there. The counters and the index ARE signed
   (`sll`/`sra 16` on every compare).

   Two measured mechanisms, both load-bearing at this size:

   * THE INDEX MUST BE WRITTEN `<< 7`, NOT `* 128`. `i` is `s16`, so
     convert.c's convert_to_integer distributes the narrowing into the
     expression and the whole index is computed in HImode -- which is why
     `y + y0` is added here WITHOUT the sign-extension the parity test uses,
     and why a second copy of $a1 is live. `* 128` blocks that distribution
     (convert.c's MULT_EXPR arm requires both operands to fit the narrow
     type and the literal 128 has type int), so the index gets cse'd with the
     parity's SImode sum: 259 words / 255 differing.
   * `src++` IS THE LAST STATEMENT OF THE BODY, not a third clause of the
     `for`. Both spellings leave `addiu $a2, $a2, 0x2` in the inner branch's
     delay slot, but they give dbr different first insns at the `case 0`
     branch's target: in the `for`-clause form the case-0 `beqz` steals
     `addiu $v0, $a3, 0x1` from its target and retargets past it, where the
     target steals `sll $v0, $a0, 16` from its FALL-THROUGH (the default arm,
     reached through the `j`) and the `j` then takes the `sra` behind it.
     3 differing at words 45/46/48, everything else already exact.

   Spellings measured, all against the merged unit (words / differing):
     src++ as the LAST statement of the body, index `<< 7`   261 / 0   <- this
     src++ as a third clause of the `for`                     261 / 3
     `for (x = 0; x < w; src++, x++)`                         261 / 3
     the same, with `case 0: continue;`                       261 / 3
     `case 0:` written last, just before `default:`           261 / 3
     `default:` arm with no trailing `break`                  261 / 3
     index `((y0 + y) << 7) + x0 + x`                         261 / 3
     `s16 *src` instead of `u16 *src`                         261 / 1
     parity written `(y0 + y) & 1` / `(x0 + x) & 1`           261 / 12
     index `(y + y0) * 128 + x + x0`                          259 / 255
     index `(y + y0) * 0x80 + x + x0`                         259 / 255
     `u16 w, h`                                               259 / 254
     `s32 x0, s32 y0` parameters                              255 / 256
     `s32 i`                                                  227 / 253
     src++ as the FIRST statement of the body                 263 / 239
     `default:` written FIRST in the switch                   264 / 261 */
void func_800B7428(s16 x0, s16 y0, u16 *src)
{
    s16 w;
    s16 h;
    s16 x;
    s16 y;
    s16 i;

    w = *src++;
    h = *src++;

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            i = ((y + y0) << 7) + x + x0;
            switch (*src)
            {
            case 0:
                break;
            case 0x135:
            case 0x137:
            case 0x154:
            case 0x15E:
                if ((y + y0) & 1)
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x15E;
                    }
                    else
                    {
                        D_80162004[i] = 0x137;
                    }
                }
                else
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x154;
                    }
                    else
                    {
                        D_80162004[i] = 0x135;
                    }
                }
                break;
            case 0x136:
            case 0x138:
            case 0x153:
            case 0x15D:
                if ((y + y0) & 1)
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x138;
                    }
                    else
                    {
                        D_80162004[i] = 0x15D;
                    }
                }
                else
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x136;
                    }
                    else
                    {
                        D_80162004[i] = 0x153;
                    }
                }
                break;
            case 0x141:
            case 0x142:
                if ((x + x0) & 1)
                {
                    D_80162004[i] = 0x142;
                }
                else
                {
                    D_80162004[i] = 0x141;
                }
                break;
            case 0x143:
            case 0x144:
            case 0x15B:
            case 0x15C:
                if ((y + y0) & 1)
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x144;
                    }
                    else
                    {
                        D_80162004[i] = 0x143;
                    }
                }
                else
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x15C;
                    }
                    else
                    {
                        D_80162004[i] = 0x15B;
                    }
                }
                break;
            case 0x13D:
            case 0x13E:
            case 0x13F:
            case 0x140:
                if ((y + y0) & 1)
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x140;
                    }
                    else
                    {
                        D_80162004[i] = 0x13F;
                    }
                }
                else
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x13E;
                    }
                    else
                    {
                        D_80162004[i] = 0x13D;
                    }
                }
                break;
            case 0x149:
            case 0x14A:
            case 0x14B:
            case 0x14C:
                if ((y + y0) & 1)
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x14C;
                    }
                    else
                    {
                        D_80162004[i] = 0x14B;
                    }
                }
                else
                {
                    if ((x + x0) & 1)
                    {
                        D_80162004[i] = 0x14A;
                    }
                    else
                    {
                        D_80162004[i] = 0x149;
                    }
                }
                break;
            default:
                D_80162004[i] = *src;
                break;
            }
            src++;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B783C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7868);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7934);

/* func_800B7A6C's twin, and the delta is exactly six words: this one looks the
   sprite record up in D_800D1868 instead of being handed it. Words 0-5 are the
   lookup -- `sll $a2, $a2, 16` / `sra $a2, $a2, 14`, gcc's sign-extend fused
   with the *4 array scale -- and words 6-60 are func_800B7A6C's words 0-54.
   Everything load-bearing is on that function; `src++` as the body's LAST
   statement is the part to keep.

   THE THIRD PARAMETER IS `s32` WITH THE CAST IN THE BODY, NOT `s16`, and this
   is the mirror image of the narrow-parameter rule recorded on func_800B68C8.
   The fused sll/sra reads exactly like a `short` parameter subscripted
   plainly, and the callee cannot tell the two apart: `s16 arg2`, `s32 arg2`
   with `D_800D1868[(s16)arg2]`, and an `s16 idx = arg2;` local are all 61
   words / 0 differing. The CALLER separates them. func_800B8E08 -- a park at 5
   DIFF -- passes a `u8` local, and an `s16` third parameter takes that park
   from 5 differing to 16, measured by substituting the prototype alone. A `u8`
   argument widens to `s32` and to `u16` for free but NOT to `s16`.

   Two orderings in the index are each worth a word, which is unusual and is
   worth knowing for call-free bodies: `(i + y) * 128 + j + x` is 0 differing,
   `(y + i) * 128 + j + x` is 1, `(i + y) * 128 + (j + x)` is 2 and
   `x + j + (i + y) * 128` is 6. Swapping the loop nesting is 17. Every local's
   width is sensitive -- `s32 i, j` is 51 words / 47 differing, `s32 x, y` is
   59 / 55, all-`s32` is 40 / 46 -- while `if (v)` for `if (v != 0)`, the
   inverted two-arm form, `while` for `for`, dropping the `v` temp, separate
   `ox`/`oy` locals and `0x80` for `128` are all 0 differing and carry no
   information at all. */
void func_800B7978(s32 arg0, s32 arg1, s32 arg2, u16 *arg3)
{
    u16 *p;
    s16 w;
    s16 h;
    s16 x;
    s16 y;
    s16 i;
    s16 j;
    u16 v;

    p = D_800D1868[(s16)arg2];
    w = *p++;
    h = *p++;
    x = arg0 - *p++;
    y = arg1 - *p++;

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            v = *p;
            if (v != 0)
            {
                arg3[(i + y) * 128 + j + x] = v;
            }
            p++;
        }
    }
}

/* A transparent 16-bit blit. `src` is a sprite record -- `u16 w, h, ox, oy`
   followed by w*h halfwords -- copied into a STRIDE-128 halfword surface at
   (x - ox, y - oy), with 0 as the transparent key. func_800B8E08 calls it
   three times with literal coordinates and a long-lived $s1 destination.

   WHERE `src++` SITS IS THE WHOLE FUNCTION, and it is the only thing a 20-row
   spelling grid found to be wrong:

   - `t = *src++;` at the top of the body puts the `addiu $a2, $a2, 2` inside
     the body, right after the load's own branch, and leaves the loop-closing
     `bnez` a `nop` delay slot: 56 words, 27 differing, the whole body shifted
     by one.
   - `t = *src;` at the top with a bare `src++;` as the LAST statement makes
     that `addiu` the backward branch's delay slot: 0 differing.
   - `for (ix = 0; ix < w; ix++, src++)` is identical to the post-increment
     form, 56 words / 27 differing. The increment clause does NOT get the slot.

   So an `addiu` on a cursor sitting in a backward branch's delay slot is
   positive evidence for a bare `p++` written at the end of the body, and it is
   the only one of the three spellings that produces it.

   The widths are all witnessed inside the function, because every call site
   passes a literal: `s32 x, y` is 54 words / 55 differing, `u16 w, h` is 51 /
   45, `s32 ix, iy` is 45 / 47. Reading the header as `src[0..3]` then
   `src += 4` is 52 / 54, and a `do/while` inner loop 50 / 47. Hoisting the row
   address per outer iteration -- `row = &dst[(iy + y) * 128]` then `row[ix +
   x]` -- is 55 / 21, so the row pointer must NOT be hoisted. The index terms'
   ORDER matters where their grouping does not: `... + ix + x` is 0 and
   `... + x + ix` is 17, `(iy + y)` is 0 and `(y + iy)` is 1, and swapping the
   two `-=` header statements is 2.

   Free, and so not witnessed at all: `if (t)` for `if (t != 0)`, `<< 7` for
   `* 128`, `while` for `for`, dropping the `t` local, `s16 *dst`, separate
   locals for the adjusted coordinates, and inverting the inner test with a
   `continue`. */
void func_800B7A6C(s16 x, s16 y, u16 *src, u16 *dst)
{
    s16 w;
    s16 h;
    s16 ix;
    s16 iy;
    u16 t;

    w = *src++;
    h = *src++;
    x -= *src++;
    y -= *src++;

    for (iy = 0; iy < h; iy++)
    {
        for (ix = 0; ix < w; ix++)
        {
            t = *src;
            if (t != 0)
            {
                dst[(iy + y) * 128 + ix + x] = t;
            }
            src++;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7B48);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089344);

/* The same animated-texture handler as func_800B7E78, three times over: an
   UnkNode.unk_10 handler whose frame counter at +0x00 wraps at 48 and which
   then runs THREE sequential switches on it -- % 24 through the jump table,
   then % 16 and % 8 through comparison trees, each swapping one of four
   texture pages into its own VRAM rect. See func_800B7E78 for everything
   load-bearing; this landed on the first compile from that function's comment
   and its own table.

   The first switch has six case labels (two pairs sharing a body) so it gets a
   real table at CASE_VALUES_THRESHOLD; the second and third have four each and
   get trees. The counter is read fresh with `lh` before each of the later two
   divides rather than held, and each divide is signed -- the `bgez`/`addiu
   0xF`/`sra 4` and `bgez`/`addiu 7`/`sra 3` pairs are the sign bias for % 16
   and % 8, which is how you read a power-of-two modulus on a signed value.

   All twelve arms share a rect-then-call tail per switch, and cross-jumping
   merges them; the arms differ only in which page they pass. */
void func_800B7B8C(s16 *p, UnkVec *b, u32 c)
{
    p[0]++;
    if (p[0] >= 0x30)
    {
        p[0] = 0;
    }
    switch (p[0] % 24)
    {
    case 0:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_80110EC8);
        break;
    case 4:
    case 20:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801110C8);
        break;
    case 8:
    case 16:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801112C8);
        break;
    case 12:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801114C8);
        break;
    }
    switch (p[0] % 16)
    {
    case 0:
        D_80111FA8[0] = 0x330;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801116C8);
        break;
    case 4:
        D_80111FA8[0] = 0x330;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801118C8);
        break;
    case 8:
        D_80111FA8[0] = 0x330;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_80111AC8);
        break;
    case 12:
        D_80111FA8[0] = 0x330;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_80111CC8);
        break;
    }
    switch (p[0] % 8)
    {
    case 0:
        D_80111FA8[0] = 0;
        D_80111FA8[1] = 0x1FB;
        D_80111FA8[2] = 0x10;
        D_80111FA8[3] = 1;
        func_800672D8(D_80111FA8, D_80111EC8);
        break;
    case 2:
        D_80111FA8[0] = 0;
        D_80111FA8[1] = 0x1FB;
        D_80111FA8[2] = 0x10;
        D_80111FA8[3] = 1;
        func_800672D8(D_80111FA8, D_80111EE8);
        break;
    case 4:
        D_80111FA8[0] = 0;
        D_80111FA8[1] = 0x1FB;
        D_80111FA8[2] = 0x10;
        D_80111FA8[3] = 1;
        func_800672D8(D_80111FA8, D_80111F08);
        break;
    case 6:
        D_80111FA8[0] = 0;
        D_80111FA8[1] = 0x1FB;
        D_80111FA8[2] = 0x10;
        D_80111FA8[3] = 1;
        func_800672D8(D_80111FA8, D_80111F28);
        break;
    }
}

/* An UnkNode.unk_10 handler, so its first parameter is the node's actor view
   (node + 0x20) -- func_80040190's cast in csrc/unk_12DEC.c calls these as
   `(void (*)(void *, UnkVec *, u32))` with node + 0x20, p->unk_08, p->unk_0C.
   +0x00 is the frame counter func_800B81AC zeroes when it installs the
   handler; this advances it, wraps at 24, and swaps one of four texture pages
   every fourth frame. `p[0] % 24` is redundant against the wrap directly above
   it, but the divide is in the target: 0x2AAAAAAB with `sra 2` is / 24.

   THIS IS THE FIRST gcc-EMITTED JUMP TABLE LANDED IN THIS UNIT, and it
   confirms the alignment finding from the inside. Its table is 21 entries,
   0x54 bytes, so it ENDS at 4 mod 8 -- but the pad always comes from the
   FOLLOWING table's own `.align 3`, whether that follower is still an
   INCLUDE_ASM or already gcc-emitted C. splat attributes that pad word
   backwards into this table's dlabel, which is why the listing shows 22 words.
   Verified byte-identical at the object level after merging.

   TWO JUMP-TABLE ENTRIES WITH THE SAME TARGET ARE SHARED CASE LABELS, NOT TWO
   IDENTICAL ARMS. `.L800B7F20` appears at indices 4 and 20 and `.L800B7F34` at
   8 and 16. Written as six separate arms with character-identical bodies it is
   74 words, 20 differing, AND a different rodata table -- gcc cross-jumped
   exactly one of the two duplicate pairs, the one where the earlier arm could
   jump into the later arm's fall-through block, and left 8 and 16 as two
   independent labels. Cross-jumping merges only the longest common TAIL and
   stops at the first differing insn, which here is each arm's own
   `addiu ..., %lo(SYM)`.

   The table's "default" target is this function's own epilogue, which is also
   where every arm falls after the call, so no `default:` label is needed and
   none is written.

   The descriptor stores go DIRECTLY to D_80111FA8[0..3] with no pointer local:
   that is what produces the asymmetric `sh $v0, %lo(D_80111FA8)($v1)` for
   element 0 against `2($a0)`/`4($a0)`/`6($a0)` for 1..3, because gcc
   materialises `&D_80111FA8` for the call argument and rewrites the later
   element addresses off it. A pointer local also reaches 0 differing and is a
   word of nothing.

   `s16 *p` with NO casts anywhere is the spelling. `u16 *p` with `(s16)` on
   both uses is byte-identical, so the pointee's signedness is unwitnessed --
   but a cast on only ONE of the two uses is 65 words / 57 differing or 68 / 63.
   The `lhu` + `sll 16` + `sra 16` triple is NOT evidence of a `u16` field
   explicitly cast: gcc's MIPS HImode load is always `lhu` and the extension is
   a separate pattern, so the triple just means "a 16-bit value used as an
   int". Binding the field through a differently-typed local alias is not free:
   `u8 *pp` plus `u16 *p = (u16 *)pp;` is 70 words / 59 differing. The cast has
   to be at the parameter.

   Parameters 2 and 3 are never read, so they carry no witness; the dispatcher
   cast is the witness and an unused parameter costs no words. */
void func_800B7E78(s16 *p, UnkVec *b, u32 c)
{
    p[0]++;
    if (p[0] >= 0x18)
    {
        p[0] = 0;
    }
    switch (p[0] % 24)
    {
    case 0:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_80110EC8);
        break;
    case 4:
    case 20:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801110C8);
        break;
    case 8:
    case 16:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801112C8);
        break;
    case 12:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801114C8);
        break;
    }
}

/* func_800B7E78's recipe with a second switch after it: the counter wraps at
   24 as there, the % 24 jump table swaps the same four texture pages, and then
   a % 6 comparison tree swaps one of three more into a different rect. Landed
   on the first compile from func_800B7E78's comment and its own table.

   The % 6 divide is the 0x2AAAAAAB magic with post_shift 0 -- the SAME magic
   word as the % 24 above it, which uses post_shift 2 (tools/decomp/magic.py
   prints 6, 12 and 24 for it). Two divides by different constants sharing one
   magic materialisation is why the `lui`/`ori` pair appears twice rather than
   being hoisted: each divide re-materialises it.

   Three cases is below CASE_VALUES_THRESHOLD so the second switch is a tree,
   and its holes fall straight to the epilogue, so it needs no `default:`. */
void func_800B7F8C(s16 *p, UnkVec *b, u32 c)
{
    p[0]++;
    if (p[0] >= 0x18)
    {
        p[0] = 0;
    }
    switch (p[0] % 24)
    {
    case 0:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_80110EC8);
        break;
    case 4:
    case 20:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801110C8);
        break;
    case 8:
    case 16:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801112C8);
        break;
    case 12:
        D_80111FA8[0] = 0x328;
        D_80111FA8[1] = 0x80;
        D_80111FA8[2] = 8;
        D_80111FA8[3] = 0x20;
        func_800672D8(D_80111FA8, D_801114C8);
        break;
    }
    switch (p[0] % 6)
    {
    case 0:
        D_80111FA8[0] = 0x10;
        D_80111FA8[1] = 0x1C1;
        D_80111FA8[2] = 0x10;
        D_80111FA8[3] = 1;
        func_800672D8(D_80111FA8, D_80111F48);
        break;
    case 2:
        D_80111FA8[0] = 0x10;
        D_80111FA8[1] = 0x1C1;
        D_80111FA8[2] = 0x10;
        D_80111FA8[3] = 1;
        func_800672D8(D_80111FA8, D_80111F68);
        break;
    case 4:
        D_80111FA8[0] = 0x10;
        D_80111FA8[1] = 0x1C1;
        D_80111FA8[2] = 0x10;
        D_80111FA8[3] = 1;
        func_800672D8(D_80111FA8, D_80111F88);
        break;
    }
}

/* Installs func_800B7B8C as a node's +0x10 handler and zeroes the two
   halfwords at +0x20/+0x22 -- past the end of UnkNode, in the Unk44144
   payload that follows it, which is why they are written through a cast and
   not a field. */
void func_800B8170(void)
{
    UnkNode *n;

    n = func_8003FC64(0);
    if (n != 0)
    {
        n->unk_10 = (u32)func_800B7B8C;
        *(s16 *)((u8 *)n + 0x20) = 0;
        *(s16 *)((u8 *)n + 0x22) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B81AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8248);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B833C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B83DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B85E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8834);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B89C4);

/* Commits one frame of a 0x22-entry pending-event queue at 0x800133A4 and
   then copies two bytes from 0x8001360A to 0x80013608. Both tables are OUT OF
   THE LOADED IMAGE (0x8002d000..0x80088760), so they are constant addresses
   reached through a base pointer with the offsets written into the index
   expressions, exactly as the landed func_80043568 does in csrc/unk_154BC.c --
   one `u8 *base = (u8 *)0x80010000;` per block, which is why the listing has
   two `lui`s of 0x8001 and shares a `%hi` between D_80013608 and D_8001360A.

   The counter must be zeroed BEFORE the base is taken, not in the `for`'s
   init clause: the other way round the lui comes out first, 2 differing.
   Same ordering rule as func_800B7314. */
void func_800B8A40(void)
{
    u8 *base;
    u8 *base2;
    s32 i;

    base = (u8 *)0x80010000;
    for (i = 0; i < 0x22; i++)
    {
        if (base[i * 2 + 0x33A5] != 0)
        {
            base[i * 2 + 0x33A4] = base[i * 2 + 0x33A5];
            base[i * 2 + 0x33A5] = 0;
        }
    }
    i = 0;
    base2 = (u8 *)0x80010000;
    for (; i < 2; i++)
    {
        base2[i + 0x3608] = base2[i + 0x360A];
    }
}

/* Clears the +0x0A halfword of all 0x43 D_800D2644 records, walking DOWNWARD
   from the last, then sets it to 1 for every record named by a non-zero byte
   in the 0x22-entry table at 0x800133A4 -- the same out-of-image base pointer
   func_800B8A40 uses.

   D_800D2644 is declared `s16 [][0x10]` so that the index keeps its 0x20
   stride in the POINTER and the 0xA stays in the memory displacement.
   Flattening it to `*(s16 *)(D_800D2644 + i * 0x20 + 0xA)` folds the 0xA into
   the pointer and gives `sh $zero, 0x0($v0)` where the target has
   `sh $zero, 0xA($v0)`: 2 differing. Only the 0x20 stride and the `s16` at
   +0x0A are witnessed; the rest of the shape is padding. */
void func_800B8A9C(void)
{
    u8 *base;
    s32 i;

    for (i = 0x42; i >= 0; i--)
    {
        D_800D2644[i][5] = 0;
    }
    i = 0;
    base = (u8 *)0x80010000;
    for (; i < 0x22; i++)
    {
        if (base[i * 2 + 0x33A4] != 0)
        {
            D_800D2644[base[i * 2 + 0x33A4]][5] = 1;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800894D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8B00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8C90);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800895F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8E08);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9034);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9044);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9054);

void func_800B90B4(void) {
}

void func_800B90BC(void)
{
    UnkNode *o;

    o = func_8003FC64(0);
    if (o != 0)
    {
        o->unk_10 = (u32)func_800B90B4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B90F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9204);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9260);

void func_800B9384(void)
{
    UnkNode *o;

    o = func_8003FC64(0);
    if (o != 0)
    {
        o->unk_10 = (u32)func_800B9260;
    }
}

void func_800B93B8(void) {
}

void func_800B93C0(void)
{
    UnkNode *o;

    o = func_8003FC64(0);
    if (o != 0)
    {
        o->unk_10 = (u32)func_800B93B8;
    }
}

void func_800B93F4(void) {
}

void func_800B93FC(void)
{
    UnkNode *o;

    o = func_8003FC64(0);
    if (o != 0)
    {
        o->unk_10 = (u32)func_800B93F4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9430);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B946C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B94A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B94FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B97D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9830);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9890);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B98B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9998);

void func_800B9B28(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    u8 *col;
    u8 blue;

    *(u32 *)((u8 *)p - 0x10) = (u32)func_800B9998;
    func_8004491C((UnkNode *)((u8 *)p - 0x20), (u32)D_80046398);
    p->unk_66 = 0;
    r->unk_1C = 0x1000;
    r->unk_1E = 0x1000;
    r->unk_20 = 0x1000;
    col = (u8 *)&D_80083160;
    r->unk_12 = 0;
    r->unk_14 = 0;
    r->unk_10 = 0;
    r->unk_16 = 0;
    r->unk_18 = 0;
    r->unk_1A = 0;
    r->unk_00 = 0;
    r->unk_04 = 0;
    r->unk_05 = 0;
    r->unk_0C = col[0xA8];
    r->unk_0D = col[0xA9];
    blue = col[0xAA];
    r->unk_08 = 0x1F;
    r->unk_0E = blue;
    q->unk_08 = 0xFF600000;
}

void func_800B9BF0(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    u8 *col;
    u8 blue;

    *(u32 *)((u8 *)p - 0x10) = (u32)func_800B9998;
    func_8004491C((UnkNode *)((u8 *)p - 0x20), (u32)D_80046398);
    p->unk_66 = 0;
    r->unk_1C = 0x1000;
    r->unk_1E = 0x1000;
    r->unk_20 = 0x1000;
    col = (u8 *)&D_80083160;
    r->unk_12 = 0;
    r->unk_14 = 0;
    r->unk_10 = 0;
    r->unk_16 = 0;
    r->unk_18 = 0;
    r->unk_1A = 0;
    r->unk_00 = 0;
    r->unk_04 = 0;
    r->unk_05 = 0;
    r->unk_0C = col[0xA8];
    r->unk_0D = col[0xA9];
    blue = col[0xAA];
    r->unk_08 = 6;
    r->unk_0E = blue;
    q->unk_08 = 0xFF600000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9CB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9D64);

void func_800B9EDC(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    if (*(u8 *)p->unk_98 == 0)
    {
        func_80033D08((u8 *)p);
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        /* The rotation is written twice and read back in between -- see
           docs/matching-levers.md, "A store that gcc deletes as dead is a
           VOLATILE field". Unqualified, cse1 forwards the just-stored value
           into the second statement's load and flow then drops the first
           `sh` as dead: 35 words, 10 differing. `volatile` on the first
           store alone keeps both the `sh` and the reload. Measured, all at
           38 words / 0 differing: volatile on this store only, on the mask
           statement only, on both, and through a block-scope
           `volatile s16 *`. Non-volatile spellings that do NOT work, all
           35 words / 10 differing: `r->unk_1A &= 0xFFF` as the second
           statement, the same mask through `*(s16 *)((u8 *)r + 0x1A)` or
           `*(u16 *)((u8 *)r + 0x1A)` (MEM_IN_STRUCT_P does not separate
           them), and `r->unk_1A = r->unk_1A & 0xFFF`. */
        *(volatile s16 *)&r->unk_1A += p->unk_9C->unk_97;
        r->unk_1A &= 0xFFF;
    }
}

void func_800B9F74(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    if (*(u8 *)p->unk_98 == 0)
    {
        func_80033D08((u8 *)p);
        *(u16 *)((u8 *)p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        /* The rotation is written twice and read back in between -- see
           docs/matching-levers.md, "A store that gcc deletes as dead is a
           VOLATILE field". Unqualified, cse1 forwards the just-stored value
           into the second statement's load and flow then drops the first
           `sh` as dead: 35 words, 10 differing. `volatile` on the first
           store alone keeps both the `sh` and the reload. Measured, all at
           38 words / 0 differing: volatile on this store only, on the mask
           statement only, on both, and through a block-scope
           `volatile s16 *`. Non-volatile spellings that do NOT work, all
           35 words / 10 differing: `r->unk_1A &= 0xFFF` as the second
           statement, the same mask through `*(s16 *)((u8 *)r + 0x1A)` or
           `*(u16 *)((u8 *)r + 0x1A)` (MEM_IN_STRUCT_P does not separate
           them), and `r->unk_1A = r->unk_1A & 0xFFF`. */
        *(volatile s16 *)&r->unk_1A += p->unk_9C->unk_97;
        r->unk_1A &= 0xFFF;
    }
}

/* THE BIGGEST PAIR LEFT, and now an ALLOC PARK rather than an unknown:
   func_800BA00C and func_800BA468, 279 words each, 558 together. The source
   is written and measures 279 of 279 words with EVERY differing word a
   register assignment -- two swapped pairs and nothing else. It is in
   docs/near-misses/func_800BA00C.c with what is settled, what is not, and
   why loop 1 is written with a `goto`. Start from that file, not from the
   listing, and not from this summary.

   It is an entity spawner with FOUR construction blocks, all the same recipe:
   func_8003FD64(type, base) to allocate, func_8004491C(obj, template) to
   initialise, a handler stored at obj->+0x10, then obj+0x20 is an actor view
   whose +0x9C gets the parent and +0x98 the parent's +0x98, then obj->+0x08
   is a third object whose +0x00/+0x04 come from the second argument.
   Block 1 is type 0x136 in a 4-iteration loop, block 2 type 0x36 in a
   4-iteration loop, blocks 3 and 4 are single, types 0x36 and 0x136. Each
   block bails to the end on a null allocation.

   Four things that will mislead if read as source:

   * THE PARAMETER POINTS INTO THE MIDDLE OF ITS OBJECT. The first argument is
     used at -0x10 and -0x20 (`sw $v0, -0x10($s3)`, `addiu $a1, $s3, -0x20`),
     and every allocated object is handled as obj and obj+0x20. So the callers
     pass an interior pointer -- the +0x20 actor view -- and the container base
     is 0x20 below it. Declaring the parameter as the container type and
     subtracting will not produce these offsets.
   * FOUR DIVISION IDIOMS, none of them a branch in the source. `bgez` +
     `addiu 0xFFF` + `sra 12` is a signed divide by 0x1000; the same with
     0x1FF/`sra 9` is 0x200, with 0x3/`sra 2` is 4, and the `mult` by
     0x38E38E39 with `sra 1` is a divide by 9 (confirm with
     tools/decomp/magic.py). Several are followed by `sll`/`subu`, which makes
     them MODULO, not division.
   * `sll $v0, $s7, 24` then `sra $v0, $v0, 24` is a cast to s8, compared
     against the `lb` at +0x97 -- a signed-char comparison, not arithmetic.
   * ONLY TWO CONSTANTS DIFFER between the two members: 0x1A against 0xA at
     w84 and 0x19 against 0x9 at w258, both stored to the third object's
     +0x08. Everything else is identical, so landing one lands both. */
INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BA00C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BA468);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BA8C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BA930);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BACB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB030);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB218);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB4A0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB508);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB57C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB640);

void func_800BB688(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    *(u32 *)((u8 *)p - 0x10) = (u32)func_800BB640;
    func_800BB57C(p, q, r);
    r->unk_08 = 0x1E;
}

void func_800BB6C4(void)
{
    func_800BB508();
}

void func_800BB6E4(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    *(u32 *)((u8 *)p - 0x10) = (u32)func_800BB6C4;
    func_800BB57C(p, q, r);
    r->unk_08 = 0x1E;
}

void func_800BB720(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    *(u32 *)((u8 *)p - 0x10) = (u32)func_800BB640;
    func_800BB57C(p, q, r);
    r->unk_08 = 0xE;
}

void func_800BB75C(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    *(u32 *)((u8 *)p - 0x10) = (u32)func_800BB6C4;
    func_800BB57C(p, q, r);
    r->unk_08 = 0xE;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB798);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB894);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BBA98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BBC1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BBCA0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC00C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC1DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC290);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC390);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC45C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC574);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC6CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC764);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC840);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC8D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BC990);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BCDCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BCE78);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BD5B0);

/* Installed at UnkNode.unk_10 by func_800BD980 and reached only through
   func_80040190's cast (csrc/unk_12DEC.c), which calls it as
   `(void (*)(void *, UnkVec *, u32))fn((u8 *)p + 0x20, p->unk_08, p->unk_0C)`
   -- so three parameters, the first being the node's +0x20 actor view. `b`
   and `c` are never read here, so their types are the dispatcher's and the
   house convention's, not this function's witness.
   func_80069EF8 is the engine's out-of-image random source (all four `jal`s
   are that one symbol); `& 0x1FF` makes each draw 0..511.
   `*(u16 *)((u8 *)a - 0x2) |= 0x8000; D_800814A0 |= 0x8000;` is the unit's
   existing kill-this-node idiom -- a - 0x2 is UnkNode.unk_1E.

   THE ONE NON-OBVIOUS SPELLING is that the abs() result reuses `t`, the
   loop body's random temporary, rather than taking a variable of its own.
   It is worth 8 words. With its own block-local variable the function is at
   8 differing, every difference a register assignment inside the range-check
   block, and no spelling of that block moves it (measured: abs() once, abs()
   twice, an explicit `if (d < 0) d = -d;`, a (u32) cast, the two values or
   the two pointers hoisted into locals, reversed operands, reversed ||,
   de Morgan, the subtraction pre-folded, the range test as
   `(u32)(d - 0x200000) > 0x200000`, and three declaration orders -- all 8).
   The mechanism is local-alloc's combine_regs: at `addu` the allocator walks
   the input operands for one that dies there and ties the output to it
   (local-alloc.c:1239ff), but it bails out when that input's pseudo is not
   block-local (`reg_qty[ureg] < 0`, local-alloc.c:472 / 1762). With a private
   variable the chain val0 -> subu -> abs -> addu becomes ONE 8-reference
   quantity, which outranks the `sltu` pair, takes $v1 across words 23..33,
   and pushes the second mover pointer out of $v1 into $a0. Sharing `t` makes
   the abs destination multi-block, so the tie is refused, the addu's output
   joins the CONSTANT instead, the val0 chain stays a 6-reference quantity at
   $v0 and the second pointer gets $v1 -- the listing's allocation exactly.
   Reusing `n` (the frame counter) for it also measures 0; reusing `lim` gives
   9 and reusing `i` gives 16. */
void func_800BD688(UnkTownBD688 *a, UnkVec *b, s32 c)
{
    UnkTown2AE8 *p;
    s32 i;
    s32 t;
    s32 lim;
    s32 n;

    n = (a->unk_22 + 1) & 7;
    a->unk_22 = n;
    switch (a->unk_20)
    {
    case 0:
        if (n == 0)
        {
            t = abs(a->unk_00[0]->unk_00 - a->unk_00[1]->unk_00);
            if (t < 0x200000 || t > 0x400000)
            {
                for (i = 1; i >= 0; i--)
                {
                    t = (func_80069EF8() & 0x1FF) << 8;
                    if (a->unk_00[i]->unk_0C > 0x7FFFF)
                    {
                        t = -t;
                    }
                    a->unk_00[i]->unk_0C = t;
                    a->unk_00[i]->unk_0C -= 0x80000;
                }
            }
            for (i = 1; i >= 0; i--)
            {
                a->unk_00[i]->unk_10 = ((func_80069EF8() & 0x1FF) - 0x100) << 9;
            }
            a->unk_00[0]->unk_14 = ((func_80069EF8() & 0x1FF) - 0x100) << 8;
            a->unk_00[1]->unk_14 = (func_80069EF8() & 0x1FF) << 7;
            if (a->unk_00[0]->unk_08 < a->unk_00[1]->unk_08)
            {
                a->unk_00[1]->unk_14 = -a->unk_00[1]->unk_14;
            }
        }
        p = a->unk_00[0];
        lim = a->unk_08 - 0x1400000;
        if (p->unk_00 < lim && a->unk_00[1]->unk_00 < lim)
        {
            a->unk_00[1]->unk_0C = 0;
            p->unk_0C = 0;
            a->unk_20 = a->unk_20 + 1;
        }
        break;
    case 1:
        *(u16 *)((u8 *)a - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BD898);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BD980);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BDB20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BDBD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BDC98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BDDD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BDE7C);

void func_800BDF98(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BDFA0);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008961C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089634);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089660);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BE0D4);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089684);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BE214);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BE4D4);

void func_800BEA80(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BEA88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BEAD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BEBA4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF15C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800898C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF1C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF3F0);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800898EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF4CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF654);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089918);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF72C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF8DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BF9E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BFB00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BFB8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BFD14);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BFF7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BFFF4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C01DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C02C4);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089944);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089960);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C035C);

void func_800C0AC8(void)
{
    func_8009E218(0, 0, 0, 0);
}

void func_800C0AF4(void)
{
    func_8009E258(0, 0, 0, 0);
}

void func_800C0B20(void)
{
    func_80093C40(0, 0, 0, 0);
}

void func_800C0B4C(void)
{
    func_80093C58(0, 0, 0, 0);
}

void func_800C0B78(void)
{
    func_8009E2C0(0, 0, 0, 0);
}

void func_800C0BA4(void)
{
    func_8009E348(0, 0, 0, 0);
}

void func_800C0BD0(void)
{
    func_8009E370(0, 0, 0, 0);
}

void func_800C0BFC(void)
{
    func_8009E3D4(0, 0, 0, 0);
}

void func_800C0C28(void)
{
    func_800C0C88();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0C48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0C88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0D80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0DE8);

void func_800C0E5C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0E64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0E7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0EAC);

void func_800C0F40(void) {
}

void func_800C0F48(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0F50);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0F60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0FA4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C0FE4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1034);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1150);

void func_800C122C(void)
{
    if (D_800D4258[1] == -5)
    {
        D_800D4258[1] = -1;
        D_800D4260[1] = -1;
    }
    if (D_800D4258[2] == -5)
    {
        D_800D4258[2] = -1;
        D_800D4260[2] = -1;
    }
    if (D_800D4258[3] == -5)
    {
        D_800D4258[3] = -1;
        D_800D4260[3] = -1;
    }
    if (D_800D4258[1] != -1)
    {
        D_800D4260[1] = D_800D4258[1];
    }
    if (D_800D4258[2] != -1)
    {
        D_800D4260[2] = D_800D4258[2];
    }
    if (D_800D4258[3] != -1)
    {
        D_800D4260[3] = D_800D4258[3];
    }
    if (D_800D4260[2] != -1)
    {
        if (D_800D4268[2] != D_800D4260[2])
        {
            func_80053DA8(0x71);
            func_800542BC();
            func_8004437C(D_800D4260[2], 0);
            if (D_800D4260[3] != -1)
            {
                func_80053DA8(D_800D4260[3]);
            }
        }
        else if (D_800D4260[3] != -1)
        {
            if (D_800D4268[3] != D_800D4260[3])
            {
                func_80053DA8(0x71);
                func_800542BC();
                func_80053DA8(D_800D4260[3]);
            }
            else if (D_80080A88 == 0)
            {
                func_80053DA8(0x71);
                func_800542BC();
                func_80053DA8(D_800D4260[3]);
            }
        }
        else
        {
            func_80053DA8(0x71);
            func_800542BC();
        }
    }
    else
    {
        func_80053DA8(0x71);
        func_800542BC();
        if (D_800D4260[3] != -1)
        {
            func_80053DA8(D_800D4260[3]);
        }
    }
    if (D_800D4260[1] != -1)
    {
        func_80053DA8(D_800D4260[1]);
    }
    D_800D4258[0] = -1;
    D_800D4258[1] = -1;
    D_800D4258[2] = -1;
    D_800D4258[3] = -1;
    D_800D4268[0] = D_800D4260[0];
    D_80080A88 = 0;
    D_800D4268[1] = D_800D4260[1];
    D_800D4268[2] = D_800D4260[2];
    D_800D4268[3] = D_800D4260[3];
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1464);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C14C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1564);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C15C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C15E0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C170C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C172C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1770);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C178C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C17A0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C17B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C17D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1804);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C181C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1854);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C18CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C18F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1920);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1954);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C19C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1A0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1A44);

void func_800C1A70(void)
{
    func_8008B408(0x3);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1A90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1ABC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1AEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1B20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1B30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1B64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1B74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1BA4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1BD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1C1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1C68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1D20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1D44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1DD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1E50);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1EA4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1F8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C202C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2074);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2124);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C21F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C22A4);

void func_800C22D8(void)
{
    func_800C22A4();
    func_80024100(0, 0, 0, 0);
}

void func_800C230C(void)
{
    func_800C22A4();
    func_800254A4(0, 0, 0, 0);
}

void func_800C2340(void)
{
    func_800C22A4();
    func_80020A20(0, 0, 0, 0);
}

void func_800C2374(void)
{
    func_800C22A4();
    func_80022C8C(0, 0, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C23A8);

void func_800C23DC(void)
{
    func_800C23A8();
    func_80020300(0, 0, 0, 0);
}

void func_800C2410(void)
{
    func_800C23A8();
    func_800212B8(0, 0, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2444);

void func_800C2478(void)
{
    func_800C2444();
    func_80020298();
}

void func_800C24A0(void)
{
    func_800C2444();
    func_800224E0();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C24C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C24FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C258C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C25C0);

void func_800C2664(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    a->unk_8C = 0x40;
    a->unk_8E = 0x40;
    ((void (*)(void *, void *, s32))func_8008F074)(a, b, D_800D4F88[((u8 *)a)[0x94]]);
    a->unk_50 = func_800C26C0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C26C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2728);

void func_800C2780(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    a->unk_8C = 0x40;
    a->unk_8E = 0x40;
    ((void (*)(void *, void *, s32))func_8008F074)(a, b, D_800D4F88[((u8 *)a)[0x94]]);
    a->unk_50 = func_800C27DC;
}

void func_800C27DC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C27E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C283C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2864);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2890);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C28E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C296C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2988);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2A10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2A60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2AB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2AE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2B38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2B6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2B88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2BE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2C80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2CB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2D0C);

void func_800C2D34(UnkTownActor *a, UnkTownRend *b, void **t)
{
    *(void ***)((u8 *)a + 0x78) = t;
    func_800C2CB0((u8 *)a, (s32)b, (*(void ***)((u8 *)a + 0x78))[func_800C2D0C(a->unk_72)], 0);
    b->unk_05 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2D94);

void func_800C2DBC(UnkTownActor *a, UnkTownRend *b, void **t)
{
    *(void ***)((u8 *)a + 0x78) = t;
    func_800C2CB0((u8 *)a, (s32)b, (*(void ***)((u8 *)a + 0x78))[func_800C2D94(a->unk_72)], 0);
    b->unk_05 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2E1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2E84);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2F14);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2F88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3050);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C30A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C30E0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3174);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C321C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C337C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3438);

void func_800C34CC(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C30E0(a, b, c);
}

void func_800C34EC(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    if (func_800C30E0(a, b, c) == 0)
    {
        if (--a->unk_6C <= 0)
        {
            func_800C382C(a, b, c);
        }
    }
}

void func_800C355C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    s32 dir;

    if (func_800C30E0(a, b, c) != 0)
    {
        return;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    *(s16 *)((u8 *)b + 0xA) = func_800C2B38(b);
    dir = (0x400 - a->unk_72) / 0x400 & 3;
    switch (dir)
    {
    case 0:
        if (a->unk_84 + a->unk_8C >= *(s16 *)((u8 *)b + 2))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 + a->unk_8C;
        break;
    case 1:
        if (a->unk_86 + a->unk_8E >= *(s16 *)((u8 *)b + 6))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 + a->unk_8E;
        break;
    case 2:
        if (*(s16 *)((u8 *)b + 2) >= a->unk_84 - a->unk_8C)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 - a->unk_8C;
        break;
    default:
        if (*(s16 *)((u8 *)b + 6) >= a->unk_86 - a->unk_8E)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 - a->unk_8E;
        break;
    }
    func_800C37C4(a, b, c);
    return;

drift:
    b->unk_0C += D_800D4FE8[dir];
    if (abs(b->unk_0C) > 0x20000)
    {
        b->unk_0C = D_800D4FF8[dir];
    }
    b->unk_10 += D_800D5008[dir];
    if (abs(b->unk_10) > 0x20000)
    {
        b->unk_10 = D_800D5018[dir];
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3780);

void func_800C37C4(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, a->unk_7C->unk_00);
    a->unk_50 = func_800C34EC;
    a->unk_6C = (u16)func_800374F4(4) * 21 + 0x15;
}

void func_800C382C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    s32 i;
    u16 r;

    b->unk_0C = 0;
    b->unk_10 = 0;
    for (i = 0; i < 0x10; i++)
    {
        r = func_800374F4(4);
        if (r == 0)
        {
            if (*(s16 *)((u8 *)b + 2) < a->unk_84 + a->unk_8C)
            {
                a->unk_72 = 0x400;
                break;
            }
        }
        else if (r == 1)
        {
            if (*(s16 *)((u8 *)b + 6) < a->unk_86 + a->unk_8E)
            {
                a->unk_72 = 0;
                break;
            }
        }
        else if (r == 2)
        {
            if (a->unk_84 - a->unk_8C < *(s16 *)((u8 *)b + 2))
            {
                a->unk_72 = 0xC00;
                break;
            }
        }
        else
        {
            if (a->unk_86 - a->unk_8E < *(s16 *)((u8 *)b + 6))
            {
                a->unk_72 = 0x800;
                break;
            }
        }
    }
    func_800C2E84(a, c, a->unk_7C->unk_04);
    a->unk_50 = func_800C355C;
}

void func_800C3960(UnkTownDispatch *d)
{
    d->unk_54();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3988);

void func_800C3A98(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2B38(b);
    if (--a->unk_6C <= 0)
    {
        *(s16 *)((u8 *)b + 0x2) = a->unk_84;
        *(s16 *)((u8 *)b + 0x6) = a->unk_86;
        a->unk_72 = *(s16 *)((u8 *)a + 0x6E);
        func_800C4174(a, b, c);
    }
    else
    {
        *(s16 *)((u8 *)b + 0x2) += (a->unk_84 - *(s16 *)((u8 *)b + 0x2)) / a->unk_6C;
        *(s16 *)((u8 *)b + 0x6) += (a->unk_86 - *(s16 *)((u8 *)b + 0x6)) / a->unk_6C;
    }
}

void func_800C3B7C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

void func_800C3BB8(UnkTownActor *a)
{
    if (--a->unk_6C <= 0)
    {
        func_800C41D4();
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3BF4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3C28);

void func_800C3C5C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C4174(a, b, c);
}

void func_800C3C7C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2B38(b);
    if (--a->unk_6C < 0)
    {
        func_800C4174(a, b, c);
    }
    else
    {
        b->unk_04 += -0x20000;
    }
}

void func_800C3D08(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2B38(b);
    if (--a->unk_6C < 0)
    {
        func_800C4174(a, b, c);
    }
    else
    {
        b->unk_04 += 0x20000;
    }
}

void func_800C3D94(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2B38(b);
    if (--a->unk_6C < 0)
    {
        func_800C4174(a, b, c);
    }
    else
    {
        b->unk_00 += -0x20000;
    }
}

void func_800C3E20(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    *(s16 *)((u8 *)b + 0xA) = func_800C2B38(b);
    if (--a->unk_6C < 0)
    {
        func_800C4174(a, b, c);
    }
    else
    {
        b->unk_00 += 0x20000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3EAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3F44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3FC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3FFC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C40D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4134);

void func_800C4174(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C2E84(a, (s32)c, a->unk_80->unk_04);
    func_800C41D4(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C41D4);

void func_800C41FC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_08);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3A98;
    a->unk_6C = 0x20;
}

void func_800C4260(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_0C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x15;
}

void func_800C42C4(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_10);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x15;
}

void func_800C4328(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_14);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C5C;
    a->unk_72 = 0x800;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C438C);

void func_800C43EC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_14);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C5C;
    a->unk_72 = 0xC00;
}

void func_800C4450(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_14);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C5C;
    a->unk_72 = 0x400;
}

void func_800C44B4(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_18);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C7C;
    a->unk_6C = 0x20;
}

void func_800C4518(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_18);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3D08;
    a->unk_6C = 0x20;
}

void func_800C457C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_18);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3D94;
    a->unk_6C = 0x20;
}

void func_800C45E0(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_18);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3E20;
    a->unk_6C = 0x20;
}

void func_800C4644(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_1C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3FC8;
    a->unk_6C = 0x15;
}

void func_800C46A8(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    Unk80082660 *e;

    D_80082660[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3FFC;
    a->unk_6C = func_8003BD84((a->unk_88 << 16) - b->unk_00,
                              (a->unk_8A << 16) - b->unk_04) / 0x20000;
    e = &D_80082660[a->unk_60];
    if (*(s8 *)((u8 *)e + 2) == -0x80)
    {
        b->unk_00 = a->unk_88 << 16;
        b->unk_04 = a->unk_8A << 16;
        *(s16 *)((u8 *)b + 0xA) += -0x40;
        *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
        func_800C4174(a, b, c);
    }
    else if (*(s8 *)((u8 *)e + 2) > 0)
    {
        a->unk_6C = (a->unk_6C << 4) / (*(s8 *)((u8 *)e + 2) + 0x10);
    }
    else if (*(s8 *)((u8 *)e + 2) < 0)
    {
        a->unk_6C = a->unk_6C * (abs(*(s8 *)((u8 *)e + 2)) + 0x10) / 16;
    }
}

void func_800C4804(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C2E84(a, (s32)c, a->unk_80->unk_24);
    func_800C46A8(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4864);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C48DC);

void func_800C4958(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    Unk80082660 *p;

    func_80033D08((u8 *)a);
    p = D_80082660;
    p[a->unk_60].unk_00 = 0;
    func_800C4174(a, b, c);
}

void func_800C49C0(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    Unk80082660 *p;

    func_80033CD8((u8 *)a, (u32)D_80045340);
    p = D_80082660;
    p[a->unk_60].unk_00 = 0;
    func_800C4174(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4A30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4A88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4AEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4C88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4D20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4D80);

void func_800C4E30(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *r)
{
    if (--a->unk_6C <= 0)
    {
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        func_800C4174(a, b, r);
    }
    else
    {
        r->unk_1C = 0x1000 - func_800644B8(a->unk_6C * 409) / 4;
        r->unk_1E = func_800644B8(a->unk_6C * 409) / 4 + 0x1000;
    }
}

void func_800C4F08(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, a->unk_80->unk_1C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C4E30;
    a->unk_6C = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4F6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5064);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C50CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5100);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C51DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5214);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C52D0);

void func_800C5368(void *arg0)
{
    func_800C3050(arg0, 0x30, D_800D51AC, D_800D51B0, D_800D517C, D_800D5184);
}

void func_800C53B0(void *arg0)
{
    func_800C3050(arg0, 0x1, D_800D51F8, D_800D51FC, D_800D51C8, D_800D51D0);
}

void func_800C53F8(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *r)
{
    if (--a->unk_6C <= 0)
    {
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        func_800C4174(a, b, r);
    }
    else
    {
        r->unk_1C = 0x1000 - func_800644B8(a->unk_6C * 409) / 4;
        r->unk_1E = func_800644B8(a->unk_6C * 409) / 4 + 0x1000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C54D0);

void func_800C5500(void *arg0)
{
    func_800C3050(arg0, 0x33, D_800D5270, D_800D5274, D_800D5240, D_800D5248);
}

void func_800C5548(void *arg0)
{
    func_800C3050(arg0, 0x51, D_800D52E4, D_800D52E8, D_800D52B4, D_800D52BC);
}

void func_800C5590(void *arg0)
{
    func_800C3050(arg0, 0x10, D_800D5358, D_800D535C, D_800D5328, D_800D5330);
}

void func_800C55D8(void *arg0)
{
    func_800C3050(arg0, 0x16, D_800D53F4, D_800D53F8, D_800D53C4, D_800D53CC);
}

void func_800C5620(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C565C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C56A8);

void func_800C5724(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5360);
    D_80082660[22].unk_00 = 0;
    a->unk_54 = func_800C5620;
    a->unk_6C = 0x15;
}

void func_800C5774(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D539C);
    D_80082660[22].unk_00 = 0;
    a->unk_54 = func_800C5620;
    a->unk_6C = 0x15;
}

void func_800C57C4(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D53B0);
    D_80082660[22].unk_00 = 0;
    a->unk_54 = func_800C5620;
    a->unk_6C = 0x15;
}

void func_800C5814(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5388);
    D_80082660[22].unk_00 = 0;
    a->unk_54 = func_800C565C;
    a->unk_6C = 0x4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5864);

void func_800C58AC(void *arg0)
{
    func_800C3050(arg0, 0x13, D_800D5478, D_800D547C, D_800D5448, D_800D5450);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C58F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C591C);

void func_800C5944(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D54BC);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C5ADC;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C599C);

void func_800C59FC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D54D4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C5C70;
}

void func_800C5A54(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_58 = D_800D5618;
    a->unk_5C = D_800D561C;
    a->unk_7C = (UnkTownArg80 *)D_800D55E8;
    a->unk_80 = (UnkTownArg80 *)D_800D55F0;
    func_800C2E84(a, c, D_800D54DC);
    a->unk_54 = func_800C3BF4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5ADC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5B48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5BA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5C70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5D38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5D94);

void func_800C5E00(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D5540);
    func_800C58F4(a, b, c);
}

void func_800C5E5C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D5554);
    func_800C58F4(a, b, c);
}

void func_800C5EB8(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D5568);
    func_800C58F4(a, b, c);
}

void func_800C5F14(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D557C);
    func_800C58F4(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5F70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5FDC);

void func_800C6094(void *arg0)
{
    func_800C3050(arg0, 0xD, D_800D55D0, D_800D55D4, D_800D55A0, D_800D55A8);
}

void func_800C60DC(void *arg0)
{
    func_800C3050(arg0, 0xD, D_800D5618, D_800D561C, D_800D55E8, D_800D55F0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6124);

void func_800C6180(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    func_8003DB94(r, D_800ED15C, 0);
    func_8008F01C(p, q, D_800D0678);
    func_8009B218(p, q, r, D_800D0690);
    *((u8 *)p + 0x4D) = 0xD;
    *((u8 *)p + 0x4C) = 0x7;
}

void func_800C620C(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    func_8003DB94(r, D_800ED15C, 0);
    func_8009B218(p, q, r, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6270);

void func_800C629C(void *arg0)
{
    func_800C3050(arg0, 0xD, D_800D572C, D_800D5730, D_800D56FC, D_800D5704);
}

void func_800C62E4(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5670);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x20;
}

void func_800C6344(void *arg0)
{
    func_800C3050(arg0, 0x23, D_800D57A4, D_800D57A8, D_800D5774, D_800D577C);
}

void func_800C638C(void *arg0)
{
    func_800C3050(arg0, 0x3F, D_800D57A4, D_800D57A8, D_800D5774, D_800D577C);
}

void func_800C63D4(void *arg0)
{
    func_800C3050(arg0, 0x20, D_800D5818, D_800D581C, D_800D57E8, D_800D57F0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C641C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6440);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C645C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6478);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C648C);

void func_800C64C8(void *a, UnkTown2AE8 *b, s32 c)
{
    func_800C3050(a, 0x9, D_800D5988, D_800D598C, D_800D5958, D_800D5960);
    func_800C641C(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6544);

void func_800C65C8(void *a, UnkTown2AE8 *b, s32 c)
{
    func_800C3050(a, 0x9, D_800D5988, D_800D598C, D_800D5958, D_800D5960);
    func_800C645C(a, b, c);
}

void func_800C6644(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

void func_800C6680(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C2E84(a, (s32)c, D_800D5944);
        a->unk_54 = func_800C6644;
        a->unk_6C = 0x5;
    }
}

void func_800C66E0(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D591C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C6680;
    a->unk_6C = 0x6;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6740);

void func_800C67B4(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D58A4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C6740;
    a->unk_6C = 0x5;
}

void func_800C6814(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C641C();
        func_800C4174(a, b, c);
    }
}

void func_800C6880(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D58CC);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C6814;
    a->unk_6C = 0x5;
}

void func_800C68E0(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C645C();
        func_800C4174(a, b, c);
    }
}

void func_800C694C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D58E0);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C68E0;
    a->unk_6C = 0x9;
}

void func_800C69AC(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C641C();
        func_800C4174(a, b, c);
    }
}

void func_800C6A18(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5908);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C69AC;
    a->unk_6C = 0x7;
}

void func_800C6A78(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C6478();
        func_800C4174(a, b, c);
    }
}

void func_800C6AE4(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D591C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C6A78;
    a->unk_6C = 0x5;
}

void func_800C6B44(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C641C();
        func_800C4174(a, b, c);
    }
}

void func_800C6BB0(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5944);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C6B44;
    a->unk_6C = 0x6;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6C10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6CE4);

void func_800C6DC8(void *arg0)
{
    func_800C3050(arg0, 0xF, D_800D5A80, D_800D5A84, D_800D5A50, D_800D5A58);
}

void func_800C6E10(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D59C4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x20;
}

void func_800C6E70(void *arg0)
{
    func_800C3050(arg0, 0x2F, D_800D5AF8, D_800D5AFC, D_800D5AC8, D_800D5AD0);
}

void func_800C6EB8(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x17, D_800D5AF8, D_800D5AFC, D_800D5AC8, D_800D5AD0);
    c->unk_12 = 0x280;
    c->unk_14 &= 0xFEFF;
}

void func_800C6F20(void *arg0)
{
    func_800C3050(arg0, 0x6, D_800D5C08, D_800D5C0C, D_800D5BD8, D_800D5BE0);
}

void func_800C6F68(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5B90);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C28;
}

void func_800C6FC0(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5BB4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3BF4;
}

void func_800C7018(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5BB4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C28;
}

void func_800C7070(void *arg0)
{
    func_800C3050(arg0, 0x3, D_800D5D48, D_800D5D4C, D_800D5D18, D_800D5D20);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C70B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7138);

void func_800C71A0(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5CAC);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C7138;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C71F8);

void func_800C722C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D5CF4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C71F8;
}

void func_800C7284(void *arg0)
{
    func_800C3050(arg0, 0x35, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
}

void func_800C72CC(void *arg0)
{
    func_800C3050(arg0, 0x37, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
}

void func_800C7314(void *arg0)
{
    func_800C3050(arg0, 0x31, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
}

void func_800C735C(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x1C, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
    c->unk_12 = 0x240;
    c->unk_14 &= 0xFEFF;
}

void func_800C73C4(void *arg0)
{
    func_800C3050(arg0, 0x34, D_800D5E3C, D_800D5E40, D_800D5E0C, D_800D5E14);
}

void func_800C740C(void *arg0)
{
    func_800C3050(arg0, 0x36, D_800D5E3C, D_800D5E40, D_800D5E0C, D_800D5E14);
}

void func_800C7454(void *arg0)
{
    func_800C3050(arg0, 0x38, D_800D5E3C, D_800D5E40, D_800D5E0C, D_800D5E14);
}

void func_800C749C(void *arg0)
{
    func_800C3050(arg0, 0x3C, D_800D5EB0, D_800D5EB4, D_800D5E80, D_800D5E88);
}

void func_800C74E4(void *arg0)
{
    func_800C3050(arg0, 0x29, D_800D5F24, D_800D5F28, D_800D5EF4, D_800D5EFC);
}

void func_800C752C(void *arg0)
{
    func_800C3050(arg0, 0x2A, D_800D5F24, D_800D5F28, D_800D5EF4, D_800D5EFC);
}

void func_800C7574(void *arg0)
{
    func_800C3050(arg0, 0x1B, D_800D5F98, D_800D5F9C, D_800D5F68, D_800D5F70);
}

void func_800C75BC(void *arg0)
{
    func_800C3050(arg0, 0x4, D_800D603C, D_800D6048, D_800D600C, D_800D6014);
}

void func_800C7604(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    if (func_800C30E0(a, b, c) == 0)
    {
        if (--a->unk_6C <= 0)
        {
            func_800C7AC4(a, b, c);
        }
    }
}

void func_800C7674(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    s32 dir;

    dir = (0x400 - a->unk_72) / 0x400 & 3;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
    if (func_800C30E0(a, b, c) != 0)
    {
        return;
    }
    switch (dir)
    {
    case 0:
        if (a->unk_84 + a->unk_8C >= *(s16 *)((u8 *)b + 2))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 + a->unk_8C;
        break;
    case 1:
        if (a->unk_86 + a->unk_8E >= *(s16 *)((u8 *)b + 6))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 + a->unk_8E;
        break;
    case 2:
        if (*(s16 *)((u8 *)b + 2) >= a->unk_84 - a->unk_8C)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 - a->unk_8C;
        break;
    default:
        if (*(s16 *)((u8 *)b + 6) >= a->unk_86 - a->unk_8E)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 - a->unk_8E;
        break;
    }
    func_800C7A6C(a, b, c);
    return;

drift:
    b->unk_0C += D_800D4FE8[dir];
    if (abs(b->unk_0C) > 0x20000)
    {
        b->unk_0C = D_800D4FF8[dir];
    }
    b->unk_10 += D_800D5008[dir];
    if (abs(b->unk_10) > 0x20000)
    {
        b->unk_10 = D_800D5018[dir];
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C78A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7A00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7A6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7AC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7BF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7C88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7CAC);

void func_800C7CB4(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7DB8);

void func_800C7E70(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    a->unk_6C--;
    if (a->unk_6C <= 0)
    {
        a->unk_72 = *(s16 *)((u8 *)a + 0x6E);
        func_800C4174(a, b, c);
    }
}

void func_800C7EB0(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    a->unk_6C--;
    if (a->unk_6C <= 0)
    {
        a->unk_72 = *(s16 *)((u8 *)a + 0x6E);
        func_800C4174(a, b, c);
    }
}

void func_800C7EF0(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C4174(a, b, c);
}

void func_800C7F10(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5FE8);
    D_80082660[4].unk_00 = 0;
    a->unk_54 = func_800C7CB4;
    a->unk_6C = 0x15;
}

void func_800C7F60(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5FE8);
    D_80082660[4].unk_00 = 0;
    a->unk_54 = func_800C7CF0;
    a->unk_6C = 0x10;
}

void func_800C7FB0(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5FE8);
    D_80082660[4].unk_00 = 0;
    a->unk_54 = func_800C7DB8;
    a->unk_6C = 0x10;
}

void func_800C8000(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5FE8);
    D_80082660[4].unk_00 = 0;
    a->unk_54 = func_800C7E70;
    a->unk_6C = 0xA;
    a->unk_72 = 0xC00;
}

void func_800C8058(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5FE8);
    D_80082660[4].unk_00 = 0;
    a->unk_54 = func_800C7EB0;
    a->unk_6C = 0xA;
    a->unk_72 = 0x400;
}

void func_800C80B0(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D5FA0);
    D_80082660[4].unk_00 = 0;
    a->unk_54 = func_800C7EF0;
    a->unk_72 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C80FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8120);

void func_800C814C(void *arg0)
{
    func_800C3050(arg0, 0x4, D_800D60C0, D_800D60C4, D_800D6090, D_800D6098);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8194);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C81E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8248);

void func_800C82AC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6110);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x1C;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C830C);

void func_800C8330(u8 *a, s32 b, u8 *c)
{
    a[0x93] = 1;
    *(u32 *)(c + 0x28) = D_80081458;
    func_800C3050(a, 0x2, D_800D61F4, D_800D6200, D_800D61C4, D_800D61CC);
}

void func_800C838C(UnkTownActor *a)
{
    func_800C3050(a, 0xA, D_800D6308, D_800D630C, D_800D62D8, D_800D62E0);
    a->unk_80->unk_04 = D_800D6244;
}

void func_800C83F0(UnkTownActor *a)
{
    func_800C3050(a, 0xA, D_800D6308, D_800D630C, D_800D62D8, D_800D62E0);
    a->unk_80->unk_04 = D_800D62C4;
}

void func_800C8454(UnkTownActor *a)
{
    func_800C3050(a, 0xA, D_800D6308, D_800D630C, D_800D62D8, D_800D62E0);
    a->unk_80->unk_04 = D_800D6220;
}

void func_800C84B8(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C8928();
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8524);

void func_800C85A0(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D62A4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C8524;
    a->unk_6C = 0x15;
}

void func_800C8600(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    a->unk_80->unk_04 = D_800D62C4;
    D_80082660[a->unk_60].unk_00 = 0;
    func_800C4174(a, b, c);
}

void func_800C8644(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    a->unk_80->unk_04 = D_800D6244;
    D_80082660[a->unk_60].unk_00 = 0;
    func_800C4174(a, b, c);
}

void func_800C8688(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    a->unk_80->unk_04 = D_800D6220;
    D_80082660[a->unk_60].unk_00 = 0;
    func_800C4174(a, b, c);
}

/* A pure passthrough: the target's jal delay slot is a nop, with no argument
   setup at all, because $a0/$a1/$a2 already hold this function's own three
   parameters. Spelled with the parameters rather than (void) because
   func_800C8644 forwards all three to func_800C4174, which is pinned at three.
   Measured 0 differing both before and after naming them. */
void func_800C86CC(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C8644(a, b, c);
}

void func_800C86EC(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    b->unk_08 += b->unk_14;
    if ((s16)func_800C2AE8(b) < *(s16 *)((u8 *)b + 0xA))
    {
        *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
        b->unk_14 = 0;
        func_800C4174(a, b, c);
    }
    else
    {
        func_80095388(b);
    }
}

void func_800C8788(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_800C2E84(a, c, D_800D6290);
        a->unk_54 = func_800C86EC;
    }
}

void func_800C8808(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D627C);
    a->unk_54 = func_800C8788;
    b->unk_14 = -0xA0000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C885C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C88AC);

void func_800C88E0(void *arg0)
{
    func_800C3050(arg0, 0xA, D_800D6360, D_800D6364, D_800D6330, D_800D6338);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8928);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C895C);

void func_800C899C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C895C();
        func_800C4174(a, b, c);
    }
}

void func_800C8A08(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D62BC);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C899C;
    a->unk_6C = 0x8;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8A68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8B5C);

void func_800C8B8C(UnkTownDispatch *d)
{
    d->unk_50();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8BB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8C3C);

void func_800C8D34(UnkTownSpawn *p, UnkVec *q, UnkTownRend *r)
{
    func_8003DB94(r, D_800EE344, 0);
    func_8008F01C(p, q, D_800D0678);
    func_8009B218(p, q, r, D_800D0690);
    *((u8 *)p + 0x4D) = 0xD;
    *((u8 *)p + 0x4C) = 0x5;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8DC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8E9C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8F80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8FF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C907C);

void func_800C90D4(void *arg0)
{
    func_800C3050(arg0, 0x3B, D_800D63EC, D_800D63F0, D_800D63BC, D_800D63C4);
}

void func_800C911C(void *arg0)
{
    func_800C3050(arg0, 0x1F, D_800D63EC, D_800D63F0, D_800D63BC, D_800D63C4);
}

void func_800C9164(void *arg0)
{
    func_800C3050(arg0, 0x52, D_800D63EC, D_800D63F0, D_800D63BC, D_800D63C4);
}

void func_800C91AC(void *arg0)
{
    func_800C3050(arg0, 0x24, D_800D6460, D_800D6464, D_800D6430, D_800D6438);
}

void func_800C91F4(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x1A, D_800D6460, D_800D6464, D_800D6430, D_800D6438);
    c->unk_12 = 0x2C0;
    c->unk_14 &= 0xFEFF;
}

void func_800C925C(void *arg0)
{
    func_800C3050(arg0, 0x44, D_800D64C0, D_800D64C4, D_800D6490, D_800D6498);
}

void func_800C92A4(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    if (func_800C30E0(a, b, c) == 0)
    {
        if (--a->unk_6C <= 0)
        {
            func_800C9610(a, b, c);
        }
    }
}

void func_800C9314(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    if (func_800C30E0(a, b, c) == 0)
    {
        if (--a->unk_6C <= 0)
        {
            func_800C9678(a, b, c);
        }
    }
}

void func_800C9384(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    s32 dir;

    if (func_800C30E0(a, b, c) != 0)
    {
        return;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    *(s16 *)((u8 *)b + 0xA) = func_800C2B38(b);
    dir = (0x400 - a->unk_72) / 0x400 & 3;
    switch (dir)
    {
    case 0:
        if (a->unk_84 + a->unk_8C >= *(s16 *)((u8 *)b + 2))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 + a->unk_8C;
        break;
    case 1:
        if (a->unk_86 + a->unk_8E >= *(s16 *)((u8 *)b + 6))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 + a->unk_8E;
        break;
    case 2:
        if (*(s16 *)((u8 *)b + 2) >= a->unk_84 - a->unk_8C)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 - a->unk_8C;
        break;
    default:
        if (*(s16 *)((u8 *)b + 6) >= a->unk_86 - a->unk_8E)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 - a->unk_8E;
        break;
    }
    func_800C95A8(a, b, c);
    return;

drift:
    b->unk_0C += D_800D4FE8[dir];
    if (abs(b->unk_0C) > 0x20000)
    {
        b->unk_0C = D_800D4FF8[dir];
    }
    b->unk_10 += D_800D5008[dir];
    if (abs(b->unk_10) > 0x20000)
    {
        b->unk_10 = D_800D5018[dir];
    }
}

void func_800C95A8(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, a->unk_7C->unk_00);
    a->unk_50 = func_800C92A4;
    a->unk_6C = (u16)func_800374F4(4) * 21 + 0x15;
}

void func_800C9610(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, a->unk_7C->unk_00);
    a->unk_50 = func_800C9314;
    a->unk_6C = (u16)func_800374F4(4) * 21 + 0x15;
}

void func_800C9678(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    s32 i;
    u16 r;

    b->unk_0C = 0;
    b->unk_10 = 0;
    for (i = 0; i < 0x10; i++)
    {
        r = func_800374F4(4);
        if (r == 0)
        {
            if (*(s16 *)((u8 *)b + 2) < a->unk_84 + a->unk_8C)
            {
                a->unk_72 = 0x400;
                break;
            }
        }
        else if (r == 1)
        {
            if (*(s16 *)((u8 *)b + 6) < a->unk_86 + a->unk_8E)
            {
                a->unk_72 = 0;
                break;
            }
        }
        else if (r == 2)
        {
            if (a->unk_84 - a->unk_8C < *(s16 *)((u8 *)b + 2))
            {
                a->unk_72 = 0xC00;
                break;
            }
        }
        else
        {
            if (a->unk_86 - a->unk_8E < *(s16 *)((u8 *)b + 6))
            {
                a->unk_72 = 0x800;
                break;
            }
        }
    }
    func_800C2E84(a, c, a->unk_7C->unk_04);
    a->unk_50 = func_800C9384;
}

void func_800C97AC(void *arg0)
{
    func_800C3050(arg0, 0xB, D_800D6598, D_800D65A0, D_800D6568, D_800D6570);
}

void func_800C97F4(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    if (func_800C30E0(a, b, c) == 0)
    {
        if (--a->unk_6C <= 0)
        {
            func_800C9DB8(a, b, c);
        }
    }
}

void func_800C9864(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    s32 dir;

    dir = (0x400 - a->unk_72) / 0x400 & 3;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
    if (func_800C30E0(a, b, c) != 0)
    {
        return;
    }
    switch (dir)
    {
    case 0:
        if (a->unk_84 + a->unk_8C >= *(s16 *)((u8 *)b + 2))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 + a->unk_8C;
        break;
    case 1:
        if (a->unk_86 + a->unk_8E >= *(s16 *)((u8 *)b + 6))
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 + a->unk_8E;
        break;
    case 2:
        if (*(s16 *)((u8 *)b + 2) >= a->unk_84 - a->unk_8C)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 2) = a->unk_84 - a->unk_8C;
        break;
    default:
        if (*(s16 *)((u8 *)b + 6) >= a->unk_86 - a->unk_8E)
        {
            goto drift;
        }
        *(s16 *)((u8 *)b + 6) = a->unk_86 - a->unk_8E;
        break;
    }
    func_800C9DB8(a, b, c);
    return;

drift:
    b->unk_0C += D_800D4FE8[dir];
    if (abs(b->unk_0C) > 0x20000)
    {
        b->unk_0C = D_800D4FF8[dir];
    }
    b->unk_10 += D_800D5008[dir];
    if (abs(b->unk_10) > 0x20000)
    {
        b->unk_10 = D_800D5018[dir];
    }
}

void func_800C9A98(UnkTownActor *a)
{
    if (--a->unk_6C <= 0)
    {
        func_800C9CDC();
    }
}

void func_800C9AD4(void *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_800C9D40(a, b, c);
    }
}

void func_800C9B44(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    if ((s16)func_800C2AE8(b) < *(s16 *)((u8 *)b + 0xA))
    {
        *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
        if (--a->unk_90 <= 0)
        {
            b->unk_14 = 0;
            func_800C9DB8(a, b, c);
        }
        else
        {
            func_800C9C94(a, b, c);
        }
    }
    else
    {
        func_80095388(b);
    }
}

void func_800C9C0C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64C8);
    a->unk_50 = func_800C97F4;
    a->unk_6C = 0x15;
}

void func_800C9C54(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64DC);
    a->unk_50 = func_800C9864;
}

void func_800C9C94(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64F0);
    a->unk_50 = func_800C9A98;
    a->unk_6C = 0x4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9CDC);

void func_800C9D40(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D6518);
    a->unk_50 = func_800C9B44;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9D80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9DB8);

void func_800C9F1C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9F58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9FBC);

void func_800CA020(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    b->unk_00 += 0x40000;
    b->unk_04 += 0x15555;
    if ((u16)c->unk_14 & 0x8000)
    {
        func_80033AA8(0x49);
        func_8008F134(a);
        func_800C30A4(a);
    }
}

void func_800CA094(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    a->unk_6C--;
    if (a->unk_6C <= 0)
    {
        a->unk_72 = *(s16 *)((u8 *)a + 0x6E);
        func_800C4174(a, b, c);
    }
}

void func_800CA0D4(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    a->unk_6C--;
    if (a->unk_6C <= 0)
    {
        a->unk_72 = *(s16 *)((u8 *)a + 0x6E);
        func_800C4174(a, b, c);
    }
}

void func_800CA114(void)
{
    func_800CA744();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA134);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA214);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA308);

void func_800CA3EC(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C4174(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA40C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA458);

void func_800CA4CC(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D652C);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800C9F1C;
    a->unk_6C = 0x15;
}

void func_800CA51C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D6504);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800C9F1C;
    a->unk_6C = 0x15;
}

void func_800CA56C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64C8);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800C9F58;
    a->unk_6C = 0x4;
}

void func_800CA5BC(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64DC);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800C9FBC;
    a->unk_6C = 0x8;
}

void func_800CA60C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64DC);
    a->unk_54 = func_800CA020;
}

void func_800CA64C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64DC);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800CA094;
    a->unk_6C = 0xA;
    a->unk_72 = 0xC00;
}

void func_800CA6A4(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64DC);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800CA0D4;
    a->unk_6C = 0xA;
    a->unk_72 = 0x400;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA6FC);

void func_800CA744(UnkTownActor *a)
{
    a->unk_54 = func_800CA134;
    a->unk_6C = 0xA;
}

void func_800CA75C(UnkTownActor *a)
{
    a->unk_54 = func_800CA214;
    a->unk_6C = 0xD;
}

void func_800CA774(UnkTownActor *a)
{
    a->unk_54 = func_800CA308;
    a->unk_6C = 0xA;
}

void func_800CA78C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64C8);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800CA3EC;
    a->unk_72 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA7D8);

void func_800CA7EC(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D6554);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800C9F1C;
    a->unk_6C = 0x15;
}

void func_800CA83C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D64DC);
    D_80082660[11].unk_00 = 0;
    a->unk_54 = func_800CA40C;
    a->unk_6C = 0x4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA88C);

void func_800CA8D4(u8 *a, s32 b, u8 *c)
{
    a[0x93] = 1;
    *(u32 *)(c + 0x28) = D_80081458;
    func_800C3050(a, 0x4B, D_800D66A4, D_800D66A8, D_800D6674, D_800D667C);
}

void func_800CA930(u8 *a, s32 b, u8 *c)
{
    a[0x93] = 1;
    *(u32 *)(c + 0x28) = D_80081458;
    func_800C3050(a, 0x4C, D_800D66A4, D_800D66A8, D_800D6674, D_800D667C);
}

void func_800CA98C(u8 *a, s32 b, u8 *c)
{
    a[0x93] = 1;
    *(u32 *)(c + 0x28) = D_80081458;
    func_800C3050(a, 0x4D, D_800D66A4, D_800D66A8, D_800D6674, D_800D667C);
}

void func_800CA9E8(u8 *a, s32 b, u8 *c)
{
    a[0x93] = 1;
    *(u32 *)(c + 0x28) = D_80081458;
    func_800C3050(a, 0x4E, D_800D66A4, D_800D66A8, D_800D6674, D_800D667C);
}

void func_800CAA44(void *arg0)
{
    func_800C3050(arg0, 0x2D, D_800D6718, D_800D671C, D_800D66E8, D_800D66F0);
}

void func_800CAA8C(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x4A, D_800D6718, D_800D671C, D_800D66E8, D_800D66F0);
    c->unk_12 = 0x1;
}

void func_800CAAE8(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3D, D_800D6718, D_800D671C, D_800D66E8, D_800D66F0);
    c->unk_12 = 0x2;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CAB44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CABB4);

void func_800CAC28(void *arg0)
{
    func_800C3050(arg0, 0x12, D_800D67A0, D_800D67A4, D_800D6770, D_800D6778);
}

void func_800CAC70(void *arg0)
{
    func_800C3050(arg0, 0x12, D_800D67DC, D_800D67E0, D_800D67AC, D_800D67B4);
}

void func_800CACB8(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_58 = D_800D67DC;
    a->unk_5C = D_800D67E0;
    a->unk_7C = (UnkTownArg80 *)D_800D67AC;
    a->unk_80 = (UnkTownArg80 *)D_800D67B4;
    func_800C2E84(a, c, D_800D675C);
    a->unk_54 = func_800C3BF4;
}

void func_800CAD40(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_58 = D_800D67A0;
    a->unk_5C = D_800D67A4;
    a->unk_7C = (UnkTownArg80 *)D_800D6770;
    a->unk_80 = (UnkTownArg80 *)D_800D6778;
    func_800C2E84(a, c, D_800D6734);
    a->unk_54 = func_800C3BF4;
}

void func_800CADC8(void *arg0)
{
    func_800C3050(arg0, 0x2E, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAE10(void *arg0)
{
    func_800C3050(arg0, 0x26, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAE58(void *arg0)
{
    func_800C3050(arg0, 0x1E, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAEA0(void *arg0)
{
    func_800C3050(arg0, 0x39, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAEE8(void *arg0)
{
    func_800C3050(arg0, 0x3E, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAF30(void *arg0)
{
    func_800C3050(arg0, 0x18, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAF78(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x19, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
    c->unk_12 = 0x240;
    c->unk_14 &= 0xFEFF;
}

void func_800CAFE0(void *arg0)
{
    func_800C3050(arg0, 0x45, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB028);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB04C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB060);

void func_800CB074(void *a, UnkTown2AE8 *b, s32 c)
{
    func_800C3050(a, 0x8, D_800D69B8, D_800D69BC, D_800D6988, D_800D6990);
    func_800CB028(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB0F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB168);

void func_800CB1E0(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

void func_800CB21C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C2E84(a, (s32)c, D_800D6938);
        a->unk_54 = func_800CB1E0;
        a->unk_6C = 0x6;
    }
}

void func_800CB27C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6910);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB21C;
    a->unk_6C = 0x7;
}

void func_800CB2DC(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800CB04C();
        func_800C4174(a, b, c);
    }
}

void func_800CB348(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6910);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB2DC;
    a->unk_6C = 0x6;
}

void func_800CB3A8(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800CB028();
        func_800C4174(a, b, c);
    }
}

void func_800CB414(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6938);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB3A8;
    a->unk_6C = 0x7;
}

void func_800CB474(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

void func_800CB4B0(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C2E84(a, (s32)c, D_800D6938);
        a->unk_54 = func_800CB474;
        a->unk_6C = 0xA;
    }
}

void func_800CB510(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6910);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB4B0;
    a->unk_6C = 0x7;
}

void func_800CB570(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800CB060();
        func_800C4174(a, b, c);
    }
}

void func_800CB5DC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D694C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB570;
    a->unk_6C = 0xA;
}

void func_800CB63C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800CB028();
        func_800C4174(a, b, c);
    }
}

void func_800CB6A8(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6974);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB63C;
    a->unk_6C = 0x7;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB708);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB73C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB770);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB7D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB7EC);

void func_800CB86C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D68FC);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB7EC;
    a->unk_6C = 0x6;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB8CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB9B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBAAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBAE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBB68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBC1C);

void func_800CBC80(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x28, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x2;
    c->unk_14 &= 0xFEFF;
}

void func_800CBCE8(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x46, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x4;
    c->unk_14 &= 0xFEFF;
}

void func_800CBD50(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x47, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x4;
    c->unk_14 &= 0xFEFF;
}

void func_800CBDB8(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x48, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x4;
    c->unk_14 &= 0xFEFF;
}

void func_800CBE20(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x49, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x4;
    c->unk_14 &= 0xFEFF;
}

void func_800CBE88(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6A98);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x1C;
}

void func_800CBEE8(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6AA0);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x1C;
}

void func_800CBF48(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6A70);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_68 = 0;
    a->unk_6A = 0;
    a->unk_54 = func_800CBFAC;
    a->unk_6C = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBFAC);

void func_800CBFE8(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D6AA8);
    a->unk_68++;
}

void func_800CC02C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D6A70);
    a->unk_68++;
}

void func_800CC070(UnkTownActor *a, UnkTown2AE8 *b, s32 c, s16 d)
{
    switch (a->unk_6A)
    {
    case 0:
        func_800C2E84(a, c, D_800D6ABC);
        *(s16 *)((u8 *)b + 0x16) = d;
        a->unk_6A++;
        break;
    case 1:
        b->unk_08 += b->unk_14;
        func_80095388(b);
        if (b->unk_14 < 0)
        {
            break;
        }
        func_800C2E84(a, c, D_800D6AD0);
        a->unk_6A++;
        break;
    case 2:
        b->unk_08 += b->unk_14;
        if ((s16)func_800C2AE8(b) < *(s16 *)((u8 *)b + 0xA))
        {
            b->unk_14 = 0;
            *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
            a->unk_6A = 0;
            a->unk_68++;
        }
        else
        {
            func_80095388(b);
        }
        break;
    }
}

void func_800CC1B0(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800CC070(a, b, c, -0xA);
}

void func_800CC1D0(void *arg0)
{
    func_800C3050(arg0, 0x21, D_800D6C7C, D_800D6C80, D_800D6C4C, D_800D6C54);
}

void func_800CC218(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x15, D_800D6C7C, D_800D6C80, D_800D6C4C, D_800D6C54);
    c->unk_12 = 0x240;
    c->unk_14 &= 0xFEFF;
}

void func_800CC280(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2C, D_800D6C7C, D_800D6C80, D_800D6C4C, D_800D6C54);
    c->unk_12 = 0x2C0;
    c->unk_14 &= 0xFEFF;
}

void func_800CC2E8(void *arg0)
{
    func_800C3050(arg0, 0x7, D_800D6D68, D_800D6D6C, D_800D6D38, D_800D6D40);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC330);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC384);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC41C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC4B0);

void func_800CC524(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *r)
{
    if (--a->unk_6C <= 0)
    {
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        func_800C4174(a, b, r);
    }
    else
    {
        r->unk_1C = 0x1000 - func_800644B8(a->unk_6C * 409) / 4;
        r->unk_1E = func_800644B8(a->unk_6C * 409) / 4 + 0x1000;
    }
}

void func_800CC5FC(void *arg0)
{
    func_800C3050(arg0, 0xB, D_800D6E28, D_800D6E2C, D_800D6DF8, D_800D6E00);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC644);

void func_800CC6FC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6DA8);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CC644;
    a->unk_6C = 0x78;
}

void func_800CC75C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6DBC);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C28;
}

void func_800CC7B4(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6DD0);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C28;
}

void func_800CC80C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6DE4);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C28;
}

void func_800CC864(void *arg0)
{
    func_800C3050(arg0, 0x1, D_800D6EC0, D_800D6EC4, D_800D6E90, D_800D6E98);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC8AC);

void func_800CC960(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6E7C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CC8AC;
    a->unk_6C = 0x12C;
}

void func_800CC9C0(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6E68);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3BF4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCA18);

void func_800CCA64(void *arg0)
{
    func_800C3050(arg0, 0xC, D_800D6FA4, D_800D6FA8, D_800D6F74, D_800D6F7C);
}

void func_800CCAAC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6EFC);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x1C;
}

void func_800CCB0C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6F10);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x1C;
}

void func_800CCB6C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6EE8);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_68 = 0;
    a->unk_6A = 0;
    a->unk_54 = func_800CCC34;
    a->unk_6C = 0;
}

void func_800CCBD0(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6EE8);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_68 = 0;
    a->unk_6A = 0;
    a->unk_54 = func_800CCC70;
    a->unk_6C = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCC34);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCC70);

void func_800CCCAC(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C4174(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCCCC);

void func_800CCCE0(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D6F24);
    a->unk_68++;
}

void func_800CCD24(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D6EE8);
    a->unk_68++;
}

void func_800CCD68(UnkTownActor *a)
{
    switch (a->unk_6A)
    {
    case 0:
        a->unk_72 = 0xC00;
        a->unk_6C = 3;
        a->unk_6A++;
        break;
    case 1:
        if (--a->unk_6C == 0)
        {
            a->unk_72 = 0x800;
            a->unk_6C = 3;
            a->unk_6A++;
        }
        break;
    case 2:
        if (--a->unk_6C == 0)
        {
            a->unk_72 = 0x400;
            a->unk_6C = 3;
            a->unk_6A++;
        }
        break;
    case 3:
        if (--a->unk_6C == 0)
        {
            a->unk_72 = 0;
            a->unk_6C = 3;
            a->unk_6A = 0;
            a->unk_68++;
        }
        break;
    }
}

void func_800CCE40(UnkTownActor *a)
{
    switch (a->unk_6A)
    {
    case 0:
        a->unk_72 = 0x400;
        a->unk_6C = 3;
        a->unk_6A++;
        break;
    case 1:
        if (--a->unk_6C == 0)
        {
            a->unk_72 = 0x800;
            a->unk_6C = 3;
            a->unk_6A++;
        }
        break;
    case 2:
        if (--a->unk_6C == 0)
        {
            a->unk_72 = 0xC00;
            a->unk_6C = 3;
            a->unk_6A++;
        }
        break;
    case 3:
        if (--a->unk_6C == 0)
        {
            a->unk_72 = 0;
            a->unk_6C = 3;
            a->unk_6A = 0;
            a->unk_68++;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD014);

void func_800CD040(s32 a, s32 b, s32 c)
{
    func_800CCF74(a, b, c, 0x10, -0x4, 0x8);
}

void func_800CD070(s32 a, s32 b, s32 c)
{
    func_800CCF74(a, b, c, 0x10, 0x8, -0x4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD0A0);

void func_800CD0CC(s32 a, s32 b, s32 c)
{
    func_800CCF74(a, b, c, 0x10, 0x8, 0x4);
}

void func_800CD0FC(s32 a, s32 b, s32 c)
{
    func_800CCF74(a, b, c, 0x10, -0x4, -0x8);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD12C);

void func_800CD158(UnkTownActor *a, UnkTown2AE8 *b, s32 c, s16 d)
{
    switch (a->unk_6A)
    {
    case 0:
        func_800C2E84(a, c, D_800D6F4C);
        *(s16 *)((u8 *)b + 0x16) = d;
        a->unk_6A++;
        break;
    case 1:
        b->unk_08 += b->unk_14;
        func_80095388(b);
        if (b->unk_14 < 0)
        {
            break;
        }
        func_800C2E84(a, c, D_800D6F60);
        a->unk_6A++;
        break;
    case 2:
        b->unk_08 += b->unk_14;
        if ((s16)func_800C2AE8(b) < *(s16 *)((u8 *)b + 0xA))
        {
            b->unk_14 = 0;
            *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
            a->unk_6A = 0;
            a->unk_68++;
        }
        else
        {
            func_80095388(b);
        }
        break;
    }
}

void func_800CD298(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800CD158(a, b, c, -0xA);
}

void func_800CD2B8(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800CD158(a, b, c, -0xF);
}

void func_800CD2D8(void *arg0)
{
    func_800C3050(arg0, 0x32, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
}

void func_800CD320(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x40, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
    c->unk_12 = 0x280;
    c->unk_14 &= 0xFEFF;
}

void func_800CD388(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x41, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
    c->unk_12 = 0x2C0;
    c->unk_14 &= 0xFEFF;
}

void func_800CD3F0(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x42, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
    c->unk_12 = 0x300;
    c->unk_14 &= 0xFEFF;
}

void func_800CD458(void *arg0)
{
    func_800C3050(arg0, 0x22, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD4A0(void *arg0)
{
    func_800C3050(arg0, 0xE, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD4E8(void *arg0)
{
    func_800C3050(arg0, 0x14, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD530(void *arg0)
{
    func_800C3050(arg0, 0x3A, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD578(void *arg0)
{
    func_800C3050(arg0, 0x2B, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD5C0(void *arg0)
{
    func_800C3050(arg0, 0x50, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD608(void *arg0)
{
    func_800C3050(arg0, 0x5, D_800D732C, D_800D7338, D_800D72FC, D_800D7304);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD650);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD70C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD87C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD9F4);

void func_800CDA6C(void *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_800CDF54(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDADC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDBB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDC2C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDCA8);

void func_800CDD98(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D7284);
    a->unk_50 = func_800CD650;
    a->unk_6C = (u16)func_800374F4(4) * 21 + 0x15;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDDFC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDE4C);

void func_800CDE9C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D7284);
    a->unk_50 = func_800CD9F4;
    a->unk_6C = (u16)func_800374F4(4) * 21 + 0x15;
}

void func_800CDF00(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D72D4);
    a->unk_50 = func_800CDA6C;
    b->unk_14 = -0xA0000;
}

void func_800CDF54(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72E8);
    a->unk_50 = func_800CDADC;
}

void func_800CDF94(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D7284);
    a->unk_50 = func_800CDBB8;
    a->unk_6C = (u16)func_800374F4(4) * 21 + 0x15;
}

void func_800CDFF8(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D72D4);
    a->unk_50 = func_800CDC2C;
    b->unk_14 = -0xA0000;
}

void func_800CE04C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72E8);
    a->unk_50 = func_800CDCA8;
}

void func_800CE08C(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

void func_800CE0C8(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    s32 y;

    if (--a->unk_6C <= 0)
    {
        a->unk_90 = 1;
        func_800CE9EC(a, b, c);
    }
    *(s16 *)((u8 *)b + 2) += (*(s16 *)((u8 *)D_80083498.unk_08 + 2) - *(s16 *)((u8 *)b + 2)) / 2;
    y = *(s16 *)((u8 *)b + 6) - 0x10;
    *(s16 *)((u8 *)b + 6) += (*(s16 *)((u8 *)D_80083498.unk_08 + 6) - y) / 2;
    *(s16 *)((u8 *)b + 0xA) += (*(s16 *)((u8 *)D_80083498.unk_08 + 0xA) - *(s16 *)((u8 *)b + 0xA)) / 2;
}

void func_800CE19C(void *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_800CEA40(a, b, c);
    }
}

void func_800CE20C(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    if ((s16)func_800C2AE8(b) < *(s16 *)((u8 *)b + 0xA))
    {
        *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
        if (--a->unk_90 <= 0)
        {
            b->unk_14 = 0;
            func_800CEA80(a, b, c);
        }
        else
        {
            func_800CE9EC(a, b, c);
        }
    }
    else
    {
        func_80095388(b);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE2D4);

void func_800CE38C(void *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_800CEB54(a, b, c);
    }
}

void func_800CE3FC(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    if ((s16)func_800C2AE8(b) < *(s16 *)((u8 *)b + 0xA))
    {
        *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
        if (--a->unk_90 <= 0)
        {
            b->unk_14 = 0;
            func_800CEB94(a, b, c);
        }
        else
        {
            func_800CEB00(a, b, c);
        }
    }
    else
    {
        func_80095388(b);
    }
}

void func_800CE4C4(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    b->unk_00 += 0x40000;
    b->unk_04 -= 0x15555;
    if ((u16)c->unk_14 & 0x8000)
    {
        func_80033AA8(0x43);
        func_8008F134(a);
        func_800C30A4(a);
    }
}

void func_800CE538(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    func_800C4174(a, b, c);
}

void func_800CE558(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    s32 y;

    if (--a->unk_6C <= 0)
    {
        a->unk_90 = 1;
        func_800C4174(a, b, c);
    }
    *(s16 *)((u8 *)b + 2) += (*(s16 *)((u8 *)D_80083498.unk_08 + 2) - *(s16 *)((u8 *)b + 2)) / 2;
    y = *(s16 *)((u8 *)b + 6) - 0x10;
    *(s16 *)((u8 *)b + 6) += (*(s16 *)((u8 *)D_80083498.unk_08 + 6) - y) / 2;
    *(s16 *)((u8 *)b + 0xA) += (*(s16 *)((u8 *)D_80083498.unk_08 + 0xA) - *(s16 *)((u8 *)b + 0xA)) / 2;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE62C);

void func_800CE6E4(UnkTownActor *a, UnkTown2AE8 *b, UnkTownRend *c)
{
    b->unk_08 += b->unk_14;
    if ((s16)func_800C2AE8(b) < *(s16 *)((u8 *)b + 0xA))
    {
        *(s16 *)((u8 *)b + 0xA) = func_800C2AE8(b);
        b->unk_14 = 0;
        func_800C4174(a, b, c);
    }
    else
    {
        func_80095388(b);
    }
}

void func_800CE780(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_800C2E84(a, c, D_800D72E8);
        a->unk_54 = func_800CE6E4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE800);

void func_800CE8CC(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    b->unk_08 += b->unk_14;
    func_80095388(b);
    if (b->unk_14 >= 0)
    {
        func_800C2E84(a, c, D_800D72E8);
        a->unk_54 = func_800CE800;
    }
}

void func_800CE94C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72AC);
    D_80082660[5].unk_00 = 0;
    a->unk_54 = func_800CE08C;
    a->unk_6C = 0x15;
}

void func_800CE99C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72AC);
    D_80082660[5].unk_00 = 0;
    a->unk_54 = func_800CE0C8;
    a->unk_6C = 0x10;
}

void func_800CE9EC(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D72D4);
    a->unk_54 = func_800CE19C;
    b->unk_14 = -0xA0000;
}

void func_800CEA40(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72E8);
    a->unk_54 = func_800CE20C;
}

void func_800CEA80(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72AC);
    a->unk_54 = func_800CE2D4;
    a->unk_6C = 0x10;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEAC8);

void func_800CEB00(UnkTownActor *a, UnkTown2AE8 *b, s32 c)
{
    func_800C2E84(a, c, D_800D72D4);
    a->unk_54 = func_800CE38C;
    b->unk_14 = -0xA0000;
}

void func_800CEB54(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72E8);
    a->unk_54 = func_800CE3FC;
}

void func_800CEB94(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72AC);
    a->unk_54 = func_800CE4C4;
    a->unk_72 = 0x400;
}

void func_800CEBDC(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72C0);
    D_80082660[5].unk_00 = 0;
    a->unk_54 = func_800CE538;
    a->unk_72 = 0;
}

void func_800CEC28(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72AC);
    D_80082660[5].unk_00 = 0;
    a->unk_54 = func_800CE558;
    a->unk_6C = 0x10;
}

void func_800CEC78(UnkTownActor *a, s32 b, s32 c)
{
    func_800C2E84(a, c, D_800D72AC);
    D_80082660[5].unk_00 = 0;
    a->unk_54 = func_800CE62C;
    a->unk_6C = 0x10;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CECC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CED24);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEDA4);

void func_800CEDB8(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3C, D_800D73CC, D_800D73D0, D_800D739C, D_800D73A4);
    c->unk_12 = 0x40;
}

void func_800CEE14(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3B, D_800D7440, D_800D7444, D_800D7410, D_800D7418);
    c->unk_12 = 0x40;
}

void func_800CEE70(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x53, D_800D74B4, D_800D74B8, D_800D7484, D_800D748C);
    c->unk_12 = 0x40;
}

void func_800CEECC(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x54, D_800D74B4, D_800D74B8, D_800D7484, D_800D748C);
    c->unk_12 = 0x40;
}

void func_800CEF28(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3A, D_800D7528, D_800D752C, D_800D74F8, D_800D7500);
    c->unk_12 = 0x40;
}

void func_800CEF84(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2D, D_800D759C, D_800D75A0, D_800D756C, D_800D7574);
    c->unk_12 = 0x40;
}

void func_800CEFE0(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3E, D_800D7610, D_800D7614, D_800D75E0, D_800D75E8);
    c->unk_12 = 0x40;
}

void func_800CF03C(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x39, D_800D7610, D_800D7614, D_800D75E0, D_800D75E8);
    c->unk_12 = 0x40;
}

void func_800CF098(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x30, D_800D7684, D_800D7688, D_800D7654, D_800D765C);
    c->unk_12 = 0x40;
}

void func_800CF0F4(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2F, D_800D76F8, D_800D76FC, D_800D76C8, D_800D76D0);
    c->unk_12 = 0x40;
}

void func_800CF150(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x34, D_800D776C, D_800D7770, D_800D773C, D_800D7744);
    c->unk_12 = 0x40;
}

void func_800CF1AC(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D77B0);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3BB8;
    a->unk_6C = 0xD;
}

void func_800CF20C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D77B8);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x4;
}

void func_800CF26C(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x10, D_800D77F0, D_800D77F4, D_800D77C0, D_800D77C8);
    c->unk_12 = 0x40;
}

void func_800CF2C8(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D783C);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3BB8;
    a->unk_6C = 0xD;
}

void func_800CF328(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D7844);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3B7C;
    a->unk_6C = 0x4;
}

void func_800CF388(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x29, D_800D787C, D_800D7880, D_800D784C, D_800D7854);
    c->unk_12 = 0x40;
}

void func_800CF3E4(void *a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2A, D_800D787C, D_800D7880, D_800D784C, D_800D7854);
    c->unk_12 = 0x40;
}

void func_800CF440(void *arg0)
{
    func_800C3050(arg0, 0xA, D_800D78F8, D_800D78FC, D_800D78C8, D_800D78D0);
}

void func_800CF488(void *arg0)
{
    func_800C3050(arg0, 0xC, D_800D796C, D_800D7970, D_800D793C, D_800D7944);
}

void func_800CF4D0(void *arg0)
{
    func_800C3050(arg0, 0xB, D_800D79E0, D_800D79E4, D_800D79B0, D_800D79B8);
}

void func_800CF518(void *arg0)
{
    func_800C3050(arg0, 0x8, D_800D7A54, D_800D7A58, D_800D7A24, D_800D7A2C);
}

void func_800CF560(void *arg0)
{
    func_800C3050(arg0, 0x9, D_800D7AC8, D_800D7ACC, D_800D7A98, D_800D7AA0);
}

void func_800CF5A8(void *arg0)
{
    func_800C3050(arg0, 0xD, D_800D7B3C, D_800D7B40, D_800D7B0C, D_800D7B14);
}

void func_800CF5F0(void *arg0)
{
    func_800C3050(arg0, 0x7, D_800D7BB0, D_800D7BB4, D_800D7B80, D_800D7B88);
}

void func_800CF638(void *arg0)
{
    func_800C3050(arg0, 0x6, D_800D7C24, D_800D7C28, D_800D7BF4, D_800D7BFC);
}

void func_800CF680(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D7C68);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800C3C28;
}

void func_800CF6D8(void *arg0)
{
    func_800C3050(arg0, 0x25, D_800D7CAC, D_800D7CB0, D_800D7C7C, D_800D7C84);
}

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008998C);
