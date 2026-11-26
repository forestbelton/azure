/* func_lo16_80018E80 -- ovl_dng_lo16, 401 target words.
   RECOVERED by agent h181, batch 35, 2026-11-25.

   PROVENANCE -- READ THIS BEFORE TRUSTING ANYTHING BELOW.  This body was
   written by a dedicated sub-agent that was KILLED MID-RUN by a weekly API
   limit before it could write a park of its own.  Its working copy survived in
   sub-8e80; I have re-measured it and preserved it here so the work is not
   lost.  **THE AGENT'S REASONING AND ITS REJECTED SPELLINGS DIED WITH ITS
   TRANSCRIPT.**  Nothing in this header is a priced fact except the two
   figures I measured myself.  Every spelling in the body is therefore
   UNMEASURED -- do not preserve any of it believing it was fought for, and do
   not write any of it up as a lever.  Splice markers have been stripped.

   MEASURED BY ME, in that copy, just now:

       built 402 words, target 401 words, 17 aligned differences
       func_lo16_80018E80: target 401 words, candidate 402 words, 346 differing (masked)

   Last thing the agent reported before it died: 399 of 401 words and 98 aligned rows; the copy on disk now measures better than that, so it kept working after its last report.

   THE RESIDUE as aligndiff gives it (`R' = register names only, `X' = a
   differing instruction, `+'/`-' = present on one side only):

       X   34   34 80018F08  lui a3,0x8001                            lui $a2, %hi(D_80012090)
       X   37   37 80018F14  lui a3,0x8001                            lui $a2, %hi(D_80012090)
       X   38   38 80018F18  lw a1,8336(a3)                           lw $v1, %lo(D_80012090)($a2)
       X   56   56 80018F60  li v0,40                                 addu $v1, $a1, $zero
       -        63 80018F7C                                           addu $v1, $a1, $zero
       X   76   77 80018FB4  sll v0,a2,0x10                           addiu $v0, $zero, 0x2
       +   82                sra v0,v0,0x10
       +   83                li v1,2
       X   86   85 80018FD4  lw v1,556(a3)                            lw $v1, %lo(D_8001022C)($a2)

   NEXT AGENT: re-derive the residue yourself and price with aligndiff, never
   with ovlverify's masked count -- on a body this size the masked count
   reports every word downstream of a single fault.  Then work the FIRST
   non-register row; the rest are usually its wake.

   DECLARATIONS
   void func_lo16_80019648(void);
   void func_lo16_80019A74(void);
   s32 func_lo16_80019684(s16, s16, s16);
   void func_lo16_800194C4(s16);
   void func_80048088(s16);
   END DECLARATIONS */

