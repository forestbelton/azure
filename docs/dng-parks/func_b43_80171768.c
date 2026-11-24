/* PARK: func_b43_80171768, csrc/ovl_dng_b43.c, agent-h52, 2026-11-23.

   TARGET 470 words. BEST BUILT 470 words, 2 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify agrees at 2 differing masked, so the
   streams are in step and there is no layout inflation here).
   1 member, 470 words, 470 words total (tools/decomp/dngfam.py), so there is
   no replication value beyond its own body. This function owns NO jump table:
   its listing has no .section .rodata block and no jtbl_ symbol.

   RELOCATIONS AUDITED. ovlverify --raw names, in the target, exactly the
   symbols this body spells and no others: %hi/%lo pairs for D_8006CCD8 x2,
   D_8006CCE8 x2, D_8006CD00, D_800814A8 x2, D_80082E80 x4, D_80082EA4,
   D_80083460 x2 and D_800E2970, plus jal to func_8009A180, func_8009A21C,
   func_8009A3D0, func_8009A540 x3, func_8009A66C x2, func_8009FD7C x2,
   func_800A02AC x2, func_800A04F0 x2, func_800A0818 x4, func_800A0E6C,
   func_800A19E4, func_800A6D30, func_800A9A0C x2, func_800BCB04 and the
   bank-local func_b43_80171F24. No wrong bank-local table is hiding under a
   mask.

   WHAT IT IS. The banked room script's walk step. The FRAME-MATCHING twin is
   the landed carved func_b57_801726C8 in csrc/ovl_dng_b57.c (466 words):
   identical prologue -- addiu $sp, $sp, -0x38 and SEVEN callee-saved saves
   $s0..$s6, with $s0 holding the g = (u8 *)&D_80083460 pointer local -- so
   b57 lends both its shape facts and its allocation facts. func_b30_801716A4
   (458 words, csrc/ovl_dng_b30.c) has the same frame and is b57 minus the
   0x20000 arm's func_800A0134 guard.

   THE FOUR RELINK DELTAS FROM b57, all read off the listing:
     1. func_b57_80172E10 -> func_b43_80171F24 (the bank-local state check).
     2. func_8009A8C0 -> func_8009A66C at BOTH step-probe sites, which is also
        what b30 has.
     3. The 0x400 arm's actor-reaim block is CROSS-JOINED with the else arm's
        copy: the 0x400 arm's e != 0 branch jumps forward to .Lb43_80171AC4,
        which is the else arm's copy of the same five statements. Written out
        TWICE in C and left to jump2, which rebuilds the share by itself. e is
        $s0 in both arms here where b57 has $v1, because this body's else arm
        keeps e live across a func_8009A540 call.
     4. The else arm is rebuilt. b57 has
        e = func_800A04F0_r(...); if (e && (e+0x1C & 0x2000) && func_800A0134
        (e, d) < 0x81 && func_8009A540(...)) bail. This body has
        e = func_800A02AC(...); if ((p+0x98 & 0x8000) && e != 0) with the
        func_8009A540 window, the shared reaim block, then a SECOND test on
        func_800A04F0_r plus func_8009A540, and only then b57's 0x20000 block
        -- and that block drops b57's func_800A0134(D_800814A8, d) < 0x81
        guard, exactly as b30 does. Note that in the target the 0x8000-clear
        path and the e == 0 path BOTH jump straight to the loop init, so the
        whole chain down to the func_800A0E6C else sits inside that one gate.

   WHAT DIFFERS, EXACTLY, IN THE PARKED BUILD (aligndiff rows):

       350   target lui   $v1, %hi(D_8006CCD8); built lui   $v0, %hi(...)
       351   target addiu $v1, $v1, %lo(...);   built addiu $v1, $v0, %lo(...)

   There is nothing else: 470 words against 470, every branch offset in step,
   every other register identical. The residue is ONE register in the %hi
   temporary of the loop body's first D_8006CCD8 lookup.

   THE MECHANISM, read out of global.c and local-alloc.c and confirmed by the
   sweep below. Two requirements that this compiler makes mutually exclusive:

   (a) THE LOOP'S HEADING VALUE MUST BE $v1. The target has
       lh $v1, 0x2A($s2) at the loop head and subu/addu $s0, $v1, $v0. A
       separate local for it lands in $a0 and sharing b57's e lands in $s0.
       The reason is global.c's set_preference (global.c:1534): for the insn
       sll $a0, $s0, 16 that casts s for func_8009A66C, SET_DEST is a
       block-local pseudo that local-alloc has already renumbered to $a0, so
       set_preference treats it as a HARD register and gives s's allocno the
       preference 4. expand_preferences (global.c:790) then IORs that
       preference onto any allocno that DIES in the insn that sets s and does
       not conflict with it -- which is exactly the heading value. find_reg
       takes the preference before the plain scan, so the heading gets $a0.
       Verified in a tools/cdkgcc -da dump: the .greg summary for this
       function prints 101 preferences: 4 and 101 in 4, where pseudo 101 is
       the heading local, set once and read in both arms.

   (b) THE DEVICE BASE MUST BE BLOCK-LOCAL. b57's documented device
       t1 = (s32)D_8006CCD8; t1 += kk; is what makes the %lo insn's
       destination have reg_n_sets != 1. cc1psx's movsi expander always puts
       the high in a FRESH pseudo, so the two halves share a register only
       when local-alloc can tie them -- and local-alloc.c:472's gate is
       reg_basic_block >= 0, i.e. the base must be referenced in ONE basic
       block. The moment the base's variable is also the heading value it is
       multi-block, global-alloc places it, and the high temp is left to
       local-alloc, which takes the lowest free register in that block, $v0.

   So the parked body takes (a) by SHARING t1 between the heading value and
   the device base, which gives the heading allocno a live range in the device
   region where $a0 is busy, kills the $a0 option and yields $v1 -- and pays
   (b) with the split lui. Every spelling that pays (b) loses (a), and the
   target does both, which means the target's heading value is a global
   allocno that does NOT inherit s's preference, i.e. one that CONFLICTS with
   s. No plain-C spelling found below produces that conflict without also
   disturbing the pre-loop arms.

   PROOF THAT THE CONFLICT IS THE RIGHT HANDLE, which is the one result here
   worth building on. Merging b57's w into s and the heading into m -- so that
   the heading's other live range is m's in the 0x2000 arm and s's other range
   is w's, which overlap -- gives CONFLICTP(heading, s) and the loop head
   comes out EXACTLY RIGHT, lh $v1 and both subu/addu, with the device's lui
   correct too, at 470 words and 7 aligned differences. All 7 are then in the
   0x2000 arm: m is $v1 there with an extra move $v1, $v0 where the target has
   the call result in $v0 directly, and the s = (s32)(p + 0x98) hoist
   schedules four words early. That build is the closest thing to the target's
   allocation and the next step is a permuter search over the 0x2000 arm with
   that merge in place, not more reading. The merge without the m half is 468
   words (w costs 2 words as an s32), and m merged with a separate w still
   needs the move, so the two halves have to move together.

   EVERY SPELLING MEASURED, with its aligned difference count at 470 words
   unless a word count is given.

   THE HEADING VALUE, with the device left as b57's own t1 local:
       shared with t1, the device base                   2   <- PARKED BODY
       its own fresh local h                             3   lands in $a0
       shared with e (b57's own choice)                  3   lands in $s0
       shared with j                                     3   $a0
       shared with s, one variable for both              3   $s0
       shared with cc2                                   5   $a0, plus 2 rows
       shared with v                                     8   $a0, plus 5 rows
       shared with y                                    10
       shared with kk                                   14
       inlined at both uses                   472 words, 218
       shared with m                          473 words, 224
       shared with x                          469 words, 353
       shared with k                          469 words, 375
       h declared s16                         473 words, 224
       h read as (s16)*(u16 *)(d + 0x2A)                 3
       h read as *(u16 *)(d + 0x2A), u16                 3
       s declared s16 instead of s32                     3
       the two arms written as one ternary               3   (h, e: identical
                                                             to the if/else)
       h also used for the d+0x74 store byte             5   drags that byte
       h also used for the d+0x74 and d+0x7C bytes       7   into $a0 too
       do { h = ...; } while (0) bump         471 words, 11  a sched barrier:
                                                             the lhu 0x98 and
                                                             the lh swap and a
                                                             nop appears
       the bump around the whole if/else      473 words, 180
       the bump around load and if/else       472 words, 170

   THE DEVICE BASE, with the heading shared into t1:
       t1, shared with the heading                       2   <- PARKED BODY
       t1 = (s32)D_8006CCD8 + kk, one statement           7
       t1 = kk; t1 += (s32)D_8006CCD8                     7
       t1 = kk + (s32)D_8006CCD8                          7
       t1 = (s32)D_8006CCD8; *(u8 *)(t1 + kk)             2
       t1 = (s32)&D_8006CCD8[0]; t1 += kk                 2
       t1 = t1 + kk instead of t1 += kk                   2
       the base load hoisted above the kk statement       2
       a fresh s32 t2 for the base                        3   device correct,
                                                             heading to $a0
       a fresh u8 *q pointer for the base                 3   same; a POINTER
                                                             local pays (b)
                                                             just as well as
                                                             b57's s32
       the base in e                                      7
       the base in v                                      5
       the base in cc2                                    5
       the base in x                                      7
       the base in j                                     11
       the base in y                                     12
       inlined, no device                                14   the lui pair
                                                              lands in $v0
                                                              and five more
                                                              rows reorder
       the base in kk                         467 words, 128
       the base in k                          469 words, 375
       the base in m                          474 words, 467
       t1 = *(u8 *)t2 through a second local             13
       both lookups through locals                      18
       t2 for the first, t1 = kk + D_8006CCE8 for the
         second                               469 words, 129
       t1 = kk as a dead carrier, t2 for the base        3

   b57's OTHER FACTS, RE-MEASURED ON THIS BODY. All HELD; none inverted.
     HELD  func_800BCB04 THROUGH THE s16 ALIAS. func_800BCB04h, a halfword
           assignment into i, is what this body needs too -- it is in the
           parked build and the body is 470 words with it.
     HELD  THE cc2 LOCAL WITH THE READ-MODIFY-WRITE IN A do { } while (0) for
           the 0x80000000 set in the 0x400 arm. In the parked build, kept.
     HELD  THE do { return; } while (0) IN THE i == 0 ARM. Kept; the target
           has addiu $v0, $s1, 0x1 in all three branches into the i++ join,
           which is what the wrapper gives.
     HELD  func_800A19E4's 3 and 6, b57's constants, not b30's 2 and 4.
     HELD  t80 = D_80082E80 as a local in the else arm's 0x20000 block, and
           the 0x2000 arm's D_80082E80 inline at all four uses.
     HELD  w = (u16 *)(p + 0x98) as a local in the 0x2000 arm. b57 records it
           as FREE on its body; here it is load-bearing in the sense that
           merging it into s costs 2 words, and it is in the parked build.
     HELD  D_800E2970 indexed as an array of a 0x14-byte record.
     HELD  g2 = (u8 *)&D_80083460 as its own local in the tail, separate from
           g. Kept, as b57 has it.
     HELD  THE g = (u8 *)&D_80083460 POINTER LOCAL ITSELF, and this is the
           one worth saying out loud. docs/dng-parks/func_b43_80171F24.c
           found that spelling INVERTED in this same unit -- 33 aligned
           differences there against 11 for the direct D_80083462 symbol --
           and attributed it to that body having six callee-saved saves where
           its twins have seven. THAT IS A FRAME FACT, NOT A UNIT FACT. This
           body has SEVEN saves and $s0 is the pointer local, exactly as b57
           and b30 have it, and the parked build uses g at all three gate
           reads. The other function landed in this run,
           func_b43_80170AA4, has SIX saves and uses the direct D_80083462
           symbol. So ovl_dng_b43 carries both allocations and the park's
           finding should be written into the levers as predicted by the
           prologue, not by the object.

   DECLARATIONS
   s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
   u8 *func_800A02AC(u8 *p, u16 x, u16 y);
   u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
   s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
   s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
   s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
   extern s16 D_8006CD00[];
   s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
   s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
   void func_8009A3D0(s32 x, s32 y, s32 f);
   void func_8009A21C(s32 a, s32 b, s32 c);
   s32 func_8009A180(u8 *a, u8 *b);
   void func_800A9A0C(u8 *);
   u16 func_800A6D30(void);
   s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
   s32 func_b43_80171F24(u8 *p, UnkVec *b, u8 *c, u8 *d);
   extern Unk80083462 D_80083460;
   extern u32 D_800814A8;
   extern u8 D_80082E80[];
   extern u16 D_80082EA4;
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   typedef struct UnkDngRoom2970
   {
       u8 pad_00[0xC];
       u16 unk_0C;
       u8 pad_0E[0x6];
   } UnkDngRoom2970;
   extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
   END DECLARATIONS */
