#include "common.h"

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b24_801723F8(u8 *p, void *b, void *c, u8 *d);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b24_80171A80();

s32 func_800AB778();

s32 func_800AD058();

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
s32 func_800AB464();

void func_80047784(u8 *p, s32 b, s32 c);
extern s16 D_80083228;
void func_80047784(u8 *, s32, s32);
void func_b24_80172B4C(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_800E2450[];

s32 func_800AB538();

extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800E2408[];
void func_800A2B04(UnkVec *, s32, s32);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD4D0(u8 *);

extern u8 D_80082E80[];
extern u8 D_800E2428[];
extern u8 D_800E2430[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s32 func_8009A180(u8 *, u8 *);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800A2C34(u8 *);
u16 func_800A6D30(void);
void func_800A9A04(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA79C(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_b24_80174A68(u8 *p, u8 *b, u8 *c, u8 *d);
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_800A0134(u8 *a, u8 *b);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_800BCB04(s32, s32, s16);
s32 func_b24_80172BB0(u8 *p, void *b, void *c, u8 *d);
extern s16 D_8006CD00[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
UnkNode *func_8003FD64(s32, void *);
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b24_801708B8(u8 *, UnkVec *, u8 *);
void func_b24_801711B0();
extern u8 D_80045340[];
extern u8 D_80083498[];
extern u8 D_800E23E0[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_801708B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80170A84);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80170CEC);

/* Spawn the kind-0x112 node this bank's scripts use for a thrown or placed
   actor: arm it with the bank-local func_b24_801711B0 handler and the
   D_800E23E0 facing table, dress its slot from the b/c/d arguments, pick one
   of three flag pairs off `a & 3', and hand it to func_800A9C18,
   func_800AA36C and this unit's func_b24_801708B8.  133 words, 0 differing.
   No jump table and no rodata of its own -- the listing opens at `glabel'.
   1 copy (dngfam.py).

   The twin is the landed 120-word func_b16_80170878 (csrc/ovl_dng_b16.c),
   itself a relink of the 113-word func_b03_80170854; read b03's body for the
   shape.  Four relink substitutions -- `p[0x13]' is 0x18, the handler and the
   two tables are func_b24_801711B0 / D_800E23E0 / D_800E2428, and the
   func_800A6D30 arm does not set `*(u32 *)(p + 0x1C) |= 0x200' -- and then
   the 13 words `dngtwin' reports are ONE added tail block, after
   func_800AA36C: clear w + 0xB5, save the slot's 0x1C / 0x1E halfwords into
   locals, zero both, call func_b24_801708B8(w, m, r) with only three
   arguments set, and put the saved pair back.  func_b24_801708B8 is still an
   INCLUDE_ASM sibling in this unit, so its 3-parameter declaration is read
   off this call site.

   THE ONE SPELLING FACT, AND IT IS AN INVERSION OF THE TWIN'S ORDER.  b16 and
   b03 both write the slot's two position bytes as `r + 0x25 = c' BEFORE the
   facing-table store and `r + 0x24 = b' AFTER `k = a & 3'.  Spelled that way
   here the body is the right length with the two `sb's TRANSPOSED across the
   `bne' -- reorg puts whichever store is last into the delay slot, and the
   target wants c's there, so b's store has to come first.  But simply
   exchanging the two statements swaps $s6 and $s7 between b and c as well,
   because with b stored first the two parameters' allocnos tie exactly --
   a `cdkgcc -da' probe's `.lreg' reads `Register 82 used 2 times across
   23 insns' and
   `Register 84 used 2 times across 23 insns', both 2 bytes, so
   allocno_compare falls through to its `v1 - v2' tie-break and the
   lower-numbered pseudo (b) takes $s6.  Hoisting the facing-table store ABOVE
   both breaks the tie the other way and costs nothing:

   | spelling                                        | words | differing |
   | as written below, table store first              |  133  |     0     |
   | b16's order: c, table, k, b                     |  133  |     2     |
   | b16's order with b and c exchanged              |  133  |     6     |

   The 2 differences are the transposed `sb's alone; the 6 are those plus the
   $s6 / $s7 exchange in the prologue saves and both stores.  Three other
   placements also measure 0 -- b,c,table,k and table,b,c,k and
   table,k,b,c -- so what the target pins down is only that b's store
   precedes c's and the table store does not sit between them.

   ALSO MEASURED: b16's separate `w' local is load-bearing, 130 words / 123
   differing with `p' at every use.  The x / y pair's type is FREE: `s16 x' /
   `s16 y' is 133 words / 0 differing, so it is NOT the reason this lands. */
u8 *func_b24_80170F9C(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    s32 k;
    s32 x;
    s32 y;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b24_801711B0;
        p[0x13] = 0x18;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *(u8 **)((u8 *)r + 0x2C) = D_800E23E0;
        *((u8 *)r + 0x24) = b;
        k = a & 3;
        *((u8 *)r + 0x25) = c;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_800E2428;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b24_80171A80;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0x92) = -0x20;
        func_800AA36C(w, m, (u8 *)r, p);
        w[0xB5] = 0;
        x = *(u16 *)((u8 *)r + 0x1C);
        y = *(u16 *)((u8 *)r + 0x1E);
        *(s16 *)((u8 *)r + 0x1C) = 0;
        *(s16 *)((u8 *)r + 0x1E) = 0;
        func_b24_801708B8(w, m, (u8 *)r);
        *(s16 *)((u8 *)r + 0x1C) = x;
        *(s16 *)((u8 *)r + 0x1E) = y;
    }
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_801711B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_801717D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80171A80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80172144);

/* The banked room script's walk step, the 469-word relink of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c).  469 words,
   0 differing, on the first compile.  No jump table and no rodata of its own
   -- the listing opens at `glabel'.  1 copy (dngfam.py).  The second
   parameter is never read and still has to be declared and passed on: the
   func_b24_80172BB0 call sets only $a0, $a2 and $a3, so its $a1 is this
   function's own incoming $a1 reaching it untouched.

   THIS IS A THIRD SOURCE, NOT EITHER LANDED RELINK.  `dngtwin' offers the
   456-word func_b25_801726EC at 0.971 and the 466-word func_b57_801726C8 at
   0.956; this body takes b25's LOOP SHAPE and b01's FRAME, and adds two
   source deltas of its own.  Read func_b01_80171C34's comment
   (csrc/ovl_dng_b01.c) for the three extra pre-loop arms, the `e'-sharing of
   the loop's heading value and the D_800E2970 0x14-byte indexing: all of
   that holds here verbatim and is not repeated.

   THE TWO DELTAS, AND THEY ACCOUNT FOR ALL 13 WORDS OVER b25:

   1. A THIRD ARM IN THE OPENING GATE, `p[0xB5] != 0', between the
      `d[0x12] >= 2' test and the func_b24_80172BB0 call, reaching the same
      func_800A9A0C refusal.  3 words: `lbu $v0, 0xB5($s5)', its load delay
      and the `bnez'.

   2. A func_80042900 TEST INSIDE THE MATCHED ARM, reusing `s' now that the
      sweep value has been stored: `s = func_80042900((s8 *)d, 0x1B)' right
      after the d + 0x71 bump, and BOTH engine-notify calls -- func_8009A3D0
      before the step and func_8009A21C after it -- guarded by
      `(s16)s == 0'.  10 words, and it is why $s0 is loaded twice with
      `sll $v0, $s0, 16'.

   WHICH OF THE TWO TWINS' STATED FACTS HELD, PRICED BOTH WAYS ON THIS BODY:

   | spelling                                        | words | differing |
   | as written below                                 |  469  |     0     |
   | `s16 i' instead of `s32 i'       (b25 fact 1)    |  481  |   253     |
   | b25's `tb' hoisted before the loop (b25 fact 3)  |  471  |   461     |
   | b01's `t1 += kk' dropped, lookup inline          |  469  |    12     |
   | same, written `(s32)D_8006CCD8 + kk'             |  469  |    12     |
   | plain `|= 0x80000000', no cc2 do-while(0)        |  469  |     4     |
   | b01's `do { return; } while (0)' KEPT            |  472  |    93     |
   | the 0x20000 arm's D_80082E80 through a local      |  468  |   238     |

   So b25's s32 `i' HELD, b01's cc2 do-while(0) reference bump HELD at the
   same 4-difference price b01 and b25 both measure for it, b01's
   `do { return; } while (0)' is REMOVED exactly as b25 removes it, and b01's
   `t1 = (s32)D_8006CCD8; t1 += kk' device HELD -- which means b25's own
   fact 3, the `tb' hoisted into the preheader, is INVERTED here.  That is
   the frame talking: this copy saves s0..s6 at sp -0x38, b01's frame, with no
   $s7 to carry a loop-invariant table pointer, where b25 saves $s7 too at
   sp -0x40.

   AND TWO OF THE TWINS' NAMED FACTS ARE FREE HERE, so do not spend a budget
   preserving them: b25's fact 2, `D_8006CD00[i]' named inline with no
   `s16 *tbl' local, measures 469 words / 0 differing EITHER WAY, as does its
   fact 4, `(s16)func_800BCB04(...)' against b01's s16-returning
   func_800BCB04h alias.  The `t80' local in the else branch's 0x20000 arm is
   free too -- 0 differing with D_80082E80 named inline at all four sites --
   and is kept only because b01 has it. */
void func_b24_801723F8(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 f;
    s32 i;
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
        if (d[0x12] >= 2 || p[0xB5] != 0 || (s16)func_b24_80172BB0(p, b, c, d) == 0)
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
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
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
                if ((s16)func_8009A8C0(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25], w);
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
            e = (s32)func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A));
            if (e != 0 && (*(s32 *)(e + 0x1C) & 0x2000) != 0 && func_800A0134((u8 *)e, d) < 0x81 &&
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
                    if (func_800A0134((u8 *)D_800814A8, d) < 0x81 &&
                        func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                      (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
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
    i = 0;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - D_8006CD00[i];
        }
        else
        {
            s = e + D_8006CD00[i];
        }
        if ((s16)func_8009A8C0((s16)s, (u8 *)c, d, 0x20) > 0)
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
            s = func_80042900((s8 *)d, 0x1B);
            if ((s16)s == 0)
            {
                func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            }
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            *(u8 *)(c + 0x24) += *(u8 *)t1;
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            if ((s16)s == 0)
            {
                func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            }
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
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
    i = (s16)func_800BCB04((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}

/* State 0x10, and hand c the facing cell that the heading at d+0x2A selects
   out of the bank-local byte table D_800E2450: the relinked twin of the
   resident overlay's func_800D9820 in csrc/ovl_dungeon_800D50FC.c.  The
   relink is the one the rest of this block shows -- a BYTE table pushed
   through func_80047784 (`>> 9 & 7', one `lbu') where the resident has a
   POINTER table pushed through func_8003DB94 (`>> 7 & 0x1C', a `lw').  The
   base is a local because it is used twice, stored at c+0x2C and indexed,
   exactly as in the twin.  25 words, 0 differing, first compile.
   24 copies (dngfam.py).

   THE ONE FACT THAT MATTERS IS STATEMENT ORDER, not the table spelling:
   the two p stores have to come before the c+0x2C store, because 0x10 is
   materialised into $v0 and spent on p+0x9A before the table address is ever
   loaded.  Writing them after the table store is 24 words / 17 differing.
   Hoisting the d+0x2A halfword into an `s16' local is 26 words / 19 -- the
   reload is re-sign-extended instead of feeding the `addu' straight.

   Measured and rejected, all 0 differing at 25 words, so the form kept here
   is the twin's for the replicator's sake:
     no local, the symbol written out twice                         0
     `*(t + idx)' instead of `t[idx]'                               0
     the local assigned inside the store, `= t = D_800E2450'        0
     the index hoisted into an `s32' local                          0
     reading the base back out of c+0x2C after storing it           0
   Measured and rejected, differing:
     the two p stores moved after the table store    24 words,  17
     the d+0x2A facing hoisted into an `s16' local   26 words,  19

   `--raw' is 6 differing and every one is a relocation -- the `jal', the
   `%hi'/`%lo' pair for D_800E2450 and the pair for D_80083228 -- so no
   masked word is hiding a constant address. */
void func_b24_80172B4C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_800E2450;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80172BB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80172D88);

/* THE PARAMETER COPY HAS TO LEAVE THE PROLOGUE'S UNSCHEDULABLE PREFIX. The
   target saves and copies its four parameters in the order $s1, $s2, $s3,
   $s0 -- parameter 0 LAST -- and the leading run of (set pseudo hard_reg)
   insns is pinned by sched.c and emitted in LUID order, so no spelling
   inside those four copies can reorder them. A plain `p = a;` in the body
   is the cure: combine folds parameter 0's prologue copy into the body copy,
   the surviving move is a user insn inside sched's region, and sched2's
   entry block (all priority 1, ranked on INSN_LUID) therefore puts it after
   the other three. Used directly, with no local, the function is 8
   differing; the same copy made of parameter 1 instead is also 8.

   Two other measured facts. `if ((s16)d != 0)` is the halfword test the
   target's `sll $v0, $s3, 16` / `bnez` is: a plain `if (d != 0)` is 80 words
   and 59 differing. The case arms are in the target's own order -- case 0,
   case 2, then case 1 falling into `default` -- and putting case 2 first is
   79 words and 47 differing. `s32 r` with `(s16)` casts at its three uses
   also measures 0; `s16 r` is kept because it needs none of them.
   81 words, 0 differing. */
s32 func_b24_80172EA8(u8 *a, void *b, void *c, s32 d)
{
    u8 *p;
    s16 r;

    p = a;
    r = func_800ADDA0(b, c, p, 3, 6, (s32)(p + 0x9C));
    if (r < 0)
    {
        return 0;
    }
    if ((s16)d != 0)
    {
        func_b24_801723F8(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b24_801723F8(p, b, c, p);
        return 0;
    case 1:
        p[0x71] &= 0x7F;
        if ((s16)func_800A2BDC(p) != 0)
        {
            *(u16 *)(p + 0x46) &= 0x7FFF;
            return 0;
        }
    default:
        p[0x71] &= 0x7F;
        if ((D_80083462 & 8) == 0)
        {
            return 1;
        }
        *(u16 *)(p + 0x46) &= 0x7FFF;
        return 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80172FEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_801730C4);

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b24_801734AC(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b24_80171A80;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80173510);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80173A60);

/* The banked room script's three-state glide, switching on p + 0x9B:
   state 0 seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair,
   state 1 decays it, state 2 steers onto the tile centre and installs
   func_b24_80171A80. The resident overlay's func_800D9C7C
   (csrc/ovl_dungeon_800D50FC.c) is the same machine -- the same three cases
   FALLING THROUGH into one another, the same `stop:' label inside case 1
   that case 0's `else' reaches by `goto', the same func_800AAA54 call -- and
   func_800D991C in the same unit supplies the divide form of case 2. Both
   of those comments carry the measured spelling facts this body relies on:
   the three-statement numerator `u = c[0x24] << 6; w = b2 - 0x20; u -= w;'
   (nine other spellings 1 to 4 differing on func_800D991C), the second
   numerator left two-statement (1 word and 145 differing when split), and
   `if (x > 0) x--;' in case 1 against `if (--x > 0)' in case 2, which
   func_800D9C7C measures as not interchangeable.

   Four deltas from func_800D9C7C, all read off the listing: the
   D_8006CCD8 index is NOT a local here -- state 0 recomputes
   `(*(u16 *)(d + 0x6A) >> 9) & 7' for each of the two tables and state 1
   never uses it, so there is no $s4 to hold it; the seeds are negated and
   scaled by 15 (`negu' then `sll 15') rather than shifted by 18; state 1
   decays by a quarter (`b->unk_0C -= b->unk_0C / 4', the
   `bgez'/`addiu 3'/`sra 2' triple) where func_800D9C7C subtracts
   `D_8006CCD8[k] << 14'; and func_800AAA54's fourth argument is the
   bank-local D_800E2408 rather than 0. 181 words, 0 differing, on the first
   compile. */
void func_b24_80174064(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = -D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = -D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        p[0x9B] += 1;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] = 2;
                return;
            }
            *(s16 *)(p + 0x96) = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (*(s16 *)(p + 0x96) > 0)
        {
            *(s16 *)(p + 0x96) -= 1;
        }
        else if (*(u16 *)(c + 0x14) & 0x6000)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_800E2408);
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            p[0x9B] += 1;
        }
        return;
    case 2:
        if (*(s16 *)(p + 0x96) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 15) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 15) / *(s16 *)(p + 0x96);
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b24_80171A80;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b24_80174338(void)
{
    func_800AD058();
}

