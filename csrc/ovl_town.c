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
    u8 pad_00[0x8];
    void *unk_08;
    void *unk_0C;
    void *unk_10;
    void *unk_14;
    void *unk_18;
    void *unk_1C;
} UnkTownArg80;

/* Passed as arg0 all over this overlay. Only the fields with a witness are
   named: a handler at +0x54, an index at +0x60 used as a stride-8 subscript
   into D_80082660, two s16s at +0x6C and +0x72 (+0x6C is a COUNTDOWN -- the
   handler installed at +0x54 decrements it and fires when it reaches zero),
   and the pointer table at +0x80. Everything else is unexplored space, not
   known filler. */
typedef struct UnkTownActor
{
    u8 pad_00[0x54];
    void (*unk_54)();
    u8 pad_58[0x8];
    s32 unk_60;
    u8 pad_64[0x8];
    s16 unk_6C;
    u8 pad_6E[0x4];
    s16 unk_72;
    u8 pad_74[0xC];
    UnkTownArg80 *unk_80;
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
    u8 pad_12[0x2E];
    s32 unk_40;
    UnkTownArg80 *unk_44;
} UnkTownTask;

/* The argument of the indirect-dispatch family, whose members load a function
   pointer from one offset and call it. The five offsets seen are not all on
   the same object: +0x00/+0x04 match UnkTownTask's handlers and +0x54 matches
   UnkTownActor's, while +0x50 and +0x68 have no other witness. One struct
   with all five slots is a CONVENIENCE -- it gets the bytes right and says
   nothing about which object each member is really handed. */
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

void func_800C3050(s32 arg0, s32 id, void *a2, void *a3, void *a4,
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
void func_800C4174(UnkTownActor *a, s32 b, s32 c);
void func_80094984(void *d, UnkTownTask *t);
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
void func_800C30E0();
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BA44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BACC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BB50);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008BBD4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C420);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C4C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C570);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C610);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C6B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C758);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008C984);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CA20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CABC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CB58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CBF4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CC90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CE08);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CF48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CF80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CFB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008CFE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008D01C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008D050);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F074);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F0A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F0D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F104);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F134);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F170);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F27C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F294);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F2D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F350);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F3CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F41C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F46C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F4BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F50C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F520);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F534);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F548);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F55C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F5B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008F60C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8008FFC8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090E78);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090F00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80090F84);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091000);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091114);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800911D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091260);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091528);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800917EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091990);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091A08);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091AF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091BD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091CE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091D6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091EB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80091F64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009204C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800920F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009221C);

void func_80092318(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092320);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800924EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092698);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800927EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092A84);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092CD4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092DA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80092ECC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093F48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80093FC8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800940D0);

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

void func_80094378(UnkTownTask *t)
{
    func_80094984(D_800D00B8, t);
    t->unk_00 = func_8009221C;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800943B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094414);

void func_80094474(UnkTownTask *t)
{
    func_80094984(D_800D0118, t);
    t->unk_00 = func_80090E78;
    t->unk_0A = 0x6;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800944BC);

void func_8009451C(UnkTownTask *t)
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

void func_80094660(UnkTownTask *t)
{
    func_80094984(D_800D00B8, t);
    t->unk_00 = func_80092ECC;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800946A0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800946FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094714);

void func_80094774(UnkTownTask *t)
{
    func_80094984(D_800D0120, t);
    t->unk_00 = func_80093328;
    t->unk_0A = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800947BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800947CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80094834);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095BC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095BF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095C20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095C50);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80095C80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800961A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800961D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096208);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800962B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096368);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096440);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096518);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800965F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800966C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800966F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096728);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096758);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80096788);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800967E0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097EFC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80097F9C);

void func_8009803C(UnkTownTask *t)
{
    if (--t->unk_0A <= 0)
    {
        func_80098928();
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800984AC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098CC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098CD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098D60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098DC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098E20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098E70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098EC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098F10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80098F48);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800993F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099468);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800994DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009953C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800995A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099614);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099680);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800996E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099754);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099764);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099790);

