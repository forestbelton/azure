/* func_8003E34C alone, carved out of the middle of unk_11940.
 *
 * **This unit stays at the project default -G0 and must NOT be added to the
 * MASPSXFLAGS_G8 list**, unlike the other gprel_* units. Its four $gp accesses
 * are asked for individually by the size records below; at -G8 the three
 * absolute bytes D_800814D1/D2/D3 would go $gp-relative too, since they are
 * one byte each and sit inside gp +/- 0x8000.
 *
 * WHY THE CARVE IS NECESSARY, and it is not the usual -G reason. The size
 * record is a FILE-SCOPE directive: maspsx applies it to every access in the
 * translation unit. func_8003E39C, which sat four lines below this function in
 * unk_11940, reaches D_800814D0 ABSOLUTELY -- it materialises &D_800814D0[1]
 * with an `la` and keeps it across its loop -- so leaving the two functions
 * together rewrote func_8003E39C's accesses to %gp_rel as well and cost it 5
 * words, which shifted the whole image by -0x14 and changed the jump-table
 * contents. `make check` caught it; verify.py cannot, because it compiles one
 * function at a time.
 *
 * That is also the historical argument for the split. ASPSX chose $gp-relative
 * addressing per translation unit, for the symbols DEFINED in that unit, so one
 * object cannot be $gp-relative in one function and absolute in another within
 * a single unit -- which means these two functions were never in one. Note the
 * tension this leaves, honestly: func_8003E4FC (now gprel_11CFC) shares
 * D_80080AD4 and D_80080ADC with this function $gp-relatively, yet
 * func_8003E39C lies between them in the text. So the "$gp symbols cluster
 * into runs of consecutive functions" rule does not hold cleanly here, and the
 * original unit boundaries around 0x8003E34C are not fully recovered. What is
 * established is that this function cannot share a unit with func_8003E39C.
 *
 * See docs/addressing-and-translation-units.md.
 */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

__asm__(".sdata\n\t.size D_800814D0,1\n\t.size D_80080AD0,1\n"
        "\t.size D_80080AD4,2\n\t.size D_80080ADC,4\n\t.text");

/* Resets the command queue and the audio state: twelve zero stores plus two
 * constants, in listing order.
 *
 * THIS FUNCTION SETTLES THE "+0/+1/+2/+3" QUESTION that
 * docs/addressing-and-translation-units.md left open. D_800814D0 is reached
 * %gp_rel here while the three bytes above it are absolute `lui %hi` / `sb
 * %lo` pairs, and the open hypothesis was that the original wrote all four
 * through one materialised base -- `((u8 *) &D_800814D0)[1]` -- with splat
 * merely naming each byte. **It did not.** Spelled that way, through the
 * existing `extern u8 D_800814D0[4]`, maspsx rewrites the +1/+2/+3 accesses to
 * %gp_rel as well: its rewrite keys on the SYMBOL and ignores the
 * displacement, so `D_800814D0+2` becomes `2($gp)` and the three `lui $at`
 * pairs disappear -- 17 words against 20, 19 differing. Declaring the three as
 * their own byte objects, with the size record on D_800814D0 alone, matches at
 * 20 of 20. So the original had four separate byte objects here. The array
 * declaration remains right for func_8003F270, which reads elements 1 and 0 as
 * a pair through one address; both spellings coexist.
 *
 * The D_80083958 stores are a plain struct access, not a pointer local: the
 * first emits `lui` + `sw %lo`, and cse then materialises the base with an
 * `addiu` for the remaining four, which is the 0x4/0x5/0x6/0x8 displacements
 * in the listing. */
void func_8003E34C(void) {
    D_800814D2[0] = 2;
    D_800814D3[0] = 0xFF;
    D_80083958.unk_00 = 0;
    D_800814D0[0] = 0;
    D_800814D1 = 0;
    D_80083958.unk_04 = 0;
    D_80083958.unk_05 = 0;
    D_80080AD0 = 0;
    D_80080ADC = 0;
    D_80080AD4 = 0;
    D_80083958.unk_06 = 0;
    D_80083958.unk_08 = 0;
}
