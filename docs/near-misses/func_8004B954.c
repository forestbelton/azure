/* func_8004B954 (unk_1C540) -- 289 of 290 words, 265 DIFF. NOT compiled.
 *
 * The ring builder: given a sprite's bounding box it walks a circle of
 * vertices and writes one GPU primitive per step, either 0x1C bytes (the
 * `mode != 0` arm, two colour words) or 0x14. The step count comes from the
 * flag byte at the fifth argument -- 0x20 set means 0x40 steps, else 0x10 set
 * means 0x20, else 0x10 -- and the angle increment is 0x1000 / steps.
 * func_80064584 and func_800644B8 are the usual sine and cosine.
 *
 * THE FRAME IS NOW EXACT, which took two shapes, and the census in advise.py
 * is what found both.
 *
 * * THE VERTEX IS `s16 v[2]`, STORED AS A WORD. The listing's locals are
 *   0x10..0x27: a four-byte object at 0x10 written as two halfwords (`sh` at
 *   0x10 and at 0x12, three times each) and READ AS A WORD (`lw` at 0x10,
 *   four times), a hole at 0x14, then four 32-bit locals at 0x18 (the x
 *   base), 0x1C (the x span), 0x20 (0x1000 / steps) and 0x24 (the primitive
 *   code word, which starts at 1, takes +0x400 and +0x800 from the flag bits
 *   and is ALSO the loop counter's start value). Two separate `s16` locals
 *   with `*(s32 *) (q - 8) = cx;` make the second one dead, gcc drops it, and
 *   every later local slides down by four -- and that spelling is not even
 *   faithful, since it writes one sign-extended halfword over four bytes
 *   where the listing writes the PAIR. `s16 v[2]` with `*(s32 *) v` reads is
 *   both correct and exactly right on the layout: 0x10, 0x12, 0x18, 0x1C,
 *   0x20 and 0x24 all agree, hole included, because the array is addressable
 *   and takes six bytes' worth of alignment.
 *
 *   Measured and not working for the same pair: a `DVECTOR` with a struct
 *   assignment (315 words -- gcc copies it field by field), an `SVECTOR`
 *   (308, and eight bytes of frame), and `*(s32 *) &cx` on two `s16` locals
 *   (259 -- the address-taken aliasing makes gcc reload everything).
 *
 * * ONE WALKING POINTER, NOT TWO. The two primitive loops are in exclusive
 *   arms, so they share it. With a separate pointer for each the frame grows
 *   to 0x58: one extra spilled local at 0x28 (seven loads, two stores) AND
 *   one extra saved register, which is eighteen words. Dropping either alias
 *   gets to 289; dropping the 0x1C-stride one is better (265 DIFF against
 *   272).
 *
 * WHAT IS LEFT: ONE WORD, and the census says where it is. Every local slot
 * now agrees. The only rows left are INCOMING ARGUMENTS, and they say the
 * listing keeps two of its parameters IN MEMORY where this source holds them
 * in registers:
 *
 *     0x50 (arg0)  listing: sw once, lw twice     here: never touched
 *     0x68 (arg6)  listing: lw three times        here: lw once
 *
 * `mode` is tested three times in the source and the listing reloads it from
 * its home slot at each test, with the `nop` that implies; this build reads
 * it once and cse shares the result. Likewise arg0 is stored to its home at
 * entry and reloaded for two of the four func_8004C010 calls. That is
 * `assign_parms`' stack home reached through `reg_equiv_mem`, which happens
 * when the PARAMETER itself goes to memory -- and func_8005B4D0's park
 * records the measurement that routing a parameter through a copy does NOT
 * reach it. So the remaining word is register pressure: the listing has two
 * more values in memory than this build does, and something must make them
 * lose the allocation.
 */

#include "common.h"

s32 func_800644B8(s32);
s32 func_80064584(s32);
void func_8004C010(u8 *, s32);