void func_80099874(void)
{
    func_80098928();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099894);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099920);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800999AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099A38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099AC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_80099B00);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A1E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A344);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A3A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A41C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A494);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A4FC);

void func_8009A56C(void)
{
    func_80098928();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A58C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A5C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A5F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A654);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A674);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A724);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A81C);

void func_8009A8A8(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A8B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A8EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009A99C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AA2C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AAB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AB74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AC0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009AC8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009ACFC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C5D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C648);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C7A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C83C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C884);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C8FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C974);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009C9DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CA24);

void func_8009CA8C(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CA94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CAFC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CB64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CB90);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CBB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CBD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CC18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CC58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CC98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CCD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CD18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CDB4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CDCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CE34);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CE80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CEA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CEE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009CFE0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009DFD8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009EBE0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009F830);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_8009FF28);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A0A34);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A38A8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6684);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A68C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6994);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6A94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6B10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6B70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A6BEC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7554);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7B14);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7B98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7C0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7C98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7D48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7DA0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A7DF8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9778);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A97D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A97F8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9C6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9C8C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9CAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9D00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9D74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9E68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800A9F14);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA044);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA068);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA10C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA5F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA844);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AA998);

void func_800AAA58(UnkTownTask *t)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB4A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB50C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AB55C);

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

void func_800AEB9C(s32 a)
{
    func_800491F4(a, D_800D1594, 0x1A);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEBC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AECA4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEE98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEEBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEEF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEF5C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AEFD8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF0E0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF148);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF1B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF234);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF254);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF2F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF36C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF448);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF4AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF520);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF600);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF674);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF734);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF784);

void func_800AF858(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF860);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF8B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF96C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AF9C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFD00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFD5C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFDD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFE10);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFF00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800AFFA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0024);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0134);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0158);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0198);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0214);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B02D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B0318);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B03B4);

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

void func_800B091C(s32 a)
{
    func_800491F4(a, D_800D15CC, 0x1F);
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B1434);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B18F8);

void func_800B1938(s32 a)
{
    func_800491F4(a, D_800D1624, 0x4);
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2DCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2E3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2F18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2F38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2F88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B2FF8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B30D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3108);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3178);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B32D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B331C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B336C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3440);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3488);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B34A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B34FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3558);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3598);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3688);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3734);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B384C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3868);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B38A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3924);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B39E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3A80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3AE4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3B18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3C04);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3C74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3D04);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3D84);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3DC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3E30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3E68);

void func_800B3EC4(s32 a)
{
    func_800491F4(a, D_800D16BC, 0x21);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3EEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B3FB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B40E0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4188);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B426C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4350);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B43E8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4494);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B44B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B44F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4588);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4648);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B46CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4720);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B474C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B47BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4898);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B48B8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4908);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4978);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B49A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4A18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4A60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4AB0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4B7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4BC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4BE4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4C38);

void func_800B4C94(s32 a)
{
    func_800491F4(a, D_800D1760, 0x29);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4CBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4D64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4E7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4E98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4ED8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B4F60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5020);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B50BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B512C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5174);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B5264);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089268);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008926C);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_80089298);

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_800892C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B52F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B53BC);

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

void func_800B6464(s32 a)
{
    func_800491F4(a, D_800D17AC, 0x18);
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

void func_800B677C(s32 a)
{
    func_800491F4(a, D_800D17C4, 0x18);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B67A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B683C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B68AC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B68C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B691C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B6990);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B90BC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B90F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9204);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9260);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9384);

void func_800B93B8(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B93C0);

void func_800B93F4(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B93FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9430);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B946C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B94A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B94FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B97D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9830);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9890);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B98B4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9998);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9B28);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9BF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9CB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9D64);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9EDC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800B9F74);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB688);

