#include "common.h"
#include "main_twin.h"

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern void func_804080A4(s32 *p);
extern void func_804008A0(s32 use_second_slot);
extern void func_80400948(void);
extern void func_80400EF8(void);
extern s32 *func_804017EC(void);
extern void func_80407830(s32 *p, s32 a);

/* THE 0x80400000 DEBUG TWIN -- memory.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x8040086C..0x804008A0   .text from 0x804081AC

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main_twin/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */

/* 0x8040086C */
const char D_8040086C[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400894 */
const char D_80400894[12] __attribute__((section(".rodata"))) = "memory.c\0 B0";


/* Live func_8002789C, with four more calls in the 0x40 arm and two printfs
 * after. The 8-byte copy of t into p[6]/p[7] has to be a STRUCT assignment
 * here for the same reason as there -- see that function's note. */
void func_804081AC(s32 *p)
{
    s32 flag = 0;
    s32 *t;
    s32 diff[2];

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            func_804084DC(p - 8);
            D_8008CA34 = 2;
            func_80400EF8();
            func_8003FD58(0, 0xF, 0, 0);
            ((u16 *)p[5])[15] |= 0x2000;
        }
        else if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            func_804008A0(p[3]);
            p[13] = 0;
            p[-4] = (s32)func_804080A4;
            return;
        }
        else if (D_801379A8.unk_00 & 0xA000)
        {
            if (D_801379A8.unk_00 & 0x8000)
            {
                s32 v = p[3];

                p[3] = 0;
                if (v != 0)
                {
                    flag = 1;
                }
            }
            else
            {
                if (p[3] == 0)
                {
                    flag = 1;
                }
                p[3] = 1;
            }
        }
        if (flag != 0)
        {
            func_80063FF8(0x502);
            func_80407830((s32 *)p[9], p[3]);
        }
    }
    t = func_804017EC();
    diff[0] = t[0] != p[6];
    diff[1] = t[1] != p[7];
    *(UnkTwoWords *)&p[6] = *(UnkTwoWords *)t;
    func_8040787C((s32 *)p[9], t, diff);
    func_8003FA78(D_80400854, D_804090F8[t[0]]);
    func_8003FA78(D_80400860, D_804090F8[t[1]]);
    func_804018FC();
}

void func_80408394(s32 *p, s32 a, s32 b)
{
    p[5] = a;
    p[0] = b;
}

/* Live func_80027A9C. func_804076C8 takes ONE argument at this site -- the
 * live call passes p[3] second and there is no load of it here. */
void func_804083A0(s32 *p)
{
    s32 *q = p - 8;

    p[8] = func_80404C84(q, p + 3);
    p[9] = (s32)func_804076C8(q);
    func_80400948();
    p[-4] = (s32)func_804081AC;
}

/* A group allocator with NO func_80040560 call and the two arms the other
 * way round: the allocation path's assert never returns, so gcc jumps past
 * the else arm instead of over the then arm. */
s32 *func_804083FC(s32 a, s32 b)
{
    s32 *q = (s32 *)D_8040C5C0;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040C5C0, 0x19);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_8040086C, D_80400894, 0x173);
            func_8007BEF0(1);
        }
    }
    else
    {
        func_804084DC(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x44);
    }
    func_80408394(r, a, b);
    q[4] = (s32)func_804083A0;
    return q;
}

void func_804084DC(s32 *p)
{
    if (p == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_8040086C, D_80400894, 0x183);
        func_8007BEF0(1);
    }
    func_80404DBC((u8 *)p[0x10]);
    func_804077D4((s32 *)p[0x11]);
    ((u16 *)p)[15] |= 0x8000;
    D_8008DAB4 |= 0x8000;
}
