/* PARK: func_8009F644 -- 136 words against a target of 137, 81 aligned
   differences.  FIRST DRAFT, 2026-11-20.

   The last of the seven 4-mod-8 jump-table functions to get a body.  It owns
   `jtbl_80088D7C' at rodata offset 0x61c, which is still in the HEAD unit, so
   it needs a THIRD carve -- a split at 0x61c -- before it can land.  The two
   carves already done (j96, j97) cover the other six; splat prints 0xA361C as
   its remaining suggestion on every run.

   WHAT IT IS.  The message/damage-number queue push.  Bails out if
   `D_800E296C & 0x10000000', if `*(u16 *)0x80013714 & 1', or if the queue is
   full at 0xF01 entries; otherwise it indexes the queue by the count at
   +0x6, decides whether to overwrite the last entry or append, and fills one
   of six shapes chosen by a jump table on `f - 8'.

   THE SIGNATURE IS FIXED BY THE UNIT, NOT BY THE LISTING.
   csrc/ovl_dungeon_decls.h:858 already declares
   `void func_8009F644(u8 *, s32, s32, s32);' and four landed call sites rely
   on it, so the definition must be `(u8 *p, s32 f, s32 a, s32 e)' even though
   m2c infers `s16' and `s8' for the second and fourth.  The NARROWING IS AT
   THE USE instead: `(q[1] & 0xF8) != (s16)f' is worth the target's
   `sll $v0, $a1, 16' / `sra $v0, $v0, 16' pair -- two words.  Declaring the
   parameter `s16' is a conflicting-types error against the header, not an
   option.

   WHAT IS ESTABLISHED
     * `g = (u8 *)0x80013710;' AS A POINTER LOCAL, FIRST STATEMENT.  The
       target materialises that address ONCE with `lui`/`ori` into a
       callee-saved register and reaches the queue through `g + 0x4',
       `g + 0x6' and `g + 0xC'.  Written as three separate literal addresses
       it is 139 words / 101 rows; as a pointer local, 134 / 80.
     * `*(u16 *)(g + 0x4) & 1', a HALFWORD read -- the target has `lhu'.
     * the `|| (s = f - 8, q[0] >= 0x7F)' COMMA in the third test, which is
       how `s' gets set on the path where the first two tests are false, and
       a `goto' into the `else' arm for the path where they are true.  That is
       m2c's block_8 and it is the only shape that reaches the switch with `s'
       set on every path.
     * the switch's three shared tails -- the `q[0] = m' store (block_17), the
       `q[3] = 0; q[2] = 0' pair (block_18) and the 0x68/0x70/0x78/0x88 arm
       FALLING THROUGH into 0x48/0x50/0x60/0x80/0x90/0x98 -- all reached by
       `goto'/fallthrough rather than written out.

   WHAT IS LEFT: ONE REGISTER AND ONE WORD.  The prologue saves exactly the
   same four registers as the target and `f', `a' and `e' land in $s5, $s6 and
   $s7 in BOTH -- but `g' is in $s2 here and $s1 in the target, and $s1 is
   not used at all by this body.  So it is one global_alloc ordering step:
   `find_reg' never allocates a register for the first time in pass 0, and in
   pass 1 it takes the lowest free one, which should be $s1.  The missing word
   is in the same region.  Price `g''s allocno off `.lreg' before sweeping.

   DECLARATIONS
   NONE -- func_8009F9E8 rides an implicit declaration, and D_800E296C and
   the existing func_8009F644 prototype are already in the header.
   END DECLARATIONS

   Measured with
     python3 tools/decomp/parkresidue.py func_8009F644
*/
void func_8009F644(u8 *p, s32 f, s32 a, s32 e)
{
    u8 *q;
    u8 *r;
    s32 i;
    s32 c;
    s32 m;
    s16 s;
    u8 v;
    u8 *g;

    g = (u8 *)0x80013710;
    if ((D_800E296C & 0x10000000) == 0 && (*(u16 *)(g + 0x4) & 1) == 0 &&
        *(u16 *)(g + 0x6) < 0xF01)
    {
        q = *(u8 **)(g + 0xC) + *(u16 *)(g + 0x6) * 2;
        i = ((u16)*(u16 *)(p + 0x2A) >> 9) & 7;
        v = q[1];
        if (v != 0)
        {
            if ((v & 7) != i || (q[1] & 0xF8) != (s16)f || (s = f - 8, q[0] >= 0x7F))
            {
                q += 2;
                *(u16 *)(g + 0x6) += 1;
                q[1] = 0;
                q[0] = 0;
                goto setsw;
            }
        }
        else
        {
        setsw:
            s = f - 8;
        }
        switch (s)
        {
        case 0x0:
        case 0x8:
        case 0x10:
        case 0x18:
        case 0x20:
        case 0x58:
            q[1] = f | i;
            m = q[0] + 1;
        store:
            q[0] = m;
        clear:
            q[3] = 0;
            q[2] = 0;
            break;
        default:
            goto clear;
        case 0x28:
        case 0x38:
        case 0xA0:
            q[1] = f | i;
            m = a | 0x80;
            goto store;
        case 0x40:
            q[1] = f | i;
            q[0] = a | 0x80;
            q[2] = e;
            q[3] = 0;
            q += 2;
            *(u16 *)(g + 0x6) += 2;
            goto clear;
        case 0x68:
        case 0x70:
        case 0x78:
        case 0x88:
            r = q;
            c = f | i;
            m = (a & 1) << 5;
            q = func_8009F9E8(c & 0xFF, m);
            if (r != q)
            {
                *(u16 *)(g + 0x6) -= 1;
                q[1] = c;
                q[0] = m | ~0x7F | (e & 0x1F);
                return;
            }
        case 0x48:
        case 0x50:
        case 0x60:
        case 0x80:
        case 0x90:
        case 0x98:
            q[1] = f | i;
            m = ((a & 1) << 5) | ~0x7F | (e & 0x1F);
            goto store;
        }
    }
}