void func_800BB6C4(void)
{
    func_800BB508();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB6E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB720);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800BB75C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C122C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C1A70);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C22D8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C230C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2340);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2374);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C23A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C23DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2410);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2444);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2478);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C24A0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C24C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C24FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C258C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C25C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2664);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C26C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2728);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2780);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2D34);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2D94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C2DBC);

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

void func_800C34CC(void)
{
    func_800C30E0();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C34EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C355C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3780);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C37C4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C382C);

void func_800C3960(UnkTownDispatch *d)
{
    d->unk_54();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3988);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3A98);

void func_800C3B7C(UnkTownActor *a, s32 b, s32 c)
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

void func_800C3C5C(UnkTownActor *a, s32 b, s32 c)
{
    func_800C4174(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3C7C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3D08);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3D94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3E20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3EAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3F44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3FC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C3FFC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C40D0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4134);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4174);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C46A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4804);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4864);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C48DC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4958);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C49C0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4A30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4A88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4AEC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4C88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4D20);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4D80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C4E30);

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

void func_800C5368(s32 arg0)
{
    func_800C3050(arg0, 0x30, D_800D51AC, D_800D51B0, D_800D517C, D_800D5184);
}

void func_800C53B0(s32 arg0)
{
    func_800C3050(arg0, 0x1, D_800D51F8, D_800D51FC, D_800D51C8, D_800D51D0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C53F8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C54D0);

void func_800C5500(s32 arg0)
{
    func_800C3050(arg0, 0x33, D_800D5270, D_800D5274, D_800D5240, D_800D5248);
}

void func_800C5548(s32 arg0)
{
    func_800C3050(arg0, 0x51, D_800D52E4, D_800D52E8, D_800D52B4, D_800D52BC);
}

void func_800C5590(s32 arg0)
{
    func_800C3050(arg0, 0x10, D_800D5358, D_800D535C, D_800D5328, D_800D5330);
}

void func_800C55D8(s32 arg0)
{
    func_800C3050(arg0, 0x16, D_800D53F4, D_800D53F8, D_800D53C4, D_800D53CC);
}

void func_800C5620(UnkTownActor *a, s32 b, s32 c)
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

void func_800C58AC(s32 arg0)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5A54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5ADC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5B48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5BA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5C70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5D38);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5D94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5E00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5E5C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5EB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5F14);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5F70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C5FDC);

void func_800C6094(s32 arg0)
{
    func_800C3050(arg0, 0xD, D_800D55D0, D_800D55D4, D_800D55A0, D_800D55A8);
}

void func_800C60DC(s32 arg0)
{
    func_800C3050(arg0, 0xD, D_800D5618, D_800D561C, D_800D55E8, D_800D55F0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6124);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6180);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C620C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6270);

void func_800C629C(s32 arg0)
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

void func_800C6344(s32 arg0)
{
    func_800C3050(arg0, 0x23, D_800D57A4, D_800D57A8, D_800D5774, D_800D577C);
}

void func_800C638C(s32 arg0)
{
    func_800C3050(arg0, 0x3F, D_800D57A4, D_800D57A8, D_800D5774, D_800D577C);
}

void func_800C63D4(s32 arg0)
{
    func_800C3050(arg0, 0x20, D_800D5818, D_800D581C, D_800D57E8, D_800D57F0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C641C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6440);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C645C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6478);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C648C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C64C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6544);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C65C8);

void func_800C6644(UnkTownActor *a, s32 b, s32 c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C6680);

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

void func_800C6814(UnkTownActor *a, s32 b, s32 c)
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

void func_800C68E0(UnkTownActor *a, s32 b, s32 c)
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

void func_800C69AC(UnkTownActor *a, s32 b, s32 c)
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

void func_800C6A78(UnkTownActor *a, s32 b, s32 c)
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

void func_800C6B44(UnkTownActor *a, s32 b, s32 c)
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

void func_800C6DC8(s32 arg0)
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

void func_800C6E70(s32 arg0)
{
    func_800C3050(arg0, 0x2F, D_800D5AF8, D_800D5AFC, D_800D5AC8, D_800D5AD0);
}

