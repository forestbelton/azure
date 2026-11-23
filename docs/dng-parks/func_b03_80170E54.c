/*
   func_b03_80170E54 -- A VERIFIED BODY THAT THE CONTAINER WILL NOT TAKE YET.

   338 words, candidate 338 words, 0 DIFFERING. aligndiff 0 as well, and
   `--raw' is 79 differing of which all 79 are relocations (22 jal, 9 j, 48
   %hi/%lo halves). This body is RIGHT. It is parked for a placement reason,
   not a matching one, and it should be re-landed unchanged the moment that
   reason is fixed.

   WHY IT IS NOT IN csrc/: it makes gcc emit ITS OWN JUMP TABLE.

   The function switches on `*(u16 *)(d + 0x46) & 0x3FFF' over 12 cases, and
   the ROM's code indexes the table at `jtbl_b03_80170808' -- which lives in
   the b03 segment's LEADING `data' subsegment (splat/dungeon.bin.yaml:352,
   file offset 0x9e9000..0x9e9054, so the table itself is at 0x9e9008). Our
   compile emits a fresh 12-entry table into .rodata instead, and the linker
   appends it past the end of the code:

       built  addiu $v0, $v0, 0x3c80      (its own table at 0x80173C80)
       ref    addiu $v0, $v0, 0x0808      (jtbl_b03_80170808)

   `make containers' therefore said `size differs by +0x120' on dungeon.bin,
   which is 0x30 bytes of new .rodata times the SIX members of this family,
   and 52.5% of the file differed from 0x9e9974 on because everything after
   the first table shifted.

   `ovlverify' cannot see this. It measures a function's own words against
   its listing and nothing else, so a body whose .rodata is wrong reports
   `0 differing' and the only thing that catches it is the container build.
   That is why the build gate is the gate.

   WHAT THE FIX LOOKS LIKE: the leading `[0x9e9000, data]' subsegment has to
   become the C file's own `.rodata' so the emitted table lands at
   0x80170808 instead of after the code, and the ROM's bytes must then stop
   being included separately. 0x54 bytes are in that subsegment and the table
   is 0x30 of them at +8, so whatever occupies 0x80170800..0x80170808 and
   0x80170838..0x80170854 has to come from the C file too, in that order.
   None of that is attempted here.

   THIS IS NOT A ONE-FAMILY PROBLEM. 229 of the carved block's functions
   reference a `jtbl_' symbol and 223 of them are still stubs, so every
   segment with a leading data subsegment will hit this. Solving it is worth
   more than a batch of landings.

   THE FAMILY: 6 members (tools/decomp/dngfam.py func_b03_80170E54), and the
   other five were cloned, verified at 0 differing, and unspliced again by
   tools/decomp/dngunsplice.py. They do NOT need re-deriving -- land this
   body and `dngclone' reproduces them. Their jump tables are their own
   addresses; agent-h2's note on the table's 12 entries is below.

   JUMP TABLE ENTRIES at jtbl_b03_80170808 (file 0x9e9008 of
   data/dungeon/dungeon.bin), read by the agent that landed this:
   0x8017121C x3, 0x80171248, 0x801711C8 x3, 0x80171190 x2, 0x80171248 x2,
   0x8017120C. So cases 1-3 reach the func_800AAF00 arm, 5-7 the
   func_800A0818 arm, 8-9 func_b03_80171DA0, 12 func_800A9A0C, and
   4/10/11/default fall to func_b03_801715F4. A sibling copy has the same
   shape at its own addresses; check the equivalent table before replicating.

   THE THREE LOAD-BEARING SPELLINGS, all measured, all still needed:

   (1) The func_b03_801715F4 tail is WRITTEN OUT AT ALL THREE SITES, not
   reached by `goto'. Shared through a `goto walk' label the body is still
   338 words but 62 aligned differences, every one of them `p' and `c'
   exchanging $s0 and $s1. From `-da': pseudo 82 (`c') is 35 refs / 500 insns
   / priority 3500 and pseudo 268 (the D_80082E80 pointer local) is 5 refs /
   28 insns / priority 3571, and 268 conflicts with `c' and `d' but not `p'.
   Duplicated, `c' is 37 refs over 512 insns, priority 3613, sorts ahead of
   268, and the map comes out c/p/d/b = $s0/$s1/$s2/$s3, which is the
   target's. See docs/matching-levers.md's entry on this lever.

   (2) ONE TABLE LOCAL PER DRESS SITE, not one shared across the three. A
   single `e' is 338 words / 2 aligned differences: the shared pseudo spans
   the whole function, local-alloc stops tying the `high' to its `lo_sum' at
   the d[0x25]==0 site, and the pair comes out `lui $v0' / `addiu $a1, $v0'
   where the target has `lui $a1' / `addiu $a1, $a1'. Writing D_80173CD4 out
   twice with no local at that site is also 0.

   (3) ARM B's D_800814A8 MUST BE A POINTER LOCAL ASSIGNED BEFORE THE d+0x2A
   STORE, so the store fills the `lw's load-delay slot:
       store written inline in the call expression ....... 339 words, 16
       call result in `m', a BYTE local before the store .. 339 words, 13
       call result in `m', a POINTER local before it ...... 338 words,  0
   `m' is free at `s16' as well as `s32' (both 0).

   RELINK DELTAS from the resident twin func_800D8C64
   (csrc/ovl_dungeon_800D50FC.c), for whoever re-lands this:
     * the three facing re-dresses go through func_80047784 on a BYTE table
       (`>> 9 & 7', one lbu) where the resident has func_8003DB94 on a
       POINTER table (`>> 7 & 0x1C', a lw);
     * THREE tables, not one: the d[0x25]==0 arm compares c+0x2C against
       D_80173CDC and stores D_80173CD4; func_800AA924 and func_800AA6B4 take
       D_80173CD4 and D_80173C84 where the resident passes NULL; the two
       state-0xE dresses use D_b03_80173C7C;
     * func_800D904C / func_800DA660 become func_b03_8017139C /
       func_b03_80173A30;
     * the 0x80000 arm does NOT carry the resident's p+0x90 = 0 /
       p+0x98 &= ~1 / b->unk_14 = 0 opener (343 words, 73 aligned with them),
       and the final guard chain has THREE tests, not four -- the resident's
       `(*(u16 *)(p + 0x98) & 1)' is absent (343 words, 52 aligned with it);
     * the `d[0x6D] > 0' arm's func_800D92C0 call is replaced by the inlined
       12-case dispatch that needs the jump table.

   DECLARATIONS
   void func_b03_8017139C(u8 *p, UnkVec *b, u8 *c, u8 *d);
   void func_b03_80171F64(u8 *p, UnkVec *b, u8 *c, u8 *d);
   s32 func_b03_8017207C(u8 *a, void *b, void *c, s32 d);
   s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
   s16 func_8009FB34(u16 x, u16 y);
   s16 func_800A1C58(u8 *p);
   s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
   void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
   extern u8 D_80173C84[];
   extern u8 D_80173CCC[];
   END DECLARATIONS

   Every one of those was in csrc/ovl_dng_b03.c when this verified and the
   unsplice left them there, so re-landing needs none of them added back.
*/
void func_b03_80170E54(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b03_8017139C(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80173CDC)
        {
            e = D_80173CD4;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80173CDC)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80173CD4) != 0)
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
            e2 = D_b03_80173C7C;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80173C84) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b03_80173A30(p, (u8 *)b, c, d);
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
            func_b03_801715F4(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b03_8017207C(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b03_801715F4(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b03_80171DA0(p, b, c, d) != 0)
            {
                return;
            }
            func_b03_80171F64(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_80173CCC, func_b03_80170E54);
            return;
        default:
            func_b03_801715F4(p, b, c, d);
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
    e3 = D_b03_80173C7C;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}