s32 func_lo16_80018E80(void)
{
    u8 *b;
    u8 *c;
    s16 *rp;
    s32 q;
    s16 k;
    s32 m;
    s32 ret;
    s32 flag;
    s32 v;
    s32 t;
    s32 i;
    s32 h;

    ret = 0;
    flag = 0;
    b = (u8 *)0x80010000;
    if (*(s32 *)(b + 0x22C) == 1)
    {
        if (D_8008146C < 2)
        {
            *(s32 *)(b + 0x2090) = 1;
            func_lo16_80019648();
        }
        else if (*(s32 *)(b + 0x2090) == 1)
        {
            *(s32 *)(b + 0x2090) = 0;
        }
    }
    t = D_80082E6B;
    if (t < 0x16)
    {
        if (t >= 0x12)
        {
            ret = 0;
            goto done;
        }
    }
    {
        c = (u8 *)0x80010000;
        q = *(s32 *)(c + 0x2090);
        if (q == 0)
        {
            k = (u16)D_8008146C;
            m = (s16)(k / 5);
            if (k >= 0x28)
            {
                flag = 1;
                D_800E296C |= 0x10000000;
                q = k;
            }
            else
            {
                q = k;
            }
            if (q == 0x28)
            {
                ret = 1;
                if (func_80033BC0(0xA2) != 0)
                {
                    m = 7;
                }
                else
                {
                    m = -1;
                    func_lo16_80019A74();
                }
                flag = 1;
            }
            else if (q < 2)
            {
                ret = (func_800A6D30() & 3) + 3;
            }
            else if (k == 2 && *(s32 *)(c + 0x22C) != 1)
            {
                ret = 2;
                D_800E296C |= 0x10000000;
            }
            else if (D_8008146C == 0x1F && func_80033BC0(0x1389) == 0)
            {
                D_800E39C8[0].unk_06 = 1;
                D_800E39C8[0].unk_07 = 1;
                D_800E3648_s[0].unk_00 = 0x37;
                D_800E3648_s[0].unk_01 = 0x13;
                D_800E3648_s[0].unk_03 = 0;
                D_800E3648_s[0].unk_02 = 0x63;
                D_800E296C |= 0x10000000;
            }
            if (m >= 0)
            {
                m = func_lo16_80019684(m + 1, 1, 1);
                if (m == 0)
                {
                    D_80080A88 = 0;
                }
                if (D_80080A88 == 0)
                {
                    func_80044698();
                    D_80080A88 = 1;
                    func_80053DA8(0x200);
                    func_800542BC();
                }
                if (m == 0)
                {
                    func_lo16_80019684(0, 1, 0);
                }
            }
            else
            {
                func_lo16_80019684(0, 1, 0);
                func_lo16_80019684(0x29, 1, 1);
                func_80053DA8(0x200);
                func_800542BC();
                func_80048088(0x38);
            }
            if (flag == 0)
            {
                func_lo16_800194C4(D_8008146C);
            }
        }
        else
        {
            flag = 1;
            if (q == 1)
            {
                ret = 9;
                D_800E296C |= 0x10000000;
                func_lo16_80019684(1, 1, 1);
                func_80053DA8(0x200);
                func_800542BC();
                func_lo16_80019684(0, 1, 0);
            }
            else if (q == 2)
            {
                v = func_800A6D30() & 7;
                if (func_lo16_80019684(v + 1, 1, 1) == 0)
                {
                    func_80044698();
                    func_80053DA8(0x200);
                    func_800542BC();
                    func_lo16_80019684(0, 1, 0);
                }
                rp = D_80081468h;
                if (rp[2] == 1)
                {
                    {
                        u8 *bb;
                        u8 *pa;

                        v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                        h = rp[2] + (func_800A6D30() & 1);
                        if (h >= 0x64)
                        {
                            h = 0x63;
                        }
                        i = 0;
                        bb = (u8 *)D_80083460h;
                        for (; i < 4; i++)
                        {
                            pa = &D_800DDC9C[i * 2 + 0x0];
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18)) = v;
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 1) = h;
                            D_800DDC9C[i * 2 + 1] = v;
                            *pa = v;
                        }
                    }
                    {
                        u8 *bb;
                        u8 *pa;

                        v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                        h = D_8008146C + (func_800A6D30() & 1);
                        if (h >= 0x64)
                        {
                            h = 0x63;
                        }
                        i = 0;
                        bb = (u8 *)D_80083460h;
                        for (; i < 4; i++)
                        {
                            pa = &D_800DDC9C[i * 2 + 0x8];
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x8) = v;
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x9) = h;
                            D_800DDC9C[i * 2 + 0x9] = v;
                            *pa = v;
                        }
                    }
                }
                else
                {
                    memcpy(D_80083478, D_80083478 + 0x10, 0x10);
                    memcpy(D_800DDC9C, D_800DDC9C + 0x10, 0x10);
                }
                {
                    u8 *bb;
                    u8 *pa;

                    v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                    h = D_8008146C + (func_800A6D30() & 1);
                    i = 0;
                    bb = (u8 *)D_80083460h;
                    for (; i < 4; i++)
                    {
                        pa = &D_800DDC9C[i * 2 + 0x10];
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x10) = v;
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x11) = h;
                        D_800DDC9C[i * 2 + 0x11] = v;
                        *pa = v;
                    }
                }
                {
                    u8 *bb;
                    u8 *pa;

                    v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                    h = D_8008146C + (func_800A6D30() & 1);
                    i = 0;
                    bb = (u8 *)D_80083460h;
                    for (; i < 4; i++)
                    {
                        pa = &D_800DDC9C[i * 2 + 0x18];
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x18) = v;
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x19) = h;
                        D_800DDC9C[i * 2 + 0x19] = v;
                        *pa = v;
                    }
                }
            }
        }
    }
done:
    *(s32 *)0x80010234 = D_8008146C;
    return ret;
}
