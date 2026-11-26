/* func_lo16_8001A768 -- ovl_dng_lo16, 280 target words.
   RECOVERED by agent h181, batch 35, 2026-11-25.

   PROVENANCE -- READ THIS BEFORE TRUSTING ANYTHING BELOW.  This body was
   written by a dedicated sub-agent that was KILLED MID-RUN by a weekly API
   limit before it could write a park of its own.  Its working copy survived in
   sub-a768; I have re-measured it and preserved it here so the work is not
   lost.  **THE AGENT'S REASONING AND ITS REJECTED SPELLINGS DIED WITH ITS
   TRANSCRIPT.**  Nothing in this header is a priced fact except the two
   figures I measured myself.  Every spelling in the body is therefore
   UNMEASURED -- do not preserve any of it believing it was fought for, and do
   not write any of it up as a lever.  Splice markers have been stripped.

   MEASURED BY ME, in that copy, just now:

       built 287 words, target 280 words, 125 aligned differences
       func_lo16_8001A768: target 280 words, candidate 287 words, 229 differing (masked)

   Last thing the agent reported before it died: it was testing whether some 16-bit values are memory rather than pseudos.

   THE RESIDUE as aligndiff gives it (`R' = register names only, `X' = a
   differing instruction, `+'/`-' = present on one side only):

       X    0    0 8001A768  addiu sp,sp,-128                         addiu $sp, $sp, -0x78
       X    1                sw ra,124(sp)
       X    2                sw s8,120(sp)
       X   11    9 8001A78C  sw a0,128(sp)                            sw $s1, 0x54($sp)
       X   12   10 8001A790  sw a3,140(sp)                            sw $s0, 0x50($sp)
       X        11 8001A794                                           sw $a0, 0x78($sp)
       X        12 8001A798                                           sw $a3, 0x84($sp)
       X   14   14 8001A7A0  lw a0,148(sp)                            lw $a0, 0x8C($sp)
       -        30 8001A7E0                                           lw $t0, 0x90($sp)
       +   35                lw a1,152(sp)
       X   66   66 8001A870  lw t3,140(sp)                            lw $t2, 0x84($sp)
       X   87   87 8001A8C4  lw t3,128(sp)                            lw $t2, 0x78($sp)
       X   88   88 8001A8C8  nop                                      addiu $v0, $v0, %lo(D_8001F6A4)
       X   90   90 8001A8D0  nop                                      lui $a0, %hi(D_8001F6A0)
       +   93                lui v0,0x0
       +   94                addiu v0,v0,0
       +  112                sra v0,a0,0x18
       -       112 8001A928                                           sra $v0, $a0, 24
       +  128                lhu t3,32(sp)
       +  130                sw t3,72(sp)
       X  135  131 8001A974  nop                                      addiu $s4, $zero, 0x2
       +  142                li s4,2
       X  147  142 8001A9A0  lhu t2,40(sp)                            lw $t2, 0x78($sp)
       X  148                lw t3,128(sp)
       X  180  174 8001AA20  lw t3,72(sp)                             lhu $t2, 0x20($sp)
       -       177 8001AA2C                                           lhu $t2, 0x28($sp)
          ... and 99 further rows, 85 of them register-name-only

   NEXT AGENT: re-derive the residue yourself and price with aligndiff, never
   with ovlverify's masked count -- on a body this size the masked count
   reports every word downstream of a single fault.  Then work the FIRST
   non-register row; the rest are usually its wake.

   DECLARATIONS
   extern s32 D_8001F664;
   extern s8 D_8001F66C[];
   s32 func_lo16_8001ABC8(s16, s16);
   END DECLARATIONS */

s32 func_lo16_8001A768(s16 *p, s32 x, s32 y, s8 *map, s32 arg4, s32 n, s32 k)
{
    u8 lo;
    u16 lx;
    u16 ly;
    u16 ln;
    u16 lk;
    s16 d;
    s32 cnt;
    s32 nx;
    s32 ny;
    s32 idx;
    s32 c;
    s32 m;
    s32 i;
    s32 j;
    s32 par;
    s32 step;
    s32 dir2;

    lx = x;
    ly = y;
    ln = n;
    lk = k;
    cnt = 0;
    d = p[0];
    nx = x + (u16)D_8006CCD8[d] * n;
    ny = y + (u16)D_8006CCE8[d] * n;
    if ((s16)nx >= 0 && (s16)nx < *(s32 *)D_8001F660 && (s16)ny >= 0 &&
        (s16)ny < D_8001F664 && ((s8 *)D_8001F6A4)[d >> 1] == 0)
    {
        idx = (s16)(nx + (s16)ny * *(s32 *)D_8001F660);
        c = map[idx];
        if (c > 0)
        {
            if (func_lo16_8001ABC8(idx, (s16)lk) == 0)
            {
                return idx;
            }
            cnt++;
        }
        else if (c < 0)
        {
            cnt++;
        }
    }
    else
    {
        cnt++;
    }
    if (cnt != 0)
    {
        d = p[0];
        D_8001F6A4[d >> 1] = 1;
        if ((s16)ln < *(s8 *)&D_8001F6A0)
        {
            D_8001F6A0 = ln;
        }
    }
    lo = D_8001F6A0;
    if ((s8)lo < (s16)ln)
    {
        m = (s8)lo;
    }
    else
    {
        m = (s16)ln;
    }
    for (i = 1; i < (s16)ln; i++)
    {
        if (i >= m)
        {
            cnt++;
        }
        else
        {
            j = (s16)ln - i;
            par = func_800A6D30() & 1;
            for (step = 2; step > 0; step--, par ^= 1)
            {
                dir2 = (p[0] + D_8001F66C[par]) & 6;
                nx = lx + D_8006CCD8[p[0]] * i + D_8006CCD8[dir2] * j;
                ny = ly + D_8006CCE8[p[0]] * i + D_8006CCE8[dir2] * j;
                if ((s16)nx >= 0 && (s16)nx < *(s32 *)D_8001F660 &&
                    (s16)ny >= 0 && (s16)ny < D_8001F664 &&
                    ((s8 *)&D_8001F6E8[par * 4])[i] == 0)
                {
                    idx = (s16)(nx + (s16)ny * *(s32 *)D_8001F660);
                    c = map[idx];
                    if (c == 0)
                    {
                        continue;
                    }
                    if (c > 0 && func_lo16_8001ABC8(idx, (s16)lk) == 0)
                    {
                        goto found;
                    }
                    ((s8 *)&D_8001F6E8[par * 4])[i] = 1;
                }
                cnt++;
            }
        }
    }
    if (cnt >= 3)
    {
        return -1;
    }
    goto recurse;
found:
    p[0] = dir2;
    return idx;
recurse:
    return (s16)func_lo16_8001A768(p, (s16)lx, (s16)ly, map, arg4,
                                   (s16)(ln + 1), (s16)lk);
}
