/* func_lo16_800165B8 -- ovl_dng_lo16, 398 target words.
   RECOVERED by agent h181, batch 35, 2026-11-25.

   PROVENANCE -- READ THIS BEFORE TRUSTING ANYTHING BELOW.  This body was
   written by a dedicated sub-agent that was KILLED MID-RUN by a weekly API
   limit before it could write a park of its own.  Its working copy survived in
   sub-65b8; I have re-measured it and preserved it here so the work is not
   lost.  **THE AGENT'S REASONING AND ITS REJECTED SPELLINGS DIED WITH ITS
   TRANSCRIPT.**  Nothing in this header is a priced fact except the two
   figures I measured myself.  Every spelling in the body is therefore
   UNMEASURED -- do not preserve any of it believing it was fought for, and do
   not write any of it up as a lever.  Splice markers have been stripped.

   MEASURED BY ME, in that copy, just now:

       built 396 words, target 398 words, 92 aligned differences
       func_lo16_800165B8: target 398 words, candidate 396 words, 349 differing (masked)

   Last thing the agent reported before it died: 92 aligned differences after a first compile.

   THE RESIDUE as aligndiff gives it (`R' = register names only, `X' = a
   differing instruction, `+'/`-' = present on one side only):

       -        30 80016630                                           addu $a0, $fp, $zero
       -        40 80016658                                           lui $v0, %hi(D_800814A8)
       +   40                lui v0,0x0
       +   52                lui v0,0x0
       +   53                addiu s4,v0,0
       X   64   63 800166B4  li s1,7                                  addu $a0, $fp, $zero
       -        67 800166C4                                           addu $a0, $fp, $zero
       X   85   85 8001670C  move a2,s1                               lui $v0, %hi(D_8006CCE8)
       X        86 80016710                                           addiu $s2, $v0, %lo(D_8006CCE8)
       X        87 80016714                                           sll $a2, $s1, 16
       X        88 80016718                                           sra $a2, $a2, 16
       -       106 80016760                                           addiu $a0, $a0, 0x20
       +  104                addiu v0,v0,32
       -       110 80016770                                           sll $a1, $a1, 6
       -       112 80016778                                           addiu $a1, $a1, 0x20
       +  109                sll v0,v0,0x6
       +  110                addiu v0,v0,32
       X  125  128 800167B8  nop                                      addu $a0, $fp, $zero
       -       135 800167D4                                           addu $a0, $fp, $zero
       X  135  139 800167E4  lbu v0,16(sp)                            addu $a1, $v0, $zero
       X  136                lbu v1,17(sp)
       -       154 80016820                                           lbu $a2, 0x10($sp)
       -       155 80016824                                           lbu $a3, 0x11($sp)
       -       156 80016828                                           addu $a0, $s5, $zero
       +  154                sw v1,12(s6)
       -       162 80016840                                           sw $v1, 0xC($s6)
          ... and 66 further rows, 27 of them register-name-only

   NEXT AGENT: re-derive the residue yourself and price with aligndiff, never
   with ovlverify's masked count -- on a body this size the masked count
   reports every word downstream of a single fault.  Then work the FIRST
   non-register row; the rest are usually its wake.

   DECLARATIONS
   void func_8003DB94(u8 *, s32, s32);
   extern u8 D_80083498[];
   extern u8 D_80045340[];
   extern u8 D_80082E80[];
   extern s16 D_80083228;
   extern u8 D_80012D6C[];
   extern s16 D_800DCE60[];
   extern s32 D_800E4938[];
   extern s32 D_800E3540;
   extern s32 D_80081484;
   extern s32 D_80081470;
   extern s8 D_80082A3B;
   extern u8 *D_800E3D18;
   extern s32 D_800E3D80[];
   extern s32 D_800E3CF8[];
   extern s32 D_800E3D48[];
   extern u8 D_800DD078[];
   extern u8 D_800DD090[];
   extern u8 D_800DD0A8[];
   extern u8 *D_800DD274[];
   extern u8 *D_800DCFB0[];
   extern u16 D_800DD264[];
   extern u16 D_800DD26C[];
   } UnkDngLo16Name13;
   UnkNode *func_8003FE78(s32, UnkNode *, s16);
   s32 func_80042900(s8 *, s8);
   void func_80089AA0(u8 *, UnkVec *, u8 *);
   void func_80094988(u8 *, u8 *, s16, s16);
   void func_800489F4_4(u8 *, s32, s32, s32) __asm__("func_800489F4");
   void func_80096088(u8 *, u8 *);
   void func_800A2B04(UnkVec *, s32, s32);
   s16 func_8009FB34(u16, u16);
   void func_8009D380(void);
   UnkNode *func_800A6C00(void);
   UnkNode *func_800BC26C(void *, s16, s32, s32);
   void func_lo16_800172A0(UnkVec *, s16);
   END DECLARATIONS */

