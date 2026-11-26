/* PARK -- func_b07_80170E54, ovl_dng_b07.
   455 target words, 455 candidate words, 1 ALIGNED DIFFERENCE.
   dngfam: 1 member, so this park is worth 455 words and no replication.

   THE WHOLE RESIDUE IS ONE REGISTER ON ONE STORE:

     aligndiff row 405  0x801714A8
       candidate   sw $s0, 0xA0($s3)
       target      sw $v0, 0xA0($s3)

   Everything else -- all 455 words, every opcode, immediate, displacement,
   branch target and the whole 12-entry jump table -- matches. ovlverify on
   the same body reads 455/455 with 1 differing.

   WHAT THE BODY IS. The +0x8C handler at the head of this bank's actor
   machine, and the b07 member of the four-body family that shares
   jtbl_*_80170808 (func_b08_80170E7C, func_b22_801724BC, func_b39_80170F6C).
   Written from the landed func_b42_801714B8 (csrc/ovl_dng_b42.c:916, 481
   words) with the relink deltas read off this listing; its twin
   func_b09_80171728 (csrc/ovl_dng_b09.c, 363 words) supplied the short tail.

   THE SHARED 12-ENTRY TABLE, SETTLED BY STANDALONE COMPILE, NOT BY THE
   VERIFIER. tools/cdkgcc -O2 -G0 -S on the unit emits
     entries, case 1..12:  L76 L76 L76 L77 L71 L71 L71 L67 L67 L77 L77 L73
     arm label addresses:  L67 < L71 < L73 < L76 < L77
   i.e. entry-address order {8,9} {5,6,7} {12} {1,2,3} default, which is
   exactly the target's 0x801711CC, 0x80171204, 0x80171248, 0x80171258,
   0x80171284. Dispatch is addiu -1 then sltiu 0xC on
   (*(u16 *)(d + 0x46) & 0x3FFF): MINVAL 1, twelve cases. Entries 4, 10 and
   11 all hold the DEFAULT label, which is also the range-check beqz target,
   so they are gcc's pad to maxval and must NOT be written as empty break
   arms. FIVE distinct targets, NINE real cases, FOUR real arms; b42 has six
   distinct targets because it splits 8 and 9 where this body merges them.
   case 12 is a label INSIDE the w[0x9A] != 0x11 then-arm.

   TWENTY-SIX SPELLINGS OF THE p + 0xA0 NODE STORE, ALL 455 WORDS AND ALL THE
   SAME SINGLE ROW. Measured with aligndiff from the body below:

     as written below (n2 = call; store; if (n2 == NULL) return)      1
     *(u32 *) / *(UnkNode **) / ((s32 *)p)[0x28] casts on the store   1
     store written before vs after the null test                     1
     if (n2 != NULL) { whole body } positive form                    1
     store from a second s32 local q, copy before the store          1
     store from a second s32 local q, copy after the store           1
     the existing s32 m reused instead of a new local                1
     store the call result, then read n2 back from p + 0xA0          1
     ... and test p + 0xA0 instead of n2                             1
     chained assignment n2 = *(UnkNode **)(p + 0xA0) = call          1
     the chained form inside the if condition                        1
     n2 = (UnkNode *)(*(s32 *)(p + 0xA0) = (s32)call)                1
     do { store; } while (0) around the store                        1
     if ((s32)n2 == 0) instead of if (n2 == NULL)                    1
     declaration order permuted (UnkNode *n2 first)                  1
     volatile store                                                  3
     the two conjunctions of the above that reach 0 separately       1

   ALL PERMUTATIONS MEASURING IDENTICALLY MEANS ALLOCATION PRIORITY IS OUT,
   and the declaration permutation confirms it. The cause is upstream of the
   allocator and I believe it is now understood:

   THE TARGET NEEDS TWO PSEUDOS WHERE cse LEAVES ONE, AND THE COMPILER SOURCE
   SAYS NO C SPELLING OF THAT STATEMENT CAN DO IT.
     calls.c:2141 -- a non-BLKmode call result is ALWAYS copy_to_reg'd into a
       fresh pseudo, so the store can never read the hard $v0 from expansion.
       calls.c:2057 would emit the move straight into a MEM target, but
       store_expr only reaches it with want_value, and the want_value arm then
       copy_to_reg's the MEM back, which is a word this body has not got.
     cse.c make_regs_eqv -- "Prefer fixed hard registers to anything. Prefer
       pseudo regs to other hard regs." $v0 is not FIXED_REGNO_P, so the
       PSEUDO always becomes qty_first_reg and canon_reg can never substitute
       $v0 into the store.
     Any second local holding the same value is a reg-reg copy inside one cse
       basic block (cse blocks end only at a CODE_LABEL), so cse collapses it
       and local-alloc's combine_regs then ties what is left.
   So the target's `sw $v0' is a pseudo that DIES AT THE STORE while a second
   pseudo lives across func_8004491C, and that needs a cse-block boundary
   between the two, which this control flow does not have.

   A SANDBOX WITNESS for the mechanism, in case someone wants to re-derive it
   cheaply: in a 12-line standalone file, the identical statement sequence
   emits `sw $v0' when the node DIES before the next call and `sw $s0' when it
   lives across it -- the register follows the live range, not the spelling.

   THE PIN DEVICE MOVES THE ROW BUT DOES NOT CLOSE IT, which is itself the
   useful datum:
     register UnkNode *raw __asm__("$2") holding the call result, store from
     raw, n2 = raw                              455 words, 1 row at 0x801714AC
   With it, row 405 (the store) MATCHES and the next insn breaks instead:
   candidate `move $a0, $v0' against target `addu $a0, $s0, $zero', because
   cse then substitutes the still-live $2 for n2 at func_8004491C's argument.
   Placing the pin before or after the n2 copy makes no difference (both 1 row
   at 0x801714AC), and adding `__asm__ volatile("" ::: "$2")' to kill cse's
   record is 3 rows -- it forces a save. So the store's register IS reachable;
   what is not reachable is reaching it without spending the next insn.

   ONE MEASURED FACT THAT IS NOT ABOUT THE RESIDUE: n2 MUST BE ITS OWN LOCAL.
   Reusing the tail's `w' pointer for the node (it already has a set in the
   case 5/6/7 arm, so this makes reg_n_sets 2) is 455 words and 54 ALIGNED
   ROWS, and the residue starts in the PROLOGUE -- the save order changes to
   s1 before s0. That is the reg_n_sets lever firing on the save list.

   TWO FACTS THAT ARE PART OF WHY THE REST MATCHES, both measured on the way:
     n2->unk_08 MUST BE RELOADED AT ALL THREE STORES. A `m2 = n2->unk_08'
     local is 453 words (TWO WORDS SHORT) and 16 aligned rows: the three
     `lw $a0, 0x8($s0)' collapse to one and the 0x808080 lui/ori then
     materialise late. Write `*(s16 *)((u8 *)n2->unk_08 + 0x2) = ...' at each
     of the three sites, as func_b37_80173EC0's spawn block does.
     The r2 block is PLAIN CASTS in func_b37's measured order -- the +0x8 load
     off D_800DEA68 ahead of the two r2[0x4]/r2[0x5] byte stores.

   WHAT I DID NOT TRY, for whoever picks this up: a permuter search (agents in
   this batch do not run it) and -da on the .greg allocation order for the
   two-pseudo question. The .cse and .lreg dumps for the q1 spelling were read
   and are what the cse paragraph above is based on: insn 1084
   `(set (reg/v:SI 88) (reg:SI 2 v0))', insn 1090 storing reg 88, insn 1093
   testing reg 88 -- one pseudo, the copy insn deleted by cse.

   DECLARATIONS
   s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
   s16 func_8009FB34(u16 x, u16 y);
   s16 func_800A1C58(u8 *p);
   s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
   void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
   u8 *func_8003DE58(u8 *, u8 *, void *, s16);
   s32 func_8004491C(UnkNode *, u32);
   extern u8 D_800DEA68[];
   void func_800D79B0();
   s32 rand(void);
   void func_b07_80171570(u8 *, UnkVec *, u8 *, u8 *);
   void func_b07_80172184(u8 *, UnkVec *, u8 *, u8 *);
   s32 func_b07_8017229C(u8 *, void *, void *, s32);
   extern u8 D_80174150[];
   extern u8 D_80174158[];
   extern u8 D_80174178[];
   extern u8 D_80174180[];
   void func_800AD4D0(u8 *);
   void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
   END DECLARATIONS */
