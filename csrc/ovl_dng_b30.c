#include "common.h"

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b30_80171DCC(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
extern void (*D_80174198[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
extern u16 D_80083462;

extern Unk80083462 D_80083460;
extern u8 D_80176660[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_801765D0[];
extern u8 D_80176650[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC7EC();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b30_80170EE4(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

s32 func_8009A180(u8 *a, u8 *b);
void func_800A9A0C(u8 *);
extern u8 D_80176668[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b30_801737B8(u8 *p, u8 *b, u8 *c, u8 *d);

u16 func_800A6D30(void);
extern u8 D_80082E80[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);

s32 func_800AB464();

s32 func_800AB1C0(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);
typedef struct UnkDng708A4SP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng708A4SP;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;

typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_b30_80175594(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

extern u8 D_801765E8[];

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b30_80171E38(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
void func_b30_80171460(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b30_801716A4(u8 *p, UnkVec *b, void *c, u8 *d);
s32 func_b30_80172114(u8 *a, void *b, void *c, s32 d);
void func_b30_80171FFC(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b30_80173C40(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_800453E0(u8 *, UnkVec *, UnkSlot *, s16);
extern u8 D_80176678[];
extern u8 D_801765D8[];
extern u8 D_801765E0[];

extern u8 D_801765F0[];
extern u8 D_801765F8[];
void func_800A2B04(UnkVec *, s32, s32);
s32 func_800A2BDC(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
extern void (*D_80176680[])(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_8006CCF8[];
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800478B8(u8 *);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);
extern u8 D_80176640[];
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD4D0(u8 *);
void func_b30_80176480(UnkVec *b, u8 *c);
void func_b30_8017516C(u8 *p, UnkVec *b, u8 *c, u8 *d);

UnkNode *func_8003FD64(s32, void *);
extern u8 D_80176670[];
s16 func_8009A350(s16, s16, s32, u16 *);
void func_b30_8017474C(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b30_80175540(u8 *p, UnkVec *v, UnkSlot *s);
UnkNode *func_8003FC64(s32);
u8 *func_8003DE58(u8 *, u8 *, void *, s16);
s32 func_b30_80175D74(u8 *p, UnkVec *v, UnkSlot *s);
void func_80044A50(UnkNode *);
void func_b30_80174B90(UnkNode *a, u8 *b, u8 *c, u8 *d);
void func_b30_80170AD0(u8 *p, UnkVec *b, u8 *c);
void func_b30_80174690(UnkNode *a);
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);
s32 func_800A56E0(s32);
extern s32 D_8008346C;
extern u8 D_80176600[];
u32 func_800A7A38(u8 *p);
u8 *func_800B23F8(s16 a, u16 b, u16 c, s16 d, s32 e);
u8 *func_800A1618(s32, s16);
void func_8004397C(u8 *);
void func_8003F320(void);
s32 func_800A0B94(s16 a, u8 *s, s16 c);
extern s32 D_800E3548_w[] __asm__("D_800E3548");
extern u8 *D_800E3D7C;
void func_80099FDC(UnkNode *n);
u8 *func_800A17E8(s16 x, s16 k);
void func_800ACB98(u8 *p, void *b, u8 *c, u8 *d);
void func_b30_80173D40(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800956B8(u8 *p, void *b, void *c, u8 *q);
s32 func_80042518(s8 *, s8);
extern u8 D_80083780[];
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
extern u8 D_80083498[];
extern u8 D_800E2968;
void func_800BC318(u8 *);
s32 func_800C6AEC(u8 *p, UnkVec *v, UnkSlot *s);
extern u8 D_80045340[];

/* This bank's 0x112 actor spawner: arm the node with func_b30_80170AD0 and
   the bank-local D_801765D8 facing table, dress the slot from the b/c/d
   arguments, pick one of three flag pairs off `a & 3' (and in the third case
   roll for the D_80176650 variant and set +0xAC from D_800E2968's three
   bands), then hand the node to func_800A9C18, this unit's
   func_b30_80174B90 and func_800AA36C. Returns the payload. 135 words,
   0 differing. 1 copy (dngfam.py).

   THE TWIN IS func_b24_80170F9C (csrc/ovl_dng_b24.c, 133 words, dngtwin
   0.806) and the D_800E2968 three-way is func_b21_80175E14's neighbour in
   csrc/ovl_dng_b21.c. Relink substitutions: `p[0x13]' is 0x1E, the handler /
   table / alt table are func_b30_80170AD0 / D_801765D8 / D_80176650, and the
   +0x8C handler is func_b30_80170EE4. The two words over b24 are its
   `*(u32 *)(p + 0x1C) |= 0x40000' and `*(s16 *)(w + 0x92) = -0x20' being
   absent here and the D_800E2968 band block replacing b24's 0x1C/0x1E save
   and restore.

   b24's ONE SPELLING FACT INVERTS, and the inversion is exactly the one its
   own comment predicts as possible. b24 lands with the facing-table store
   FIRST (`table, b, k, c') and prices `c, table, k, b' at 2 rows; written
   b24's way here the body is 135 words / 8 aligned differences, every row a
   $s5/$s6 exchange between the parameter `c' and the mover `m'. The full
   24-permutation sweep over the four statements {+0x24 = b, +0x25 = c,
   +0x2C = table, k = a & 3}:

     D,F,C,E (as below) 0   D,E,F,C 0   D,F,E,C 0   E,D,F,C 0
     E,F,*   4           F,*,*   4       C,*,*   8   D,C,*  11   C,*,F,* 12

   So what the target pins down is that the +0x24 store precedes the +0x25
   store AND the table store does not come before them -- the opposite
   constraint from b24's, on a body whose statements are otherwise the same.
   b24's mechanism (an exact allocno tie broken on pseudo number) is why: the
   pair in competition here is `c' against `m', not `b' against `c'.

   | spelling                               | words | aligned diffs |
   | as written below                        |  135  |      0        |
   | b24's `table, b, k, c' order            |  135  |      8        |
   | `s32 a'                                 |  131  |     33        |
   | `w = (u8 *)n + 0x20' instead of `w = p' |  135  |      0        |
   | `D_800E2968 <= 9' for `< 0xA'           |  135  |      0        |

   `s16 a' is load-bearing and worth four words: the target has NO prologue
   truncation of $a0 and instead a `sll 16'/`sra 16' at the func_800A9C18
   call and another on the `a & ~3' test, which is combine moving the
   extension past both ANDs. The `w' ALIAS itself is load-bearing (b24 prices
   it at 130 words / 123 differing and that was not re-measured here), but
   HOW `w' is computed is free -- recomputing `(u8 *)n + 0x20' measures the
   same 0, so it is the second pseudo that matters and not the copy. b21's
   `sltiu' note reproduces: the unsigned compare is cse's, not the source's. */
u8 *func_b30_801708B4(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b30_80170AD0;
        p[0x13] = 0x1E;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x24) = b;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_801765D8;
        k = a & 3;
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
        else
        {
            if ((a & ~3) == 0)
            {
                if ((*(u32 *)(p + 0x14) & 0x200) == 0)
                {
                    if ((func_800A6D30() & 1) != 0)
                    {
                        func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                        *(u8 **)((u8 *)r + 0x2C) = D_80176650;
                    }
                }
            }
            if (D_800E2968 < 0xA)
            {
                w[0xAC] = 2;
            }
            else if (D_800E2968 < 0x14)
            {
                w[0xAC] = 1;
            }
            else
            {
                w[0xAC] = 0;
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b30_80170EE4;
        func_b30_80174B90(n, p + 0x2A, w + 0x9A, w + 0x98);
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}

/* func_b34_801718E0 (csrc/ovl_dng_b34.c, 0.948) -- the per-frame actor driver
   whose fourteen landed siblings descend from the resident func_800D8728.
   b34 is the right starting body and not one of the fourteen because THIS
   copy also writes the three-way ground-height arm's common tail ONCE, with
   the +0x90 / +0x92 stores nested inside the else arm. Bank-local renames:
   D_80174F20 -> D_80176680 and the +0x8C gate's handler
   func_b34_80171CE8 -> func_b30_80170EE4. 261 words, 0 differing.

   THE WHOLE FIGHT WAS THE p/q SITE SPLIT, AND THE LISTING SETTLES IT WITHOUT
   A DEVICE. b34's body compiles here at 261 words / 49 aligned differences,
   and all 49 rows are the same fault: the parameter took $s1 and the `q'
   alias $s2, where the target has them the other way round. `-da' prices it
   exactly -- with b34's split the parameter is 37 refs over 435 insns
   (5*37/435 = 0.425) against q's 19 over 205 (4*19/205 = 0.371), so
   allocno_compare ranks the parameter first and it takes the lower register.

   The fix is not a reference bump: FIVE SITES NAME q IN THE TARGET AND p IN
   b34, and each one is visible as an instruction that already agrees while
   everything around it is swapped. They are the fourth argument of the +0x8C
   continuation call, the fourth argument of the D_80176680 dispatch, the
   `lb' of +0x6D in the facing-moved test, the fourth argument of
   func_800AA36C, and the `lh' of +0x2A in the facing index. Moving those
   five off p and onto q takes q to 24 refs and p to 32 and reverses the
   rank. No `do { } while (0)', no duplicated arm: it is the more natural C
   as well, since those five are the sites that read the actor through the
   same alias the +0x88 and +0x1C sites use.

   THREE OTHER SHAPE DELTAS from b34, all read off the listing:
   1. func_800A9E70 is gated: `if (p[0x9A] != 0x17 && p[0x9A] != 0x19)'.
      Those five words (lbu/addiu/beq/addiu/beq) are the +3 against b34's
      258. `dir' is read BEFORE the gate, as the target's
      lbu 0x6D / lbu 0x9A / sll / sra order shows.
   2. there is NO null check on the +0x2C facing table. b34 hoists it into an
      `e' local and tests it; here the subscript is taken straight off
      `*(u8 **)(c + 0x2C)'.
   3. ONE `s16 k' where b34 declares `k1' and `k2'. The target holds the
      `& 7' index in two registers, $s3 for the +0x94 store and $s0 for the
      compare, the +0x2C subscript and the D_8006CCF8 test.

   Measured on THIS body, every alternative priced with aligndiff:

   | spelling                                       | words | aligned diffs |
   | as written below                               |  261  |      0        |
   | b34's split, `p' at the five q sites           |  261  |     49        |
   | b34's `e' local with its NULL test             |  263  |     55        |
   | `s32 k' instead of `s16 k'                     |  258  |     57        |
   | p[0x9D] = 0 AFTER the +0x1C set (b34's order)  |  261  |      7        |
   | no `q' alias at all, `p' everywhere            |  258  |     95        |
   | one handler local instead of `h' and `h2'      |  261  |      3        |
   | b34's two copies `k1'/`k2'                     |  261  |      0        |

   The last row is b34's third fact and it is WEAKER than b34 states: two
   narrowed copies measure 0 here as well, so `k1'/`k2' is not wrong, just
   unnecessary. The handler-local figure reproduces b34's own 3 to the digit.
   */
void func_b30_80170AD0(u8 *p, UnkVec *b, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *q;
    s16 dir;
    s32 r;
    s32 v;
    s16 k;

    q = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b30_80170EE4)
        {
            h(p, b, c, p);
        }
        else
        {
            p[0x71] &= 0x7F;
        }
        return;
    }
    dir = (s8)p[0x6D];
    if (p[0x9A] != 0x17 && p[0x9A] != 0x19)
    {
        if (func_800A9E70(p, b, c, p) != 0)
        {
            return;
        }
    }
    h2 = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h2 != NULL)
    {
        h2(p, b, c, q);
    }
    D_80176680[p[0x9A]](p, b, c, q);
    if (dir != *(s8 *)(q + 0x6D))
    {
        func_800AA36C(p, b, c, q);
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        v = ((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7;
        k = v;
        if (*(s16 *)(p + 0x94) != k)
        {
            func_80047738(c, (*(u8 **)(c + 0x2C))[k], *(s8 *)(c + 0x4));
            *(s16 *)(p + 0x94) = v;
        }
        if (D_8006CCF8[k] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        if ((*(s32 *)(q + 0x1C) & 0x20) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                func_800478B8(c);
            }
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
        }
        func_800A020C(*(s32 *)(q + 0x1C), c + 0xC);
    }
    else if ((*(u16 *)(c + 0x14) & 0x800) != 0)
    {
        *(u16 *)(c + 0x14) &= 0x8FFF;
    }
    else
    {
        *(u16 *)(c + 0x14) |= 0x7000;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if ((*(u16 *)(p + 0x98) & 0x8) != 0)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if ((*(u16 *)(p + 0x98) & 0x4) == 0)
    {
        r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
        if ((s16)r < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(q + 0x88) < (s16)r)
            {
                *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
            }
            else
            {
                if ((s16)r >= *(s16 *)(q + 0x88))
                {
                    *(s32 *)(p + 0x90) = 0;
                }
                else
                {
                    *(s16 *)(p + 0x92) = r - *(u16 *)(q + 0x88);
                }
                b->unk_14 = 0;
                p[0x9D] = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
            }
            if ((*(u32 *)(q + 0x1C) & 0x40000000) != 0)
            {
                *(u32 *)(q + 0x1C) &= 0xBFFFFFFF;
                r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(q + 0x88) - 0x20);
                *(u16 *)(p + 0x92) += *(u16 *)(q + 0x88) - r;
                *(u16 *)(q + 0x88) = r;
            }
        }
        else
        {
            *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
        }
    }
    else
    {
        *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
}

/* The banked room script's main actor handler and this bank's default +0x8C
   continuation: the three early bails, the D_801765D8 facing-table swap, and
   then the twelve-way dispatch on `*(u16 *)(d + 0x46) & 0x3FFF'. 351 words,
   0 differing, on the first compile.

   IT OWNS A JUMP TABLE: jtbl_b30_80170808, 12 entries, `.align 3', in this
   unit's .rodata. Read off the listing, index = (that mask) - 1:
     cases 1 2 3  -> .Lb30_801712E0, the func_800AAF00 arm
     case  4      -> .Lb30_8017130C, default
     cases 5 6 7  -> .Lb30_8017128C, the func_800A0818 re-aim arm
     case  8      -> .Lb30_80171220, the func_b30_80171E38 arm
     case  9      -> .Lb30_80171248, the 0x20000000 arm
     cases 10 11  -> .Lb30_8017130C, default
     case  12     -> .Lb30_801712D0, the func_800A9A0C arm
   Cases 4, 10 and 11 carry no label of their own. MEASURED on the object:
   the table cc1psx emits lands at .rodata offset 0x38 with the section
   0xb4 long, which is byte-for-byte where the stub's listing-supplied copy
   was -- D_b30_8017086C stays at 0x0, D_b30_80170898 at 0x2c,
   jtbl_b30_80170838 at 0x68, jtbl_b30_80170850 at 0x80 and
   jtbl_b30_801708A0 at 0xa0, all unchanged. The twelve entries resolve to
   the six labels above at a single consistent function base, so the
   table's shape is right.

   TWIN: the landed carved sibling func_b37_80170E68 in csrc/ovl_dng_b37.c,
   345 words. THIS IS THAT BODY PLUS SIX WORDS, all in case 9: b37 calls
   func_b37_80174008 and returns, and here case 9 tests
   `*(s32 *)(d + 0x14) & 0x20000000' and picks func_b30_80173C40 or
   func_b30_80171FFC, with cross-jumping sharing the func_b30_80171FFC call
   with case 8's tail. Everything else is a rename:
   D_801741DC -> D_80176650, D_801741D4 -> D_80176678,
   D_80174194 -> D_801765D8, D_8017419C -> D_801765E0,
   D_801741CC -> D_80176660, and the callees func_b37_801713CC ->
   func_b30_80171460, func_b37_80171624 -> func_b30_801716A4,
   func_b37_80173CEC -> func_b30_801737B8, func_b37_801720AC ->
   func_b30_80172114, func_b37_80171DD0 -> func_b30_80171E38,
   func_b37_80171F94 -> func_b30_80171FFC.

   MEASURED ON THIS BODY, the one spelling that matters: the D_800814A8
   pointer load has to be emitted BETWEEN func_800A0818's return and the
   store of its result to d + 0x2A, which is what `m = func_800A0818(...);
   w = (u8 *)D_800814A8; *(s16 *)(d + 0x2A) = m;' gives. Any spelling that
   puts the store first -- dropping `m', or dropping `w', or dropping both --
   is 352 words / 118 differing, identically, because that is the only thing
   the three have in common.

   `(s16)func_b30_80172114(p, b, c, 0)' is load-bearing: without the cast it
   is 350 words / 175 differing.

   MEASURED AND FREE, so NOT the reason this lands -- b37's comment calls the
   first of these load-bearing on ITS body and it is not on this one:
     - the `case 12:' label sitting inside the `if (w[0x9A] != 0x11)' body
       and the fallthrough from cases 5/6/7 into cases 1/2/3. Written out
       flat -- the 0x11 test with its own `return', the func_800AAF00 call
       duplicated, and `case 12:' an arm of its own -- it is 351 words /
       0 differing. jump.c rebuilds both shared blocks by itself here.
     - the `e' / `e2' / `e3' locals holding the facing table. All three
       inlined at every use it is 351 words / 0 differing.

   `--raw' is 82 differing and every row is a relocation -- 23 `jal's,
   11 `j's and 46 `%hi'/`%lo' halves, including the two that take this
   function's own address for the func_800AAF00 arm -- so no masked word is
   hiding a constant address. */
void func_b30_80170EE4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        func_b30_80171460(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80176650)
        {
            e = D_80176678;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80176650)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80176678) != 0)
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
            e2 = D_801765D8;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_801765E0) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b30_801737B8(p, (u8 *)b, c, d);
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
            func_b30_801716A4(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 && (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b30_80172114(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b30_801716A4(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
            if ((s16)func_b30_80171E38(p, b, c, d) != 0)
            {
                return;
            }
            func_b30_80171FFC(p, b, c, d);
            return;
        case 9:
            if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
            {
                func_b30_80173C40(p, b, c, d);
                return;
            }
            func_b30_80171FFC(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_80176660, func_b30_80170EE4);
            return;
        default:
            func_b30_801716A4(p, b, c, d);
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
                    *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
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
    e3 = D_801765D8;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_801765D0 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b30_80171DCC.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b30_80171DCC is called only on
   `(s16)t == 3 && !(D_80083462 & 0x80) && !(c[0x14] & 0x8000)`, and 0xF is
   the else arm.  The listing shows it: `bne $v0, $v1, .Lb07_8017172C` lands
   on an `addiu $v0, 0xF` of its own, the copy reorg left behind when it
   stole the else arm's `addiu` into the two `bnez` delay slots.  Written the
   twin's way round the function is 143 words and 46 differing: the two arms
   swap, and jump.c then cross-jumps the `sw $zero, 0x8C($s4)` that the
   target keeps in both.

   DEVICE: `x + ((x + x) - x - x)` on func_800A0818's first argument.  It is
   the phantom zero of docs/matching-levers.md :11693 -- cse keeps the three
   insns, flow counts them as references on `x`, and combine deletes them, so
   it emits nothing and ties nothing.  Its job here is allocation only:
   without it `d` outranks the `x`/`t` quantity in global-alloc and the two
   swap callee-saved registers, `d` taking $s0 and `x` $s1 where the target
   has $s1 and $s0 -- 145 words, 28 differing, every one of them that rename
   (and `t`, which shares `x`'s register, renames with it).  What the
   function does without it: the same 145 instructions with two callee-saved
   registers exchanged.  It does NOT warn under -Wall: nothing is read
   uninitialised.

   Measured and rejected, all 145 words unless stated:
     the phantom on func_8009A3D0's first argument instead          7
     the phantom as its own statement, `x = x + ((x + x) - x - x);` 2
     the phantom on `y` as well as `x`                              28
     `__asm__("" ::"r"(x));` before the func_8009A3D0 call          2
     no device at all                                               28
     no device, declaring `x` and `y` first                         28
     no device, `s16 x, y`                                          28
     no device, `d` copied into a local before the first test       28
     merging `t` into `x`                                           53
     merging `r` into `y`                                146 words, 112
     a local for the 0x300/0x3000 select                 146 words, 118
     `x` and `y` inline, no locals                       144 words, 120
     `x` and `y` computed before the table swap          146 words, 134 */
void func_b30_80171460(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_801765D0)
        {
            *(u8 **)(c + 0x2C) = D_801765D0;
            func_80047784(c, D_801765D0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        r = func_800A0818(/* DEVICE: the phantom zero.  See the header. */
                          x + ((x + x) - x - x), y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C), (u16 *)(p + 0x98));
        t = func_8009A66C((s16)r, c, d, 0x20);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = r;
        if ((s16)t == 3 && (D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            func_b30_80171DCC(p, b, c, d);
        }
        else
        {
            p[0x9A] = 0xF;
        }
        *(s32 *)(p + 0x8C) = 0;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        if ((D_80083462 & 0x80) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            n = d[0x71];
            if (n > 0)
            {
                *(s16 *)(p + 0x96) = 8 / n;
            }
        }
    }
}

/* The banked room script's walk step, the relinked twin of the landed carved
   func_b57_801726C8 (csrc/ovl_dng_b57.c, 466 words). 458 words, 0 differing,
   on the first compile. THIS COPY'S OWN DELTAS, all four read off the
   listing: func_800A19E4's third and fourth arguments are 2 and 4 where b57
   has 3 and 6; the state check is the bank-local func_b30_80171E38 where b57
   calls func_b57_80172E10; the step probe is func_8009A66C where b57 calls
   func_8009A8C0; and the LAST arm's 0x20000 block has NO
   `func_800A0134(D_800814A8, d) < 0x81' guard before func_8009A540 -- b57
   does, and that guard is the whole 8-word difference in size between the
   two bodies.

   MEASURED ON THIS BODY, and all three of b57's devices are needed here too,
   at this body's own prices:
     - the 0x80000000 in the local `cc2' with the read-modify-write in a
       `do { } while (0)': without it 458 words / 4 differing, the loaded
       word and the `lui 0x8000' swapping $v0 and $v1 (b57: also 4).
     - `t1 = (s32)D_8006CCD8; t1 += kk;' so the `%lo' insn's destination has
       reg_n_sets != 1: written inline as `*(u8 *)(kk + (s32)D_8006CCD8)' it
       is 11 differing (b57: 8). The SECOND lookup must stay inline.
     - the `return' in the i == 0 arm wrapped in a `do { } while (0)': a
       plain `return;' is 11 differing (b57: 4), reorg filling only one of
       the three branches into the `i++' join with the `addiu $v0, $s1, 0x1'
       copy instead of all three.
   All three are most likely macro residue rather than devices -- each wraps
   one coherent unit of work -- but the macro is not recoverable.

   `i = func_800BCB04h(...)' through the s16 alias decides the whole
   function's register assignment, exactly as b57 says: as
   `i = (s16)func_800BCB04x(...)' off an `s32'-returning declaration it is
   458 words / 124 differing (b57: 126), because the halfword assignment
   keeps `i's allocno references up and allocno_compare allocates it before
   `d' and `c'.

   The loop's heading value SHARES `e' with the two actor lookups: in its own
   local it is 3 differing (b57: 3), the `lh' of d + 0x2A landing in $a0
   where the target has $v1.

   MEASURED AND FREE, so NOT the reason this lands -- b57 carries both and
   this body does not need either:
     - `w = (u16 *)(p + 0x98)' in the +0x20000 arm: inlined at both uses it
       is 458 words / 0 differing.
     - `t80 = D_80082E80' in the last arm's +0x20000 block: inlined at all
       four uses it is 458 words / 0 differing. (b57's 23-differing note is
       about the OTHER arm's D_80082E80, which is inline in both bodies.) */
void func_b30_801716A4(u8 *p, UnkVec *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b30_80171E38(p, b, c, d) == 0)
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
    func_800A19E4((s32)c, d, 2, 4, p + 0x9C);
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
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24],
                                                       D_80082E80[0x25], w);
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
    i = 0;
    tbl = D_8006CD00;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - tbl[i];
        }
        else
        {
            s = e + tbl[i];
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

/* State 0x10 with a 0x9E countdown of 2, and hand c the bank-local
   D_801765E8 byte the facing at d+0x2A selects, through func_80047784 rather
   than the resident twin's func_8003DB94. 27 words, 0 differing.
   Shape is func_800D9820's (ratio 0.923): the table base is a POINTER LOCAL
   because it is used twice -- stored at c+0x2C and indexed -- but here the
   table is bytes and the index a plain subscript (`sra 9`, `andi 7`), not the
   resident's byte offset into a pointer array.
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x3.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b30_80171DCC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    *(s16 *)(p + 0x9E) = 3;
    t = D_801765E8;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80176660 facing table and re-aim through func_8009C93C.

   TWO THINGS THAT ARE NOT VISIBLE IN THE LOGIC.

   THE FRAME IS 0x40 WHERE THE CALLS NEED 0x38, SO THE ORIGINAL DECLARED AN
   UNREFERENCED 8-BYTE AGGREGATE LOCAL. The widest call here is
   func_8009C93C's five arguments, which is a 0x18 outgoing-argument area and
   a 0x38 frame -- which is exactly what func_800CA788 gets for the same call
   set. gcc allocates the slot for an aggregate even when flow deletes every
   reference to it, so the extra 8 bytes are a leftover declaration. Only the
   size is witnessed, never the type. No local at all, and `s32 unused;`
   instead of the array, both give 113 words and 16 differing -- every
   $sp-relative offset and both ends of the frame.

   AND THE SHARED `-1` EXIT BELONGS TO THE *LAST* GATE, NOT THE FIRST ONE THAT
   REACHES IT. All four failure exits are one `goto fail`, and the label sits
   in the second func_800A2B5C test, which is where the merged
   `j .Lb03_80171F3C` / `addiu $v0, $zero, -0x1` block is emitted -- as the
   fall-through of that test's `beqz`, with reorg then copying the constant
   into the other three branches' delay slots. func_800CA788's note puts its
   own label in its SECOND test; that is its layout, not a rule.

   | spelling                                  | words | differing |
   | four plain `return -1;`, no label         |  113  |    35     |
   | label in the second D_80083460 gate       |  113  |    36     |
   | label in the `& 0x8` arm                  |  112  |    73     |
   | label in the second func_800A2B5C test    |  113  |     0     |

   func_800A04F0 is reached through a unit-local alias because the landed
   definition in csrc/ovl_dungeon_8009F644.c is `void` while this call site
   uses $v0, and its fourth parameter has to be declared `s16` here rather
   than the definition's `u16`: `u16` makes the `lh` of d + 0x2A an
   `lh`/`andi $a3, $a3, 0xFFFF` pair -- 113 words, 1 differing.

   `d[0x6D] -= 1;` is also BEFORE the func_8009C93C call, not after it as in
   func_800CA788: after the call is 113 words, 8 differing, because the store
   can no longer sink past the `lh` of d + 0x2A. 113 words, 0 differing. */
s32 func_b30_80171E38(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    u8 *e;
    s32 unused[2];

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
    if ((s16)func_800A2CB8(d, t) == 0)
    {
        return 0;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (*(u16 *)(g + 0x2) & 0x8) != 0)
    {
        goto fail;
    }
    if ((u16)(0x40 + -func_800A0134(t, d)) > 0x80)
    {
        return 0;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        goto fail;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
    fail:
        return -1;
    }
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    d[0x84] = 0x7C;
    d[0x85] = 0;
    e = D_80176660;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* The banked room script's unconditional "same slot" entry, the 70-word
   sibling of func_b03_80171DA0 above (the relinked twin of the resident
   overlay's func_800CA788) with the cursor pick and the heading window cut
   out: past the single D_80083462 0x2000 gate and the two func_800A2B5C
   tests, put the actor into state 0x11, swap the D_80176660 facing table
   into the slot's +0x2C and re-aim through func_8009C93C.

   THIS ONE IS `void` AND IT CARRIES NO LEFTOVER LOCAL. All three gates fall
   straight into the epilogue with no $v0 set, and the calls here need exactly
   the 0x30 frame the five register saves and func_8009C93C's 0x18 outgoing
   area add up to -- so the 8-byte aggregate func_b03_80171DA0's comment had
   to invent is absent. Adding `s32 unused[2];` back is 12 differing, every
   $sp-relative offset and both ends of the frame.

   AND THE +0x84/+0x85 PAIR MOVED TO THE TAIL. func_b03_80171DA0 stores them
   before the table swap; here they are AFTER the func_8009C93C call, which is
   the only place the scheduler cannot hoist them from. In the sibling's
   position they are 28 differing. `d[0x6D] -= 1;` stays BEFORE that call as
   in the sibling: after it is 10 differing.

   | spelling                                   | words | differing |
   | as written below                            |  70   |     0     |
   | +0x84/+0x85 before the table swap           |  70   |    28     |
   | `s32 unused[2];` restored                   |  70   |    12     |
   | `d[0x6D] -= 1;` after func_8009C93C         |  70   |    10     |
   | p stores in +0x9A, +0x9B, +0x8C order       |  70   |     2     |
   | plain `if (func_800A2B5C(d) != 0)`          |  68   |    51     |

   The `(s16)` on both func_800A2B5C tests is the target's `sll $v0, $v0, 16`
   / `bnez` halfword test; dropping it costs the two `sll`s and 51 differing.
   Three equal spellings, all 0: the gate read as
   `g = (u8 *)&D_80083460; *(u16 *)(g + 0x2)` the way the sibling spells it,
   D_80176660 used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b30_80171FFC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *e;

    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    p[0x9A] = 0x11;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    e = D_80176660;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    d[0x84] = 0x7C;
    d[0x85] = 0;
}

/* func_b57_801731CC (csrc/ovl_dng_b57.c, 0.962) MINUS its `(s16)d' gate: the
   same func_800ADDA0 dispatch and the same four arms, but this copy never
   mentions the fourth parameter, so the frame is -0x28 with three saves where
   the twin's is -0x30 with four. The two dispatch constants are 2 and 4 where
   the twin passes 3 and 6, and case 2 calls this unit's func_b30_801716A4.
   75 words, 0 differing, first compile.

   Of the twin's four stated facts:
   - HELD, cheaper here: the `p = a;' copy. The target copies its parameters
     $s1, $s2, $s0 -- parameter 0 LAST -- exactly as the twin's does. Using
     `a' directly with no `p' local is 75 words / 5 aligned differences; the
     twin prices the same change at 8, so the fact carries but its price does
     not.
   - HELD: the arm emission order case 0, case 2, case 1-into-default.
     Putting case 2 before case 0 is 73 words / 11 aligned differences.
   - FREE: `s16 r'. Declared `s32 r' with `(s16)' casts on the two tests is
     75 words / 0 aligned differences, which is what the twin's own note
     already says -- it is a readability choice, not a lever, on both bodies.
   - NOT APPLICABLE: `if ((s16)d != 0)' versus `if (d != 0)'. There is no `d'
     gate in this copy at all; the three words it costs the twin
     (sll/bnez/addu) plus the $s3 save, copy and restore are the whole of the
     6-word delta. The relink dropped the statement the fact was about. */
s32 func_b30_80172114(u8 *a, void *b, void *c, s32 d)
{
    u8 *p;
    s16 r;

    p = a;
    r = func_800ADDA0(b, c, p, 2, 4, (s32)(p + 0x9C));
    if (r < 0)
    {
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b30_801716A4(p, b, c, p);
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

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test: the outer gate's failure falls through to the epilogue and the inner
   gate's failure returns, so both branches land on the same word, and the
   installed handler is the bank-local func_b30_80170EE4. 31 words,
   0 differing, first compile. */
void func_b30_80172240(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b30_80170EE4;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* func_b36_801723D4 (csrc/ovl_dng_b36.c, 0.987) line for line with ONE shape
   delta: case 0's entry gate. Its gate is the flag test
   `(*(u16 *)(c + 0x14) & 0x6000)'; here it is a COUNTDOWN on the same +0x9E
   sub-counter case 1 then uses, `if ((*(s16 *)(p + 0x9E) -= 1) > 0) break;',
   which is the extra 2 words (225 against the twin's 223: lhu/addiu/sh/
   sll/bgtz against lhu/andi/beqz). Tables and handler are this bank's:
   D_801765F0 (case 0), D_801765F8 (case 2), func_b30_80170EE4. 225 words,
   0 differing, first compile.

   All three of the twin's stated facts HELD, re-measured here with aligndiff:
   - the tail gate's 0x2000 arm is the `then'. Written
     `if ((v & 0x2000) == 0) { 0x410/0x20000 } else { 0x46 }' this is still
     225 words and 17 aligned differences -- the twin's figure to the digit.
   - the 0x410 and 0x20000 tests stay NESTED. Folded to
     `else if ((v & 0x410) == 0 && (v & 0x20000) != 0)' is 225 words /
     10 aligned differences; fold_truthop merges the two andi.
   - the two numerators keep the resident's asymmetry. Case 1's first
     numerator collapsed to the SECOND one's single-expression form (dropping
     the `u' local) is 225 words / 3 aligned differences, so the
     three-statement spelling is load-bearing here and not just family
     style. */
void func_b30_801722BC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        if ((*(s16 *)(p + 0x9E) -= 1) > 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_801765F0;
        func_80047784(c, D_801765F0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0x9E) = 5;
        *(s32 *)(p + 0xA0) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
        if (*(s16 *)(p + 0x9E) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x9E);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x9E);
            *(s32 *)(p + 0xA0) = -func_800644B8(*(s16 *)(p + 0x9E) * 409) << 9;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA0);
        if ((*(s16 *)(p + 0x9E) -= 1) < 0)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u16 *)(p + 0x98) &= ~8;
            *(s32 *)(d + 0x1C) |= 0x8000000;
            p[0x9B] += 1;
        }
    case 2:
        if ((*(s32 *)(d + 0x1C) & 0x8000000) != 0)
        {
            *(u16 *)(p + 0x98) &= ~8;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            *(u8 **)(c + 0x2C) = D_801765F8;
            func_80047784(c, D_801765F8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B] += 1;
        }
        break;
    }

    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return;
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_800AD594(d, 4);
    func_800A4ACC(d);
    g = (u8 *)&D_80083460;
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(u16 *)(g + 0x8) -= 1;
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) != 0)
        {
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
    }
    else if ((v & 0x410) == 0)
    {
        if ((v & 0x20000) != 0)
        {
            *(s16 *)(d + 0x2A) =
                func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
        }
    }
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        func_800A9A04(d);
    }
}

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
void func_b30_80172640(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        }
    }
}

/* The four-state knock-back handler. State 0 raises bit 0 in +0x98 and FALLS
   THROUGH into state 1, which branches on the slot's +0x14: with 0x8000 up it
   jumps straight to state 3 after re-aiming through func_8009C12C, and with
   only 0x6000 up it zeroes the mover's three velocities, dresses the slot off
   the bank-local D_80176600, spawns the follower with func_b30_80174690 and
   plays 0x804. State 2 counts +0x96 up and re-aims on tick 2 or on 0x8000,
   then on any of 0xE000 swaps in D_80176668 and steps the state. State 3
   waits for 0xE000, hands the actor back to func_800AD594 / func_800A4ACC,
   re-installs func_b30_80170EE4 at +0x8C and clears D_8008346C. 144 words,
   0 differing, FIRST COMPILE. 1 copy (dngfam.py).

   NO TWIN -- dngtwin reports nothing. The dress sites are this unit's own
   idiom (nine landed bodies use the same `D_80083228 + d + 0x2A' facing
   expression) and func_b30_80174690's call site here is what fixed that
   body's parameter: `func_b30_80174690(p - 0x20)'.

   | spelling                                  | words | aligned diffs |
   | as written below                          |  144  |      0        |
   | `break' after state 0 (no fall-through)   |  145  |      4        |
   | the +0x14 read inline at both tests       |  144  |      3        |
   | `b->unk_14 = b->unk_10 = b->unk_0C = 0'   |  144  |      4        |

   THE +0x14 TEST NEEDS A LOCAL. Both arms' tests read one `lhu $v1' and the
   `|= 0x6000' inside the first arm has its OWN `lhu' -- so the two tests
   share a source local while the read-modify-write does not use it. Written
   inline the `else if' is a new cse basic block (its label breaks the walk)
   and the load is emitted twice: 3 rows. The MISSING FALL-THROUGH is the
   brief's rule in reverse -- adding a `break' makes the body one word LONG,
   not short, because state 0's two stores can no longer share state 1's
   block.

   MEASURED AND FREE: the three velocity zeroes as three separate descending
   statements instead of the chain (144 / 0). Only the DIRECTION is priced:
   the target stores +0x14, +0x10, +0xC, which the chain gives with +0x14 as
   the INNER lvalue. */
void func_b30_801726A4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 t;

    switch (p[0x9B])
    {
    case 0:
        *(u16 *)(p + 0x98) |= 1;
        p[0x9B] += 1;
    case 1:
        t = *(u16 *)(c + 0x14);
        if ((t & 0x8000) != 0)
        {
            p[0x9B] = 3;
            *(u16 *)(c + 0x14) |= 0x6000;
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
        }
        else if ((t & 0x6000) != 0)
        {
            b->unk_0C = b->unk_10 = b->unk_14 = 0;
            *(u8 **)(c + 0x2C) = D_80176600;
            func_80047784(c, D_80176600[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            func_b30_80174690((UnkNode *)(p - 0x20));
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
            func_800A56E0(0x804);
        }
        break;
    case 2:
        *(u16 *)(p + 0x96) += 1;
        if ((s16) * (u16 *)(p + 0x96) == 2 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u8 **)(c + 0x2C) = D_80176668;
            func_80047784(c, D_80176668[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B] += 1;
            *(u16 *)(p + 0x98) &= 0xFFFE;
        }
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_800AD594(d, 0x100);
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
            D_8008346C = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
            func_800A4ACC(d);
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_801728E4);

/* func_b55_80172B4C (csrc/ovl_dng_b55.c, 0.889) -- the same three-state glide
   on p + 0x9B, with the same fallthrough, the same `stop:' label inside case
   1 that case 0's `else' reaches by `goto', the same quarter decay, the same
   `<< 15' seeds and the same two-numerator divide. Bank-local renames:
   D_80173EEC -> D_80176640 and the installed handler
   func_b55_80171514 -> func_b30_80170EE4. 224 words, 0 differing.

   The 43 extra words against b55's 181 are all ADDED STATEMENTS, not added
   references -- one new call at the head of case 0 and a SECOND countdown
   woven through all three states:

   1. `func_b30_80176480(b, c)' opens case 0, before func_800AD4D0.
   2. a +0xA8 counter beside the existing +0x96 one. `if ((*(s16 *)(p + 0xA8)
      -= 1) == 0) func_b30_8017516C(p, b, c, d);' stands at the top of case 1
      and again after case 2's divide, and case 0 seeds +0xA8: -1 on the
      +0x14 0x8000 path and, on the other path, 1 or -1 according to the low
      two bits of a func_800A6D30 roll.
   3. that same roll gates a third func_b30_8017516C call inside case 0's
      0x8000 arm: `if (m == 0)'. The target computes the mask ONCE, as the
      single `andi $a0, $v0, 0x3' sitting in a branch delay slot, and holds
      it in $a0 across both arms -- so the source has an `m' local and not
      two `r & 3' expressions.

   The constant 1 reaching +0xA8 through $s4 is NOT a device and needs no
   spelling: cse2 hoists it because `case 1:' already compares p[0x9B]
   against 1, which is why this body saves a fifth callee-saved register
   where b55 saves four.

   TWO SPELLINGS HAD TO BE FOUND OFF THE RESIDUE and both are shape facts,
   not allocation ones. The `m' local is one: written as b55 would, with
   `r & 3' at both arms, cse does NOT unify the two and the body is 226 words
   / 9 aligned differences. And the 1 / -1 pair must be written with the
   NON-ZERO arm as the `then' -- `if (m != 0) -1 else 1' -- because the target
   emits the -1 store first; the reading arm order is 224 words / 5 aligned
   differences, every row the two stores and the branch between them.

   Measured here, every alternative priced with aligndiff:

   | spelling                                       | words | aligned diffs |
   | as written below                               |  224  |      0        |
   | no `m', `r & 3' at both arms (b55's shape)     |  226  |      9        |
   | `if (m == 0) 1 else -1', the reading order     |  224  |      5        |
   | case 2's SECOND numerator split three-statement|  225  |     12        |
   | case 2's FIRST numerator collapsed to one      |  224  |      3        |
   | `if (--*(s16 *)(p + 0xA8) == 0)' at all three  |  224  |      0        |
   | `goto stop' written out as a third copy        |  224  |      0        |

   Of b55's three stated facts: the three-statement FIRST numerator and the
   two-statement SECOND one both HELD (3 and 12 differences the other way --
   func_800D991C prices splitting the second at 145, so the fact carries but
   its price does not); its `goto stop' is FREE here, because writing the
   teardown out a third time also measures 0 and jump2 cross-joins the copies
   by itself. The +0xA8 countdown's two spellings are interchangeable, so
   nothing in the target constrains that choice. */
void func_b30_80172D04(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 m;

    switch (p[0x9B])
    {
    case 0:
        func_b30_80176480(b, c);
        func_800AD4D0(d);
        b->unk_0C = -D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = -D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        p[0x9B] += 1;
        if (d[0x28] != 0)
        {
            m = func_800A6D30() & 3;
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                if (m == 0)
                {
                    func_b30_8017516C(p, b, c, d);
                }
                *(s16 *)(p + 0xA8) = -1;
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] = 2;
                return;
            }
            *(s16 *)(p + 0x96) = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
            if (m != 0)
            {
                *(s16 *)(p + 0xA8) = -1;
            }
            else
            {
                *(s16 *)(p + 0xA8) = 1;
            }
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if ((*(s16 *)(p + 0xA8) -= 1) == 0)
        {
            func_b30_8017516C(p, b, c, d);
        }
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
            func_800AAA54(p, b, c, D_80176640);
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
        if ((*(s16 *)(p + 0xA8) -= 1) == 0)
        {
            func_b30_8017516C(p, b, c, d);
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
        *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b30_80173084(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b30_801730A4(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b30_801730C4(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b30_801730E4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* The banked room script's three-state tail, switching on the state index at
   p + 0x9B: the relinked twin of the resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c), which is this body statement for statement
   with four local changes -- two separate bank-local facing tables
   (D_80176650 in state 0, D_80176668 in state 1's re-dress) where the
   resident uses D_800E262C for both, func_80047784 for func_8003DB94,
   func_b30_801737B8/func_b30_80170EE4 for func_800DA660/func_800D8C64, and
   NEITHER of the twin's two `*(s32 *)(d + 0x1C) &= ~0x200' clears: state 1's
   0x8000 arm and state 2 install the handler and nothing else.
   228 words, 0 differing.

   STATE 1 OPENS WITH A func_80042900 GATE THE RESIDENT TWIN DOES NOT HAVE,
   and its false arm is the re-dress block the long refusal chain also falls
   into -- so the whole chain is the body of
   `if ((s16)func_80042900((s8 *)d, 1) != 0)' and the re-dress follows it.

   THE `p[0x9B] += 1;' TAIL IS WRITTEN OUT PER ARM, NOT `goto'-ed, and
   jump2's cross-jumping puts the single copy back: as `goto bump' it is
   226 words / 29 aligned differences, because with one bump block the two
   `sh' stores either side of it come out in the same registers and
   cross-jumping merges them too, costing state 0's `sh $v0, 0xA($v1)' and
   the load-delay `nop' the target has. Written per arm the two `sh's differ
   ($v1 address/$v0 value in state 0, $v0 address/$v1 value in state 1), the
   merge stops at the bump, and the word count is the target's.

   DEVICE: state 0's bump is wrapped in a `do { } while (0)'. Without it the
   body is 228 words / 18 aligned differences and every one of them is `p'
   and `b' holding each other's callee-saved register -- the target has
   p = $s2, b = $s3 and the plain body gives b = $s2, p = $s3. flow.c counts
   a reference once per enclosing loop depth, so the zero-trip loop makes
   state 0's two `p' references count twice, and that is enough to lift `p'
   past `b' in allocno_compare without lifting it past `c' and `d'. The
   cancelled pointer pair `p++; p--;' after the declarations overshoots --
   it also gives `p' a second set, which turns off update_equiv_regs'
   live-length doubling, and `p' comes out in $s0 ahead of `d': 49 aligned
   differences. Measured and equal at 0 differing, so the exact spelling of
   the wrapped statement is not the lever: `++p[0x9B];' inside the
   do-while, and widening it to wrap the `*(u16 *)(h + 0xA) -= 1;' as well.
   Measured and rejected: the do-while around state 1's bump instead is
   226 words / 11 differences and around both bumps 226 / 37;
   `p[0x9B] = p[0x9B] + 1;' long-hand in state 0 and declaring `b' as
   `void *' are both 18, i.e. no change at all.

   `&D_80083460' is FOUR separate pointer locals, one per arm, as in both
   resident twins; sharing state 0's with state 1's re-dress is 226 words /
   30 differences. */
void func_b30_8017311C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
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
        *(u8 **)(c + 0x2C) = D_80176650;
        func_80047784(c, D_80176650[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        do
        {
            p[0x9B] += 1;
        } while (0);
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
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
                func_b30_801737B8(p, (u8 *)b, c, d);
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
        }
        *(u8 **)(c + 0x2C) = D_80176668;
        func_80047784(c, D_80176668[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_80176650 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b30_80170EE4. The resident
   overlay's func_800DA398 (csrc/ovl_dungeon_800D50FC.c) is the same machine
   over the same record and its comment covers the shape; the landed
   func_b05_80173BEC is the nearer twin, since like this copy it opens state 1
   with the same d[0x25] test that closes it, so the dress/bump tail is
   written out twice. Representative of a 10-member family.

   THIS COPY CARRIES NO FLAG UPDATES. The resident twin's
   `*(s32 *)(d + 0x1C) |= 0x40000` at both state-1 dress sites and its
   `&= ~0x208` in state 2 are absent here: writing them in, which is exactly
   func_b05_80173BEC's body, is 207 words with 142 differing.

   THE `p[0x9B] += 1;` BUMP MUST BE WRITTEN OUT IN ALL THREE REACHING ARMS,
   NOT REACHED BY A SHARED `goto` LABEL, and that is the only thing that makes
   this function land. jump2 cross-jumps the three copies back into the one
   the target has, so they cost no code -- but flow counts them first, and
   they are worth four references on `p`. One shared label is 195 words with
   15 differing, EVERY ONE OF THEM A REGISTER: `p` takes $s3 and `b` takes
   $s2, the other way round from the target. Two copies -- the opening arm
   reaching the tail's label -- is the same 195 words and the same 15.

   The cause is read off the compiler, not guessed. global.c's
   allocno_compare ranks on floor_log2 (n_refs) * n_refs / live_length, and
   from `.flow` one bump copy gives `p` 9 references over 173 insns against
   `b`'s 5 over 47:

       p = 3 *  9 / 173 = 0.156     loses to
       b = 2 *  5 /  47 = 0.213

   so `p` is allocated second. Three copies put `p` at 13 references over 177
   insns -- 3 * 13 / 177 = 0.220 -- and it wins. The threshold either side was
   solved before compiling: 12 references scores 3 * 12 / 177 = 0.203 and
   still loses, which is why two copies cannot work and three must.
   (docs/matching-levers.md, "HOW MANY REFERENCES YOU NEED IS AN ARITHMETIC
   PROBLEM, NOT A SWEEP".)

   `&D_80083460` IS FOUR SEPARATE POINTER LOCALS, one per arm. Sharing one
   across the arms is 191 words with 146 differing -- the gate load sinks and
   the arms merge.

   The facing tables are NAMED DIRECTLY at all three dress sites. One shared
   `u8 *` local for the three is 195 words with 6 differing, the `%hi`/`%lo`
   pair splitting across registers at each site; three separate locals, one
   per site, also measure 0 and the bare symbols are kept because they need no
   extra declaration.

   Other spellings measured and rejected: `d[0x6D]` read as `u8` rather than
   `*(s8 *)(d + 0x6D)` is 1 differing, an `lbu` for the target's `lb`;
   `*(s32 *)(d + 0x64)` for the `lh` of d + 0x64 is 1 differing; dropping the
   `(s16)` cast on the first func_800A2C34 result is 194 words with 103
   differing. Measured and equal at 195 words / 0 differing, so none of these
   is the reason it lands: re-reading d + 0x1C at both flag tests instead of
   through the `v` local, writing state 2 as a positive `if` arm instead of an
   early return, nesting the d + 0x64 test instead of `&&`, and the three
   separate table locals above.

   195 words, 0 differing. */
void func_b30_801734AC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *j;
    u8 *k;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(u8 **)(c + 0x2C) = D_80176650;
        func_80047784(c, D_80176650[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80176668;
            func_80047784(c, D_80176668[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            p[0x9B] += 1;
            return;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
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
            func_b30_801737B8(p, (u8 *)b, c, d);
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
        if (d[0x25] == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80176668;
        func_80047784(c, D_80176668[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(g + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        return;
    }
}

/* The four-argument installer's banked twin: func_800DA660 in the resident
   overlay is its first half verbatim, and this one adds the gate's `else`.
   50 words, 0 differing.
   THE `(s16)` CAST ON func_800AD9B4 IS THE ONE LOAD-BEARING SPELLING: the
   target tests the result as a halfword, `sll $v0, 16` then `blez` with no
   `sra`. Written `func_800AD9B4(c, d) > 0` it is 49 words / 38 differing.
   Measured and equal at 50 words / 0 differing, so none of these is the
   reason it lands: passing the four arguments explicitly to func_800AC82C
   (they are already in $a0..$a3, and the declaration is empty-paren, so the
   call emits the bare `jal` either way); hoisting D_801765D0 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b30_801737B8(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80176650 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_801765D0;
        func_80047784(c, D_801765D0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    }
}

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
void func_b30_80173880(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        }
    }
}

/* 14 words, 0 differing. */
void func_b30_801738E4(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* 14 words, 0 differing. */
void func_b30_8017391C(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* 14 words, 0 differing. */
void func_b30_80173954(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* The hand-over at the end of a transformation. Copy the staged actor's two
   position bytes and +0x88 heading onto the live slot, re-place the mover, and
   then: if the staged node's +0x1E carries 0x8000, adopt it -- mark +0x6D when
   it is kind 0x1E with a non-zero +0x28, publish it into D_800E3D7C's +0xAC
   (and take over the +0x60 cursor), bump D_80083460's +0xA counter unless it
   is kind 0x1E, copy the whole Unk42710 across with func_80042710, drop
   0x20000000, set or clear 0x200 from func_80042900's verdict, tag it 0x1E,
   re-register it with func_80099FDC, hand it to func_b30_80173D40 and clear
   +0xA4. Otherwise, if the staged kind is in range, is not 0x1E and +0x9B is
   still 0, tear the staged actor down through func_800ACB98 when neither of
   func_800A17E8's kind-1 and kind-3 lookups claims it. 173 words, 0 differing.
   1 copy (dngfam.py).

   NO TWIN -- dngtwin reports nothing. The func_80042710 / Unk42710 block and
   the D_800E3D7C publish come from csrc/ovl_dungeon_800A4B88.c's
   func_800A504C (callee-set intersection, 6 of 8 symbols).

   | spelling                                      | words | aligned diffs |
   | as written below (`s32 h', `s16 t')            |  173  |      0        |
   | `s16 h', `s16 t'                               |  173  |     28        |
   | `s32 h', `s32 t'                               |  168  |     38        |
   | `s16 h', `s32 t'                               |  170  |     12        |
   | `s32 h', `s8 t'                                |  172  |      9        |
   | `s32 h', `u8 t'                                |  171  |     11        |
   | `s32 h', `u16 t'                               |  171  |     12        |
   | `s16 h', `u8 t' / `s8 t' / `u16 t' / `s16 t'   | 171/170/171/173 | 11/16/17/28 |
   | `&= ~0x201' for `&= ~0x200'                    |  173  |      1        |
   | one base local for all three D_80083460 sites  |  173  |      2        |
   | no `t = h', the first lookup called on `h'     |  174  |     50        |

   THE TWO LOCAL WIDTHS ARE A CONJUNCTION AND NEITHER HALF REACHES 0. This is
   the clearest case of the batch-28 product rule in this unit: the full 2x5
   cross is in the table and the minimum off the `s32 h' row is 9 while the
   minimum off the `s16 t' row is 28 -- only the pair measures 0. A
   one-at-a-time sweep from `s16 h' / `s32 t' (12 rows) would have moved to
   `u8 t' (11) and stopped. The mechanism is visible in the residue: with a
   word-wide flag the body is THREE WORDS SHORT, missing `addu $s0, $v0' after
   the `sltu' and an `addu $v0, $s0' before each of the two `beqz's -- the
   narrow flag is what makes gcc materialise the boolean in a temp and read it
   back through a copy, and a narrow `h' then adds a sign-extension the target
   does not have.

   TWO SMALLER FACTS. `t = h' before the first lookup is load-bearing and
   worth 50 rows: the target holds the staged kind in TWO pseudos, $s1 for
   the kind-3 lookup and its `bne', and $s0 for the kind-1 lookup and the
   `xor', with $s0 then reused as the flag. And the three D_80083460 /
   D_800E3D7C bases need THREE single-set locals: merged, `g' has three sets,
   loses `adjust_priority's single-set boost and emits
   `lui $v0 / addiu $v1, $v0' where the target has the tied
   `lui $v1 / addiu $v1, $v1'. 2 rows. */
void func_b30_8017398C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *n;
    u8 *m;
    u8 *q;
    u8 *r;
    u8 *e;
    u8 *g;
    u8 *g2;
    u8 *g3;
    s16 k;
    s32 h;
    s16 t;

    n = *(u8 **)(p + 0xA4);
    r = *(u8 **)(n + 0xC);
    c[0x24] = r[0x24];
    c[0x25] = r[0x25];
    q = n + 0x20;
    *(s16 *)(d + 0x88) = *(u16 *)(q + 0x88);
    func_800A2B04(b, c[0x24], c[0x25]);
    m = *(u8 **)(p + 0xA4);
    if ((*(u16 *)(m + 0x1E) & 0x8000) != 0)
    {
        if (q[0x13] == 0x1E && q[0x28] != 0)
        {
            d[0x6D] = 1;
        }
        if ((*(s32 *)(d + 0x14) & 0x4000) != 0)
        {
            k = (s16)func_800A1BD0(q);
            g = D_800E3D7C;
            e = (u8 *)(k * 4 + (s32)g);
            *(s32 *)(e + 0xAC) = (s32)d;
            *(s32 *)(e + 0xE4) = 0;
            if (*(u8 **)(g + 0x60) == q)
            {
                *(u8 **)(g + 0x60) = d;
            }
        }
        if (q[0x13] != 0x1E && q[0x28] == 0)
        {
            g3 = (u8 *)&D_80083460;
            *(u16 *)(g3 + 0xA) += 1;
        }
        *(s32 *)(d + 0x14) = 0;
        *(s32 *)(d + 0x1C) = 0;
        func_80042710((Unk42710 *)d, (Unk42710 *)q);
        *(u32 *)(d + 0x14) &= 0xDFFFFFFF;
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            *(u32 *)(d + 0x1C) |= 0x200;
        }
        else
        {
            *(u32 *)(d + 0x1C) &= ~0x200;
        }
        d[0x13] = 0x1E;
        func_80099FDC((UnkNode *)(p - 0x20));
        *(s32 *)(d + 0x60) = *(s32 *)(q + 0x60);
        func_b30_80173D40(p, b, c, d);
        *(u8 **)(p + 0xA4) = NULL;
    }
    else if ((s8)q[0x13] - 1 >= 0 && q[0x13] != 0x1E && p[0x9B] == 0)
    {
        h = m[0x33];
        t = h;
        e = func_800A17E8(t, 1);
        if (e == NULL)
        {
            t = 1;
        }
        else
        {
            t = e[1] != t;
        }
        if (t != 0)
        {
            e = func_800A17E8(h, 3);
            if (e != NULL && e[1] == h)
            {
                t = 0;
            }
            if (t != 0)
            {
                g2 = (u8 *)&D_80083460;
                if (*(u8 **)(g2 + 0xC) == q)
                {
                    *(u8 **)(g2 + 0xC) = NULL;
                }
                func_800ACB98(q, *(u8 **)(q - 0x18), *(u8 **)(q - 0x14), q);
                p[0x9B] += 1;
            }
        }
    }
}

/* The actor's bail-out: if D_80083460's +0x2 0x2000 bit is up just strip the
   node's +0x71 top bit, otherwise -- once func_800A2BDC says the engine is
   idle -- drop into state 0x17/0, clear the +0x8C handler and the +0x96
   countdown, re-dress the slot off the bank-local D_80176670 facing table,
   raise 0x10000000 in the node's +0x1C, bump D_80083460's +0xA counter and
   clear the node's +0x46 top bit. 64 words, 0 differing, first compile.
   1 copy (dngfam.py).

   dngtwin's only lead is func_b32_80174B14 at 0.711 and it is NOT what this
   came from: the body is func_b10_801751C0's gate and tail (csrc/ovl_dng_b10.c,
   43 words) with this unit's own D_80176670 dress block spliced in, found by
   grepping `func_800A2BDC' -- five landed bodies use it and four spell the
   test the same way. BOTH of b10's facts HOLD here, at bigger prices:

   | spelling                                        | words | aligned |
   | as written below                                 |  64  |    0    |
   | the +0x2 gate off the `D_80083462' symbol        |  64  |   17    |
   | `func_800A2BDC(d) == 0' with no (s16) cast       |  63  |    1    |
   | `*(s16 *)(p + 0x96) = 0' BEFORE the call         |  64  |    2    |

   The `g' pointer local is what puts ONE `addiu $s2, %lo(D_80083460)' in the
   prologue to serve both the +0x2 gate and the +0xA bump; b10 priced the
   same choice at 38 rows. The cast is worth one word, the `sll 16' that
   truncates the callee's result -- the same figure b10 records. The +0x96
   store is INDEPENDENT of the call, so its position is a price and not
   something the listing's delay slot can be read for: it is one word either
   way and reorg sinks it into the `jal's slot only from after. */
void func_b30_80173C40(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        d[0x71] &= 0x7F;
    }
    else if ((s16)func_800A2BDC(d) == 0)
    {
        p[0x9A] = 0x17;
        p[0x9B] = 0;
        *(s32 *)(p + 0x8C) = 0;
        *(u8 **)(c + 0x2C) = D_80176670;
        func_80047784(c, D_80176670[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x96) = 0;
        *(s32 *)(d + 0x1C) |= 0x10000000;
        *(u16 *)(g + 0xA) += 1;
        *(u16 *)(d + 0x46) &= 0x7FFF;
    }
}

/* Below the sweep's 18-word floor, so no twin search: 14 words read straight
   off the listing. A state entry -- park the record in state 0x19 with its
   substate, continuation and +0x96 counter cleared, drop 0x800 from the
   tile's +0x14 flags and raise 0x10000000 on the actor's +0x1C.
   14 words, 0 differing, first compile.

   It is a LEAF: no $sp adjustment, no saves, and the last store sits in the
   `jr $ra' delay slot. The second parameter is never mentioned -- $a1 is
   untouched -- and the signature keeps it only because the unit's other
   handlers have it. One spelling measured and FREE, so it is NOT why this
   lands: `*(u16 *)(c + 0x14) &= 0xF7FF' against `&= ~0x800' -- both emit the
   same `andi 0xF7FF' at 14 words / 0 aligned differences. */
void func_b30_80173D40(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    p[0x9A] = 0x19;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(c + 0x14) &= 0xF7FF;
    *(s16 *)(p + 0x96) = 0;
    *(s32 *)(d + 0x1C) |= 0x10000000;
}

/* 0x2c bytes of this bank's own rodata, between its jump tables,
   read by func_b30_80173D78. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b30_8017086C[] = {
    0x81,
    0x40,
    0x82,
    0x83,
    0x82,
    0x81,
    0x82,
    0x8E,
    0x81,
    0x66,
    0x82,
    0x94,
    0x81,
    0x40,
    0x82,
    0x94,
    0x82,
    0x92,
    0x82,
    0x81,
    0x82,
    0x8E,
    0x82,
    0x93,
    0x82,
    0x86,
    0x82,
    0x8F,
    0x82,
    0x92,
    0x82,
    0x8D,
    0x81,
    0x40,
    0x82,
    0x89,
    0x82,
    0x8E,
    0x82,
    0x94,
    0x82,
    0x8F,
    0x00,
    0x00,
};

/* 0x8 bytes of this bank's own rodata, between its jump tables,
   read by func_b30_80173D78. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b30_80170898[] = {
    0x81,
    0x44,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80173D78);

/* The four-state dissolve this bank installs when an actor leaves. State 0
   re-tags the node onto func_800C6AEC, seeds the 0x10-frame +0x96 countdown
   and switches the slot to the 0x8000000 blend, and FALLS THROUGH into state
   1, which eases the three +0xC..+0xE greys towards 0x80 by
   (0x80 - v) / +0x96 until the countdown reaches 0. State 2 runs the +0xF
   counter down and, at 1, unlinks the node, restores the 0x808080 grey and
   the +0x12 / +0x14 pair and spawns func_b30_80174B90's follower. State 3 is
   the teardown: drop 0x10000000 from the actor's +0x1C, re-scale through
   func_8009A21C unless 0x400000 is up, decrement D_80083460's +0xA, and then
   either park the actor in +0x10 and call func_800AAA54, or -- when +0x28 is
   set -- either hand the D_800E3D7C row to func_800956B8 or re-arm the 0xA
   animation. 193 words, 0 differing. 1 copy (dngfam.py).

   NO TWIN -- dngtwin reports nothing. func_800C6AEC and the D_80045340
   re-tag come from csrc/ovl_dungeon_800A4B88.c's func_800ACBE4 / func_800ACD74
   (the same grep that found func_b30_80175F70's donors), and
   func_b30_80174B90's four-argument call site here is the one that fixes its
   second parameter's type.

   | spelling                                      | words | aligned diffs |
   | as written below                              |  193  |      0        |
   | `break' after state 0 (no fall-through)       |  193  |     10        |
   | state 2's teardown arm written FIRST          |  193  |      8        |
   | `*(s16 *)(p + 0x96) = 0x10' AFTER the call    |  193  |      2        |
   | `c[0xC] = c[0xE] = c[0xD] = ...' in state 1   |  193  |      5        |

   TWO THINGS WORTH CARRYING. State 2's arms must be written with the CHEAP
   arm first: `if (c[0xF] >= 2) { c[0xF] -= 1; break; }' and the teardown
   after it, which is the inverse of how the state reads. The target's
   `bnez $v0, .Lb30_801743B0' jumping FORWARD to the teardown is the tell --
   8 rows the other way. And state 1's chain stores +0xC, +0xE, +0xD, an
   order no one writes by hand: it comes out of `c[0xD] = c[0xE] = c[0xC] =
   ...' by the inner-lvalue rule, and the natural descending spelling is 5.

   MEASURED AND FREE, so it is NOT the reason this lands: state 3's 0x2000
   scale test read off `*(s32 *)(d + 0x1C)' instead of the pre-mask local `t'
   (193 / 0). The masked store clears only 0x10000000, so cse forwards either
   spelling to the same register and the two are indistinguishable. */
void func_b30_8017427C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *g;
    u8 *g2;
    s16 k;
    s16 k2;
    s32 t;

    switch (p[0x9B])
    {
    case 0:
        *(s16 *)(p + 0x96) = 0x10;
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_800C6AEC);
        *(u32 *)(c + 0xC) = 0x8000000;
        *(s16 *)(c + 0x10) = 0x20;
        *(s16 *)(c + 0x12) = *(u16 *)(c + 0x12) - 0x80;
        *(u16 *)(c + 0x14) |= 0xC;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        p[0x9B] += 1;
    case 1:
        k = *(s16 *)(p + 0x96);
        if (k != 0)
        {
            c[0xD] = c[0xE] = c[0xC] = c[0xC] + (0x80 - c[0xC]) / k;
            *(u16 *)(p + 0x96) -= 1;
            break;
        }
        p[0x9B] += 1;
        break;
    case 2:
        if (c[0xF] >= 2)
        {
            c[0xF] -= 1;
            break;
        }
        n = (UnkNode *)(p - 0x20);
        func_80044A50(n);
        func_8004491C(n, (u32)D_80045340);
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x10) = 0;
        *(s16 *)(c + 0x12) = *(u16 *)(c + 0x12) + 0x80;
        *(u16 *)(c + 0x14) &= 0xFFF3;
        func_b30_80174B90(n, d + 0x2A, p + 0x9A, p + 0x98);
        p[0x9B] += 1;
        break;
    case 3:
        *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        t = *(s32 *)(d + 0x1C);
        *(s32 *)(d + 0x1C) = t & ~0x10000000;
        if ((*(s32 *)(d + 0x1C) & 0x400000) == 0)
        {
            func_8009A21C(c[0x24], c[0x25], (t & 0x2000) ? 0x300 : 0x3000);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        if (d[0x28] == 0)
        {
            *(s32 *)(g + 0x10) = (s32)(d - 0x20);
            func_800AAA54(p, b, c, D_80176640);
        }
        else if ((*(s32 *)(d + 0x14) & 0x4000) != 0 && (*(s32 *)(d + 0x1C) & 0x10000) != 0)
        {
            k2 = (s16)func_800A1BD0(d);
            if (k2 < 0)
            {
                return;
            }
            g2 = D_800E3D7C;
            func_800956B8(g2, D_80083780, D_80082E80, *(u8 **)(k2 * 4 + (s32)g2 + 0xD0));
            *(s32 *)(d + 0x1C) &= ~0x10000;
        }
        else if ((s16)func_80042900((s8 *)d, 0xA) != 0)
        {
            func_800A48F0((s8 *)d, 0xA, (s8)func_80042518((s8 *)d, 0xA));
        }
        break;
    }
}

/* 14 words, 0 differing. */
void func_b30_80174580(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* 14 words, 0 differing. */
void func_b30_801745B8(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* NO TWIN: dngtwin's four leads for this stub are all still INCLUDE_ASM
   (func_s23_800241D8, func_s22_800249B8, func_s70_80024ACC,
   func_s63_80024AA4), so this is plain decompilation off the listing.
   It republishes the slot's three +0x1C coordinates into the mover, re-skins
   the tile and, when the tile came back masked, raises 0x8000 both in the
   halfword BELOW the record and in the global D_800814A0.
   40 words, 0 differing, first compile.

   The parameter `b' is never copied to a callee-saved register: all three of
   its stores are emitted before the func_800478B8 call -- the third in its
   delay slot -- so $a1 is still live for them. That falls out of the
   statement order and is not a device.

   One measured spelling, and it is load-bearing: `*(u8 **)(p + 0x1C)' is
   re-read at each of the three stores and NOT hoisted into a local. A
   `u8 *e = *(u8 **)(p + 0x1C);' with three uses is 36 words / 10 aligned
   differences -- cse keeps ONE load there, where the target reloads three
   times, so the source genuinely does not hold the pointer.
   D_800814A0 needs no declaration; it is `extern u32' in
   include/structs.h. */
void func_b30_801745F0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x6);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(p + 0x1C) + 0xA);
    func_800478B8(c);
    if (*(u16 *)(c + 0x14) & 0x6000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The SPAWNER for the follower node func_b30_801745F0 drives, called as
   `func_b30_80174690(p - 0x20)' from func_b30_801726A4's state 2: allocate a
   0x112 node behind the caller's OWN node, park the caller's renderable and
   mover in the payload at +0x18 and +0x1C, dress the new slot (0x1000 scale,
   0x808080 grey, the 0x44 sprite, +0x12 pulled back 0x80 and the 0xC bits)
   and copy the caller's +0x28 clip word across. 47 words, 0 differing, first
   compile. 1 copy (dngfam.py).

   NO TWIN -- dngtwin reports nothing for this stub. The skeleton is the
   corpus-wide `func_8003FD64(0x112, ...)' spawner (func_b05_80170884 and 18
   others) and the dress block is func_b29_801752EC's; what is this body's
   own is the SECOND ARGUMENT, the caller's node rather than D_80083498,
   which is why the payload pointers are the caller's and not a global's.

   LOAD-BEARING, MEASURED, both read off the listing:
   | spelling                                          | words | aligned |
   | as written below                                  |   47  |    0    |
   | `*(s16 *)(r + 0x1E) = *(s16 *)(r + 0x1C) = 0x1000'|   47  |    2    |
   | +0x28 read as `(u8 *)a->unk_0C + 0x28'            |   47  |    9    |
   The first is the inner-lvalue rule again: the target stores +0x1E then
   +0x1C. The second is the interesting one -- the target RELOADS the payload
   word it has just written (`lw $v0, 0x18($a1)'), which it only does if the
   source reads it back through `q'; going to `a->unk_0C' directly keeps the
   value in a register and costs 9 rows. The reload survives cse because the
   dress stores through `r' invalidate it. */
void func_b30_80174690(UnkNode *a)
{
    UnkNode *n;
    u8 *q;
    u8 *r;

    n = func_8003FD64(0x112, a);
    if (n != NULL)
    {
        r = (u8 *)n->unk_0C;
        n->unk_10 = (u32)func_b30_801745F0;
        q = (u8 *)n + 0x20;
        *(u32 *)(q + 0x18) = a->unk_0C;
        *(UnkVec **)(q + 0x1C) = a->unk_08;
        *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x12) = *(u16 *)(r + 0x12) - 0x80;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s32 *)(r + 0x28) = *(s32 *)(*(u8 **)(q + 0x18) + 0x28);
        func_80047784(r, 0x44, 0);
        func_8004491C(n, (u32)D_80045340);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017474C);

/* The SPAWNER for the func_b30_8017474C node: a 0x100 node behind the
   caller's own, with the caller's renderable and mover parked at payload
   +0x14 and +0x18, the new slot dressed from the caller's (0x1000 scale,
   0x808080 grey, sprite 0x46, the caller's +0x28 and +0x12 copied), the
   caller's three mover halves copied into the node's own mover, and the
   four call arguments plus a 0x1000 parked in the payload AFTER the
   func_8004491C link. 79 words, 0 differing, first compile. 1 copy
   (dngfam.py).

   This is func_b30_80174690 (47 words, four listings up in this unit) with
   a different node size, handler and sprite plus the extra payload block --
   the in-unit sibling route, no twin needed. func_8004491C's second
   argument here is the address of THIS UNIT's func_b30_80175540, not the
   D_80045340 that every other spawner in the corpus passes; the listing's
   `%hi/%lo(func_b30_80175540)' is unambiguous, so the forward declaration of
   that already-landed body is new in the declaration block.

   MEASURED, all three load-bearing:
   | spelling                                          | words | aligned |
   | as written below                                  |   79  |    0    |
   | `*(s16 *)(r + 0x1E) = *(s16 *)(r + 0x1C) = 0x1000'|   79  |    2    |
   | +0x28 and +0x12 read off `a->unk_0C' not the      |       |         |
   |   payload reload                                  |   79  |    2    |
   | the three mover halves read off the +0x18 payload |       |         |
   |   reload instead of `a->unk_08'                   |   79  |    3    |
   The middle two are the same mechanism in opposite directions and the
   target picks a different one at each site: the +0x28/+0x12 pair RELOADS
   the payload word it has just written (two `lw 0x14($s0)'), while the three
   mover copies re-read the PARAMETER's `a->unk_08' (three `lw 0x8($s3)')
   even though the same pointer is sitting in the payload at +0x18. Read
   which base each `lw' uses; there is no house rule here. */
void func_b30_80174B90(UnkNode *a, u8 *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *r;

    n = func_8003FD64(0x100, a);
    if (n != NULL)
    {
        r = (u8 *)n->unk_0C;
        n->unk_10 = (u32)func_b30_8017474C;
        q = (u8 *)n + 0x20;
        *(u32 *)(q + 0x14) = a->unk_0C;
        *(UnkVec **)(q + 0x18) = a->unk_08;
        *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s32 *)(r + 0x28) = *(s32 *)(*(u8 **)(q + 0x14) + 0x28);
        *(s16 *)(r + 0x12) = *(u16 *)(*(u8 **)(q + 0x14) + 0x12);
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA);
        func_80047784(r, 0x46, 0);
        func_8004491C(n, (u32)func_b30_80175540);
        *(u8 **)(q + 0x0) = b;
        *(u8 **)(q + 0x4) = c;
        *(u8 **)(q + 0x8) = d;
        *(UnkNode **)(q + 0xC) = a;
        *(s16 *)(q + 0x1C) = 0x1000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80174CCC);

/* Probe the cell one step away in direction `dir': ask func_8009A350 for the
   destination tile's flag word, reject it if any of 0xB700 is up, then write
   func_800BCB04's height for the cell centre through the fifth argument and
   return whether that height is below 0x200. 64 words, 0 differing.
   1 copy (dngfam.py).

   NO TWIN at any ratio and no donor needed: the step table pair
   D_8006CCD8 / D_8006CCE8 and the `((v + table[dir]) << 6) + 0x20) & 0xFFE0'
   cell-centre form are already in this unit's declaration block and in
   csrc/ovl_dungeon.c's func_8009A350 callers.

   THE ONE FIGHT WAS THE RETURN SHAPE, and it is a jump.c result worth
   carrying. Written with three separate early `return 0's -- the obvious
   reading of the listing's two `addu $v0, $zero, $zero' sites -- the body is
   61 words / 14 aligned differences: with a single-predecessor `v0 = 0'
   block, jump_optimize rewrites `if (c) return 1; return 0;' into a bare
   `slti' and the branch, the `addiu 1' and the second zero all vanish. The
   target keeps them, so its zero block must have SEVERAL predecessors --
   hence ONE trailing `return 0' with the two rejections falling into it. The
   first rejection's `$v0 = 0' in the `beqz' delay slot is reorg stealing
   that block's first insn, not a second zero in the source.

   MEASURED AND FREE, so neither is the reason this lands:
   `s16 z' instead of `s32 z' (64 / 0 -- combine drops whichever of the two
   sign-extensions is redundant, so the width is invisible here), and the
   `&&' written as a nested `if' with its own `return 0' (64 / 0, as expected
   for two tests on different operands). */
s32 func_b30_8017506C(s16 x, s16 y, s32 z, s16 dir, s16 *out)
{
    u16 w;

    if (func_8009A350(x, y, dir, &w) != 0 && (w & 0xB700) == 0)
    {
        *out = func_800BCB04((((x + D_8006CCD8[dir]) << 6) + 0x20) & 0xFFE0, (((y + D_8006CCE8[dir]) << 6) + 0x20) & 0xFFE0, z - 0x20);
        if (*out < 0x200)
        {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017516C);

/* Walk the tag chain from `p` and hand every node to func_b30_80175594 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_b30_80175540(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_b30_80175594(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80175594);

/* A thrown-object step: advance the mover by its velocity, decay the two
   horizontal velocity components by 1/32 per frame, run the +0x20 countdown
   and raise 0x8000 in the node's -0x2 and in D_800814A0 when it expires;
   while it is alive, push the slot's three scale/offset halfwords out and
   re-aim the +0x18 sprite from the facing the pointer at the payload's +0x0
   names, nudging it by 0x100 at the two cardinal values 0x400 and 0xC00.
   69 words, 0 differing. 1 copy (dngfam.py).

   NO TWIN -- dngtwin reports nothing. The mover step and the teardown are
   func_b30_801763D0's (44 words, in this unit) and the facing index is the
   unit's own `(D_80083228 + <heading> + 0x100) >> 9) & 7' idiom, here
   reached through a POINTER in the payload rather than off `d + 0x2A'.

   THE WHOLE FIGHT WAS ONE LOCAL'S DECLARED TYPE, and all three widths are
   distinguishable -- this is the sharpest case of the narrow-local lever
   measured so far, because the two narrow widths differ from each other:

   | spelling                                  | words | aligned diffs |
   | `u16 v' (as written below)                |   69  |      0        |
   | `s16 v'                                   |   69  |      6        |
   | `s32 v'                                   |   68  |      3        |
   | dress stores in +0x1C, +0x1A, +0x1E order |   69  |      8        |
   | the two 1/32 decays in the other order    |   69  |     10        |

   `s32 v' is ONE WORD SHORT: the target copies the shift result into a
   second register for the two compares (`addu $a0, $v1, $zero') and with a
   word-wide local there is only one pseudo. A narrow local supplies the
   copy -- the extension is a plain `move' because gcc knows `(k & 7) + 2'
   shifted left 9 cannot set bit 15. But `s16' puts the COPY in $v1 and the
   shift result in $a0, the exact reverse of the target, for a bit-identical
   6-row residue that is a pure two-register exchange with nothing moved;
   `u16' assigns them the other way and lands. Declaration order was
   permuted first and all four two-local splits measured the identical 6,
   which is what ruled allocation priority out and sent me to the width.

   THE THREE DRESS STORES ARE INDEPENDENT, so their source order is priced
   and not readable: the target EMITS +0x1C, +0x1A, +0x1E and the source that
   produces it is +0x1C, +0x1E, +0x1A -- sched1 transposes the last two. The
   other four permutations are 8, 15 and 15.

   The +0xC velocity is READ TWICE in the target (two `lw 0xC($a1)') and the
   +0x10 velocity once, which is why the two decays must stay in +0xC then
   +0x10 order; reversed it is 10 rows. */
void func_b30_80175C60(u8 *p, UnkVec *b, u8 *c)
{
    u16 v;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C += b->unk_0C >> 5;
    b->unk_10 += b->unk_10 >> 5;
    if ((s16)(--*(u16 *)(p + 0x20)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x80;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x80;
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x40;
        v = ((((D_80083228 + *(s16 *)(*(u8 **)(p + 0x0)) + 0x100) >> 9) & 7) + 2) << 9;
        *(s16 *)(c + 0x18) = v;
        if (v == 0x400 || v == 0xC00)
        {
            *(s16 *)(c + 0x18) = v + 0x100;
        }
    }
}

/* Zero the scratchpad word at 0x1F80013C, then walk the tag chain from `p'
   and hand every node to func_800453E0 with -0x10 as the fourth argument.
   23 words, 0 differing, on the first compile.

   THE BEST TWIN IS func_b30_80175540, FOUR LISTINGS UP IN THIS FILE: the
   same walk with a different per-node call and no scratchpad store, which
   is the whole 21 -> 23 word delta. (dngtwin named func_s72_80025034 at
   0.909, which is that same body in csrc/ovl_dng_s72.c; the in-unit one is
   the same shape and was already here.)

   MEASURED AND FREE, so NOT the reason this lands:
     - the scratchpad address written as the absolute `*(s32 *)0x1F80013C'
       rather than through a pointer local. Through
       `sp = (u8 *)0x1F800000; *(s32 *)(sp + 0x13C) = 0;' it is 23 words /
       0 differing. That REFINES the lever func_b06_80173E08 and
       func_b08_801740F4 record: the pointer local is needed only when the
       store is INSIDE the loop, where gas's `lui $at'/`sw' macro cannot be
       hoisted. Here the store is before the loop and both spellings are
       the same two words.
     - `-0x10' versus `0xFFF0' as the fourth argument: 23 words /
       0 differing either way, the `s16' parameter truncating both the same.

   LOAD-BEARING: the `for (;;)' with the NULL test in the MIDDLE of the body.
   That is what puts the `addiu $s0, $a2, 0x20' in the `beqz's delay slot.
   Rewritten as a peeled first call plus a `while (n != NULL)' it is
   24 words / 18 differing. */
s32 func_b30_80175D74(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    *(s32 *)0x1F80013C = 0;
    for (;;)
    {
        func_800453E0(p, v, s, -0x10);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* The 0x300 spawner for func_b30_80175C60 (69 words, two listings down):
   project the caller's slot through func_8003DE58, and if that succeeds
   allocate a node behind the caller's own, dress its slot at 0x400 scale
   with the 0x808080 grey and sprite 0x45, seed its mover from the caller's
   three halves plus the projection and from the two resident step tables at
   the actor's facing, park `p + 0x2A' in the payload at +0x0 and the
   0x10-frame countdown at +0x40, then aim the +0x18 sprite exactly the way
   func_b30_80175C60 re-aims it every frame. 104 words, 0 differing.
   1 copy (dngfam.py).

   NO TWIN -- dngtwin reports nothing for this stub. The func_8003DE58 /
   SVECTOR / `(u16)out.vx' block is csrc/ovl_dng_b29.c's func_b29_80175AF4
   (found by grepping func_8003DE58); the tail is this unit's own
   func_b30_80175C60, which is why the +0x18 aim needed no work at all.

   | spelling                                        | words | aligned |
   | as written below                                |  104  |    0    |
   | the facing index inline at BOTH table sites     |  106  |   23    |
   | `v = ... *(s16 *)(p + 0x2A) ...' in the tail    |  103  |    4    |
   | `s16 v'                                         |  104  |    6    |
   | `s32 v'                                         |  103  |    5    |

   THE INDEX NEEDS A LOCAL, and this is the clean case of the cse rule: the
   two table reads are separated by the `sh $v0, 0xE($a3)' store, which
   invalidates the `lhu 0x2A($s4)' entry, so written inline the whole
   `lhu / srl / andi' chain is emitted TWICE and the body is two words long.
   The target shares one `$v1', so the source shares one local.

   `u16 v' in the tail is the SAME fact func_b30_80175C60 states and it
   reproduces to the digit -- `s16' is 6 aligned rows there and 6 here, a
   pure two-register exchange both times. The tail must also RELOAD the
   payload word it has just written rather than use `p + 0x2A' again; that is
   one word and 4 rows, the same shape as func_b30_80174690's +0x28 reload.

   MEASURED AND FREE, so neither is the reason this lands: reading the two
   step tables as their declared `s16' arrays instead of through a `(u16 *)'
   cast (104 / 0 -- the listing's `lhu' is the halfword store's truncation,
   not the load's signedness), and `s16 k' for `s32 k' (104 / 0). */
void func_b30_80175DD0(u8 *p, UnkVec *b, u8 *c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u16 v;
    s32 k;
    SVECTOR out;

    if (func_8003DE58(*(u8 **)(c + 0x8), c, &out, d) != NULL)
    {
        n = func_8003FD64(0x300, p - 0x20);
        if (n != NULL)
        {
            r = (u8 *)n->unk_0C;
            m = n->unk_08;
            n->unk_10 = (u32)func_b30_80175C60;
            *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x400;
            *(u32 *)(r + 0xC) = 0x808080;
            *(s32 *)(r + 0x28) = *(s32 *)(c + 0x28);
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u16)out.vx;
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u16)out.vy;
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA) + (u16)out.vz;
            k = (*(u16 *)(p + 0x2A) >> 9) & 7;
            *(s16 *)((u8 *)m + 0xE) = D_8006CCD8[k];
            *(s16 *)((u8 *)m + 0x12) = D_8006CCE8[k];
            *(s16 *)((u8 *)n + 0x40) = 0x10;
            *(u8 **)((u8 *)n + 0x20) = p + 0x2A;
            func_80047784(r, 0x45, 0);
            func_8004491C(n, (u32)func_b30_80175D74);
            v = ((((D_80083228 + *(s16 *)(*(u8 **)((u8 *)n + 0x20)) + 0x100) >> 9) & 7) + 2) << 9;
            *(s16 *)(r + 0x18) = v;
            if (v == 0x400 || v == 0xC00)
            {
                *(s16 *)(r + 0x18) = v + 0x100;
            }
        }
    }
}

/* A four-state fade handler on the node's own +0x1E: state 0 eases the three
   +0xC..+0xE greys towards 0x80 by (0x80 - v) / ++c[0xF] and snaps them to
   0x808080 when the divisor reaches 8; state 1 runs the +0xF countdown down
   and on zero re-tags the node onto D_80045340 with +0x12 pushed back and the
   0xC bits dropped; state 2 waits for the object at `*(u8 **)(p + 0x4)' to
   report kind 0x14, then re-tags onto func_800C6AEC with the opposite +0x12 /
   +0x14 edit; state 3 eases the greys back to 0 by `-v / ++c[0xF]' and, past
   8, raises 0x8000 in the node's -0x2 and in D_800814A0 and calls
   func_800BC318. Every path ends in func_800478B8(c). 135 words,
   0 differing, FIRST COMPILE. 1 copy (dngfam.py).

   THIS BODY IS A BLEND, which is why dngtwin reports nothing for it at any
   ratio: states 0 and 1 are csrc/ovl_dungeon_800A4B88.c's func_800ACE34 and
   states 2 and 3 are func_800ACBE4's two states, from the same resident
   file. Found by grepping func_800C6AEC (5 hits corpus-wide). Both donors'
   stated facts HELD unchanged, and the four I priced on this body are:

   | spelling                                      | words | aligned |
   | as written below                              |  135  |    0    |
   | `c[0xE] -= c[0xE] / c[0xF]' in state 3        |  135  |    4    |
   | `c[0xE] = c[0xD] = c[0xC] += -c[0xC] / ...'   |  135  |    3    |
   | `switch (*(u16 *)(p + 0x1E))'                 |  135  |    1    |
   | state 0's three eases in +0xE..+0xC order     |  135  |    4    |

   The negate-then-divide is func_800ACBE4's fact reproduced: the target has
   `negu'/`div' where the subtract form is `div'/`subu'. The dispatch load is
   `lh' and the three state bumps are `lhu'/`addiu'/`sh', so the switch reads
   +0x1E SIGNED and the bumps read it UNSIGNED -- one word either way, but the
   unsigned switch costs a row.

   THE STATE BUMP IS WRITTEN OUT THREE TIMES, once at the end of each of
   states 0, 1 and 2, and jump2 cross-joins the three copies into the single
   block the listing shows between state 2's arm and state 3's. There is no
   `goto' and no shared tail in the source. */
void func_b30_80175F70(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;

    switch (*(s16 *)(p + 0x1E))
    {
    case 0:
        c[0xF] += 1;
        c[0xC] += (0x80 - c[0xC]) / c[0xF];
        c[0xD] += (0x80 - c[0xD]) / c[0xF];
        c[0xE] += (0x80 - c[0xE]) / c[0xF];
        if (c[0xF] >= 8)
        {
            *(u32 *)(c + 0xC) = 0x808080;
            *(u16 *)(p + 0x1E) += 1;
        }
        break;
    case 1:
        if (--c[0xF] == 0)
        {
            n = (UnkNode *)(p - 0x20);
            func_80044A50(n);
            *(u16 *)(c + 0x12) += 0x80;
            *(u16 *)(c + 0x14) &= 0xFFF3;
            func_8004491C(n, (u32)D_80045340);
            *(u16 *)(p + 0x1E) += 1;
        }
        break;
    case 2:
        if (**(u8 **)(p + 0x4) == 0x14)
        {
            n = (UnkNode *)(p - 0x20);
            func_80044A50(n);
            *(u16 *)(c + 0x12) -= 0x80;
            *(u16 *)(c + 0x14) |= 0xC;
            func_8004491C(n, (u32)func_800C6AEC);
            c[0xF] = 0;
            *(u16 *)(p + 0x1E) += 1;
        }
        break;
    case 3:
        if (c[0xF] < 8)
        {
            c[0xF] += 1;
            c[0xC] = c[0xD] = c[0xE] += -c[0xE] / c[0xF];
        }
        else
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            func_800BC318(p - 0x20);
        }
        break;
    }
    func_800478B8(c);
}

/* Build the entity the actor's +0x60 names and return its node. The field is
   a union: a POSITIVE value is a D_800E3548 spot index and goes through
   func_800A7A38 into func_800B23F8, while a non-positive one is a POINTER to
   an actor whose resource func_800A1618 loads (kind 1, falling back to kind
   3) and whose builder func_800A0B94 returns to be called through. Either way
   the new entity gets +0x14/+0x1C cleared, func_80042710's copy from the
   caller and the 0x20000000 / 0x2000x00 flag pair; then, if the caller
   carries 0x4000, the node is published into D_800E3D7C's +0xAC slot with the
   old occupant pushed to +0xE4. 145 words, 0 differing. 1 copy (dngfam.py).

   NO TWIN -- dngtwin reports nothing. Found by the callee-set intersection:
   six of eight symbols hit csrc/ovl_dungeon_800A4B88.c, whose func_800A504C
   supplies the func_800A1618 / func_800A0B94 call-through-the-result idiom,
   the Unk42710 casts and the D_800E3D7C publish.

   | spelling                                          | words | aligned |
   | as written below                                  |  145  |    0    |
   | ONE local: `q = *(u8 **)(d + 0x60)' tested direct |  144  |    4    |
   | `g = D_800E3D7C' read BEFORE `k = ...'            |  144  |   48    |
   | no `g' local, the symbol inline in the sum        |  145  |    2    |
   | `e = g + k * 4' (pointer sum)                     |  145  |    4    |
   | the tail as `if (w == NULL) return NULL; ...'     |  145  |    3    |

   FOUR FACTS WORTH CARRYING, and three are from the levers file verbatim.
   (1) TWO LOCALS FOR +0x60: the target loads it into $a0, tests it there and
   COPIES it into $s1 inside the else arm, which a single pointer local
   cannot produce -- it goes straight into the callee-saved register and the
   body is a word short. So the source tests an integer and casts to a
   pointer in the arm that uses it.
   (2) THE `addu's OPERAND ORDER: the target computes `k * 4 + D_800E3D7C'
   INDEX FIRST, which `pointer_int_sum' cannot emit, so the sum is written
   through `(s32)' and not as a pointer sum. 4 rows.
   (3) THE SYMBOL READ NEEDS ITS OWN STATEMENT, AND IT MUST COME AFTER THE
   INDEX. A `SYMBOL_REF' costs no insn at expand time, so inline it has no
   LUID and the `lui' lands one slot late (2 rows); hoisted ABOVE the
   func_800A1BD0 call it has to survive the call and the body loses a word
   for 48 rows. Between the call and the multiply is the only place it works.
   (4) THE TAIL IS A TERNARY. `if (w == NULL) return NULL;' puts the `$v0 = 0'
   in its own block and emits `bnez'; the ternary gives the target's `beqz'
   with the zero in the delay slot. 3 rows. */
UnkNode *func_b30_8017618C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *q;
    u8 *w;
    u8 *(*f)(s32, s32, s32, s32);
    u8 *e;
    s16 k;
    s32 t;
    s32 r;
    u8 *g;

    r = *(s32 *)(d + 0x60);
    if (r > 0)
    {
        w = func_800B23F8((*(u32 *)(d + 0x1C) >> 13) & 1, c[0x24], c[0x25], *(s16 *)(d + 0x88),
                          func_800A7A38((u8 *)&D_800E3548_w[(s16)(*(u16 *)(d + 0x60) - 1)]));
        if (w != NULL)
        {
            *(s32 *)(w + 0x14) = 0;
            *(s32 *)(w + 0x1C) = 0;
            func_80042710((Unk42710 *)w, (Unk42710 *)d);
            *(u32 *)(w + 0x14) |= 0x20000000;
            *(u32 *)(w + 0x1C) = (*(u32 *)(w + 0x1C) | 0x2000200) & 0xFFFEFFFF;
        }
    }
    else
    {
        q = (u8 *)r;
        D_800E2968 = 1;
        w = func_800A1618(q[0x13], 1);
        if (w == NULL || w[1] == 0)
        {
            w = func_800A1618(q[0x13], 3);
            if (w == NULL)
            {
                return NULL;
            }
        }
        func_8004397C(q);
        f = (u8 * (*)(s32, s32, s32, s32)) func_800A0B94(q[0x13], w, 1);
        func_8003F320();
        w = f((*(s32 *)(d + 0x1C) & 0x2000) ? 7 : 4, c[0x24], c[0x25], *(s16 *)(d + 0x88));
        if (w != NULL)
        {
            func_80042640((Unk42710 *)w, w[0x13]);
            *(s32 *)(w + 0x14) = 0;
            *(s32 *)(w + 0x1C) = 0;
            func_80042710((Unk42710 *)w, (Unk42710 *)d);
            w[0x28] += 1;
            w[0x13] = q[0x13];
            *(s16 *)(*(u8 **)(w - 0x14) + 0x12) = *(u16 *)(*(u8 **)(q - 0x14) + 0x12) & 3;
            *(u32 *)(w + 0x14) |= 0x20000000;
            *(u32 *)(w + 0x1C) = (*(u32 *)(w + 0x1C) | 0x2000000) & 0xFFFEFFFF;
        }
    }
    if ((*(s32 *)(d + 0x14) & 0x4000) != 0)
    {
        k = (s16)func_800A1BD0(d);
        g = D_800E3D7C;
        e = (u8 *)(k * 4 + (s32)g);
        t = *(s32 *)(e + 0xAC);
        *(s32 *)(e + 0xAC) = (s32)w;
        *(s32 *)(e + 0xE4) = t;
    }
    return w != NULL ? (UnkNode *)(w - 0x20) : NULL;
}

/* The +0x10 driver of the sixteen 0x300 nodes func_b30_80176480 (84 words,
   two listings down) spawns: run the +0x20 countdown, and while it is still
   positive fade the slot's three greys towards 0 in proportion to it and
   step the mover by its own velocity under a 0x60000 gravity; when it runs
   out raise 0x8000 in the node's -0x2 and in D_800814A0 so the engine reaps
   it. 44 words, 0 differing, first compile. 1 copy (dngfam.py).

   NO TWIN AT ANY RATIO -- dngtwin reports nothing for this stub and the body
   has no calls, so the callee-set intersection cannot run either. What found
   it was a grep for the constant 0x60000, which led to func_b43_80172404's
   `b->unk_14 += 0x60000' gravity, and the teardown couple is the
   corpus-wide `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000' idiom
   (func_b01_801708E8 and 25 others).

   LOAD-BEARING, MEASURED: the chain must be written `c[0xE] = c[0xD] =
   c[0xC] = ...', i.e. with the INNER lvalue at +0xC, because the target
   stores +0xC, +0xD, +0xE in that order. `c[0xC] = c[0xD] = c[0xE] = ...'
   is 44 words / 4 aligned differences. (This is the opposite direction from
   func_b01_801708E8's, which wants +0xE first -- the rule is the inner
   lvalue stores first, not a house order.)

   MEASURED AND FREE, so it is NOT the reason this lands: the countdown
   spelled `t = --*(s16 *)(p + 0x20);' with no cast, 44 words / 0 aligned
   differences. The `(s16)(--*(u16 *)...)' form below is what the listing
   reads literally -- `lhu' and a `sll 16'/`sra 16' AFTER the `sh' -- but
   combine drops the redundant extension on the signed load, so the lvalue's
   width is free here. */
void func_b30_801763D0(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    t = (s16)(--*(u16 *)(p + 0x20));
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        c[0xE] = c[0xD] = c[0xC] = c[0xC] - c[0xC] / t;
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        b->unk_14 += 0x60000;
    }
}

/* Spray sixteen 0x300 particle nodes driven by func_b30_801763D0 (44 words,
   two listings down): each gets the caller's +0x28 clip word and +0x12 pulled
   back 0x80, a 0x800 scale pair, the 0x808080 grey, sprite 0x41, and a mover
   seeded from the caller's three halves with four random perturbations -- the
   height pulled up by rand() & 0x1F, a doubled negative Z step, and two
   whole-unit X/Z velocities. Each node's payload +0x20 gets the 0x10-frame
   countdown func_b30_801763D0 runs down. 84 words, 0 differing. 1 copy
   (dngfam.py).

   NO TWIN -- dngtwin reports nothing. It is func_b30_80174690's spawner
   skeleton (in this unit) wrapped in the count loop, and func_80069EF8 is
   `rand' in splat/common_syms.txt, so it needs no declaration.

   | spelling                                          | words | aligned |
   | as written below                                  |   84  |    0    |
   | the 0x14 `|=' BEFORE the 0x12 store               |   84  |   13    |
   | `for (i = 0; i < 0x10; i++)'                      |   85  |    4    |
   | `s16 i'                                           |   87  |    6    |
   | `*(s16 *)(r + 0x1E) = *(s16 *)(r + 0x1C) = 0x800' |   84  |    2    |
   | `-(((rand() & 0xF) + 0x8) << 1)'                  |   84  |    3    |
   | 0xC/0x10/0x28 in the other five orders            |   84  | 2,10,14,15,16,16,17,19 |

   TWO THINGS WORTH CARRYING. First, THE SOURCE WRITES +0x12 BEFORE +0x14
   ALTHOUGH THE LISTING EMITS +0x14 FIRST -- sched1 transposes the two
   independent stores because the +0x12 one ends up in the `jal's delay slot.
   func_b30_80174690 in this same unit has the same pair of stores and there
   the source order and the emitted order AGREE, so this is not a bank idiom:
   it is priced per site, and the full 3x2 sweep over the five dress
   statements is in the table above. Second, THE LOOP COUNTS DOWN IN THE
   SOURCE. `for (i = 0; i < 0x10; i++)' is one word LONG with an
   `slti'/`bnez' pair where the target has a bare `bgtz', i.e. check_dbra_loop
   did NOT reverse it -- so the brief's "an emitted count-down means the
   source counts up" is a prior and not a law; here the down-count is the
   source. `s16 i' is three words long, the counter being re-extended inside
   the loop. */
void func_b30_80176480(UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    s32 i;

    for (i = 0x10; i > 0; i--)
    {
        n = func_8003FC64(0x300);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_b30_801763D0;
            func_8004491C(n, (u32)D_80045340);
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x800;
            *(u32 *)(r + 0xC) = 0x808080;
            *(s16 *)(r + 0x10) = 0x20;
            *(s32 *)(r + 0x28) = *(s32 *)(c + 0x28);
            *(s16 *)(r + 0x12) = *(u16 *)(c + 0x12) - 0x80;
            *(u16 *)(r + 0x14) |= 0xC;
            func_80047784(r, 0x41, 0);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA) - (rand() & 0x1F);
            *(s16 *)((u8 *)m + 0x16) = (-(rand() & 0xF) - 0x8) << 1;
            m->unk_0C = ((rand() & 0x3) - 0x2) << 16;
            m->unk_10 = ((rand() & 0x3) - 0x2) << 16;
            *(s16 *)((u8 *)n + 0x40) = 0x10;
        }
    }
}