u8 *func_8004B954(s32 c0, s32 c1, u8 *p, u8 *r, u8 *f, s32 *out, s32 mode)
{
    s16 v[2];
    s32 bx;
    s32 dx;
    s32 step;
    s32 code;
    u8 *q;
    u8 *e;
    s32 n;
    s32 dy;
    s32 by;
    s32 i;
    s32 ang;

    q = p;
    e = q;
    code = 1;
    dx = *(s16 *) (e + 0x10) - *(s16 *) (e + 8);
    dy = *(s16 *) (e + 0x22) - *(s16 *) (e + 0xA);
    if ((f[0] & 0x20) != 0)
    {
        n = 0x40;
    }
    else if ((f[0] & 0x10) != 0)
    {
        n = 0x20;
    }
    else
    {
        n = 0x10;
    }
    step = 0x1000 / n;
    if ((f[0] & 0x40) != 0)
    {
        if ((f[0] & 1) != 0)
        {
            code += 0x400;
            bx = *(s16 *) (p + 8) - dx;
        }
        else
        {
            bx = *(s16 *) (p + 8);
        }
        if ((f[0] & 2) != 0)
        {
            by = *(s16 *) (p + 0xA) - dy;
            code += 0x800;
        }
        else
        {
            by = *(s16 *) (p + 0xA);
        }
        n = n / 4;
    }
    else
    {
        dy = dy >> 1;
        dx = dx >> 1;
        by = *(s16 *) (p + 0xA) + dy;
        bx = *(s16 *) (p + 8) + dx;
    }
    if (dx != 0 && dy != 0)
    {
        v[0] = (func_80064584(0) * dx >> 12) + bx;
        v[1] = (func_800644B8(0) * dy >> 12) + by;
        *(s16 *) (e + 8) = (s8) f[8] + bx;
        *(s16 *) (e + 0xA) = (s8) f[9] + by;
        if (mode != 0)
        {
            e[3] = 6;
        }
        else
        {
            e[3] = 4;
        }
        q = e + 4;
        *(s32 *) (p + 4) = *(s32 *) (f + 4);
        func_8004C010(q, c0);
        func_8004C010(q, c1);
        e[7] = f[1] & 0x7F;
        q = e + 0xC;
        if (mode != 0)
        {
            r += 0xC;
            *(s32 *) (e + 0xC) = *(s32 *) r;
            func_8004C010(q, c0);
            func_8004C010(q, c1);
            i = code;
            *(s32 *) (e + 0x14) = *(s32 *) (e + 0xC);
            q = e + 0x18;
            if (code + n >= i)
            {
                ang = i * step;
                do
                {
                    *(s32 *) e = *(s32 *) p;
                    *(s32 *) (q - 0x14) = *(s32 *) (p + 4);
                    *(s32 *) (q - 0x10) = *(s32 *) (p + 8);
                    *(s32 *) (q - 0xC) = *(s32 *) (p + 0xC);
                    *(s32 *) (q - 8) = *(s32 *) v;
                    *(s32 *) (q - 4) = *(s32 *) (p + 0x14);
                    v[0] = (func_80064584(ang) * dx >> 12) + bx;
                    i++;
                    e += 0x1C;
                    v[1] = (func_800644B8(ang) * dy >> 12) + by;
                    ang += step;
                    *(s32 *) q = *(s32 *) v;
                    q += 0x1C;
                } while (code + n >= i);
            }
        }
        else
        {
            i = code;
            q = e + 0x10;
            if (code + n >= i)
            {
                ang = i * step;
                do
                {
                    *(s32 *) e = *(s32 *) p;
                    *(s32 *) (q - 0xC) = *(s32 *) (p + 4);
                    *(s32 *) (q - 4) = *(s32 *) v;
                    *(s32 *) (q - 8) = *(s32 *) (p + 8);
                    v[0] = (func_80064584(ang) * dx >> 12) + bx;
                    i++;
                    e += 0x14;
                    v[1] = (func_800644B8(ang) * dy >> 12) + by;
                    ang += step;
                    *(s32 *) q = *(s32 *) v;
                    q += 0x14;
                } while (code + n >= i);
            }
        }
        *out = n;
    }
    else
    {
        if (mode != 0)
        {
            r += 0xC;
        }
        *out = 0;
    }
    return r;
}