void func_800C6EB8(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x17, D_800D5AF8, D_800D5AFC, D_800D5AC8, D_800D5AD0);
    c->unk_12 = 0x280;
    c->unk_14 &= 0xFEFF;
}

void func_800C6F20(s32 arg0)
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

void func_800C7070(s32 arg0)
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

void func_800C7284(s32 arg0)
{
    func_800C3050(arg0, 0x35, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
}

void func_800C72CC(s32 arg0)
{
    func_800C3050(arg0, 0x37, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
}

void func_800C7314(s32 arg0)
{
    func_800C3050(arg0, 0x31, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
}

void func_800C735C(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x1C, D_800D5E04, D_800D5E08, D_800D5DD4, D_800D5DDC);
    c->unk_12 = 0x240;
    c->unk_14 &= 0xFEFF;
}

void func_800C73C4(s32 arg0)
{
    func_800C3050(arg0, 0x34, D_800D5E3C, D_800D5E40, D_800D5E0C, D_800D5E14);
}

void func_800C740C(s32 arg0)
{
    func_800C3050(arg0, 0x36, D_800D5E3C, D_800D5E40, D_800D5E0C, D_800D5E14);
}

void func_800C7454(s32 arg0)
{
    func_800C3050(arg0, 0x38, D_800D5E3C, D_800D5E40, D_800D5E0C, D_800D5E14);
}

void func_800C749C(s32 arg0)
{
    func_800C3050(arg0, 0x3C, D_800D5EB0, D_800D5EB4, D_800D5E80, D_800D5E88);
}

void func_800C74E4(s32 arg0)
{
    func_800C3050(arg0, 0x29, D_800D5F24, D_800D5F28, D_800D5EF4, D_800D5EFC);
}

void func_800C752C(s32 arg0)
{
    func_800C3050(arg0, 0x2A, D_800D5F24, D_800D5F28, D_800D5EF4, D_800D5EFC);
}

void func_800C7574(s32 arg0)
{
    func_800C3050(arg0, 0x1B, D_800D5F98, D_800D5F9C, D_800D5F68, D_800D5F70);
}

void func_800C75BC(s32 arg0)
{
    func_800C3050(arg0, 0x4, D_800D603C, D_800D6048, D_800D600C, D_800D6014);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7604);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7674);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C78A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7A00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7A6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7AC4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7BF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7C88);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7CAC);

void func_800C7CB4(UnkTownActor *a, s32 b, s32 c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7DB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7E70);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C7EB0);

void func_800C7EF0(UnkTownActor *a, s32 b, s32 c)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8000);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8058);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C80B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C80FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8120);

void func_800C814C(s32 arg0)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8330);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C838C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C83F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8454);

void func_800C84B8(UnkTownActor *a, s32 b, s32 c)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8600);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8644);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8688);

void func_800C86CC(void)
{
    func_800C8644();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C86EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8788);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8808);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C885C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C88AC);

void func_800C88E0(s32 arg0)
{
    func_800C3050(arg0, 0xA, D_800D6360, D_800D6364, D_800D6330, D_800D6338);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8928);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C895C);

void func_800C899C(UnkTownActor *a, s32 b, s32 c)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8D34);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8DC0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8E9C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8F80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C8FF0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C907C);

void func_800C90D4(s32 arg0)
{
    func_800C3050(arg0, 0x3B, D_800D63EC, D_800D63F0, D_800D63BC, D_800D63C4);
}

void func_800C911C(s32 arg0)
{
    func_800C3050(arg0, 0x1F, D_800D63EC, D_800D63F0, D_800D63BC, D_800D63C4);
}

void func_800C9164(s32 arg0)
{
    func_800C3050(arg0, 0x52, D_800D63EC, D_800D63F0, D_800D63BC, D_800D63C4);
}

void func_800C91AC(s32 arg0)
{
    func_800C3050(arg0, 0x24, D_800D6460, D_800D6464, D_800D6430, D_800D6438);
}

