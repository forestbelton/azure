/* func_8004B954 (unk_1C540) -- 283 of 290 words, 261 DIFF. NOT compiled.
 *
 * The ring builder: given a sprite's bounding box it walks a circle of
 * vertices and writes one GPU primitive per step, either 0x1C bytes (the
 * `mode != 0` arm, two colour words) or 0x14. The step count comes from the
 * flag byte at the fifth argument -- 0x20 set means 0x40 steps, else 0x10 set
 * means 0x20, else 0x10 -- and the angle increment is 0x1000 / steps.
 * func_80064584 and func_800644B8 are the usual sine and cosine.
 *
 * TWO DRAFTS ARE PARKED HERE, because they are wrong in opposite directions
 * and each one has half the answer. The source below is the 283-word one.
 *
 * THE 283-WORD DRAFT BELOW IS NOT SEMANTICALLY FAITHFUL, and that is
 * deliberate -- it is parked for its measurements, not as a candidate. It
 * stores `*(s32 *) (q - 8) = cx;` from a single `s16`, which writes cx
 * sign-extended over four bytes where the listing writes the cx/cy PAIR as
 * one word. `cy` is consequently dead and gcc drops it. Do not build on this
 * draft as though it were correct C for this function; build on the
 * `s16 v[2]` form described below, which is faithful and has the frame
 * layout right, and find its extra local.
 *
 * WHAT THE FRAME SAYS (advise.py's census; it is the whole diagnostic here)
 *
 * The listing's frame is 0x50 with the saved registers from 0x28, so the
 * locals are 0x10..0x27: a FOUR-BYTE object at 0x10 written as two halfwords
 * (`sh` at 0x10 and at 0x12, three times each) and READ AS A WORD (`lw` at
 * 0x10, four times), a hole at 0x14, and four 32-bit locals at 0x18 (the x
 * base), 0x1C (the x span), 0x20 (0x1000 / steps) and 0x24 (the primitive
 * code word, which starts at 1 and takes +0x400 and +0x800 from the flag
 * bits and is ALSO the loop counter's start value).
 *
 * The pair at 0x10 is the vertex. Written as two `s16` locals (`cx`, `cy`)
 * and stored with `*(s32 *) (q - 8) = cx;` the second one is dead, gcc drops
 * it, and the remaining four 32-bit locals slide down to 0x14..0x20 -- every
 * local displacement off by four. Written as `s16 v[2]` with the stores
 * spelled `*(s32 *) (q - 8) = *(s32 *) v;` THE LAYOUT IS EXACTLY RIGHT:
 * 0x10, 0x12, 0x18, 0x1C, 0x20 and 0x24 all agree, hole included, because
 * the array is addressable and takes six bytes' worth of alignment. That
 * draft is 307 words, seventeen too many, with one extra local at 0x28.
 *
 * Measured and not working for the same pair: a `DVECTOR` with a struct
 * assignment (315 words -- gcc copies it field by field), an `SVECTOR`
 * (308, and eight bytes of frame), and `*(s32 *) &cx` on two `s16` locals
 * (259 words -- the address-taken aliasing makes gcc reload everything).
 *
 * WHAT IS LEFT IN THE 283-WORD DRAFT
 *
 * Seven words short, and the diff localises them: three in the flag-test
 * chain at the top (the listing tests 0x20, then 0x10, with the 0x40 case
 * as the branch TARGET rather than the fallthrough -- that much is fixed
 * below) and about three in EACH of the two primitive loops. The loops are
 * short because this draft keeps the vertex in registers where the listing
 * stores both halves to the stack every iteration and reloads the pair as a
 * word -- which is the same defect as the layout one, so the `s16 v[2]`
 * form is the one to build on once its extra local is found.
 *
 * Note the listing keeps arg0, arg1 and arg3 in their own HOME slots
 * (0x50, 0x54, 0x5C) and reloads them rather than holding them in
 * callee-saved registers -- including `arg3 += 0xC`, which is a
 * load/addiu/store through the home slot.
 */

#include "common.h"

s32 func_800644B8(s32);
s32 func_80064584(s32);
void func_8004C010(u8 *, s32);

u8 *func_8004B954(s32 c0, s32 c1, u8 *p, u8 *r, u8 *f, s32 *out, s32 mode)
{
    s16 cx;
    s16 cy;
    s32 bx;
    s32 dx;
    s32 step;
    s32 code;
    u8 *q;
    u8 *e;
    u8 *s;
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
    s = q;
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
        cx = (func_80064584(0) * dx >> 12) + bx;
        cy = (func_800644B8(0) * dy >> 12) + by;
        *(s16 *) (s + 8) = (s8) f[8] + bx;
        *(s16 *) (s + 0xA) = (s8) f[9] + by;
        if (mode != 0)
        {
            s[3] = 6;
        }
        else
        {
            s[3] = 4;
        }
        q = s + 4;
        *(s32 *) (p + 4) = *(s32 *) (f + 4);
        func_8004C010(q, c0);
        func_8004C010(q, c1);
        s[7] = f[1] & 0x7F;
        q = s + 0xC;
        if (mode != 0)
        {
            r += 0xC;
            *(s32 *) (s + 0xC) = *(s32 *) r;
            func_8004C010(q, c0);
            func_8004C010(q, c1);
            i = code;
            *(s32 *) (s + 0x14) = *(s32 *) (s + 0xC);
            q = s + 0x18;
            if (code + n >= i)
            {
                ang = i * step;
                do
                {
                    *(s32 *) s = *(s32 *) p;
                    *(s32 *) (q - 0x14) = *(s32 *) (p + 4);
                    *(s32 *) (q - 0x10) = *(s32 *) (p + 8);
                    *(s32 *) (q - 0xC) = *(s32 *) (p + 0xC);
                    *(s32 *) (q - 8) = cx;
                    *(s32 *) (q - 4) = *(s32 *) (p + 0x14);
                    cx = (func_80064584(ang) * dx >> 12) + bx;
                    i++;
                    s += 0x1C;
                    cy = (func_800644B8(ang) * dy >> 12) + by;
                    ang += step;
                    *(s32 *) q = cx;
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
                    *(s32 *) (q - 4) = cx;
                    *(s32 *) (q - 8) = *(s32 *) (p + 8);
                    cx = (func_80064584(ang) * dx >> 12) + bx;
                    i++;
                    e += 0x14;
                    cy = (func_800644B8(ang) * dy >> 12) + by;
                    ang += step;
                    *(s32 *) q = cx;
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
