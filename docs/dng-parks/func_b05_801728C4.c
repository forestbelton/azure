/* PARK: func_b05_801728C4, csrc/ovl_dng_b05.c, agent-h202, 2026-11-25.

   TARGET 509 words. BEST BUILT 509 words, 6 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports the same 6 masked.
   1 member, 509 words (tools/decomp/dngfam.py) -- no replication value.

   VERDICT: ALLOC, and it is ONE REGISTER. Every opcode, immediate and
   displacement matches. The whole residue is the local `q' (the slot pointer
   read out of `*(u8 **)(X - 0x14)'): the target holds it in $a0 and this body
   holds it in $v1. The six rows are words 91, 105, 107, 109, 111 and 114 --
   the two `lw -0x14' sites, the NULL test, and the two `lbu 0x24/0x25' reads
   of the shared tail.

   IT OWNS A JUMP TABLE AND THE TABLE IS SETTLED.
   jtbl_b05_80170858 sits at 0x80170858, which is 0 MOD 8, so `.align 3' is
   satisfied and a C `switch' can produce it. 7 words = 7 cases + 0 pad
   (tools/decomp/jtcheck.py), dispatch bound 7, 7 distinct targets.
   MINVAL IS 1: the listing has `addiu $v1, $v0, -0x1' before the
   `sltiu $v0, $v1, 0x7', so entry k is case k+1. Entries:
     1 -> g = d + 0x8    2 -> g = d + 0xB    3 -> g = d + 0xE
     4 -> .Lb05_801729F4, which is ALSO the range check's `beqz' target, so
          case 4 is NOT written and `default:' IS REAL (the beqz
          discriminator: the out-of-range target IS a table entry).
     5 -> k = 1 then d + 0x8   6 -> k = 1 then d + 0xB
     7 -> k = 1 then d + 0xE
   ARM ORDER WAS MEASURED, not assumed: the fall-through order written below
   (7/3, 6/2, 5/1, default) reproduces the listing's block layout exactly --
   the three `k = 1' stubs at 0x8017298C/94/9C, then the `else' compare chain,
   then the three shared bodies at 0x801729DC/E4/EC and the default at
   0x801729F4. The `else' arm (the `(d + 0x1C) & 0x2000' == 0 side) is a
   three-case compare chain whose arms jump INTO the table arms' blocks;
   jump2 cross-joins them, so the unit gets ONE new table and not two.

   THE DONOR is the landed func_b42_801730E4 (csrc/ovl_dng_b42.c, 403 words,
   dngcallee.py 17/21 symbols with a clean gap at 16/21 -- dngtwin found
   NOTHING at 0.70 and xsibs nothing at all). Its whole state-0 slot-pick,
   its D_8006DE24 / func_800A05A4 / __builtin_abs block and its teardown are
   this body's verbatim. Deltas: state 0 cues func_800DA840 on an 8-byte
   stack record and re-arms +0x96 at 0x7530 instead of 0xA, state 1 has no
   spawn, state 2 is a whole different block (func_8003DE58 projection, one
   0x112 node, an 8-node particle burst, a D_80174850 re-dress) and state 3
   re-dresses from D_80174820.

   MEASURED AND LOAD-BEARING (all from this body, 509 words unless stated):

   | spelling                                                    | words | aligned |
   | as written below                                            |  509  |    6    |
   | `*(u16 *)(c + 0x14) |= 0x800' AFTER the two p-stores         |  509  |   16    |
   | ... and `p + 0x96 = 6' before `p + 0x98 |= 0x80' (ascending) |  508  |   22    |
   | `p + 0x96 = 6' moved ABOVE the c + 0x14 store               |  508  |   25    |
   | the three `k != 0' arms with `i = g[0]' as a local           |  505  |  143    |
   | one `e' local for BOTH the D_800814A8 value and &tbl[g[0]]   |  509  |    8    |
   | `e' kept in the k != 0 arm (b42's spelling)                  |  509  |    8    |
   | `e3' separate from `q' in the e[0x12]==2 arm                 |  509  |    8    |
   | `s32 k' instead of `s16 k'                                   |  508  |   14    |
   | `if (k == 1)' instead of `if (k != 0)'                       |  509  |   10    |
   | the `k != 0' arms EXCHANGED (`if (k == 0)' first)            |  510  |   34    |
   | the three `vec' stores moved ABOVE the `if (k != 0)'         |  512  |   27    |
   | `x'/`y' reused as one local for __builtin_abs               |  515  |   32    |
   | __builtin_abs written as two `if (v < 0) v = -v'            |  508  |   20    |
   | `tbl = D_8006DE24' dropped (inline &D_8006DE24[g[0]])        |  506  |   38    |
   | the NULL-test pointer inlined twice instead of in `q'        |  506  |   14    |
   | `j = e2[0x12]' hoisted above the d + 0x60 read               |  508  |   15    |

   THE ONE THAT MATTERS, and it is a cse-aliasing fact worth reusing: the
   STORE TO p + 0x96 MUST COME AFTER the `p + 0x98 |= 0x80', because that
   store is what stops cse carrying p + 0x98's value past the
   `(c + 0x14 & 0x8000)' gate. With 0x96 written first, cse knows
   p + 0x98 == orig | 0x80 and combine folds the second `|= 0x8000' into one
   `ori 0x8080' -- 509 words, 16 aligned differences, and the target's
   re-read `lhu $v0, 0x98($s5)' disappears. Same base register, different
   constant offsets, and gcc 2.7.2's cse invalidates anyway.

   WHAT WAS TRIED FOR THE REMAINING SIX AND DID NOT MOVE THEM (all 509 / 6):
   `q' and the else arm's pointer as separate locals; `q' declared first,
   last, and in between; `*(u8 *)(d + 0x72) = *(u8 *)(q + 0x24)' in place of
   `d[0x72] = q[0x24]' at both sites (the MEM_IN_STRUCT_P axis); `q != 0' for
   `q != NULL'; `(u8 *)D_800814A8 - 0x14' for `D_800814A8 - 0x14'; the
   d + 0x72/0x73 tail written ONCE and reached by `goto' from the k != 0 arm
   instead of duplicated; `__builtin_abs' inlined on the memory reads.
   Flat under order, set and cast, so by the brief's own ladder this is a
   DEPENDENCE/priority question rather than a spelling one.

   THE MECHANISM, as far as reading gcc 2.7.2 gets it: `q' spans blocks, so
   global_alloc assigns it. MIPS defines no REG_ALLOC_ORDER, so find_reg
   scans 0,1,2,... and $v1 (3) comes before $a0 (4); both are in
   `regs_used_so_far' from the start because global.c:352 seeds that set with
   every call_used_reg. So the target's $a0 means $v1 is in `used1' for this
   allocno -- either hard_reg_conflicts (a local_alloc pseudo in $v1 live
   where q is live) or regs_someone_prefers. Nothing in the target's listing
   holds $v1 across q's range in any of q's three blocks, so the likeliest
   remaining cause is a COPY PREFERENCE for $a0 that find_reg's
   hard_reg_copy_preferences post-pass applies on top of the scan result.
   A permuter run is the cheap next step; this is one register on an
   otherwise byte-exact 509-word body.

   DECLARATIONS
   s32 func_80042900(s8 *, s8);
   void func_80042B68(s8 *, s8);
   s16 func_8009FD40(u8 *, u8 *);
   void func_8009A028(u8 *);
   void func_800A2FE0(u8 *);
   void func_800A32A4(u8 *);
   void func_800ACF88(u8 *);
   void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
   s32 func_8004491C(UnkNode *, u32);
   void func_800DA840(u8 *, s16);
   s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
   s32 func_800A05A4(u8 *, s32, s32, s16, s32);
   u8 *func_8003DE58(u8 *, u8 *, void *, s16);
   s32 func_8003F270(void);
   extern u8 D_80174850[];
   extern s16 D_8008346A;
   s32 rand(void);
   END DECLARATIONS
*/
void func_b05_801728C4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    UnkNode *n;
    SVECTOR vec;
    SVECTOR out;
    u8 *g;
    u8 *e;
    u8 *e2;
    u8 *e3;
    u8 *q;
    u8 *r;
    u8 *w;
    u8 *h;
    s16 k;
    s32 i;
    s32 x;
    s32 y;

    k = 0;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 7:
                k = 1;
            case 3:
                g = d + 0xE;
                break;
            case 6:
                k = 1;
            case 2:
                g = d + 0xB;
                break;
            case 5:
                k = 1;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        else
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 3:
                g = d + 0xE;
                break;
            case 2:
                g = d + 0xB;
                break;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        if (g[0] != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFF7F;
            if (k != 0)
            {
                *(s32 *)(d + 0x60) = D_800814A8;
                q = *(u8 **)(D_800814A8 - 0x14);
                d[0x72] = q[0x24];
                d[0x73] = q[0x25];
            }
            else
            {
                tbl = D_8006DE24;
                e2 = (u8 *)&tbl[g[0]];
                if (e2[0x12] == 2)
                {
                    q = *(u8 **)(d + 0x60);
                    if (q != NULL)
                    {
                        q = *(u8 **)(q - 0x14);
                        d[0x72] = q[0x24];
                        d[0x73] = q[0x25];
                    }
                }
                else
                {
                    *(s32 *)(d + 0x60) =
                        func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                    x = *(s8 *)(d + 0x72);
                    y = *(s8 *)(d + 0x73);
                    d[0x72] = __builtin_abs(x);
                    d[0x73] = __builtin_abs(y);
                }
            }
            vec.vx = *(u16 *)((u8 *)b + 0x2);
            vec.vy = *(u16 *)((u8 *)b + 0x6);
            vec.vz = *(u16 *)((u8 *)b + 0xA);
            if (func_800A94A0(d, g, k, p + 0x98) != 0)
            {
                *(u16 *)(c + 0x14) &= 0xF7FF;
                func_800A56E0(0x703);
                func_800DA840((u8 *)&vec, (g[0] - 1) % 3);
                *(s16 *)(p + 0x96) = 0x7530;
                p[0x9B]++;
            }
        }
        else
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(s32 *)(p + 0x8C) = (s32)func_b05_80170E84;
            d[0x73] = 0;
            d[0x72] = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        break;
    case 1:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B]++;
    case 2:
        if (func_8003DE58(*(u8 **)(c + 0x8), c, &out, 0) == NULL)
        {
            out.vx = out.vy = out.vz = 0;
        }
        if ((*(s8 *)(c + 0x4) == 3 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(u16 *)(p + 0x98) |= 0x80;
            *(s16 *)(p + 0x96) = 6;
            if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
            {
                *(u16 *)(p + 0x98) |= 0x8000;
                n = func_8003FD64(0x112, D_80083498);
                if (n != NULL)
                {
                    func_8004491C(n, (u32)D_80045340);
                    w = (u8 *)n + 0x20;
                    n->unk_10 = (u32)func_b05_801740FC;
                    *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u16)out.vx;
                    *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u16)out.vy;
                    *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)b + 0xA) + (u16)out.vz;
                    *(s32 *)(w + 0xA4) = 0;
                    *(s16 *)(w + 0x96) = 4;
                    r = (u8 *)n->unk_0C;
                    *(s32 *)(r + 0x28) = *(s32 *)(c + 0x28);
                    *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x1000;
                    *(u16 *)(r + 0x14) = *(u16 *)(c + 0x14);
                    *(s16 *)(r + 0x12) = *(u16 *)(c + 0x12);
                    *(s32 *)(r + 0xC) = *(s32 *)(c + 0xC);
                    func_80047784(r, 0x2D, 0);
                }
            }
        }
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(u16 *)(p + 0x98) &= 0x7FFF;
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if ((*(u16 *)(p + 0x98) & 0x8000) != 0)
        {
            for (i = 7; i >= 0; i--)
            {
                n = func_8003FD64(0x112, D_80083498);
                if (n != NULL)
                {
                    func_8004491C(n, (u32)D_80045340);
                    n->unk_10 = (u32)func_b05_801740FC;
                    *(s16 *)((u8 *)n->unk_08 + 0x2) =
                        *(u16 *)((u8 *)b + 0x2) + (u16)out.vx + (rand() & 0xF) - 8;
                    *(s16 *)((u8 *)n->unk_08 + 0x6) =
                        *(u16 *)((u8 *)b + 0x6) + (u16)out.vy + (rand() & 0xF) - 8;
                    *(s16 *)((u8 *)n->unk_08 + 0xA) =
                        *(u16 *)((u8 *)b + 0xA) + (u16)out.vz + (rand() & 0xF) - 8;
                    *(s32 *)((u8 *)n->unk_08 + 0xC) = ((rand() & 0xFF) - 0x80) << 11;
                    *(s32 *)((u8 *)n->unk_08 + 0x10) = ((rand() & 0xFF) - 0x80) << 11;
                    *(s32 *)((u8 *)n->unk_08 + 0x14) = -(rand() & 0xFF) << 12;
                    w = (u8 *)n + 0x20;
                    *(s32 *)(w + 0xA4) = 0;
                    *(s16 *)(w + 0x96) = 7;
                    r = (u8 *)n->unk_0C;
                    *(s32 *)(r + 0x28) = *(s32 *)(c + 0x28);
                    *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x1000;
                    *(u16 *)(r + 0x14) = *(u16 *)(c + 0x14);
                    *(s16 *)(r + 0x12) = *(u16 *)(c + 0x12) - 0x80;
                    *(s16 *)(r + 0x10) = 0x60;
                    *(s32 *)(r + 0xC) = 0x808080;
                    *(u16 *)(r + 0x14) |= 0xC;
                    func_80047784(r, 0x2F, 0);
                }
            }
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(u8 **)(c + 0x2C) = D_80174850;
        func_80047784(c, D_80174850[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B]++;
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        e = D_80174820;
        if (*(u8 **)(c + 0x2C) != e)
        {
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        h = (u8 *)&D_80083460;
        if (*(s32 *)(h + 0xC) != 0)
        {
            break;
        }
        *(u16 *)(h + 0xA) -= 1;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s32 *)(p + 0x8C) = (s32)func_b05_80170E84;
        func_800A4ACC(d);
        if (*(s8 *)(d + 0x6D) > 0)
        {
            d[0x6D] -= 1;
        }
        d[0x73] = 0;
        d[0x72] = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A56E0(0xB4);
        break;
    }
}
