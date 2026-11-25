/* func_lo16_8001CFB8 -- ovl_dng_lo16, 79 target words.
   PARKED by agent h172, batch 35, 2026-11-25.

   BEST: 82 candidate words, 31 ALIGNED differences (54 under ovlverify).

   THE RESIDUE IS THREE EXTRA WORDS AND THEY ARE TWO EXTRA LOOP CURSORS.
   The target walks the 0x20 slots with exactly TWO cursors:

       $s1 = D_800E3648 + 0,  += 4     (read 0x1, stored 0x0/0x1/0x2/0x3)
       $s2 = D_800E39C8 + 0,  += 0x18  (stored 0x6/0x7/0x8)

   and the candidate emits FOUR:

       $s2 = D_800E3648 + 0            the `s' pointer local
       $s0 = $s2 + 2                   <<< an extra giv, the gate read
                                           becomes `lbu $v0, -1($s0)'
       $v0 = D_800E39C8 + 0
       $s1 = $v0 + 8                   <<< the `t' local biased to its LAST
                                           field (unk_08)

   Both extra biases are the offset of the LAST field written to that record,
   which is the unit's documented giv-bias rule -- so loop.c IS reducing the
   field accesses here and is NOT reducing them in the target.  In the target
   both registers are plain BIVs (source pointer locals) with constant
   displacements, which is free on MIPS, so loop.c should see benefit 0 and
   skip.  Why it does not skip in the candidate is the open question.

   Note the t-cursor's bias cannot come from a bias rule at all in the target:
   its accesses are 0x6/0x7/0x8 and its register is base+0, i.e. the pointer
   local itself, never a giv.

   WHAT THE FUNCTION DOES.  Gated on `(D_800E296C & 0x10000000) == 0', it
   picks a count `n = ((p[3] * p[2]) >> 6... ' -- no: `>> 2' here -- plus
   `rand() & 7', clamps it to 0x1C at 0x1D, then scans D_800E3648's slots for
   one whose +0x1 byte is zero and fills it: the paired D_800E39C8 record's
   +0x6/+0x7 get p[0]/p[1] plus a bounded random offset, the slot gets
   0x13/0x15/n/0xC0, and the record's +0x8 word is cleared.

   SPELLINGS MEASURED (candidate words / aligned rows):

     `(rand() & 7) + ((p[3] * p[2]) >> 2)'                81w / 39
     `((p[3] * p[2]) >> 2) + (rand() & 7)'  (product
       first -- the target's `addu $v1, $v1, $v0' puts the
       product in rs) and `i = 0' moved ahead of the two
       pointer inits                                     82w / 33
     + slot store order with unk_00 LAST instead of first 82w / 36
       -- the +2 cursor does NOT move, so the giv-bias rule is NOT what is
          choosing it here
     + both rand() calls before both stores, `u16' results 85w / 80
     + both rand() calls before both stores, results as
       `s32 r = rand(...) & 0xFFFF'                       85w / 35
     + the ORIGINAL store placement with `s32 r = ... &
       0xFFFF' for the first result only                  82w / 31  <- kept

   The last row matters beyond this body: `s32 r = f() & 0xFFFF' KEEPS the
   `andi' that `u16 r = (u16)f()' lets combine delete, and that one change
   landed func_lo16_8001D328 (97/97) from one word short.  It is priced FREE
   here only because other faults dominate.

   WHAT TO TRY NEXT: stop loop.c reducing the two records' field accesses.
   Untried: array subscripts (`D_800E3648_s[i]', `D_800E39C8[i]') with the
   counter incremented at the BOTTOM rather than the top -- that changes which
   expression is the biv and is the one structural alternative not swept here;
   and `-da' on `.greg' to read what benefit loop.c assigned the two givs,
   which is a question about the compiler's INPUT and so is the kind of thing
   `-da' is actually good for.

   DECLARATIONS
   extern u32 D_800E296C;
   typedef struct UnkDungeonSlot
   {
       u8 unk_00;
       u8 unk_01;
       u8 unk_02;
       u8 unk_03;
   } UnkDungeonSlot;
   extern UnkDungeonSlot D_800E3648_s[] __asm__("D_800E3648");
   typedef struct UnkDungeonSpotB
   {
       u8 pad_00[0x6];
       u8 unk_06;
       u8 unk_07;
       s32 unk_08;
       UnkNode *unk_0C;
       s16 unk_10;
       s16 unk_12;
       u16 unk_14;
       u8 pad_16[0x2];
   } UnkDungeonSpotB;
   extern UnkDungeonSpotB D_800E39C8[];
   s32 func_800A6DA4(u32, u32);
   u16 func_800A6D30();
   END DECLARATIONS

   UnkDungeonSpotB is copied from csrc/ovl_dungeon_decls.h, not invented.
*/
void func_lo16_8001CFB8(s16 *p)
{
    UnkDungeonSlot *s;
    UnkDungeonSpotB *t;
    s32 i;
    s16 n;
    s32 r;

    if ((D_800E296C & 0x10000000) != 0)
    {
        return;
    }
    n = (((u16)p[3] * (u16)p[2]) >> 2) + (func_800A6D30() & 7);
    if (n >= 0x1D)
    {
        n = 0x1C;
    }
    i = 0;
    s = D_800E3648_s;
    t = D_800E39C8;
    do
    {
        i++;
        if (s->unk_01 == 0)
        {
            r = func_800A6DA4(0, (u16)((u16)p[2] - 1)) & 0xFFFF;
            t->unk_06 = p[0] + r;
            t->unk_07 = p[1] + (func_800A6DA4(0, (u16)((u16)p[3] - 1)) & 0xFFFF);
            s->unk_00 = 0x13;
            s->unk_01 = 0x15;
            s->unk_03 = 0xC0;
            s->unk_02 = n;
            t->unk_08 = 0;
            return;
        }
        s++;
        t++;
    } while (i < 0x20);
}