void func_800C91F4(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x1A, D_800D6460, D_800D6464, D_800D6430, D_800D6438);
    c->unk_12 = 0x2C0;
    c->unk_14 &= 0xFEFF;
}

void func_800C925C(s32 arg0)
{
    func_800C3050(arg0, 0x44, D_800D64C0, D_800D64C4, D_800D6490, D_800D6498);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C92A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9314);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9384);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C95A8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9610);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9678);

void func_800C97AC(s32 arg0)
{
    func_800C3050(arg0, 0xB, D_800D6598, D_800D65A0, D_800D6568, D_800D6570);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C97F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9864);

void func_800C9A98(UnkTownActor *a)
{
    if (--a->unk_6C <= 0)
    {
        func_800C9CDC();
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9AD4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9B44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9C0C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9C54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9C94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9CDC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9D40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9D80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9DB8);

void func_800C9F1C(UnkTownActor *a, s32 b, s32 c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9F58);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800C9FBC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA020);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA094);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA0D4);

void func_800CA114(void)
{
    func_800CA744();
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA134);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA214);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA308);

void func_800CA3EC(UnkTownActor *a, s32 b, s32 c)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA60C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA64C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA6A4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA6FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA744);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA75C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA774);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA78C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA8D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA930);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA98C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CA9E8);

void func_800CAA44(s32 arg0)
{
    func_800C3050(arg0, 0x2D, D_800D6718, D_800D671C, D_800D66E8, D_800D66F0);
}

void func_800CAA8C(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x4A, D_800D6718, D_800D671C, D_800D66E8, D_800D66F0);
    c->unk_12 = 0x1;
}

void func_800CAAE8(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3D, D_800D6718, D_800D671C, D_800D66E8, D_800D66F0);
    c->unk_12 = 0x2;
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CAB44);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CABB4);

void func_800CAC28(s32 arg0)
{
    func_800C3050(arg0, 0x12, D_800D67A0, D_800D67A4, D_800D6770, D_800D6778);
}

void func_800CAC70(s32 arg0)
{
    func_800C3050(arg0, 0x12, D_800D67DC, D_800D67E0, D_800D67AC, D_800D67B4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CACB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CAD40);

void func_800CADC8(s32 arg0)
{
    func_800C3050(arg0, 0x2E, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAE10(s32 arg0)
{
    func_800C3050(arg0, 0x26, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAE58(s32 arg0)
{
    func_800C3050(arg0, 0x1E, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAEA0(s32 arg0)
{
    func_800C3050(arg0, 0x39, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAEE8(s32 arg0)
{
    func_800C3050(arg0, 0x3E, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAF30(s32 arg0)
{
    func_800C3050(arg0, 0x18, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

void func_800CAF78(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x19, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
    c->unk_12 = 0x240;
    c->unk_14 &= 0xFEFF;
}

void func_800CAFE0(s32 arg0)
{
    func_800C3050(arg0, 0x45, D_800D6854, D_800D6858, D_800D6824, D_800D682C);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB028);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB04C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB060);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB074);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB0F0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB168);

void func_800CB1E0(UnkTownActor *a, s32 b, s32 c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB21C);

void func_800CB27C(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6910);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB21C;
    a->unk_6C = 0x7;
}

void func_800CB2DC(UnkTownActor *a, s32 b, s32 c)
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

void func_800CB3A8(UnkTownActor *a, s32 b, s32 c)
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

void func_800CB474(UnkTownActor *a, s32 b, s32 c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CB4B0);

void func_800CB510(UnkTownActor *a, s32 b, s32 c)
{
    Unk80082660 *t;

    func_800C2E84(a, c, D_800D6910);
    t = D_80082660;
    t[a->unk_60].unk_00 = 0;
    a->unk_54 = func_800CB4B0;
    a->unk_6C = 0x7;
}

void func_800CB570(UnkTownActor *a, s32 b, s32 c)
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

void func_800CB63C(UnkTownActor *a, s32 b, s32 c)
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

void func_800CBC80(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x28, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x2;
    c->unk_14 &= 0xFEFF;
}

void func_800CBCE8(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x46, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x4;
    c->unk_14 &= 0xFEFF;
}

void func_800CBD50(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x47, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x4;
    c->unk_14 &= 0xFEFF;
}

void func_800CBDB8(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x48, D_800D6B14, D_800D6B18, D_800D6AE4, D_800D6AEC);
    c->unk_12 = 0x4;
    c->unk_14 &= 0xFEFF;
}

void func_800CBE20(s32 a, s32 b, UnkTownArg2 *c)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBF48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBFAC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CBFE8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC02C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC070);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC1B0);

void func_800CC1D0(s32 arg0)
{
    func_800C3050(arg0, 0x21, D_800D6C7C, D_800D6C80, D_800D6C4C, D_800D6C54);
}

void func_800CC218(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x15, D_800D6C7C, D_800D6C80, D_800D6C4C, D_800D6C54);
    c->unk_12 = 0x240;
    c->unk_14 &= 0xFEFF;
}

void func_800CC280(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2C, D_800D6C7C, D_800D6C80, D_800D6C4C, D_800D6C54);
    c->unk_12 = 0x2C0;
    c->unk_14 &= 0xFEFF;
}

void func_800CC2E8(s32 arg0)
{
    func_800C3050(arg0, 0x7, D_800D6D68, D_800D6D6C, D_800D6D38, D_800D6D40);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC330);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC384);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC41C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC4B0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CC524);

void func_800CC5FC(s32 arg0)
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

void func_800CC864(s32 arg0)
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

void func_800CCA64(s32 arg0)
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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCB6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCBD0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCC34);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCC70);