void func_b43_80171768(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 t1;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b43_80171F24(p, b, c, d) == 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u8 **)(g + 0xC) == d)
        {
            *(u16 *)(d + 0x46) = 0xC008;
        }
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)c, d, 3, 6, p + 0x9C);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            e = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if (e != 0)
            {
                t = *(u8 **)(e - 0x14);
                *(s16 *)(d + 0x2A) =
                    func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                d[0x71] &= 0x7F;
                return;
            }
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
        else
        {
            if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL)
            {
                d[0x71] &= 0x7F;
                return;
            }
        }
    }
    else if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (*(u8 *)(c + 0x24) == (u16)x && *(u8 *)(c + 0x25) == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                w = (u16 *)(p + 0x98);
                m = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), x, y, w);
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) =
                        func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25], w);
                }
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    else
    {
        j = *(s8 *)(c + 0x26);
        if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
        else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            e = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if ((*(u16 *)(p + 0x98) & 0x8000) != 0 && e != 0)
            {
                if (func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                {
                    t = *(u8 **)(e - 0x14);
                    *(s16 *)(d + 0x2A) =
                        func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                    d[0x71] &= 0x7F;
                    return;
                }
                if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL &&
                    func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                if ((*(s32 *)(d + 0x1C) & 0x20000) != 0)
                {
                    t80 = D_80082E80;
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25],
                                                       (u16 *)(p + 0x98));
                    if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25]) != 0)
                    {
                        if (func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24),
                                          *(u8 *)(c + 0x25), (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                        {
                            d[0x71] &= 0x7F;
                            return;
                        }
                    }
                }
                else
                {
                    func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
                }
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        t1 = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = t1 - tbl[i];
        }
        else
        {
            s = t1 + tbl[i];
        }
        if ((s16)func_8009A66C((s16)s, (u8 *)c, d, 0x20) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = *(u8 *)(c + 0x24);
            *(d + (d[0x71] & 0x7F) + 0x7C) = *(u8 *)(c + 0x25);
            d[0x71]++;
            func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            *(u8 *)(c + 0x24) += *(u8 *)t1;
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                do
                {
                    return;
                } while (0);
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
        return;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    p[0x9C] = *(u8 *)(c + 0x26);
    d[0x6D]--;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(d + 0x6D) == 0)
    {
        d[0x71] &= 0x7F;
        return;
    }
    i = func_800BCB04h((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20,
                       (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}
