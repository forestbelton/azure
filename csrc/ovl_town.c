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
void func_800B62A4();
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
void func_800B3440();
void func_800B47BC();
void func_800B4B7C();
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C758);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FAC0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D20C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D424);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D5C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D73C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D7D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D808);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D940);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009D9E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009DA50);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009E504);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F4C0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A1080);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A8CA8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB1E0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD8CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD914);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD968);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AD99C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADA1C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADB04);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADB30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADB5C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADD80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADFBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800ADFE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE090);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE0F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE148);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE1AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE2A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AE30C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0700);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1188);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1718);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1DCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1F10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1F48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1F80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2068);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B260C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2CE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2D00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2DA0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2F38);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B30D8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B32D4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3440);

void func_800B3488(u8 *a)
{
    func_800B3440(a + 0x20);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B34A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B34FC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4720);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B48B8);

void func_800B4908(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + -0x30;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x16;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4978);

void func_800B49A8(UnkTownGrid **p)
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        p[i]->unk_04->unk_08 = i / 5 * 0x80 + 0x60;
        p[i]->unk_04->unk_0A = i % 5 * 0x10 + 0x88;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4A18);

void func_800B4A60(UnkTownGrid **p)
{
    p[0x26]->unk_04->unk_08 = -0x40;
    p[0x26]->unk_04->unk_0A = 0x30;
    p[0x27]->unk_04->unk_08 = 0xC4;
    p[0x27]->unk_04->unk_0A = 0x30;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4AB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4B7C);

void func_800B4BC4(u8 *a)
{
    func_800B4B7C(a + 0x20);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4BE4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4C38);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B512C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5174);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B52F4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B544C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B555C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892E8);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892EC);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892F0);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B557C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5774);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B58B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5918);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B59A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5AC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5B20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5C60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5D30);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089304);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089308);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089310);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089318);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008931C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089324);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5D70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5E10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5E28);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6030);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6094);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B61C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B62A4);

void func_800B63EC(void)
{
    func_800B62A4();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B640C);

u32 func_800B6464(s32 a)
{
    return func_800491F4(a, D_800D17AC, 0x18);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B648C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6508);

void func_800B6578(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6580);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B65D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B66E8);

void func_800B6754(UnkTownDispatch *d)
{
    d->unk_00();
}

u32 func_800B677C(s32 a)
{
    return func_800491F4(a, D_800D17C4, 0x18);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B67A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B683C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B68AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B68C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B691C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6B10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6BC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6C30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6C94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6CCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6CEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6D54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6D94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6E3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6F54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6FB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B70EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B72B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7314);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7384);

void func_800B73D0(void)
{
    func_800B8248();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B73F0);

void func_800B7420(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7428);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B783C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7868);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7934);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7978);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7A6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7B48);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089344);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7B8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7E78);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B7F8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8170);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B81AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8248);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B833C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B83DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B85E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8834);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B89C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8A40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B8A9C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BD688);

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
