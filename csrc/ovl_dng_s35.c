#include "common.h"

typedef struct UnkDngS36Scale
{
    u8 unk_00[0x18];
} UnkDngS36Scale;
extern u8 D_s35_80024004[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s35", func_s35_8002401C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s35", func_s35_80024594);

/* Copy this bank's six-entry {u16, u16} scale table into the frame and write
   the entry the caller's +0x50 state selects into the renderable's +0x1C/+0x1E
   scale pair, clamping an out-of-range index to 0.  47 words, 0 differing,
   FIRST COMPILE.

   NO TWIN at any ratio and nothing shares a symbol with it (D_s35_80024004 is
   bank-local).  The donor is csrc/ovl_dng_s67.c's landed func_s67_80024C50,
   found by its comment rather than by a ratio: the same alignment-1 block move
   of a bank table into a frame struct, the same `<< 2' index.

   THREE FACTS, all priced from this body at 0 differing:
     * `u8 *d = t.unk_00' as a POINTER LOCAL.  Indexing through `t.unk_00'
       directly is 49 words / 13 aligned differences -- two words and a
       register exchange for one folded pointer.
     * The index is `i << 2', not `i * 4': 4 differing.  s67 priced the same
       fact at 2 on its own body.
     * The 0x1C store before the 0x1E store: swapped is 4 differing.
   MEASURED AND FREE, so neither is the reason this lands:
   `memcpy(d, D_s35_80024004, 0x18)' in place of the struct assignment (0
   either way, where s67 priced the same swap at 18 -- it has stack parameters
   for the scheduler to sink past the block move and this one has none), and
   `if ((u16)i >= 6)' in place of `if (i < 0 || i >= 6)' (0 either way; both
   reach build_range_check's `andi 0xFFFF' + `sltiu 6'). */
void func_s35_80024784(u8 *p, s16 i)
{
    UnkDngS36Scale t;
    u8 *d = t.unk_00;
    UnkSlot *s;

    t = *(UnkDngS36Scale *)D_s35_80024004;
    if (i < 0 || i >= 6)
    {
        i = 0;
    }
    s = (UnkSlot *)*(u32 *)(p - 0x14);
    s->unk_1C = *(u16 *)(d + (i << 2));
    s->unk_1E = *(u16 *)(d + (i << 2) + 2);
}