void func_lo16_800165B8(void)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *q;
    u8 *b;
    u8 *g;
    s32 i;
    s32 d;
    s32 v;
    s32 k;
    u8 c;
    u8 x;
    u8 y;
    u16 t;

    D_800DCF5A = 1;
    n = func_8003FE78(0, (UnkNode *)D_80083498, 0x53);
    n->unk_10 = (u32)func_80089AA0;
    func_8004491C(n, (u32)D_80045340);
    m = (UnkVec *)D_80083780;
    n->unk_08 = m;
    func_8003DB4C((int *)m, 6);
    r = (UnkSlot *)D_80082E80;
    n->unk_0C = (u32)r;
    func_8003DB4C((int *)r, 0xC);
    p = (u8 *)n + 0x20;
    D_800814A8 = (u32)p;
    D_800E3D7C = p;
    p[0x13] = 0;
    q = p;
    if ((D_80082E60.unk_16 & 1) == 0)
    {
        for (;;)
        {
            do
            {
            } while (func_800A4E2C(&x, &y) < 0);
            if (D_80012090 != 0)
            {
                break;
            }
            if (func_80033BC0(0x1389) != 0)
            {
                break;
            }
            if (D_8008146C != 0x1F)
            {
                break;
            }
            d = func_800BCB04((x << 6) | 0x20, (y << 6) | 0x20, -0x400);
            for (i = -1; i < 2; i++)
            {
                func_8009A350(x, y, i, &t);
                if ((t & 0x8000) != 0)
                {
                    goto retry;
                }
                if (abs((func_800BCB04(((x << 6) + 0x20 + (D_8006CCD8[i & 7] << 6)) & 0xFFE0,
                                       ((y << 6) + 0x20 + (D_8006CCE8[i & 7] << 6)) & 0xFFE0,
                                       -0x400) &
                         0xFF) -
                        (d & 0xFF)) >= 0x21)
                {
                    goto retry;
                }
            }
            break;
        retry:;
        }
    }
    else
    {
        x = D_80082E60.unk_10;
        y = D_80082E60.unk_12;
    }
    i = 7;
    *((u8 *)r + 0x24) = x;
    *((u8 *)r + 0x25) = y;
    func_800A2B04(m, x, y);
    func_8009A21C(x, y, 0x300);
    v = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), -0x400);
    *(s16 *)((u8 *)m + 0xA) = v;
    *(s16 *)(q + 0x88) = v;
    *(s16 *)(q + 0x8A) = v;
    *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
    *(u16 *)(p + 0xA2) |= 0x10;
    r->unk_1E = 0x1000;
    r->unk_1C = 0x1000;
    *(s32 *)(q + 0x5C) = (s32)n;
    *(s32 *)(q + 0x58) = (s32)n;
    r->unk_28 = D_80080A80;
    func_80094988(p, q, x, y);
    *(s32 *)(q + 0x14) &= 0xFFEFFFFF;
    do
    {
        func_800489F4_4((u8 *)r, D_800DD078[i], 0, 0);
        D_800E3D80[i] = r->unk_08;
        func_800489F4_4((u8 *)r, D_800DD0A8[i], 0, 0);
        D_800E3CF8[i] = r->unk_08;
        func_800489F4_4((u8 *)r, D_800DD090[i], 0, 0);
        D_800E3D48[i] = r->unk_08;
        i--;
    } while (i >= 0);
    D_800E3D18 = (u8 *)D_800E3CF8;
    if ((s16)func_80042900((s8 *)q, 0xA) != 0)
    {
        *(u8 **)((u8 *)r + 0x2C) = (u8 *)D_800DD274;
        func_8003DB94((u8 *)r, (s32)D_800DD274[((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    else
    {
        func_800489F4_4((u8 *)r, 0xB0, 0, 1);
        *(u8 **)((u8 *)r + 0x2C) = (u8 *)D_800DCFB0;
    }
    *(s16 *)(p + 0x94) = -1;
    *(s16 *)(p + 0x118) = 0;
    *(s16 *)(q + 0x2A) = 0x400 - (((u16)D_80083228 + 0x100) & 0xE00);
    func_800BC26C(D_80083498, 1, (s32)((u8 *)r + 0x2C), (s32)(p + 0x2A));
    p[0x9A] = 0xFF;
    *(s32 *)(q + 0x1C) &= 0xFFEFFFFF;
    func_80096088(p, q);
    r->unk_14 |= 0x8000;
    for (i = 0; i < 2; i++)
    {
        c = D_80012D6C[i];
        *(p + i + 0xFA) = c;
        if (c == 2)
        {
            *(p + i + 0xFA) = 1;
        }
    }
    b = (u8 *)0x80010000;
    g = (u8 *)&D_80083160;
    *(u16 *)(p + 0xA2) |= 0x10;
    *(s32 *)(q + 0x14) |= 0x4000;
    *(UnkDngLo16Name13 *)(q + 0x34) = *(UnkDngLo16Name13 *)0x8001020C;
    *(s8 *)(q + 0x42) = 0;
    *(s8 *)(q + 0x41) = 0;
    *(u32 *)(p + 0xDC) = 0x2C808080;
    *(u32 *)(p + 0xE0) = 0x2C808080;
    k = D_800DD264[(s16)*(u16 *)(b + 0x20A2)];
    *(s16 *)(g + 0xC4) = k;
    D_800DCE60[0] = k;
    D_800DCE60[2] = 0;
    D_800DCE60[1] = *(u16 *)(g + 0xC6);
    D_800DCE60[3] = D_800DD26C[(s16)*(u16 *)(b + 0x20A0)];
    D_800E4938[0] = 0;
    D_800E4938[1] = (s32)n;
    D_800E4938[2] = 0;
    D_80083460h[1] |= 2;
    *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
    *(s8 *)(p + 0x9C) = -2;
    func_8009D380();
    D_800E3540 = 0;
    D_80081484 = 0;
    D_80081470 = 0;
    func_lo16_800172A0(m, D_800DCE60[3]);
    D_80082A3B = -0x24;
    if ((*(u16 *)(b + 0x3714) & 4) != 0)
    {
        func_800A6C00();
    }
}