/* A one-call forwarder with a tail store: func_800AB538 takes this
   function's own incoming $a0..$a3 untouched and p + 0xA2 is cleared after
   it returns. The empty-paren `s32 func_800AB538();' declaration already in
   this unit is what allows the argument-free call -- func_800AB538's landed
   definition is K&R with four parameters
   (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in the project
   disagrees with it. 11 words, 0 differing, on the first compile. */
void func_b24_80174358(u8 *p)
{
    func_800AB538();
    *(s16 *)(p + 0xA2) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b24_80174384(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b24_801743A4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b24_80171A80;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the bank-local D_800E2428 facing table, state 1 is the long
   refusal chain, state 2 hands the actor back to func_b24_80171A80.
   235 words, 0 differing, on the first compile.  No jump table and no rodata
   of its own -- the listing opens at `glabel'.

   THE TWIN IS THE LANDED func_b57_80174954 (csrc/ovl_dng_b57.c) AND THE
   WHOLE DELTA IS A DELETION: state 1 here does NOT open with that body's
   `if ((s16)func_80042900(d, 1) == 0) { dress; flag; gate; bump; return; }'
   arm -- it drops straight into the `&D_80083460 + 0x2' 0x1000 gate.  That
   missing arm is exactly the 6 words `dngtwin' reports (241 against 235):
   the twin writes the dress/flag/bump tail out TWICE and gcc cross-jumps the
   copies into one block, so deleting the first copy costs only its test --
   `jal func_80042900', the `addiu $a1, $zero, 0x1', the `sll 16', the
   `beqz' and their two fills.  The rest is three relink substitutions:
   D_800E2428 / D_800E2430 for D_80175C90 / D_80175CB0, func_b24_80174A68 for
   func_b57_80175078 in the 0x80000 arm, and func_b24_80171A80 for
   func_b57_80171FA4 at both p + 0x8C stores.  Every one of the twin's three
   stated facts HELD, re-measured here with both spellings priced:

   | spelling                                        | words | differing |
   | as written below                                 |  235  |     0     |
   | one shared `&D_80083460' local, not h / k2 / m   |  231  |   181     |
   | the two tail bumps in the other order           |  232  |   183     |
   | state 1's facing table through a `u8 *' local    |  235  |     5     |

   The tail's flag update is `*(s32 *)(d + 0x1C) |= 0x40000' sitting BEFORE
   the c + 0x14 0x8000 gate, unconditionally, and state 2 changes no flag word
   at all -- both as in the twin.  1 copy (dngfam.py). */
void func_b24_801743DC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k2;
    u8 *m;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E2428;
        func_80047784(c, D_800E2428[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if (d[0x25] == 0)
        {
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            func_800AA79C(p, b, c, d);
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b24_80174A68(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            e = D_80082E80;
            if (*(s8 *)(c + 0x26) != *(s8 *)(e + 0x26) || *(s8 *)(c + 0x26) < 0)
            {
                if ((s16)func_8009FD40(e, c) >= 2)
                {
                    goto again;
                }
            }
            if ((func_800A6D30() & 7) == 0)
            {
                func_80042B68((s8 *)d, 1);
            }
        }
    again:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E2430;
        func_80047784(c, D_800E2430[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b24_80171A80;
            return;
        }
        p[0x9B] += 1;
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b24_80171A80;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80174788);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80174A68);

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b24_80174B58(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b24_80171A80;
        }
    }
}

/* 14 words, 0 differing. */
void func_b24_80174BBC(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b24_80171A80;
    }
}

/* 14 words, 0 differing. */
void func_b24_80174BF4(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b24_80171A80;
    }
}

/* 14 words, 0 differing. */
void func_b24_80174C2C(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b24_80171A80;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80174C64);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_801754AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_8017558C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80175BA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80175F44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b24", func_b24_80176084);

/* 14 words, 0 differing. */
void func_b24_80176304(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b24_80171A80;
    }
}

/* 14 words, 0 differing. */
void func_b24_8017633C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b24_80171A80;
    }
}