void func_800CCCAC(UnkTownActor *a, s32 b, s32 c)
{
    func_800C4174(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCCCC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCCE0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCD24);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCD68);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCE40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF18);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF30);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF48);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF60);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CCF74);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD014);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD040);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD070);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD0A0);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD0CC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD0FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD12C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD158);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD298);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD2B8);

void func_800CD2D8(s32 arg0)
{
    func_800C3050(arg0, 0x32, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
}

void func_800CD320(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x40, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
    c->unk_12 = 0x280;
    c->unk_14 &= 0xFEFF;
}

void func_800CD388(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x41, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
    c->unk_12 = 0x2C0;
    c->unk_14 &= 0xFEFF;
}

void func_800CD3F0(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x42, D_800D71F4, D_800D71F8, D_800D71C4, D_800D71CC);
    c->unk_12 = 0x300;
    c->unk_14 &= 0xFEFF;
}

void func_800CD458(s32 arg0)
{
    func_800C3050(arg0, 0x22, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD4A0(s32 arg0)
{
    func_800C3050(arg0, 0xE, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD4E8(s32 arg0)
{
    func_800C3050(arg0, 0x14, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD530(s32 arg0)
{
    func_800C3050(arg0, 0x3A, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD578(s32 arg0)
{
    func_800C3050(arg0, 0x2B, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD5C0(s32 arg0)
{
    func_800C3050(arg0, 0x50, D_800D7268, D_800D726C, D_800D7238, D_800D7240);
}

void func_800CD608(s32 arg0)
{
    func_800C3050(arg0, 0x5, D_800D732C, D_800D7338, D_800D72FC, D_800D7304);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD650);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD70C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD87C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CD9F4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDA6C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDADC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDBB8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDC2C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDCA8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDD98);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDDFC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDE4C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDE9C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDF00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDF54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDF94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CDFF8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE04C);

void func_800CE08C(UnkTownActor *a, s32 b, s32 c)
{
    if (--a->unk_6C <= 0)
    {
        func_800C4174(a, b, c);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE0C8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE19C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE20C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE2D4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE38C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE3FC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE4C4);

void func_800CE538(UnkTownActor *a, s32 b, s32 c)
{
    func_800C4174(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE558);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE62C);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE6E4);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE780);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE800);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE8CC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CE9EC);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEA40);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEA80);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEAC8);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEB00);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEB54);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEB94);