void func_b07_80170E54(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR out;
    u16 x;
    s16 n;
    s32 v;
    s32 m;
    u8 *e;
    u8 *e2;
    u8 *e3;
    u8 *e4;
    u8 *w;
    UnkNode *n2;
    u8 *r2;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b07_80171570(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174190)
        {
            e = D_80174188;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174190)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80174188) != 0)
        {
            return;
        }
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            e2 = D_80174148;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174150) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b07_80173EF4(p, (u8 *)b, c, d);
            e3 = D_80174140;
            *(u8 **)(c + 0x2C) = e3;
            func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            func_800AAB10(p, b, c, d);
        }
    }
    n = func_8009FB34(c[0x24], c[0x25]);
    c[0x26] = n;
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u16 *)(c + 0x24) == D_80082EA4)
        {
            func_b07_801717B4(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b07_8017229C(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b07_801717B4(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b07_80171FC0(p, b, c, d) != 0)
            {
                return;
            }
            func_b07_80172184(p, b, c, d);
            return;
        case 5:
        case 6:
        case 7:
            m = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
            w = (u8 *)D_800814A8;
            *(s16 *)(d + 0x2A) = m;
            if (w[0x9A] != 0x11)
            {
            case 12:
                func_800A9A0C(d);
                return;
            }
        case 1:
        case 2:
        case 3:
            func_800AAF00(p, b, c, (s32)D_80174140, func_b07_80170E54);
            return;
        default:
            func_b07_801717B4(p, b, c, d);
            return;
        }
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) == 0)
    {
        if ((s8)n < 0 || (D_800E2970_s[(s8)n].unk_0C & 2) == 0)
        {
            if ((v & 0x430) == 0)
            {
                if ((s16)func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    *(s16 *)(d + 0x2A) =
                        func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
                }
            }
        }
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((*(u16 *)(c + 0x14) & 0x40) != 0)
    {
        return;
    }
    e4 = D_80174148;
    if (*(u8 **)(c + 0x2C) != e4 && *(u8 **)(c + 0x2C) != D_80174140)
    {
        *(u8 **)(c + 0x2C) = e4;
        func_80047784(c, e4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0xAA) = 1;
    }
    else if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(s16 *)(p + 0xAA) = *(u16 *)(p + 0xAA) - 1;
        if (*(s16 *)(p + 0xAA) < 0)
        {
            *(s16 *)(p + 0xAA) = (rand() & 0x1F) + 0x20;
        }
        if (*(s16 *)(p + 0xAA) < 2)
        {
            *(u8 **)(c + 0x2C) = D_80174148;
        }
        else
        {
            *(u8 **)(c + 0x2C) = D_80174140;
        }
        func_80047784(c,
                      (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                      0);
    }
    if (func_8003DE58(*(u8 **)(c + 0x8), c, &out, 1) == NULL)
    {
        return;
    }
    if (*(u8 **)(c + 0x2C) != D_80174148)
    {
        return;
    }
    n2 = func_8003FD64(0x112, D_80083498);
    *(s32 *)(p + 0xA0) = (s32)n2;
    if (n2 == NULL)
    {
        return;
    }
    func_8004491C(n2, (u32)D_80045340);
    n2->unk_10 = (u32)func_800D79B0;
    *(s16 *)((u8 *)n2->unk_08 + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u16)out.vx;
    *(s16 *)((u8 *)n2->unk_08 + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u16)out.vy;
    *(s16 *)((u8 *)n2->unk_08 + 0xA) = *(u16 *)((u8 *)b + 0xA) + (u16)out.vz;
    r2 = (u8 *)n2->unk_0C;
    *((u8 *)n2 + 0xBB) = 0;
    *(s16 *)(r2 + 0x1E) = 0xC00;
    *(s16 *)(r2 + 0x1C) = 0xC00;
    *(s32 *)(r2 + 0xC) = 0x808080;
    *(u8 **)r2 = D_800DEA68;
    *(u16 *)(r2 + 0x14) |= 0xC;
    *(u32 *)(r2 + 0x8) = *(u32 *)(D_800DEA68 + 4);
    r2[0x4] = 0;
    r2[0x5] = 0;
}
