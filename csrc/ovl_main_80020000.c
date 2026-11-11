#include "common.h"
#include "main.h"

/* THE 0x80020000 OVERLAY'S .bss, WHICH CANNOT BE .bss. Nothing else is here.
   The nine translation units the overlay was built from are in csrc/main/,
   and each one now carries its own data; this file holds the one run that no
   unit can.

   WHAT THIS RUN IS. 0x80028530..0x8002BB6C is each unit's zero-initialised
   state, concatenated in link order -- c_server.c's counters, event
   descriptors, card_dir and card_st first, then one node blob per window
   unit. In the original source these were plain uninitialised globals, so
   this is the overlay's .bss, and docs/overlays.md gives the per-unit block
   for all nine. It is NOT carved along those blocks, for one reason:

       A UNIT CAN ONLY OWN THE FIRST DATA RUN. Every explicit `.data`
       subsegment sits at a rom address above all of the segment's text, so
       the nine auto-emitted `main/*.o(.data)` lines always come FIRST in the
       linker script's DATA group, ahead of any data file's. The image has two
       runs per unit -- tables at 0x80027E20, then state here -- and one
       object contributes one .data run, so the units take the tables and the
       second run has to live in one object after them. That is this file.

   The alternative is a `*_state.c` per unit, which does build, but it invents
   a second translation unit per real one and the original had nine.

   AND IT CANNOT BE REAL .bss, which would let the units keep both runs. Two
   reasons, in docs/owning-data.md SS6: the run holds two high-entropy words
   that are not zero (0xF97F8BEF at 0x8002A98C, 0x6F7C7327 at 0x8002B964, the
   alignment gaps between unit blocks), and a NOBITS section would write zeros
   over both. splat's section-group ORDER is not one of the reasons -- that is
   a yaml option -- and neither is the linker script, which takes as many data
   objects as the yaml lists, in the order it lists them.

   SO: EVERY OBJECT HERE NEEDS AN EXPLICIT INITIALISER, even the all-zero
   ones. Without one cc1psx emits `.comm`, the object lands in the unit's real
   `.bss`, and splat's generated `.bss` input-section line sits between the
   segment's .rodata and its .text -- so a forgotten `= {0}` does not shorten
   the data, it moves every function in the overlay.

   DEFINITION ORDER IS ADDRESS ORDER. cc1psx lays .data objects out in the
   order they are written here. The 0x494 bytes above 0x8002BB6C are the zero
   pad to the 0x269000 sector boundary and stay in asm, as
   `ovl_main_80020000_pad`. */
/* ------------------------------------------------------------------------- */
/* 0x80028530 */
/* Four two-word counters, one word per card slot and indexed by
   is_second_card_slot(): func_8002168C counts repeats of the card state in
   D_80028530 and remembers the state itself in D_80028538, and func_800217D4
   does the same for the button value in D_80028540 / D_80028548. Two
   elements, not two scalars -- measured 2026-11-09, `D_80028530[i]` and the
   old `(&D_80028530)[i]` spelling build byte-identical. */
s32 D_80028530[2] = {0, 0};

/* 0x80028538 */
s32 D_80028538[2] = {0, 0};

/* 0x80028540 */
s32 D_80028540[2] = {0, 0};

/* 0x80028548 */
s32 D_80028548[2] = {0, 0};

/* 0x80028550 */
/* The eight card event descriptors OpenEvent fills in. */
long gSwCardEOFEvent = 0;

/* 0x80028554 */
long gSwCardErrorEvent = 0;

/* 0x80028558 */
long gSwCardTimeoutEvent = 0;

/* 0x8002855C */
long gSwCardNewEvent = 0;

/* 0x80028560 */
long gHwCardEOFEvent = 0;

/* 0x80028564 */
long gHwCardErrorEvent = 0;

/* 0x80028568 */
long gHwCardTimeoutEvent = 0;

/* 0x8002856C */
long gHwCardNewEvent = 0;

/* 0x80028570 */
/* firstfile2/nextfile walk this 15-row directory buffer 0x28 bytes at a
   time (count_card_files); count_free_blocks reads row word 6 and
   card_file_exists compares the 0x15-byte name. */
struct DIRENTRY card_dir[15] = {{{0}}};

/* 0x800287C8 */
/* The card state machine's step. */
s32 card_st = 0;

/* 0x800287CC */
s32 card_slot = 0;

/* 0x800287D0 */
/* The two per-slot outputs of those counters: func_8002168C writes
   D_800287D0[i] and func_800217D4 writes D_800287D8[i], and start_card_server
   clears all four. */
s32 D_800287D0[2] = {0, 0};

/* 0x800287D8 */
s32 D_800287D8[2] = {0, 0};

/* 0x800287E0 */
s32 card_evflg = 0;

/* 0x800287E4 */
CARD_STATE gCardState = CARD_STATE_OK;

/* 0x800287E8 */
/* One 0x9E8-byte UnkNode buffer -- func_8003FE78 is passed 0x27A words.
   D_80028808 names its +0x20 word, the only part the overlay reaches by
   name, so the buffer is spelled as three objects. */
u8 D_800287E8[0x20] = {0};

/* 0x80028808 */
s32 D_80028808 = 0;

/* 0x8002880C */
/* The rest of it. */
u8 D_8002880C[0x9C4] = {0};

/* 0x800291D0 */
/* A 0x112-word UnkNode buffer. */
u8 D_800291D0[0x448] = {0};

/* 0x80029618 */
/* Five 0x3E4-byte rows, one 0xF9-word UnkNode each (func_80023FF0
   indexes them as D_80029618[b]). */
u8 D_80029618[5][0x3E4] = {{0}};

/* 0x8002A98C */
/* Four bytes of non-zero residue between two buffers, with no symbol and
   no reader. See docs/owning-data.md on why this region cannot be .bss. */
u32 D_8002A98C = 0xF97F8BEF;

/* 0x8002A990 */
/* A 0x394-word UnkNode buffer. */
u8 D_8002A990[0xE50] = {0};

/* 0x8002B7E0 */
/* A 0x1C-word UnkNode buffer. */
u8 D_8002B7E0[0x70] = {0};

/* 0x8002B850 */
/* A 0x45-word UnkNode buffer. */
u8 D_8002B850[0x114] = {0};

/* 0x8002B964 */
/* The second four bytes of residue. */
u32 D_8002B964 = 0x6F7C7327;

/* 0x8002B968 */
/* A 0x68-word UnkNode buffer. */
u8 D_8002B968[0x1A0] = {0};

/* 0x8002BB08 */
/* A 0x19-word UnkNode buffer, and the last object in the overlay: the
   0x494 bytes above it are the zero pad to the 0x269000 sector boundary. */
u8 D_8002BB08[0x64] = {0};