INCLUDE_ASM("asm/nonmatchings/ovl_town", func_800CEBDC);

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

void func_800CEDB8(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3C, D_800D73CC, D_800D73D0, D_800D739C, D_800D73A4);
    c->unk_12 = 0x40;
}

void func_800CEE14(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3B, D_800D7440, D_800D7444, D_800D7410, D_800D7418);
    c->unk_12 = 0x40;
}

void func_800CEE70(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x53, D_800D74B4, D_800D74B8, D_800D7484, D_800D748C);
    c->unk_12 = 0x40;
}

void func_800CEECC(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x54, D_800D74B4, D_800D74B8, D_800D7484, D_800D748C);
    c->unk_12 = 0x40;
}

void func_800CEF28(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3A, D_800D7528, D_800D752C, D_800D74F8, D_800D7500);
    c->unk_12 = 0x40;
}

void func_800CEF84(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2D, D_800D759C, D_800D75A0, D_800D756C, D_800D7574);
    c->unk_12 = 0x40;
}

void func_800CEFE0(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x3E, D_800D7610, D_800D7614, D_800D75E0, D_800D75E8);
    c->unk_12 = 0x40;
}

void func_800CF03C(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x39, D_800D7610, D_800D7614, D_800D75E0, D_800D75E8);
    c->unk_12 = 0x40;
}

void func_800CF098(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x30, D_800D7684, D_800D7688, D_800D7654, D_800D765C);
    c->unk_12 = 0x40;
}

void func_800CF0F4(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2F, D_800D76F8, D_800D76FC, D_800D76C8, D_800D76D0);
    c->unk_12 = 0x40;
}

void func_800CF150(s32 a, s32 b, UnkTownArg2 *c)
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

void func_800CF26C(s32 a, s32 b, UnkTownArg2 *c)
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

void func_800CF388(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x29, D_800D787C, D_800D7880, D_800D784C, D_800D7854);
    c->unk_12 = 0x40;
}

void func_800CF3E4(s32 a, s32 b, UnkTownArg2 *c)
{
    func_800C3050(a, 0x2A, D_800D787C, D_800D7880, D_800D784C, D_800D7854);
    c->unk_12 = 0x40;
}

void func_800CF440(s32 arg0)
{
    func_800C3050(arg0, 0xA, D_800D78F8, D_800D78FC, D_800D78C8, D_800D78D0);
}

void func_800CF488(s32 arg0)
{
    func_800C3050(arg0, 0xC, D_800D796C, D_800D7970, D_800D793C, D_800D7944);
}

void func_800CF4D0(s32 arg0)
{
    func_800C3050(arg0, 0xB, D_800D79E0, D_800D79E4, D_800D79B0, D_800D79B8);
}

void func_800CF518(s32 arg0)
{
    func_800C3050(arg0, 0x8, D_800D7A54, D_800D7A58, D_800D7A24, D_800D7A2C);
}

void func_800CF560(s32 arg0)
{
    func_800C3050(arg0, 0x9, D_800D7AC8, D_800D7ACC, D_800D7A98, D_800D7AA0);
}

void func_800CF5A8(s32 arg0)
{
    func_800C3050(arg0, 0xD, D_800D7B3C, D_800D7B40, D_800D7B0C, D_800D7B14);
}

void func_800CF5F0(s32 arg0)
{
    func_800C3050(arg0, 0x7, D_800D7BB0, D_800D7BB4, D_800D7B80, D_800D7B88);
}

void func_800CF638(s32 arg0)
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

void func_800CF6D8(s32 arg0)
{
    func_800C3050(arg0, 0x25, D_800D7CAC, D_800D7CB0, D_800D7C7C, D_800D7C84);
}

INCLUDE_RODATA("asm/nonmatchings/ovl_town", D_8008998C);
