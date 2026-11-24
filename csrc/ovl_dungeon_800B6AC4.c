#include "common.h"
#include "ovl_dungeon_decls.h"

extern u32 D_800835E4[];
extern u8 D_800DD108[];
extern u8 D_800DD110[];
extern u8 D_800DD118[];
extern u8 D_800E0A2A[];
extern u8 D_800E0A33[];
extern u8 D_800E2968;

/* D_800814A0 IS ALREADY `extern u32' IN include/structs.h, so a second
   declaration of the bare name is a conflicting-types error.  The `__asm__'
   alias is how this repo gives one symbol a second spelling, and csrc/main.h
   does exactly this for this very symbol with `extern u32 D_800814A0_a[1]'.
   Only func_800B6AC4's case 10 uses the const spelling.  The other six
   D_800814A0 sites in this unit are landed through the bare name and must
   stay that way.

   WHAT THE `const' BUYS IS THE LAST TWO ROWS OF func_800B6AC4.
   varasm.c:773 sets `RTX_UNCHANGING_P' on a readonly decl's MEM, and
   `anti_dependence' (sched.c:866) returns 0 outright when the READ is
   unchanging.  That drops the only link between the `lw %lo(D_800814A0)' and
   the `sw 0x2C' table store, and that link is what blocked the load on the
   memory unit (mips.md:155, multiplicity 1) in the cycle after the store and
   handed the slot to the table's `addiu %lo' -- which inverted two sched2
   fillers at aligned 763 and 769.  The STORE casts the const away, so it
   stays an ordinary changing reference and the output dependence that pins it
   after the table store survives.  With the bare `extern u32' name on the
   same two statements the arm is 2 aligned rows, with this one it is 0. */
extern const u32 D_800814A0_c __asm__("D_800814A0");
typedef u8 *(*UnkDungeonSpawner)(s32, u8, u8, s16);
void func_800B6AC4(u8 *p, UnkVec *b, u8 *c, u8 *d);

/* dungeon.bin's overlay, translation unit 3 of 3: func_800B6AC4 to the next
 * boundary.  THE SPLIT IS FORCED BY JUMP-TABLE ALIGNMENT, not by size --
 * see splat/dungeon.bin.yaml for the offsets and why each one is where it
 * is.  The prototypes below are what this unit used to get for free by
 * being in the same file as the definitions; they are generated from those
 * definition lines, so they say exactly what the single-unit compile said.
 */

/* func_800B69DC is defined in csrc/ovl_dungeon_800B45E0.c, the unit
   immediately before this one. The carve generated cross-unit prototypes
   only for callees that LANDED code mentions, and the sole caller here is
   func_800B6AC4, still INCLUDE_ASM -- so its park could not be measured at
   all until this line existed. */
void func_800B69DC(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_8008D330(UnkDungeonActor *p, void *b, void *c, void *d);
void func_80091934(UnkDungeonActor *p, void *b, void *c, void *d);
void func_80091A38(u8 *p, void *b, void *c, u8 *d);
void func_8009704C(u8 *p, UnkVec *b, u8 *c, u8 *d);
u8 *func_8009929C(s32 c, u8 *p);
u8 *func_800992E8(u8 *p, u8 *dst);
void func_8009A078(void);
s32 func_8009A180(u8 *a, u8 *b);
s32 func_8009FCAC(s16 i);
s16 func_800A07D0(s16 a, s16 b, s16 c, s16 d);
s32 func_800A0B94(s16 a, u8 *s, s16 c);
void func_800A0E44(void);
void func_800A1D4C(u8 *p, s16 f);
s32 func_800A384C(u8 *a, u8 *b, u8 *c, s32 s);
u8 *func_800A3D18(u8 *a, u8 *b, s16 c);
s16 func_800A48F0(s8 *p, s8 kind, s8 c);
s16 func_800A57B4(u8 *p, s16 n);
s32 func_800A6620(u8 *p, s32 f);
void func_800A6B70(u8 *p, s32 b, UnkDungeonSprite *s);
UnkNode *func_800A7700(s32 x, s32 y, s32 ang, s32 *q);
u32 func_800A7A38(u8 *p);
s32 func_800AD568(u8 *p);
void func_800B1B10(u8 *p, s16 x, s16 y, s16 d, s16 e, s16 f);
UnkNode *func_800B4C7C(u16 a, u8 *p, s16 v, s16 k);

/* The 12-case +0x9B state machine over jtbl_80089310.  812 words, 0 differing.
   FIVE SPELLINGS CARRIED IT, each measured, and the last of them is the one
   that had kept the park at 2 aligned rows for a day:

   (1) EVERY D_80083460 BASE AND EVERY SCRATCH POINTER NEEDS ITS OWN LOCAL --
       h/h2/h3/h4, r, e2/e7/e9/e10, n5, base9/base10.  One `h' shared by cases
       2, 3 and 7 crosses a call in case 3, so global alloc gives the single
       pseudo a callee-saved register everywhere and case 2's
       `lui/addiu D_80083460' comes out in $s0 where the target has $v1.
       Splitting them took the function from 280 aligned rows to 96 in one
       step and, with it, fixed the whole parameter register map.
   (2) CASE 2's FIRST D_80013714 READ IS THE CONSTANT ADDRESS
       `*(u16 *)0x80013714' and the other four are structs.h's SYMBOL.  As the
       symbol the `lui' is two insns and reorg steals one into the delay slot
       of the preceding `bnez $v0, .L800B7750', where the target has a `nop'.
       In case 7 the `%hi' is CSE'd into $s3 and serves three reads, which the
       assembler macro cannot do, so that site must NOT be the constant.
   (3) CASE 10's d+0x2A LOAD IS `((s16 *)d)[0x15]', NOT `*(s16 *)(d + 0x2A)'.
       expr.c:5344 sets MEM_IN_STRUCT_P when the TOP NODE of the INDIRECT_REF's
       address is a PLUS_EXPR, and that is what lets true_dependence's
       in-struct exception sink the D_800814A0 store past the load.  The
       flagless cast is 818 words and 46 rows on this body.
   (4) CASE 10's TABLE STORE IS `*(u8 **)(c + 0x2C)', NOT `((u8 **)c)[0xB]'.
       Here the SAME flag is wanted ABSENT.  The array form sets it, the
       store's dependences on the D_800814A0 pair vanish and the store floats.
       152 bodies with the array form measure 9 rows and the same bodies with
       the pointer cast measure 2.
   (5) CASE 10 READS D_800814A0 THROUGH THE const ALIAS `D_800814A0_c' AND
       WRITES IT THROUGH `*(u32 *)&D_800814A0_c'.  The alias carries its own
       comment at the top of this file.  With the bare name on the same two
       statements the arm is 2 aligned rows, with the alias it is 0.

   WHAT DOES NOT WORK, so that it is not tried again.  On the 2-row body all
   five sweeps come back empty -- widthsweep, stmtsweep, refsweep over 28
   names and 7680 insertion points, devsweep over 4959 wrappers a round, and
   partsweep.  So do 420 legal statement orders of case 10 and a 150-minute
   permuter run of 219965 candidates whose best find measured 19 rows.  The
   residue was a transposition of the two sched2 fillers at aligned 763 and
   769, and every way of de-birthing the table pseudo to fix it costs more
   than it buys -- a cancelled `t10++; t10--;' 13 rows, a real second set
   sharing another arm's table 19 to 22, a read-modify-write 27 rows at 814
   words (case 8 cross-jumps into case 10's tail at .L800B770C, which pins the
   call's index to `t10[idx]'), and a `register u8 *t10 __asm__("$5")' pin 6.
   Lever (5) fixes the transposition without touching that pseudo at all. */
void func_800B6AC4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *q;
    u8 *g;
    u8 *h;
    u8 *h2;
    u8 *h3;
    u8 *h4;
    u8 *n;
    u8 *m;
    u8 *r;
    u8 *e2;
    u8 *e7;
    u8 *e9;
    u8 *e10;
    u8 *n5;
    u8 *base9;
    u8 *base10;
    u8 *t10;
    u8 *w;
    void *s;
    s16 k;
    s32 x;
    u32 y;
    s32 v;
    s32 z;
    s16 out[4];

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        q = &D_80083160;
        if ((((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) == 2)
        {
            *(u8 **)(c + 0x2C) = D_800DD108;
            func_80048A44(c, D_800DD108[((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(s16 *)(p + 0x96) = 0x12;
            p[0x9B]++;
        }
        else
        {
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
        }
        break;
    case 1:
        if (--*(s16 *)(p + 0x96) == 0)
        {
            func_800A56E0(0x701);
        }
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u8 **)(c + 0x2C) = D_800DD110;
            func_80048A44(c, D_800DD110[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
            if ((D_80013714 & 1) == 0)
            {
                func_800B1768(0x22, 0xF2, 0xBA, 0x105, 0, 8);
                func_800B1768(0x23, 0xE0, 0xCE, 0x104, 1, 8);
            }
            ((u8 **)p)[0x30] = *(u8 **)(p + 0xBC);
            *(u32 *)D_80081484 = **(u32 **)(p + 0xBC);
            ((UnkDungeonActor *)p)->unk_8C = func_800B69DC;
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0x2) |= 4;
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x6800;
        }
        if (D_80083462 & 4)
        {
            break;
        }
        k = 0;
        if (*(u16 *)0x80013714 & 1)
        {
            r = func_8009F868();
            if (r == NULL)
            {
                func_800B1768(0x22, 0xF2, 0xBA, 0x105, 0, 8);
                func_800B1768(0x23, 0xE0, 0xCE, 0x104, 1, 8);
                break;
            }
            D_800E3544 = r[1] & 0xF8;
            if (D_800E3544 == 0x60)
            {
                k = 1;
            }
            else
            {
                func_8009F988();
                *(u8 **)(c + 0x2C) = D_800DD118;
                func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
                *(u16 *)(c + 0x14) &= 0xF7FF;
                *(s32 *)(p + 0x8C) = 0;
                p[0x9B] = 4;
                break;
            }
        }
        else
        {
            if (*(s32 *)(g + 0x8) & 0x40)
            {
                k = 1;
            }
        }
        if (k)
        {
            func_8009F644(d, 0x60, 0, 0);
            func_800A56E0(0x500);
            *(u16 *)(c + 0x14) &= 0x97FF;
            p[0x9B] = 3;
            y = (func_800A6D30() & 3) + 1;
            (*(u8 **)(p + 0xBC))[2] -= y;
            e2 = *(u8 **)(p + 0xBC);
            if (*(s8 *)(e2 + 0x2) <= 0)
            {
                e2[2] = 0;
                *(u8 **)(c + 0x2C) = D_800DD118;
                func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
                *(u16 *)(c + 0x14) &= 0xF7FF;
                p[0x9B] = 5;
                *(s16 *)(d + 0x8A) = func_8009904C(*(u8 **)(p + 0xBC));
            }
            func_800B4C7C(0xA4, d, (s16)(0x64 - *(s8 *)(*(u8 **)(p + 0xBC) + 0x2)), 0);
        }
        else
        {
            if (D_80013714 & 1)
            {
                break;
            }
            if ((*(s32 *)(g + 0x10) & 0x20) == 0)
            {
                break;
            }
            func_8009F644(d, 0xA8, 0, 0);
            *(u8 **)(c + 0x2C) = D_800DD118;
            func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800A56E0(0x515);
            *(s32 *)(p + 0x8C) = 0;
            p[0x9B] = 4;
        }
        break;
    case 3:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            h2 = (u8 *)&D_80083460;
            if ((*(u16 *)(h2 + 0x2) & 2) == 0)
            {
                func_80099F70(*(u8 **)(d + 0x5C));
                func_80099F04(*(u8 **)(d + 0x5C));
                *(u16 *)(h2 + 0x2) |= 0x812;
            }
            p[0x9B] = 2;
        }
        break;
    case 4:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            func_80098B38(D_80081484);
            ((UnkDungeonActor *)p)->unk_8C = func_8008ACDC;
            D_800E296C &= 0xFFDFFFFF;
        }
        break;
    case 5:
        n5 = (u8 *)func_80098CF8(p, b, c, D_80081484);
        if (n5 != NULL)
        {
            *(s16 *)(n5 + 0xCC) = 2;
            p[0x9A] = 0x22;
            p[0x9B] = 6;
        }
        break;
    case 6:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) == 0)
            {
                out[1] = 0;
                out[0] = 0;
                out[2] = -D_800DDC40[0];
            }
            func_800B8024(*(s16 *)((u8 *)b + 0x2) + out[0], *(s16 *)((u8 *)b + 0x6) + out[1],
                          *(s16 *)((u8 *)b + 0xA) + out[2]);
            p[0x9B]++;
        }
        break;
    case 7:
        D_800E2968 = 1;
        n = ((UnkDungeonSpawner)func_800A0B94((((u8 **)p)[0x30])[0],
                                              func_800A1618((((u8 **)p)[0x30])[0], 2), 1))(
            2, c[0x24], c[0x25], (s16)(*(u16 *)(d + 0x88) - 0x20));
        if (n == NULL)
        {
            break;
        }
        func_8003E188(n[0x13], 0);
        func_800A90E8(((u8 **)p)[0x30]);
        e7 = *(u8 **)(n - 0x14);
        func_8009A3D0(e7[0x24], e7[0x25], 0x300);
        if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) == 0)
        {
            out[1] = 0;
            out[0] = 0;
            out[2] = -D_800DDC40[0];
        }
        m = *(u8 **)(n - 0x18);
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u16)out[0];
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u16)out[1];
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA) + (u16)out[2];
        *(s16 *)(n + 0x88) = *(s16 *)(m + 0xA);
        *(u8 **)(d + 0x60) = n;
        func_80042640((Unk42710 *)n, n[0x13]);
        func_800424E0(n, n[0x13], NULL);
        n[0x43] = 0xFF;
        *(u8 **)(n + 0x60) = d;
        *(u32 *)(n + 0x1C) |= 0x80000;
        *(u32 *)(n + 0x14) |= 0x104000;
        *(u16 *)(*(u8 **)(d + 0x60) + 0x98) |= 0xC;
        strcpy(n + 0x34, func_8003C06C(n[0x13]));
        *(u32 *)(n + 0x18) = D_800835E4[D_800E3D7C[0x11]];
        while (D_800835E4[n[0x11] + 1] <= *(u32 *)(n + 0x18))
        {
            func_800A1D4C(n, 0);
        }
        n[0x28] = n[0x29];
        n[0x26] = n[0x68];
        n[0x27] = n[0x69];
        if ((D_80013714 & 1) == 0)
        {
            D_800DCF4F = 1;
            h3 = (u8 *)&D_80083460;
            *(u16 *)(h3 + 0xA) += 1;
        }
        s = func_800990FC();
        w = func_8009929C(8, s);
        w = func_80099194((u8 *)D_8007359C[0][n[0x13]].unk_04, w);
        w = func_80099194(D_800E0A2A, w);
        w = func_80099734(n, w);
        w = func_80099194(D_800E0A33, w);
        if ((D_80013714 & 1) == 0)
        {
            w = func_8009929C(0x11, w);
            w = func_8009929C(0x4C, w);
            w = func_80099254(D_800E0458, w);
        }
        func_80099290(w);
        func_800A5720(s);
        *(s16 *)(p + 0x96) = 0x20;
        p[0x9B]++;
        if ((D_80013714 & 1) == 0)
        {
            h4 = (u8 *)&D_80083460;
            if (*(s16 *)(h4 + 0xA) >= 2)
            {
                *(u16 *)(h4 + 0xA) -= 1;
            }
        }
        else
        {
            h4 = (u8 *)&D_80083460;
            if (*(s16 *)(h4 + 0xA) > 0)
            {
                *(u16 *)(h4 + 0xA) -= 1;
            }
        }
        break;
    case 8:
        if (D_8008346A == 0)
        {
            if (--*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0 &&
                *(s32 *)(g + 0x10) == 0)
            {
                break;
            }
            *(u8 **)(c + 0x2C) = D_800DD130;
            func_80048A44(c, D_800DD130[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 9:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u8 **)(c + 0x2C) = D_800DD138;
            func_80048A44(c, D_800DD138[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(c + 0x14) |= 0x200;
            func_80093C70(p, (u8 *)b, c);
            func_80093D8C(p, (u8 *)b, c);
            v = func_80099090();
            base9 = (u8 *)0x80010000;
            e9 = *(u8 **)(base9 + *(s16 *)(d + 0x8A) * 4 + 0x29C);
            e9[1] = 0x13;
            x = (((u8 **)p)[0x30])[0];
            e9[3] = v;
            e9[2] = 0;
            e9[0] = x;
            D_800E3DF0[v] = (s32)&D_800E3E48[v];
            D_800E3E48[v] = *(Unk800E3E48 *)*(u8 **)(d + 0x60);
            *(u32 *)(*(u8 **)(d + 0x60) + 0x1C) |= 0x400000;
            *(u32 *)(*(u8 **)(d + 0x60) + 0x14) &= 0xFFEFFFFF;
            func_800A56E0(0x704);
            p[0x9B]++;
        }
        break;
    case 10:
        if (*(u32 *)(*(u8 **)(d + 0x60) + 0x1C) & 0x800000)
        {
            func_8009A028(*(u8 **)(d + 0x60));
            base10 = (u8 *)0x80010000;
            e10 = *(u8 **)(base10 + *(s16 *)(d + 0x8A) * 4 + 0x29C);
            e10 = (u8 *)D_800E3DF0[e10[3] & 0x1F];
            *(u32 *)(e10 + 0x14) &= ~0x4000;
            *(u16 *)(*(u8 **)(d + 0x60) - 0x2) |= 0x8000;
            t10 = D_800DD140;
            z = D_800814A0_c;
            *(u8 **)(c + 0x2C) = t10;
            *(u32 *)&D_800814A0_c = z | 0x8000;
            func_80048A44(c, t10[((D_80083228 + ((s16 *)d)[0x15] + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 11:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u16 *)(c + 0x14) &= 0xFDFF;
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        }
        break;
    }
}

/* The mover-spawn state machine for a kind-0x12 node: case 0 asks
   func_8009B88C for a free cell and builds the mover, case 1 counts the
   +0xC timer down, case 2 hands the mover to func_800A3D18 and retires.
   251 words, 0 differing. FIVE SPELLINGS CARRIED IT, each measured:

   * D_80083460's base is THREE SEPARATE LOCALS, one per site. As one `f`
     reassigned in three blocks it is one pseudo, global-alloc gives the
     whole thing $a1, and the %hi lands in a different register from the
     %lo at every site: 250 words, 58 differing. Distinct locals give the
     target's $v1/$v0/$v1, each a two-insn split into ONE register, and the
     third site stops stealing the D_800E3D7C load's delay slot.
   * `g2 = D_800E3D7C;` BEFORE the +0xC store, with `g2` passed to
     func_8009A180. Reading the global inside the call puts the arg's
     lui/lw after the store's li/sh in the RTL and sched1 then leaves them
     there, so the delay slot gets `addu $a0, $s1, $zero` instead of the
     `sh`: 251 words, 6 differing.
   * `x`/`y` are copied into two `s32` temporaries BEFORE the mover call.
     Passed straight out of their `s16` slots the two `lh`s land at the
     argument setup (245 words, 212 differing); as `s16` temporaries they
     load `lhu` and cost a sll/sra pair each (252 words, 226). Only the
     s32 copies give the target's `lh $s0`/`lh $s1` ahead of the two calls.
   * the message handle is `h = func_800990FC(); mh = h;` with `h`
     REASSIGNED down the chain -- func_800956B8's shape (csrc:5127). As one
     nested expression `h` and `mh` are tied into $s0 and the argument is a
     copy out of it, where the target copies the return into $a1 and saves
     $a1 into $s0: 2 differing.
   * the tail's node pointer is the SAME local as the clamp's sum. With its
     own `u8 *` it stays in $s1 (the mover's register) instead of the
     target's $v0, and the clamp's copy to the index disappears entirely:
     see the DEVICE note below for why the sum must still be live later.

   The loop is `while (D_800835E8[q[0x11]] <= *(u32 *)(q + 0x18))`. Written
   `*(u32 *)(q + 0x18) >= D_800835E8[q[0x11]]` the operands are evaluated the
   other way round, sched1 fills the `lbu`'s delay slot with the reload and
   the loop is a word short -- which is the whole seven-word deficit this
   function was parked with, NOT the duplicated guard: the guard is
   `cse`-forwarded from the `sw` above it and comes out right either way. */
void func_800B7774(u8 *p, u8 *s, u8 *c)
{
    u8 *g;
    u8 *f;
    u8 *f2;
    u8 *f3;
    u8 *q;
    u8 *h;
    u8 *g2;
    u8 *mh;
    s16 x;
    s16 y;
    s32 x2;
    s32 y2;
    s32 m;
    u16 o;
    s16 n;
    s16 r;
    s32 i;

    g = (u8 *)&D_80083160;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if (func_8009B88C(NULL, *(u16 *)(s + 0x2) >> 6, *(u16 *)(s + 0x6) >> 6, &x, &y) == 0)
        {
            f = (u8 *)&D_80083460;
            *(s16 *)(f + 0xA) = *(u16 *)(f + 0xA) - 1;
            func_800997FC(D_800E0A42);
            *(s16 *)(p - 0x2) = *(u16 *)(p - 0x2) | 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        x2 = x;
        y2 = y;
        q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(p[0], func_800A1618(p[0], 2), 1))(
            2, x2, y2, (s16)(*(u16 *)(s + 0xA) - 0x20));
        *(u8 **)(p + 0x4) = q;
        if (q != NULL)
        {
            func_8003E188(q[0x13], 0);
            func_800A90E8(p);
            func_80042640((Unk42710 *)q, q[0x13]);
            func_800424E0(q, q[0x13], 0);
            q[0x43] = 0xFE;
            func_800A48F0((s8 *)q, 0x1B, 0);
            m = D_800E3D7C[0x11] + 0x32;
            i = m;
            /* DEVICE: the do/while(0) is worth the LAST WORD of this function
               and nothing else -- without it the function is still 251 words
               but `slti $v0, $v1, 0x64` reads the clamp result where the
               target reads the sum, 1 differing. local-alloc's
               `optimize_reg_copy_1` (local-alloc.c:704, reached from
               update_equiv_regs at :1028) sees `i = m` with `m` not yet dead,
               scans forward, finds m's REG_DEAD on the compare and rewrites
               the compare to read `i` -- and its scan BREAKS on a
               NOTE_INSN_LOOP_BEG (local-alloc.c:730), which a do/while(0)
               puts exactly between the copy and the compare. Read as the
               expansion of a clamp macro. Legal spellings ruled out, all 251
               words / 1 differing: `i = m; if (m >= 0x64)`, `if (m > 0x63)`,
               `if (0x63 < m)`, the unsigned compare, `if/else` with the two
               arms, `i = m < 0x64 ? m : 0x63`, a third variable for the
               compare, a dead `m = 0x63` in the arm, and the three loop forms
               `while (m >= 0x64) { i = 0x63; m = 0; }`, the same as a `for`,
               and a do/while(0) INSIDE the arm (the note has to sit between
               the copy and the compare, not after the jump). `i = m >= 0x64 ?
               0x63 : m` is 252 words / 234. */
            do
            {
                if (m >= 0x64)
                {
                    i = 0x63;
                }
            } while (0);
            *(u32 *)(q + 0x18) = D_800835E8[i];
            while (D_800835E8[q[0x11]] <= *(u32 *)(q + 0x18))
            {
                func_800A1D4C(q, 0);
            }
            h = func_800990FC();
            mh = h;
            h = func_8009929C(8, h);
            h = func_80099194(D_800E0A56, h);
            h = func_80099734(q, h);
            h = func_8009929C(0xA, h);
            h = func_80099194(D_800E0A6A, h);
            func_80099290(h);
            func_800A5720(mh);
            g2 = D_800E3D7C;
            *(s16 *)(p + 0xC) = 0x10;
            func_8009A180(q, g2);
            (*(s16 *)(p + 0xA))++;
        }
        break;
    case 1:
        n = *(u16 *)(p + 0xC) - 1;
        *(s16 *)(p + 0xC) = n;
        if (n <= 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0 || *(u32 *)(g + 0x10) != 0)
        {
            (*(s16 *)(p + 0xA))++;
        }
        break;
    case 2:
        q = *(u8 **)(p + 0x4);
        if (*(s16 *)(p + 0xE) == 0)
        {
            *(u8 **)(q + 0x60) = func_800A3D18(*(u8 **)(q - 0x14), q, 2);
            f2 = (u8 *)&D_80083460;
            *(s16 *)(f2 + 0x2) = *(u16 *)(f2 + 0x2) | 2;
            *(u16 *)(q + 0x46) = 0x800C;
            if (*(u8 **)(q + 0x60) != NULL)
            {
                r = func_800A384C(q, *(u8 **)(q + 0x60), &o, 1);
                *(s16 *)(q + 0x2A) = o;
                if (r >= 0)
                {
                    *(s16 *)(q + 0x46) = r | 0x8000;
                }
            }
        }
        *(u32 *)(D_800E3D7C + 0x110) = 0;
        *(u32 *)(q + 0x1C) |= 0x400000;
        f3 = (u8 *)&D_80083460;
        *(s16 *)(f3 + 0xA) = *(u16 *)(f3 + 0xA) - 1;
        *(s16 *)(p - 0x2) = *(u16 *)(p - 0x2) | 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    m = *(s32 *)(p + 0x4);
    if (m != 0 && (*(u16 *)((u8 *)m - 0x2) & 0x8000) != 0)
    {
        *(s16 *)(p + 0xE) = 1;
    }
}

/* Spawner: a kind-0x12 node carrying func_800B7774. The first two arguments
   are read back out of the new mover (`lh`) while the third is the halfword
   just stored, sign-extended -- which is what writing all three stores and
   all three reads as `*(s16 *)` gives, with cse forwarding only the last.
   45 words, 0 differing. */
UnkNode *func_800B7B60(u8 *p, u32 *q)
{
    UnkNode *n;
    UnkVec *m;
    UnkVec *s;
    u8 *g;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B7774;
        *(u32 *)((u8 *)n + 0x20) = *q;
        s = *(UnkVec **)(p - 0x18);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)s + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)s + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)s + 0xA);
        func_800B8024(*(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return n;
}

/* An UnkNode +0x10 step (func_800B7D74 installs it), so the three
   parameters are func_80040190's cast and all three are read incoming.
   The three byte stores of ONE shared local come out 0xC / 0xE / 0xD and
   the source has to be written that way: `0xC, 0xD, 0xE` is the same 88
   words at 2 differing, with only those two `sb` displacements swapped.
   (That is the opposite of the three-read-modify-write lever, where the
   listing's last two offsets are the reverse of the source -- here only the
   FIRST store is a read-modify-write and the other two are plain stores of
   the local, so sched1 has nothing to hoist past.) 88 words, 0 differing. */
void func_800B7C14(u8 *p, UnkVec *b, u8 *c)
{
    s32 v;
    s16 t;

    if (*(s32 *)c != 0)
    {
        func_800478B8(c);
        if (*(u16 *)(c + 0x14) & 0x4000)
        {
            func_8003DB94(c, *(void **)(p + 0x8), 0);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C -= b->unk_0C >> 4;
    b->unk_10 -= b->unk_10 >> 4;
    b->unk_14 += 0x40000;
    if (*(s16 *)(p + 0x12) != 0)
    {
        if (c[0xC] != 0)
        {
            v = c[0xC] - 8;
            c[0xC] = v;
            c[0xE] = v;
            c[0xD] = v;
        }
    }
    *(s16 *)(c + 0x16) += *(u16 *)(p + 0x14);
    *(s16 *)(c + 0x18) += *(u16 *)(p + 0x16);
    t = *(u16 *)(p + 0x10) - 1;
    *(s16 *)(p + 0x10) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One 0x212 debris node per `sel`, 0..4, each carrying func_800B7C14 as its
   +0x10 step: case 0 is a random tumble with the D_8006E5D4 template parked
   at the view's +0x08 for the step to re-apply, cases 1 and 2 drive the
   mover from D_80083178's +0xB0 bearing biased by -0x300 and +0x500 with the
   template written straight into the renderable's +0x08, and cases 3 and 4
   are the same bearing unbiased and negated with the template applied
   immediately. Everything joins a shared tail that whitens the renderable
   and scatters the view's +0x10/+0x14/+0x16. func_800B8024 calls it with
   1, 2, 3, 4 and then 0 ten times. The jump table is jtbl_80089340 at rodata
   offset 0xBE0, 0 mod 8, so cc1psx's `.align 3` is a no-op.

   `q = (u8 *)n + 0x20` AND `r = (u8 *)n->unk_0C` MUST COME AFTER THE THREE
   MOVER STORES. Written before them the first parameter is still live when
   `q` is born, so it cannot reuse $s2 and the function takes an eighth
   callee-saved register: 174 words of 172, 168 differing. With `q` born
   after the `sh $s2, 0x2($s1)` that kills `x`, the two share $s2 exactly as
   the target does. 172 words, 0 differing.

   D_80083178 is a POINTER LOCAL assigned before the first call, which is
   where the prologue's `lui`/`addiu` pair comes from -- the unit's own
   spelling for this symbol. */
void func_800B7D74(s32 x, s32 y, s32 z, s32 sel)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    u8 *g;

    g = (u8 *)&D_80083178;
    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B7C14;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        q = (u8 *)n + 0x20;
        r = (u8 *)n->unk_0C;
        switch (sel)
        {
        case 0:
            *(s16 *)((u8 *)m + 0xE) = (rand() & 0x1F) - 0x10;
            *(s16 *)((u8 *)m + 0x12) = (rand() & 0x1F) - 0x10;
            *(s16 *)((u8 *)m + 0x16) = -8 - (rand() & 0x1F);
            *(u32 *)(q + 0x8) = (u32)D_8006E5D4;
            func_8003DB94(r, D_8006E5D4, 0);
            break;
        case 1:
            *(u32 *)(r + 0x8) = (u32)D_8006E24C;
            *(s16 *)((u8 *)m + 0x16) = -0x10 - (rand() & 0xF);
            *(s32 *)((u8 *)m + 0xC) = func_80064584(*(s16 *)(g + 0xB0) - 0x300) << 7;
            *(s32 *)((u8 *)m + 0x10) = func_800644B8(*(s16 *)(g + 0xB0) - 0x300) << 7;
            *(s16 *)(q + 0x12) = 1;
            break;
        case 2:
            *(u32 *)(r + 0x8) = (u32)D_8006E258;
            *(s16 *)((u8 *)m + 0x16) = -8 - (rand() & 0xF);
            *(s32 *)((u8 *)m + 0xC) = func_80064584(*(s16 *)(g + 0xB0) + 0x500) << 7;
            *(s32 *)((u8 *)m + 0x10) = func_800644B8(*(s16 *)(g + 0xB0) + 0x500) << 7;
            *(s16 *)(q + 0x12) = 1;
            break;
        case 3:
            *(u32 *)(q + 0x8) = (u32)D_8006E51C;
            func_8003DB94(r, D_8006E51C, 0);
            *(s32 *)((u8 *)m + 0xC) = func_80064584(*(s16 *)(g + 0xB0)) << 7;
            *(s32 *)((u8 *)m + 0x10) = func_800644B8(*(s16 *)(g + 0xB0)) << 7;
            *(s16 *)((u8 *)m + 0x16) = -0xC - (rand() & 0xF);
            break;
        case 4:
            *(u32 *)(q + 0x8) = (u32)D_8006E57C;
            func_8003DB94(r, D_8006E57C, 0);
            *(s32 *)((u8 *)m + 0xC) = -func_80064584(*(s16 *)(g + 0xB0)) << 7;
            *(s32 *)((u8 *)m + 0x10) = -func_800644B8(*(s16 *)(g + 0xB0)) << 7;
            *(s16 *)((u8 *)m + 0x16) = -0xC - (rand() & 0xF);
            break;
        }
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)(q + 0x14) = (rand() & 0x1FF) - 0x100;
        *(s16 *)(q + 0x16) = (rand() & 0x1FF) - 0x100;
        *(s16 *)(q + 0x10) = (rand() & 0x1F) + 0x20;
    }
}

/* 45 words, 0 differing. The first call needs no argument setup at all,
   because $a0/$a1/$a2 are still the incoming parameters. */
void func_800B8024(u8 *a, u8 *b, u8 *c)
{
    s32 i;

    func_800B7D74(a, b, c, 1);
    func_800B7D74(a, b, c, 2);
    func_800B7D74(a, b, c, 3);
    func_800B7D74(a, b, c, 4);
    for (i = 10; i > 0; i--)
    {
        func_800B7D74(a, b, c, 0);
    }
    func_800A56E0(0x513);
}

/* The 0x12 node's +0x10 step for func_800B8228's world marker: a three-case
   switch on the actor view's +0xA2, case 0 falling through into case 1. The
   +0xA4 countdown is spent with `if (--*(s16 *)(p + 0xA4) == 0)` -- the
   decrement's own value is what the target tests (`sll 16` with no `sra`),
   so a separate `-= 1` then a re-read is the wrong shape. The last case's
   fourth argument is `(s32)D_8006E240`, copied from func_800B8228's own call
   two functions down. 84 words, 0 differing on the first compile. */
void func_800B80D8(u8 *p, UnkVec *m, u8 *r)
{
    switch (*(s16 *)(p + 0xA2))
    {
    case 0:
        if ((*(u16 *)(r + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0xA2) = 2;
            return;
        }
        *(s16 *)(p + 0xA4) = 0xC;
        *(s16 *)(p + 0xA2) += 1;
        /* fallthrough */
    case 1:
        m->unk_14 += 0x30000;
        m->unk_08 += m->unk_14;
        if (*(u16 *)(r + 0x1C) < 0x1000)
        {
            *(u16 *)(r + 0x1C) += 0x50;
            *(u16 *)(r + 0x1E) += 0x50;
        }
        if (--*(s16 *)(p + 0xA4) == 0)
        {
            m->unk_14 = 0;
            *(s16 *)(p + 0xA2) += 1;
        }
        break;
    case 2:
        func_800A7A7C(*(s16 *)(p + 0xB0), *(s16 *)(p + 0xB2), *(u16 *)((u8 *)m + 0xA) - 0x20, (s32)D_8006E240,
                      p + 0x98);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* Spawn the 0x12 node for a world-space marker, or hand the same three
   coordinates to func_800A7A7C when the allocator is empty.

   THE THREE COORDINATES ARE `u32`, NOT `u16`. The `>> 6` pair is a LOGICAL
   shift, so they are unsigned; declared u16, cc1psx masks each with
   `andi 0xFFFF` before the shift, which forces two more callee-saved
   registers and their saves and restores -- 85 words against 77 with 84
   differing. The +0xB8 copy is a 4-byte object of ALIGNMENT 1, hence the
   `lwl`/`lwr` + `swl`/`swr` pairs. 77 words, 0 differing. */
UnkNode *func_800B8228(u32 x, u32 y, u32 z, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        func_8004491C(n, (u32)D_80045340);
        n->unk_10 = (u32)func_800B80D8;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        *(s16 *)((u8 *)m + 0x16) = -0x12;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x1E) = 0xC90;
        *(s16 *)(r + 0x1C) = 0xC90;
        *(u32 *)(r + 0x8) = func_800A7A38(d);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0xB0) = x >> 6;
        *(s16 *)(q + 0xB2) = y >> 6;
        *(UnkDungeonName4 *)((u8 *)n + 0xB8) = *(UnkDungeonName4 *)d;
        func_800BC26C(n, 0, 0, 0);
        func_800A56E0(0x51C);
    }
    else
    {
        func_800A7A7C((s16)(x >> 6), (s16)(y >> 6), (s16)z, (s32)D_8006E240, d);
    }
    return n;
}

void func_800B835C(u8 *p, s32 *b, s32 c, s32 d)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s32 cc;
    s32 dd;
    s16 lean;
    s32 t;
    s32 t2;

    cc = c;
    dd = d;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(r, q);
    w[0] = *(u16 *)b + ((s16) * ((u16 *)b + 2) >> 1);
    t = *((s16 *)b + 1) + ((s16) * ((u16 *)b + 3) >> 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    func_800B84E4(w, p, r, (u16)dd);
    if ((u16)cc != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)b;
        t2 = *((s16 *)b + 1);
        if (lean)
        {
            t2 -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t2;
        *(u32 *)(u + 0xC) = b[1];
        AddPrim(r, u);
    }
    __asm__("" ::"r"(cc), "r"(dd));
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, b);
    AddPrim(r, q);
}

/* 211 words, 0 differing.  The fan/arc emitter: one POLY_GT4 per step of a
   `*(u16 *)(p + 0xE)'-step sweep, each quad spanning the previous step's
   screen point and the current one.  Three spellings carry it, and every one
   of them is about WHERE AN INSN IS BORN, not what it computes.

   (1) THE SWEEP ANGLE IS NOT A VARIABLE.  $s1 is loop.c's giv for
       `i * 0x1000'.  Its initialiser `addu $s1, $s5, $zero' is emitted by
       strength_reduce before `loop_start', i.e. AFTER the `addiu $s0, $s4,
       0x7' giv -- the only way that pair can come out in that order, because
       every source statement in the preheader has a SMALLER INSN_LUID than
       anything loop.c inserts there and sched2's tie-break is LUID order.
       Written as a second counter with `ang += 0x1000', those two words
       transpose and no ordering of the preheader's own statements moves
       them: 211 words / 5 aligned.  loop.c emits `ang = 0'; cse2
       copy-propagates `i' into it, which is where the `move' comes from.

   (2) `dd = (u16)d;' BELONGS INSIDE THE LOOP.  As a statement before the
       loop its `andi' sits in the entry block, where sched2 sinks it to the
       end and reorg takes it for the guard's delay slot instead of `i = 0'
       (211 / 5).  Inside the loop it is a move_movables invariant, hoisted
       into the preheader AHEAD of both givs -- movables are moved before
       strength_reduce runs, and each `emit_insn_before (..., loop_start)'
       lands after the previous one.

   (3) THE LOOP IS A `for', NOT AN `if' + `do'/`while'.  Both shapes emit the
       same `lhu'/`beqz' guard, but jump.c's `duplicate_loop_exit_test'
       COPIES the test, and that copy is one more REG_N_REFS for `i' -- 8
       against `dd''s 7.  global.c's priority is
       `floor_log2 (refs) * refs / live_length' and `i''s live range strictly
       contains `dd''s (118 insns against 113), so without that eighth
       reference `dd' is allocated first and takes $s5: `i' and `dd' swap
       registers and seven words change (42, 43, 66, 159, 162, 179, 190).
       With the `if' + `do'/`while' and everything else as below: 211 / 8,
       all eight rows that swap.

   The `__asm__("")' after the loop-top copy stays load-bearing: it makes
   sched.c flush every pending read and write list, so
   `sp->unk_70.w = sp->unk_78.w' comes out as the target's serial
   `lw'/`nop'/`sw' instead of being interleaved into the first UV chain's
   load-delay slots.  Removing it: 210 words / 7. */
void func_800B84E4(s16 *a, u8 *p, u8 *ot, s32 d)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPF *sp = (UnkDungeonSPF *)0x1F800000;
    UnkDungeonGT4W *q;
    s32 i;
    s32 dd;
    s32 t;
    s32 tp;

    q = *(UnkDungeonGT4W **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_74 = 0;
    sp->unk_7C = 0;
    sp->unk_78.h[0] = *(u16 *)a + p[0xC];
    sp->unk_78.h[1] = *((u16 *)a + 1);
    sp->unk_08.w = p[0x8];
    sp->unk_0C.w = p[0x9];
    sp->unk_10.w = p[0xA];
    sp->unk_14.w = p[0xB];
    for (i = 0; i < *(u16 *)(p + 0xE);)
    {
        dd = (u16)d;
        sp->unk_70.w = sp->unk_78.w;
        __asm__("");
        t = sp->unk_08.b + (sp->unk_10.w >> 1);
        q->unk_18.b[0] = t;
        q->unk_0C.b[0] = t;
        t = sp->unk_0C.b + (sp->unk_14.w >> 1);
        q->unk_18.b[1] = t;
        q->unk_0C.b[1] = t;
        q->unk_30.b[0] = q->unk_0C.b[0] + ((sp->unk_10.w * func_80064584(i * 0x1000 / *(u16 *)(p + 0xE))) >> 13);
        q->unk_30.b[1] = q->unk_0C.b[1] + ((sp->unk_14.w * func_800644B8(i * 0x1000 / *(u16 *)(p + 0xE))) >> 13);
        i++;
        sp->unk_78.h[0] = *(u16 *)a + ((p[0xC] * func_80064584(i * 0x1000 / *(u16 *)(p + 0xE))) >> 12);
        sp->unk_78.h[1] = *((u16 *)a + 1) + ((p[0xD] * func_800644B8(i * 0x1000 / *(u16 *)(p + 0xE))) >> 12);
        q->unk_08 = q->unk_14 = *(s32 *)a;
        q->unk_20 = sp->unk_78.w;
        q->unk_2C = sp->unk_70.w;
        q->unk_24.b[0] = q->unk_0C.b[0] + ((sp->unk_10.w * func_80064584(i * 0x1000 / *(u16 *)(p + 0xE))) >> 13);
        q->unk_24.b[1] = q->unk_0C.b[1] + ((sp->unk_14.w * func_800644B8(i * 0x1000 / *(u16 *)(p + 0xE))) >> 13);
        q->unk_0E = *(u16 *)(p + 0x6);
        tp = *(u16 *)(p + 0x4);
        q->unk_1A = tp;
        if (dd != 0)
        {
            q->unk_1A = (tp & 0xFF9F) | ((dd - 1) << 5);
        }
        q->unk_04 = *(u32 *)(p + 0x10);
        q->unk_10 = *(u32 *)(p + 0x10);
        q->unk_1C = *(u32 *)(p + 0x14);
        q->unk_28 = *(u32 *)(p + 0x14);
        SetPolyGT4(q);
        if (dd != 0)
        {
            *((u8 *)q + 0x7) |= 2;
        }
        AddPrim(ot, q);
        q++;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* The 0x12 tag node's +0x10 step, a three-state machine on the view's +0x20.
   State 0 paints the renderable white at scale 0x1000, installs the
   D_80045340 tag and falls through into state 1; state 1 tracks the parent
   slot's projection; state 2 eases the mover toward the destination over the
   +0x26 countdown, spawning the func_800B8EA8 trail each frame, and on the
   last frame snaps to the destination, fires func_800B8D64 and marks the
   node dead. 252 words, 0 differing.

   THE +0x20 COUNTER IS AN INDEX OFF THE CAST POINTER, `*((s16 *)p + 0x10)`,
   not `*(s16 *)(p + 0x20)`. Only the index form leaves a top-level
   PLUS_EXPR, which is what makes the store MEM_IN_STRUCT_P (expr.c:5343);
   true_dependence (sched.c:854) then drops the dependence between that
   varying in-struct store and the non-varying, non-in-struct
   `lw %lo(D_800814A0)`, so sched1 lifts the load over it and the load-delay
   slot is filled by the `addiu`. Through the flat cast the load stays below
   both stores and takes a `nop`: 253 words, 26 differing. Permuting the
   three statements of that tail instead reaches 253/26 at best (ACB 254/26,
   BAC 253/28, BCA 254/29, CAB and CBA 253/28).

   x, y and z are `s32`, not `s16`. As `s16` they load with `lhu` where the
   target has `lh` and each pays its own `sll 16`/`sra 16` at the call:
   256 words, 76 differing. */
void func_800B8830(u8 *p, UnkVec *b, u8 *c)
{
    UnkSlot *s;
    u8 *v;
    s16 out[4];
    s32 pos[3];
    u16 trail[4];
    s32 x;
    s32 y;
    s32 z;
    s32 i;

    switch (*(s16 *)(p + 0x20))
    {
    case 0:
        c[0xE] = 0x80;
        c[0xD] = 0x80;
        c[0xC] = 0x80;
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        *(u16 *)(c + 0x14) |= 0xC;
        func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
        (*((s16 *)p + 0x10))++;
        /* fallthrough */
    case 1:
        s = (UnkSlot *)(*(UnkNode **)p)->unk_0C;
        if ((s->unk_14 & 0x8000) != 0 || func_8003DE58((u8 *)s->unk_08, s, out, 0) != 0)
        {
            v = (u8 *)(*(UnkNode **)p)->unk_08;
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(v + 0x2);
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(v + 0x6);
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(v + 0xA);
            if ((s->unk_14 & 0x8000) == 0)
            {
                *(s16 *)((u8 *)b + 0x2) += out[0];
                *(s16 *)((u8 *)b + 0x6) += out[1];
                *(s16 *)((u8 *)b + 0xA) += out[2];
            }
            else
            {
                *(s16 *)((u8 *)b + 0xA) -= 0x20;
            }
        }
        if ((**(u16 **)(p + 0x8) & 0x80) != 0)
        {
            *(s16 *)(p + 0x26) = 9;
            (*((s16 *)p + 0x10))++;
        }
        break;
    case 2:
        x = *(s16 *)((u8 *)b + 0x2);
        y = *(s16 *)((u8 *)b + 0x6);
        z = *(s16 *)((u8 *)b + 0xA);
        v = (u8 *)(*(UnkNode **)(p + 0x4))->unk_08;
        pos[0] = *(s32 *)(v + 0x0);
        pos[1] = *(s32 *)(v + 0x4);
        pos[2] = (*(s16 *)(p + 0x32) - 0x50) << 16;
        i = ((D_80083228 + *(s16 *)(D_800814A8 + 0x2A) + 0x100) >> 9) & 7;
        if (func_8003DE58(((u8 **)D_800E3D18)[i], (UnkSlot *)(*(UnkNode **)(p + 0x4))->unk_0C, out, 0) != 0)
        {
            pos[0] += out[0] << 16;
            pos[1] += out[1] << 16;
            pos[2] += out[2] << 16;
        }
        b->unk_00 += (pos[0] - b->unk_00) / *(s16 *)(p + 0x26);
        b->unk_04 += (pos[1] - b->unk_04) / *(s16 *)(p + 0x26);
        b->unk_08 += (pos[2] - b->unk_08) / *(s16 *)(p + 0x26) -
                     (func_800644B8(*(s16 *)(p + 0x26) * 170) << 7);
        trail[0] = *(u16 *)((u8 *)b + 0x2);
        trail[1] = *(u16 *)((u8 *)b + 0x6);
        trail[2] = *(u16 *)((u8 *)b + 0xA);
        func_800B8EA8(p - 0x20, x, y, z, trail, *(void **)(p + 0xC), 4, 0x1000, 1);
        (*(s16 *)(p + 0x26))--;
        if (*(s16 *)(p + 0x26) <= 0)
        {
            b->unk_00 = pos[0];
            b->unk_04 = pos[1];
            b->unk_08 = pos[2];
            func_800B8D64(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA));
            *(u16 *)(p - 0x2) |= 0x8000;
            (*((s16 *)p + 0x10))++;
            D_800814A0 |= 0x8000;
            return;
        }
        break;
    }
    (*(s16 *)(p + 0x28))++;
}

/* Allocate the 0x12 node for a floating tag. THE +0x20 VIEW IS A STRUCT, and
   that is worth the whole function: see UnkDungeonTag. Through a `u8 *` the
   four stores are not MEM_IN_STRUCT_P, sched1 cannot lift the D_800814A8 load
   over them, and the best of 24 store permutations x four pointer spellings
   is 2 differing. 52 words, 0 differing. */
void func_800B8C20(s32 a, s32 e, u8 *c, u8 *d)
{
    UnkDungeonTag *n;
    u8 *r;

    n = (UnkDungeonTag *)func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        r = *(u8 **)((u8 *)n + 0xC);
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800B8830;
        if (c != NULL)
        {
            *(u8 **)(r + 0x8) = c;
        }
        else
        {
            *(u8 **)(r + 0x8) = D_800DF2F8;
        }
        *(s16 *)(r + 0x10) = 0x60;
        n = (UnkDungeonTag *)((u8 *)n + 0x20);
        if (d != NULL)
        {
            n->unk_0C = d;
        }
        else
        {
            n->unk_0C = D_800DF2F8;
        }
        n->unk_00 = a;
        n->unk_04 = D_80083498;
        n->unk_32 = *(u16 *)(D_800814A8 + 0x88);
        n->unk_08 = e;
    }
}

/* A node +0x10 step: flash the renderable's three colour bytes while the +0x2
   countdown lasts, then mark the node dead.

   TWO VARIABLES FOR ONE COUNTDOWN -- the s16 the field holds and an s32 copy
   of it. With a single s16 the comparison folds into `sll 16`/`bgtz`, the
   `<< 3` then reads the RAW halfword and gcc spends a pointless
   `move $a1, $v1` where the target has the `sra`: 29 words, 2 differing.
   Widening once into `u` makes the sign-extension real and both uses take it;
   `(s32)t * 8 + 0x20` with one variable is still 1 differing, so the cast
   alone is not enough -- it has to be a separate binding. The colour bytes
   are written 0xC, 0xE, 0xD: reorg puts the last of the three in the `j`'s
   delay slot, and C, D, E is a further 2 differing. 29 words, 0 differing. */
void func_800B8CF0(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;
    s32 u;

    t = *(u16 *)(p + 0x2) - 1;
    *(s16 *)(p + 0x2) = t;
    u = t;
    if (u <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else if (t & 1)
    {
        c[0xC] = (u << 3) | 0x20;
        c[0xE] = (u << 3) | 0x20;
        c[0xD] = (u << 3) | 0x20;
    }
    else
    {
        *(u32 *)(c + 0xC) = 0x101010;
    }
}

/* Spawn the 0x212 node. THE MOVER POINTER IS CACHED IN A LOCAL: written as
   three `n->unk_08->...` stores the pointer is RELOADED before each one -- a
   store through the cast can alias it -- and the function is 49 words against
   45 with 27 differing. 45 words, 0 differing. */
void func_800B8D64(s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B8CF0;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = x;
        *(s16 *)(m + 0x6) = y;
        *(s16 *)(m + 0xA) = ang;
        r = (u8 *)n->unk_0C;
        *(u8 **)(r + 0x8) = D_800DF358;
        *(s16 *)(r + 0x1E) = 0x800;
        *(s16 *)(r + 0x1C) = 0x800;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)((u8 *)n + 0x22) = 6;
    }
}

/* 36 words, 0 differing. */
void func_800B8E18(u8 *p, void *b, u8 *c)
{
    *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) -=
        *(u16 *)(c + 0x1E) * *(s16 *)(p + 0x2) / 24;
    c[0xC] = c[0xD] = c[0xE] -= *(s16 *)(p + 0x2) * 8;
    if (c[0xC] == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the kind-0x12 node under `a`.

   TWO PARAMETER WIDTHS ARE FORCED, both on STACK arguments, and they pull in
   opposite directions. The NINTH must be `s16`: as s32 the entry `lw` dies
   and gcc reloads it after the calls instead of keeping it in a callee-saved
   register, 70 words against 72. The SEVENTH is `s16` but its value has to be
   COPIED INTO AN s32 LOCAL before use: read directly, gcc tests the
   parameter's HImode pseudo with a bare `sll 16` and takes the raw register
   for `g - 1`, so the `sra` never appears -- 71 words, 29 differing.
   Declaring it s32 and narrowing with `(s16)g` into that same local is
   71 / 63. 72 words, 0 differing. */
void func_800B8EA8(void *a, s32 b, s32 c, s32 d, u16 *e, void *f, s16 g, s32 h, s16 i)
{
    UnkNode *n;
    u8 *v;
    u8 *s;
    s32 t;

    n = func_8003FD64(0x12, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B8E18;
        func_8004491C(n, (u32)func_800CEEFC);
        v = (u8 *)n->unk_08;
        *(s16 *)(v + 0x2) = e[0];
        *(s16 *)(v + 0x6) = e[1];
        *(s16 *)(v + 0xA) = e[2];
        *(s16 *)(v + 0xE) = b;
        *(s16 *)(v + 0x12) = c;
        *(s16 *)(v + 0x16) = d;
        s = (u8 *)n->unk_0C;
        *(void **)(s + 0x8) = f;
        *(s16 *)(s + 0x1E) = h;
        *(s16 *)(s + 0x1C) = h;
        *(s32 *)(s + 0xC) = 0x808080;
        t = g;
        if (t != 0)
        {
            *(s16 *)(s + 0x10) = (t - 1) << 5;
            *(u16 *)(s + 0x14) |= 0xC;
        }
        *(s16 *)((u8 *)n + 0x22) = i;
    }
}

/* 95 of 95 words, 0 differing.
 *
 * THE LEVER IS THE 0x8D0 STORE BEING AN ARRAY REF.  `((u32 *)X)[0x234]' and
 * `*(u32 *)(X + 0x8D0)' are the same store (0x234 * 4 == 0x8D0) and differ
 * only in MEM_IN_STRUCT_P: expr.c:5351 sets that flag when an INDIRECT_REF's
 * operand is a PLUS_EXPR, which an ARRAY_REF on a cast pointer is and a
 * dereference of a cast PLUS is not.  `validate_equiv_mem' (local-alloc)
 * treats a store it cannot disambiguate as a clobber of the fifth
 * parameter's frame slot, so with the cast spelling `update_equiv_regs'
 * cannot move `lw $s5, 0x50($sp)' out of the prologue: it stays there and
 * sched2 ranks it above the neighbouring `lui', taking that slot.  With the
 * array ref the load moves past the store, gains a true dependence on it
 * (sched2 priority 1 -> 3) and is emitted where the target has it, one insn
 * before the first `jal'.  Measured on this body: array-ref store 0 rows,
 * cast store 19 rows, and that is the whole difference -- the two spellings
 * of the `ee' copy below make no difference once the store is right.
 * (Second instance of docs/matching-levers.md "A STORE'S MEM_IN_STRUCT_P
 * DECIDES WHETHER AN INCOMING *STACK* PARAMETER'S LOAD CAN MOVE TO ITS USE".)
 *
 * TWO DEVICES, both with no reason in the source beyond the register
 * assignment they buy, and both measured:
 *   - `do { } while (0);' around the func_800B9144 call.  Without it the
 *     function is 93 words: flow.c:2128 weights the references inside it by
 *     loop_depth, which is what keeps `r'/`g' in callee-saved registers and
 *     gives the target's $fp save/restore pair something to hold.  A bare
 *     block is 93/21 -- it is the loop depth, not the braces.
 *   - `g++; g--;' INSIDE that wrapper, before the call.  It compiles to
 *     nothing (combine folds the two addiu) and is worth 2 rows: without it
 *     the body is 95/2.  The pair has to be on `g' and inside the wrapper:
 *     `q' or `b2' there is 95/2, `p' is 95/37, `r' is 96/37, the pair after
 *     the call is 95/2, and the pair outside the wrapper is 95/2.
 *
 * Also measured and NOT needed, against the park this closes: the `ee = e;'
 * copy, the `bb = b;' copy, a separate `t2', and the park's
 * `__asm__("" ::"r"(b2));' barrier -- the asm device is gone.  `s16 lean'
 * and not `s32' is still load-bearing (95/56 as `s32'), and so is hoisting
 * `cw = (u16)c;' out of the `if' (95/17 written `if ((u16)c != 0)').
 */
void func_800B8FC8(u8 *p, s32 *b, s32 *b2, s32 c, s32 e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;
    u16 cw;

    cw = (u16)c;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    ((u32 *)D_80083160.unk_00)[0x234] = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(r, q);
    w[0] = *(u16 *)b2;
    t = *((s16 *)b2 + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    do
    {
        g++;
        g--;
        func_800B9144((u8 *)w, p, r, (s16)e);
    } while (0);
    if (cw != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)b;
        t = *((s16 *)b + 1);
        if (lean)
        {
            t -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t;
        *(u32 *)(u + 0xC) = b[1];
        AddPrim(r, u);
    }
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, b);
    AddPrim(r, q);
}

void func_800B9144(u8 *a, u8 *p, u8 *ot, s16 d)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPD *sp = (UnkDungeonSPD *)0x1F800000;
    UnkSlot *s;
    UnkVec *v;
    volatile u8 *e;
    u8 *q;
    s32 *flag;
    s32 t;
    s32 t2;
    s32 u1;
    s32 u2;
    s32 x;
    s32 y;
    s32 c;
    s32 hr;
    s32 t20;
    s32 t22;
    s32 (*h)(u8 *, UnkVec *, UnkSlot *, u8 *, u8 *);
    s32 ta;
    s32 tb;
    s32 tc;
    s32 td;

    s = (UnkSlot *)*(u32 *)(p - 0x14);
    v = *(UnkVec **)(p - 0x18);
    e = (volatile u8 *)*(u32 *)((u8 *)s + 0x8);
    sp->unk_70[3].vz = 0;
    sp->unk_70[2].vz = 0;
    sp->unk_70[1].vz = 0;
    sp->unk_70[0].vz = 0;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = (u32)ot;
    sp->unk_C0 = 0;
    if (d != 0)
    {
        func_80067EF4(q, 0, 0);
        AddPrim(sp->unk_20, q);
        q += 0xC;
    }
    PushMatrix();
    x = *(u16 *)a;
    sp->unk_00 = x;
    y = *(u16 *)(a + 0x2);
    sp->unk_02 = y;
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_38 = 0x1000;
    t20 = s->unk_20;
    sp->unk_108 = t20;
    t22 = s->unk_22;
    sp->unk_10A = t22;
    sp->unk_40 = (s16)x + (s16)t20;
    sp->unk_44 = (s16)y + (s16)t22;
    sp->unk_48 = 0;
    TransMatrix((MATRIX *)0x1F800050, (VECTOR *)0x1F800040);
    RotMatrix((SVECTOR *)&s->unk_16, (MATRIX *)0x1F800050);
    ScaleMatrix((MATRIX *)0x1F800050, (VECTOR *)0x1F800030);
    SetRotMatrix((MATRIX *)0x1F800050);
    SetTransMatrix((MATRIX *)0x1F800050);
    sp->unk_24 = s->unk_14;
    flag = (s32 *)0x1F800094;
    do
    {
        if ((e[0x0] & 0x20) == 0)
        {
            sp->unk_08.w = *(u8 *)(e + 0x8);
            sp->unk_0C.w = *(u8 *)(e + 0x9);
            sp->unk_10.w = *(u8 *)(e + 0xA);
            sp->unk_14.w = *(u8 *)(e + 0xB);
            if ((e[0x0] ^ sp->unk_24) & 1)
            {
                ta = -(s8)e[0x2] - sp->unk_108;
                sp->unk_70[2].vx = ta;
                sp->unk_70[0].vx = ta;
                ta = ta - sp->unk_10.h;
                sp->unk_70[3].vx = ta;
                sp->unk_70[1].vx = ta;
            }
            else
            {
                tb = (s8)e[0x2] - sp->unk_108;
                sp->unk_70[2].vx = tb;
                sp->unk_70[0].vx = tb;
                tb = tb + sp->unk_10.h;
                sp->unk_70[3].vx = tb;
                sp->unk_70[1].vx = tb;
            }
            if ((e[0x0] ^ sp->unk_24) & 2)
            {
                tc = -(s8)e[0x3] - sp->unk_10A;
                sp->unk_70[1].vy = tc;
                sp->unk_70[0].vy = tc;
                tc = tc - sp->unk_14.h;
                sp->unk_70[3].vy = tc;
                sp->unk_70[2].vy = tc;
            }
            else
            {
                td = (s8)e[0x3] - sp->unk_10A;
                sp->unk_70[1].vy = td;
                sp->unk_70[0].vy = td;
                td = td + sp->unk_14.h;
                sp->unk_70[3].vy = td;
                sp->unk_70[2].vy = td;
            }
            RotTransSV(&sp->unk_70[0], &sp->unk_98, flag);
            RotTransSV(&sp->unk_70[1], &sp->unk_A0, flag);
            RotTransSV(&sp->unk_70[2], &sp->unk_A8, flag);
            RotTransSV(&sp->unk_70[3], &sp->unk_B0, flag);
            *(s32 *)(q + 0x8) = sp->unk_98;
            *(s32 *)(q + 0x10) = sp->unk_A0;
            *(s32 *)(q + 0x18) = sp->unk_A8;
            *(s32 *)(q + 0x20) = sp->unk_B0;
            q[0x3] = 9;
            u1 = sp->unk_10.w - 1;
            t = u1 + sp->unk_08.w;
            sp->unk_10.w = t;
            if ((t & 0x100) != 0)
            {
                sp->unk_10.w = t - 1;
            }
            u2 = sp->unk_14.w - 1;
            t2 = u2 + sp->unk_0C.w;
            sp->unk_14.w = t2;
            if ((t2 & 0x100) != 0)
            {
                sp->unk_14.w = t2 - 1;
            }
            sp->unk_14.w = sp->unk_14.w << 8;
            sp->unk_0C.w = sp->unk_0C.w << 8;
            if (s->unk_12 != 0)
            {
                if ((sp->unk_24 & 0x100) != 0)
                {
                    *(s16 *)(q + 0xE) = s->unk_12;
                }
                else
                {
                    *(s16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
                }
            }
            else
            {
                *(s16 *)(q + 0xE) = *(u16 *)(e + 0x6);
            }
            *(s16 *)(q + 0xC) = sp->unk_0C.h + sp->unk_08.h;
            *(s16 *)(q + 0x14) = sp->unk_0C.h + sp->unk_10.h;
            if (s->unk_10 != 0)
            {
                *(s16 *)(q + 0x16) = s->unk_10 + (*(u16 *)(e + 0x4) & 0xFF9F);
            }
            else
            {
                *(s16 *)(q + 0x16) = *(u16 *)(e + 0x4);
            }
            *(s16 *)(q + 0x1C) = sp->unk_14.h | sp->unk_08.h;
            *(s16 *)(q + 0x24) = sp->unk_14.h | sp->unk_10.h;
            if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x20))
            {
                q[0x14] = q[0x24]--;
            }
            if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x22))
            {
                q[0x1D] = q[0x25]--;
            }
            c = *(u8 *)(e + 0x1);
            *((u8 *)s + 0xF) = c;
            if ((sp->unk_24 & 8) != 0)
            {
                *((u8 *)s + 0xF) = (sp->unk_24 & 4) ? (c | 2) : (c & 0xFD);
            }
            *(u32 *)(q + 0x4) = *(u32 *)((u8 *)s + 0xC);
            AddPrim(sp->unk_20, q);
            q += 0x28;
        }
        else
        {
            h = *(s32(**)(u8 *, UnkVec *, UnkSlot *, u8 *, u8 *))(e + 0x8);
            if (h == NULL)
            {
                goto endtest;
            }
            hr = h(p, v, s, (u8 *)e, q);
            if (hr > 0)
            {
                e = (volatile u8 *)(hr | 0x80000000);
            }
            else
            {
                q = (u8 *)hr;
            }
        }
    endtest:
        if (*(s8 *)e < 0)
        {
            break;
        }
        e += 0xC;
    } while (1);
    if (d != 0)
    {
        func_80067EF4(q, 0, 1);
        AddPrim(sp->unk_20, q);
        q += 0xC;
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* The party-follow step a kind-0x212 node carries (func_800B9964 installs it
   at +0x10, so the three parameters are func_80040190's cast). While +0x0C is
   zero it latches the leader out of `**p` into D_800DF364 -- announcing 0x507
   and arming the four-frame glide whenever the leader changes, or falling
   back to D_800E3D7C when there is none -- and ramps the renderable's
   +0x1C/+0x1E up by 0x200 to a 0x1000 ceiling; afterwards it eases them back
   down by 1/+0x0E per frame. Then it tints the sprite by whether the leader
   is the engine's +0xAC actor and glides the mover onto the leader's
   position over the +0x0E frames left.

   TWO SPELLINGS, EACH WORTH ONE WORD.
   (1) The +0x0C increment reads `*((u16 *)p + 6)`, not `*(u16 *)(p + 0xC)`.
   Only the pointer-add form is MEM_IN_STRUCT_P, and only an in-struct load
   hoists over the `%lo(D_800DF364)` store -- which is what fills the `lhu`'s
   load-delay slot with the symbol's `lui`. The cast form is 167 words of 168
   with a `nop` there.
   (2) The three colour constants are stored in ALL THREE arms, not assigned
   to one local and stored after the join: cross-jumping merges the `sw` back
   and the value keeps $v0, where the shared-local form hands it $a0, costs a
   hoisted `lui` and is 166 words / 87 differing.
   168 words, 0 differing. */
void func_800B96C4(u8 *p, UnkVec *b, u8 *c)
{
    u8 *r;
    u8 *s;
    u8 *w;
    s32 t;
    s32 n;

    if (*(s16 *)(p + 0xC) == 0)
    {
        w = *(u8 **)(*(u8 **)p);
        if (w != NULL)
        {
            if (D_800DF364 != w)
            {
                func_800A56E0(0x507);
                *(s16 *)(p + 0xE) = 4;
                D_800DF364 = *(u8 **)(*(u8 **)p);
            }
        }
        else
        {
            *(s16 *)(p + 0xE) = 4;
            D_800DF364 = D_800E3D7C;
            *(s16 *)(p + 0xC) = *((u16 *)p + 6) + 1;
        }
        if (*(u16 *)(c + 0x1C) < 0x1000)
        {
            t = *(u16 *)(c + 0x1E) + 0x200;
            *(s16 *)(c + 0x1E) = t;
            *(s16 *)(c + 0x1C) = t;
            if ((u16)t > 0x1000)
            {
                *(s16 *)(c + 0x1E) = 0x1000;
                *(s16 *)(c + 0x1C) = 0x1000;
            }
        }
    }
    else
    {
        n = *(s16 *)(p + 0xE);
        if (n != 0)
        {
            t = *(u16 *)(c + 0x1E) - *(u16 *)(c + 0x1E) / n;
            *(s16 *)(c + 0x1E) = t;
            *(s16 *)(c + 0x1C) = t;
        }
        else
        {
            *(s16 *)(c + 0x1E) = 0;
            *(s16 *)(c + 0x1C) = 0;
        }
    }
    w = *(u8 **)(*(u8 **)p);
    if (w == NULL)
    {
        *(s32 *)(c + 0xC) = 0x808080;
    }
    else if (w == *(u8 **)(D_800E3D7C + 0xAC))
    {
        *(s32 *)(c + 0xC) = 0x101080;
    }
    else
    {
        *(s32 *)(c + 0xC) = 0x801010;
    }
    r = D_800DF364;
    s = *(u8 **)(r - 0x18);
    if (*(s16 *)(p + 0xE) != 0)
    {
        b->unk_00 += (*(s32 *)(s + 0x0) - b->unk_00) / *(s16 *)(p + 0xE);
        b->unk_04 += (*(s32 *)(s + 0x4) - b->unk_04) / *(s16 *)(p + 0xE);
        b->unk_08 += ((*(s16 *)(r + 0x88) - *(s16 *)((u8 *)b + 0xA)) << 16) / *(s16 *)(p + 0xE);
        if (--*(s16 *)(p + 0xE) == 0)
        {
            if (*(s16 *)(p + 0xC) == 0)
            {
                *(s16 *)((u8 *)b + 0x2) = *(u16 *)(s + 0x2);
                *(s16 *)((u8 *)b + 0x6) = *(u16 *)(s + 0x6);
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(r + 0x88);
            }
            else
            {
                *(u16 *)(p - 0x2) |= 0x8000;
                D_8008346C = 0;
                D_800814A0 |= 0x8000;
            }
        }
    }
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x40;
    func_800478B8(c);
}

/* A spawner: allocate a 0x12 node, install func_80045C34 as the engine hook
   and func_800B96C4 as the node's step, seed the mover from D_80083780 and
   the owner's +0x88 facing, dress the renderer, point the view's +0x00 back
   at the owner, publish the node in D_8008346C and open the message box.
   69 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING, both worth one word and both measured.
   (1) `q` MUST BE ASSIGNED ABOVE THE func_8003DB94 CALL. cse rewrites the
   bare `*q` into `0x20($s2)` but leaves `*(q + 0xE)` alone, so `q` has one
   use left and combine folds the `addiu` away -- 68 words -- unless a CALL
   sits between the assignment and the use, which combine will not cross.
   (2) THE +0x10 INSTALL IS A CAST, NOT `n->unk_10`. As a COMPONENT_REF the
   store carries MEM_IN_STRUCT_P, sched1 proves it independent of the two
   node loads and sinks it four insns, and the two address materialisations
   interleave: 10 differing with every opcode already right. */
UnkNode *func_800B9964(u8 *p)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *q;
    u8 *g;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        func_8004491C(n, (u32)func_80045C34);
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800B96C4;
        g = D_80083780;
        m = n->unk_08;
        r = (UnkSlot *)n->unk_0C;
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = *(s16 *)(g + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(s16 *)(g + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(*(u8 **)p + 0x88);
        func_8003DB94(r, D_80079444, 0);
        r->unk_1E = 0x100;
        r->unk_1C = 0x100;
        r->unk_10 = 0x20;
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_06 = 4;
        r->unk_14 |= 0xC;
        *(s16 *)(q + 0xE) = 4;
        *(u8 **)q = p;
        D_8008346C = (s32)n;
        func_800C77D0(n, (u8 *)m, 8, D_800DCE66);
    }
    return n;
}

/* The kind-0x212 party-follow step func_800BA074 installs at +0x10, so the
   three parameters are func_80040190's cast and share func_800B96C4's shape.
   While the +0x0E glide lasts it eases the record's +0x14/+0x18/+0x08 onto the
   leader's mover (reached as `**p` minus 0x18, func_800B96C4's spelling) and
   drops the height by the leader's D_800DDC40 footprint; when the glide ends it
   snaps them and latches the facing. Then it re-targets onto the engine's +0xAC
   or +0xB0 actor when D_80083160's +0x10 asks, retires the node on the 0x20 bit,
   announces on the 0x40 bit, and finally writes the mover from the camera
   bearing at D_80083160's +0xC8.

   383 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING, both measured.

   (1) THE TWO `= 8` STORES AT +0x0E ARE `*((s16 *)p + 7)`, THE POINTER-ADD
   FORM. Only the pointer-add form is MEM_IN_STRUCT_P (func_800B96C4's rule).
   A varying-address STRUCT store is what sched.c's true_dependence excuses
   against the fixed-address non-struct `lw %lo(D_800E3D7C)` beside it, so the
   global's load hoists over the `sh` and the literal 8 keeps its own register
   one insn longer -- the target's `addiu $v1,$zero,8` / `lw` / `sh $v1`.
   Spelled `*(s16 *)(p + 0xE) = 8` the two conflict, the `sh` goes first and
   the load has to re-materialise after it: 383 words, 8 aligned differences,
   at both sites.

   (2) THE TWO `&D_80083460` BASES ARE SEPARATE LOCALS, one per arm. Shared,
   the local is referenced in two regions, so it is a GLOBAL allocno and
   local-alloc can no longer tie the `high` pseudo to the `lo_sum` that
   consumes it: both materialisations come out two-register, `lui $v0` /
   `addiu $v1,$v0,%lo`, where the target has the one-register `lui $v1` /
   `addiu $v1,$v1,%lo`. Worth 4 aligned differences.

   What was right on the first compile and should not be disturbed: the
   leader actor is RE-SPELLED INLINE at every one of its nine sites, never
   bound to a local -- each intervening store kills the cse and the target
   reloads `*(u8 **)(*(u8 **)p)` every time; the countdown is an `s16 k`
   assigned from `*(u16 *)(p + 0xE) - 1`, which folds `k <= 0` into
   `sll 16`/`bgtz` with no `sra` (func_800B8CF0's rule); and D_80083160 is
   reached through `Unk80083160 *g`, the pointer local that keeps
   `addiu $s3, $v0, %lo(D_80083160)` in a callee-saved register across all
   four reads (func_8009BE2C's rule). */
void func_800B9A78(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g;
    u8 *s;
    u8 *q;
    u8 *q2;
    s32 n;
    s32 i;
    s16 k;

    g = &D_80083160;
    s = *(u8 **)(*(u8 **)(*(u8 **)p) - 0x18);
    if (*(s16 *)(p + 0xE) != 0)
    {
        *(s32 *)(p + 0x14) += (*(s32 *)(s + 0x0) - *(s32 *)(p + 0x14)) / *(s16 *)(p + 0xE);
        *(s32 *)(p + 0x18) += (*(s32 *)(s + 0x4) - *(s32 *)(p + 0x18)) / *(s16 *)(p + 0xE);
        if (*(s16 *)(p + 0x12) != 2)
        {
            *(s32 *)(p + 0x8) += (*(s32 *)(s + 0x8) - (D_800DDC40[(*(u8 **)(*(u8 **)p))[0x13]] << 16) -
                                  *(s32 *)(p + 0x8)) /
                                     *(s16 *)(p + 0xE) -
                                 ((func_800644B8(*(s16 *)(p + 0xE) << 8) * *(s16 *)(p + 0xE)) << 6);
            if ((*(s32 *)(*(u8 **)(*(u8 **)p) + 0x1C) & 0x80000) != 0)
            {
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) - 0x80;
            }
        }
        else
        {
            *(s32 *)(p + 0x8) += ((*(s16 *)(*(u8 **)(*(u8 **)p) + 0x88) << 16) - *(s32 *)(p + 0x8)) / *(s16 *)(p + 0xE);
        }
    }
    k = *(u16 *)(p + 0xE) - 1;
    *(s16 *)(p + 0xE) = k;
    if (k <= 0)
    {
        *(s32 *)(p + 0x14) = *(s32 *)(s + 0x0);
        *(s32 *)(p + 0x18) = *(s32 *)(s + 0x4);
        if (*(s16 *)(p + 0x12) != 2)
        {
            n = *(s16 *)(*(u8 **)(*(u8 **)p) + 0x88) - D_800DDC40[(*(u8 **)(*(u8 **)p))[0x13]];
            if ((*(s32 *)(*(u8 **)(*(u8 **)p) + 0x1C) & 0x40000) != 0)
            {
                n -= 0x40;
            }
            *(s16 *)(p + 0xA) = n;
            if ((*(s32 *)(*(u8 **)(*(u8 **)p) + 0x1C) & 0x80000) != 0)
            {
                *(s16 *)(p + 0xA) = n - 0x80;
            }
        }
        else
        {
            *(s16 *)(p + 0xA) = *(u16 *)(*(u8 **)(*(u8 **)p) + 0x88);
        }
        *(s16 *)(p + 0xE) = 0;
    }
    if (*(s16 *)(p + 0xC) == 0)
    {
        if (*(s16 *)(p + 0xE) == 0)
        {
            if ((g->unk_10 & 2) != 0 && func_800BA33C(*(u8 **)(D_800E3D7C + 0xAC)) != 0 &&
                *(u8 **)(D_800814A8 + 0xAC) != *(u8 **)(*(u8 **)p))
            {
                *((s16 *)p + 7) = 8;
                *(u32 *)p = (u32)(D_800E3D7C + 0xAC);
                if (*(s16 *)(p + 0x12) == 2)
                {
                    *(s32 *)(c + 0xC) = 0x101080;
                }
            }
            if ((g->unk_10 & 1) != 0 && func_800BA33C(*(u8 **)(D_800E3D7C + 0xB0)) != 0 &&
                *(u8 **)(D_800814A8 + 0xB0) != *(u8 **)(*(u8 **)p))
            {
                *((s16 *)p + 7) = 8;
                *(u32 *)p = (u32)(D_800E3D7C + 0xB0);
                if (*(s16 *)(p + 0x12) == 2)
                {
                    *(s32 *)(c + 0xC) = 0x801010;
                }
            }
        }
        if ((g->unk_10 & 0x20) != 0)
        {
            if (*(s16 *)(p + 0x12) == 0)
            {
                q = (u8 *)&D_80083460;
                *(s16 *)(q + 0xA) = *(u16 *)(q + 0xA) - 1;
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        if ((g->unk_10 & 0x40) != 0)
        {
            *(s16 *)(p + 0xC) = 1;
            if (*(s16 *)(p + 0x12) == 0)
            {
                if ((*(u8 **)(*(u8 **)p))[0x13] == 0)
                {
                    func_8009F644(D_800E3D7C, 0x48, (s16)func_80098C80(*(u8 **)(p + 0x2C)), 0);
                }
                else
                {
                    func_8009F644(D_800E3D7C, 0x50, *(u8 **)(*(u8 **)p) == *(u8 **)(D_800E3D7C + 0xB0),
                                  (s16)func_80098C80(*(u8 **)(p + 0x2C)));
                }
            }
        }
    }
    else
    {
        if (*(s16 *)(p + 0x12) == 0)
        {
            func_800C77D0(*(u8 **)(*(u8 **)p) - 0x20, *(u8 **)(*(u8 **)(*(u8 **)p) - 0x18), 8, D_800DCE66);
            if ((s16)func_80098920(*(u8 **)(*(u8 **)p), *(u8 **)(p + 0x2C), 0x15, 0) < 0)
            {
                return;
            }
            q2 = (u8 *)&D_80083460;
            *(s16 *)(q2 + 0xA) = *(u16 *)(q2 + 0xA) - 1;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    k = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(p + 0x10) = k;
    if (*(s16 *)(p + 0x12) == 2)
    {
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x40;
    }
    else
    {
        *(s32 *)(p + 0x4) += func_800644B8(k << 7) << 5;
    }
    if (*(s16 *)(p + 0x12) == 0)
    {
        i = *(u16 *)(p + 0x24) & 0x1F;
        *(s16 *)(p + 0x24) = i;
        if (i < 0x10)
        {
            *(s16 *)(c + 0x18) = *(u16 *)(c + 0x18) + 0x80;
        }
        *(s16 *)(c + 0x18) = ((*(u16 *)(c + 0x18) + 0x400) & 0x7FF) - 0x400;
        *(s16 *)(p + 0x24) = *(u16 *)(p + 0x24) + 1;
    }
    if (*(s32 *)c != 0)
    {
        func_800478B8(c);
    }
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(p + 0x16) + ((func_80064584(g->unk_C8) * *(s16 *)(p + 0x1C)) >> 11);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(p + 0x1A) + ((func_800644B8(g->unk_C8) * *(s16 *)(p + 0x1C)) >> 11);
    b->unk_08 = *(s32 *)(p + 0x4) + *(s32 *)(p + 0x8) + (*(s16 *)(p + 0x20) << 15);
}

extern u8 D_80083780[];
extern u8 D_800DF368[];

void func_800B9A78();
s32 func_800BA074(u8 *a)
{
    UnkNode *t[3];
    UnkVec *m;
    u8 *r;
    u8 *v;
    s32 i;
    /* The constant 2 has to reach a register BEFORE `g2`'s address does, and
       the only way to put it there is a local initialised in the loop's own
       init clause. loop.c's move_movables emits the movables with
       emit_insn_before(.., loop_start), so they land in the preheader in the
       order scan_loop found them, which is body order: with `2` first
       materialised at the `i == last` test, `g2 = D_80083780` is movable #1
       and the constant #2, and global.c's allocno_compare then reads
       2*7/234 = 598 for the address against 2*7/232 = 603 for the constant,
       so the constant takes $s7 and the address $fp -- the target's two
       registers, swapped, and nothing else differs. Setting `last` here
       instead leaves it before the preheader entirely, which makes the
       address 232 and the constant 242 and reverses the pair. It has to be
       AFTER `i = 0` (hence the comma, not a statement before the loop):
       whichever of the two is emitted first fills the delay slot of
       `bnez $v0`, and the target's delay slot holds `i = 0`.
       `g2` cannot simply be moved down past the `i == last` test instead --
       scan_loop refuses to hoist it there. Past the `if (t[i] != NULL)` jump
       maybe_never is 1, `g2` is a user variable so condition (2) is out, and
       its three uses straddle two basic blocks so condition (1) is too; the
       measured result is 176 of 178 words, 173 differing. Spelling the
       address inline instead makes it a non-uservar that hoists from
       anywhere, but then the %lo folds into each load and the lo_sum
       disappears: 175 words. */
    s32 last;
    u8 *g;
    u8 *g2;

    if (func_8003FA44(3) == 0)
    {
        return 0;
    }
    for (i = 0, last = 2; i < 3; i++)
    {
        g2 = D_80083780;
        t[i] = func_8003FC64(0x12);
        if (t[i] != NULL)
        {
            t[i]->unk_10 = (u32)func_800B9A78;
            if (i == last)
            {
                func_8004491C(t[2], (u32)func_80045C34);
            }
            else
            {
                func_8004491C(t[i], (u32)D_80045340);
            }
            m = t[i]->unk_08;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(g2 + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(g2 + 0x6);
            r = (u8 *)t[i]->unk_0C;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(s32 *)(r + 0xC) = 0x2C808080;
            *(s16 *)(r + 0x6) = 4;
            if (i == 0)
            {
                func_800C77D0(t[0], (u8 *)m, 8, D_800DCE66);
                *(u32 *)(r + 0x8) = func_8004A658(a[1], a[0]);
            }
            else if (i == 1)
            {
                *(u32 *)(r + 0x8) = (u32)D_800DF368;
            }
            else
            {
                func_8003DB94(r, D_80079444, 0);
                *(u16 *)(r + 0x14) |= 0xC;
            }
            v = (u8 *)t[i] + 0x20;
            *(s16 *)(v + 0xA) = *(u16 *)(g2 + 0xA) - D_800DDC40[0];
            *(u32 *)v = (u32)&D_800814A8;
            *(s16 *)(v + 0x12) = i;
            *(s16 *)(v + 0x16) = *(u16 *)((u8 *)m + 0x2);
            *(s16 *)(v + 0x1A) = *(u16 *)((u8 *)m + 0x6);
            if (i != 0)
            {
                *(u32 *)(v + 0x28) = (u32)t[0];
            }
            else
            {
                *(u32 *)(v + 0x2C) = (u32)a;
            }
            if (func_800BA33C(*(u8 **)(D_800E3D7C + 0xAC)) != 0)
            {
                *(s16 *)(v + 0xE) = 8;
                *(u32 *)v = (u32)(D_800E3D7C + 0xAC);
                if (*(s16 *)(v + 0x12) == last)
                {
                    *(s32 *)(r + 0xC) = 0x101080;
                }
            }
            else if (func_800BA33C(*(u8 **)(D_800E3D7C + 0xB0)) != 0)
            {
                *(s16 *)(v + 0xE) = 8;
                *(u32 *)v = (u32)(D_800E3D7C + 0xB0);
                if (*(s16 *)(v + 0x12) == last)
                {
                    *(s32 *)(r + 0xC) = 0x801010;
                }
            }
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    func_800B1768(0, 0x27, 0x40, 0x209, 0, 0);
    func_800B1B10(a, 0x4C, 0x50, 0x200, 0, 2);
    return (s32)t[0];
}

/* THE `!= 0` MUST BE SPLIT OFF THE AND. `return (x & 0x20000) != 0;` folds
   to srl 17 / andi 1 (3 differing); cc1psx expands statement by statement,
   so assigning the AND to a local first leaves a plain register compare and
   gives lui / and / sltu. 18 words, 0 differing. */
s32 func_800BA33C(u8 *p)
{
    s32 v;

    if (p == NULL || (p[0x13] == 0x18 && p[0xB5] == 1))
    {
        return 0;
    }
    v = *(s32 *)(p + 0x1C) & 0x20000;
    return v != 0;
}

/* The node walk of func_800BC054, with a 62x62 quad parked in the
   scratchpad's four RotAverage4 input vectors first.

   THE TWELVE STORES ARE FIVE CHAINED ASSIGNMENTS. The stores keep source
   order here, so the target's order (8C, 84, 7C, 74, then 80, 70, then
   88, 78, then 7A, 72, then 8A, 82) is the source's, and each DESCENDING run
   is one chained assignment, because a chained assignment emits its stores
   right to left. All 24 regular orders -- record-major and field-major, both
   record directions, every field permutation -- were measured: the floor is
   10 differing and plain ascending is 14. 36 words, 0 differing. */
s32 func_800BA384(u8 *p, UnkVec *v, UnkSlot *s)
{
    Unk463ECSP *sp = (Unk463ECSP *)0x1F800000;
    UnkNode *n;

    sp->unk_70[0].vz = sp->unk_70[1].vz = sp->unk_70[2].vz = sp->unk_70[3].vz = 0;
    sp->unk_70[0].vx = sp->unk_70[2].vx = -0x1F;
    sp->unk_70[1].vx = sp->unk_70[3].vx = 0x1F;
    sp->unk_70[0].vy = sp->unk_70[1].vy = -0x1F;
    sp->unk_70[2].vy = sp->unk_70[3].vy = 0x1F;
    for (;;)
    {
        func_800BA414(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* The per-node billboard pass func_800BA384 drives: park the mover's
   position in the scratchpad matrix at +0xD0, compose it with the camera's
   own rotation, RotAverage4 the four 62x62 corners func_800BA384 left at
   +0x70 into the POLY_F4 at the +0x8D0 cursor, and -- when the OT index is
   in range and at least one corner is on screen -- hang the quad and the
   0xC-byte primitive behind it on the OT row, consuming 0x24 bytes of
   cursor. PushMatrix/PopMatrix bracket the whole thing.

   THE FOUR ON-SCREEN TESTS ARE `|`, NOT `||`: each `or` sits in the NEXT
   term's branch delay slot, which is the signature of one left-associative
   `|` expression over four `&&` terms -- docs/matching-levers.md, "Four `||`
   terms whose `or`s sit in the delay slots are `|`, written INLINE".

   THE CURSOR IS BUMPED INSIDE THE `if` AND STORED BACK AFTER IT: the
   `sw $s1, 0x8D0($v0)` in PopMatrix's delay slot is at the JOIN, so a
   skipped quad writes the cursor back unchanged.

   The scratchpad is a struct typedef (UnkDungeonSP3, declared at the top)
   rather than raw casts, the same MEM_IN_STRUCT_P rule every other
   0x1F800000 record in this unit follows; +0xE4/+0xE8/+0xEC are the `t`
   vector of the MATRIX at +0xD0, which is why they are `sw` and not `sh`.
   169 words, 0 differing on the first compile. */
void func_800BA414(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDungeonSP3 *sp = (UnkDungeonSP3 *)0x1F800000;
    Unk80083160 *g;
    u8 *q;
    u8 *r;

    g = &D_80083160;
    sp->unk_20 = g->unk_00 + 0xB0;
    sp->unk_D0.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_D0.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_D0.t[2] = *(s16 *)((u8 *)v + 0xA);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    PushMatrix();
    sp->unk_100 = 0;
    sp->unk_104 = 0;
    RotMatrix((SVECTOR *)&sp->unk_100, &sp->unk_D0);
    ReadRotMatrix(&sp->unk_11C);
    CompMatrix(&sp->unk_11C, &sp->unk_D0, &sp->unk_50);
    SetTransMatrix(&sp->unk_50);
    SetRotMatrix(&sp->unk_50);
    sp->unk_C0 = RotAverage4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                             (s32 *)(q + 0x8), (s32 *)(q + 0xC), (s32 *)(q + 0x10), (s32 *)(q + 0x14),
                             &sp->unk_90, &sp->unk_94) -
                 d - 8;
    if ((u32)sp->unk_C0 < 0x1E0)
    {
        if (((u16)(*(u16 *)(q + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0xA) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0xC) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0xE) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x14) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x16) + 0x20) < 0x121))
        {
            *(s32 *)(q + 0x4) = *(s32 *)((u8 *)s + 0xC);
            SetPolyF4(q);
            r = q + 0x18;
            q[0x7] |= 2;
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
            q += 0x24;
            func_80067F20((u32 *)r, 0, 0, 0x20, 0);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, r);
        }
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    PopMatrix();
}

/* A node +0x10 handler, the same shape as func_800A6B70 next door.
   `extern u16 D_800DF37C;` as a SCALAR: the `%lo` sits inside the `lhu`, not
   in a separate `addiu`. 43 words, 0 differing. */
void func_800BA6B8(u8 *p, s32 b, UnkDungeonSprite *s)
{
    s32 v;

    if (D_800DF37C != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    v = (func_800644B8(D_80083164[0] << 8) >> 7) - 0x80;
    if (p[0x30] != 0)
    {
        s->unk_0C = v;
    }
    if (p[0x31] != 0)
    {
        s->unk_0D = v;
    }
    if (p[0x32] != 0)
    {
        s->unk_0E = v;
    }
}

/* Spawner: a kind-0x202 node carrying func_800BA6B8 at +0x10 and
   func_800BA384 as its draw callback.

   `x * 0x40 + 0x20`, NOT `(x << 6) + 0x20`. The shift form lets gcc drop the
   u16 parameter's zero-extension, which is the `andi $v0, $s1, 0xFFFF` the
   target has at each use: 41 words of 43, 30 differing, and the four
   parameter copies then come out in declaration order where the target copies
   $a2/$a3 first. 43 words, 0 differing. */
UnkNode *func_800BA764(u16 x, u16 y, s16 ang, s32 w)
{
    UnkNode *n;
    UnkVec *m;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BA6B8;
        func_8004491C(n, (u32)func_800BA384);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 0x40 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 0x40 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        *(s32 *)((u8 *)n + 0x50) = w;
    }
    return n;
}

void func_800BA810(u8 *p, u16 kind)
{
    u8 *q;
    u8 *v;
    s32 i;
    s32 cz;
    s32 cx;
    s32 h;
    s32 t;
    s16 t2;
    s32 d;
    s32 idx;
    s32 j;
    u16 fa;
    u8 *w;

    if (p == NULL)
    {
        /* DEVICE: 3 differing (rows 44/46/47, the preamble's `lw` and the two
           `lhu`s off it).  `v` has to be a GLOBAL allocno to come out in $v1:
           with it block-local, local-alloc's qty_compare ties it with both
           `lhu` temps at the same priority and the tie goes to the lowest
           quantity number, which is `v`, so it takes $v0 and the two temps
           shift one register each.  Sharing `v` with the +0x60 tail below is
           what made it global, but then the tail's pointer is $v1 where the
           target has $v0 (that is the separate `w`).  A plain
           `__asm__("" ::"r"(v))` in an arm of the chain does make it global
           and is 3 DIFFERING, but it is a USE, so `v` is then live across the
           whole compare chain and lands in $a0.  An asm OUTPUT in this
           already-taken early-return arm adds the second block without
           extending the live range at all; it must be __volatile__, since a
           non-volatile asm whose only output is dead is deleted by flow and
           the reference goes with it (5 differing). */
        __asm__ __volatile__("" : "=r"(v));
        D_800DF374 = NULL;
        func_800403BC((s32)func_800BA6B8);
        return;
    }
    if (D_800DF374 == p && D_800DF37A == *(s16 *)(p + 0x2A) &&
        D_800DF378 == kind)
    {
        return;
    }
    fa = *(u16 *)(p + 0x2A);
    D_800DF374 = p;
    D_800DF378 = kind;
    D_800DF37A = fa;
    func_800403BC((s32)func_800BA6B8);
    t = kind & 0x3FFF;
    t2 = t;
    v = *(u8 **)(p - 0x18);
    h = *(s16 *)(p + 0x88);
    cx = *(u16 *)(v + 0x2) >> 6;
    cz = *(u16 *)(v + 0x6) >> 6;
    d = (*(u16 *)(p + 0x2A) >> 9) & 7;
    if (t2 == 8)
    {
        i = 1;
        if (p[0x13] == 0x16)
        {
            if (p[0x48] == 0xF)
            {
                i = 0xA;
            }
        }
        while (i > 0)
        {
            cx += D_8006CCD8[d];
            cz += D_8006CCE8[d];
            h = (s16)func_800BCB04(((cx << 6) + 0x20) & 0xFFE0,
                                   ((cz << 6) + 0x20) & 0xFFE0, (s16)(h - 0x20));
            if (h >= 0x201)
            {
                return;
            }
            func_800BA764(cx, cz, h, 0xFF);
            i--;
        }
    }
    else if (t2 == 0xB)
    {
        i = 1;
        while (i > 0)
        {
            cx += D_8006CCD8[d];
            cz += D_8006CCE8[d];
            h = (s16)func_800BCB04(((cx << 6) + 0x20) & 0xFFE0,
                                   ((cz << 6) + 0x20) & 0xFFE0, (s16)(h - 0x20));
            if (h >= 0x201)
            {
                return;
            }
            func_800BA764(cx, cz, h, 0xFF00);
            i--;
        }
    }
    else if (t2 == 0xC)
    {
        func_800BA764(cx, cz, h, 0xFFFF);
    }
    else if ((u32)(t - 1) < 3)
    {
        j = (t2 - 1) * 3;
        idx = *(p + j + 0x8);
        if (D_8006DE24_n[idx].unk_12 == 1)
        {
            i = D_800DF380[idx] & 0x1F;
            while (i > 0)
            {
                cx += D_8006CCD8[d];
                cz += D_8006CCE8[d];
                h = (s16)func_800BCB04(((cx << 6) + 0x20) & 0xFFE0,
                                       ((cz << 6) + 0x20) & 0xFFE0, (s16)(h - 0x20));
                if (h >= 0x201)
                {
                    return;
                }
                func_800BA764(cx, cz, h, 0xFF);
                i--;
            }
        }
        else if (D_8006DE24_n[idx].unk_12 == 2)
        {
            q = *(u8 **)(p + 0x60);
            if (q != NULL)
            {
                w = *(u8 **)(q - 0x18);
                func_800BA764(*(u16 *)(w + 0x2) >> 6, *(u16 *)(w + 0x6) >> 6,
                              *(s16 *)(q + 0x88), 0xFF);
            }
        }
    }
}
/* The 32-element fan of semi-transparent Gouraud quads a kind-0x202-style node
   carries at +0x10. Each element's +0x12 phase byte advances by 4 a frame and
   wraps (or sticks at 0x30 while p[0x10] is set); the pair of RotTransPers
   input vectors sits on a circle of radius func_80064584/func_800644B8 of
   `(i + (*(s16 *)(p + 0xE) << 6)) << 7'; the three colour channels are gated by
   the slot's +0x14 flag word; and each visible element emits one 0x24-byte
   POLY_G4 plus the 0xC-byte primitive func_80067F20 fills behind it.

   THREE SPELLINGS, AND THE FIRST IS THE WHOLE RESIDUE.
   (1) `s16 i = 0;' DECLARED AND INITIALISED IMMEDIATELY AFTER `g', with the
   loop written `for (; i < 0x20; i++)'. 12 differing -> 0. The target sets `i'
   at word 8, ahead of all three constant pointer locals, and that one position
   decides which of `sp' and `fb' owns the shared `lui 0x1F80' -- the target has
   `lui $s2' then `addu $s6, $s2'; with `i = 0' left in the `for' it is
   `lui $s6' then `move $s2, $s6'. `i' initialised in the `for', or declared
   `s16 i = 0;' after `q', are both 12, and all six declaration orders of
   {sp, fa, fb} are 12 or 16 -- the lever is `i', not the pointers.
   (2) `*(p + i + 0x12)' and NOT `p[i + 0x12]': 13 -> 12. The subscript form
   folds to `addu $v1, $v0, $s4' (index first) where the target has
   `addu $v1, $s4, $v0', the same operand order its own `addu $s0, $s4, $s0'
   uses later for the identical address.
   (3) The two RotTransPers out-parameters at +0x90/+0x94 are CONSTANT
   ADDRESSES IN POINTER LOCALS, not members of UnkDungeonSPA: that is what
   gives one `lui'/`ori' in the prologue and a one-word `addu $a2, $s7, $zero'
   at each call, where `&sp->unk_90' is a two-word `addiu' per call. Same
   device func_800B9144 uses for its `flag'.
   The +0x12 phase byte has both faces in the listing and they are not
   interchangeable: `u8' for the increment (`lbu'/`addiu'/`sb', with cse
   forwarding the stored value) and `(s8)' at every test and use.
   215 words, 0 differing. */
s32 func_800BABA8(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g = &D_80083160;
    s16 i = 0;
    UnkDungeonSPA *sp = (UnkDungeonSPA *)0x1F800000;
    s32 *fa = (s32 *)0x1F800090;
    s32 *fb = (s32 *)0x1F800094;
    UnkDungeonG4 *q;
    s32 n;
    s32 cc;
    s32 z;
    s32 r;
    s32 gg;
    s32 bb;

    q = *(UnkDungeonG4 **)(D_80083160.unk_00 + 0x8D0);
    p[0x11] = 0;
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    sp->unk_00 = *(u16 *)((u8 *)b + 0x2);
    sp->unk_02 = *(u16 *)((u8 *)b + 0x6);
    sp->unk_04 = *(u16 *)((u8 *)b + 0xA);
    for (; i < 0x20; i++)
    {
        *(p + i + 0x12) += 4;
        if ((s8) * (p + i + 0x12) >= 0x30)
        {
            if (p[0x10] != 0)
            {
                *(p + i + 0x12) = 0x30;
                continue;
            }
            *(p + i + 0x12) = 0;
        }
        q->unk_04 = 0;
        q->unk_0C = 0;
        sp->unk_70[1].vx = sp->unk_70[0].vx =
            sp->unk_00 + ((func_80064584((i + (*(s16 *)(p + 0xE) << 6)) << 7) << 4) >> 11);
        sp->unk_70[1].vy = sp->unk_70[0].vy =
            sp->unk_02 + ((func_800644B8((i + (*(s16 *)(p + 0xE) << 6)) << 7) << 4) >> 11);
        z = sp->unk_04 - (*(s8 *)(p + i + 0x12) << 1);
        sp->unk_70[0].vz = z;
        sp->unk_70[1].vz = z + 0x40;
        n = rand();
        cc = *(s8 *)(p + i + 0x12);
        if (cc > 0x20)
        {
            n = (s16)n / (cc - 0x20);
        }
        else if (cc < 0x10)
        {
            n = (s16)n / (0x10 - cc);
        }
        r = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x1) != 0)
        {
            r = n;
        }
        q->unk_14 = r;
        q->unk_1C = r;
        gg = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x4) != 0)
        {
            gg = n;
        }
        q->unk_15 = gg;
        q->unk_1D = gg;
        bb = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x2) != 0)
        {
            bb = n;
        }
        q->unk_16 = bb;
        q->unk_1E = bb;
        sp->unk_C0 = RotTransPers(&sp->unk_70[0], &q->unk_08, fa, fb);
        q->unk_10.w = q->unk_08;
        q->unk_10.h[0] += 4;
        sp->unk_C0 += RotTransPers(&sp->unk_70[1], &q->unk_18, fa, fb);
        q->unk_20.w = q->unk_18;
        q->unk_20.h[0] += 2;
        sp->unk_C0 >>= 1;
        *((u8 *)q + 0x3) = 8;
        *((u8 *)q + 0x7) = 0x3A;
        AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
        q++;
        func_80067F20((u32 *)q, 0, 0, 0x60, 0);
        AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
        q = (UnkDungeonG4 *)((u8 *)q + 0xC);
        p[0x11]++;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    return 0;
}

/* State 0 seeds the 0x40-byte noise table at +0x12 and falls into state 1
   WITHOUT breaking. THE INDEX MUST BE WRITTEN `(p + i)[0x12]`: as
   `p[i + 0x12]` the `addu` comes out index-first, which is exactly one
   differing word. `i` is s16, so the index and the bound test each pay their
   own `sll 16`/`sra 16`. 80 words, 0 differing. */
void func_800BAF04(u8 *p)
{
    s16 i;

    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        for (i = 0; i < 0x40; i++)
        {
            (p + i)[0x12] = rand() & 0x1F;
        }
        *(s16 *)(p + 0xE) = 0;
        p[0x10] = 0;
        p[0x32] = 0;
        p[0x11] = 1;
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_800BABA8);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
    case 1:
        *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        if (*(s16 *)(p + 0xE) >= 0x11)
        {
            *(s16 *)(p + 0xE) = 0;
        }
        p[0x32] = p[0x32] + 1;
        if ((s8)p[0x32] >= 0x21)
        {
            p[0x32] = 0x20;
            p[0x10] = 1;
        }
        if (p[0x11] == 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* Spawn the func_800BAF04 node for the payload `p`, parented on p's own node.
   NO LOCAL FOR THE PARENT NODE: `parent = (UnkNode *)(p - 0x20);` used twice
   costs a whole callee-saved register -- 28 words against 25, with $s1 saved
   and restored -- because the pointer stays live across the call.
   25 words, 0 differing. */
void func_800BB044(u8 *p)
{
    UnkNode *n;

    n = func_8003FD64(0x202, p - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BAF04;
        *(u32 *)((u8 *)n + 0x20) = (u32)p;
        n->unk_08 = *(UnkVec **)(p - 0x18);
        *(s16 *)((u8 *)n + 0x2C) = *(u16 *)(p + 0x2A);
        func_800A56E0(0x703);
    }
}

/* The two arms duplicate the call rather than share a third-argument
   variable: both load +0x10 and +0x12 for themselves and only the `jal` is
   cross-jumped. 41 words, 0 differing. */
void func_800BB0A8(u8 *p, void *b, u8 *c)
{
    if (D_80083462 & 0x10)
    {
        if (--*(s16 *)(p + 0x1E) <= 0)
        {
            if (*(s16 *)(p + 0x1C) == 1)
            {
                func_8009A3D0(*(s16 *)(p + 0x10), *(s16 *)(p + 0x12), 2);
            }
            else
            {
                func_8009A3D0(*(s16 *)(p + 0x10), *(s16 *)(p + 0x12), 0x10);
            }
            func_800B5F80(*(s16 *)(p + 0x20));
            return;
        }
    }
    *(u16 *)(c + 0x1A) += 0x40;
}

/* The UnkNode +0x10 handler func_800C7380 installs: the engine reaches it
   through func_80040190's cast in csrc/unk_12DEC.c, so the three parameters
   are (payload, mover, record) and the mover is unread here.

   A three-case switch on +0x14 -- case 0 FALLS THROUGH into case 1, which is
   why the clamp arm has no branch over the +0x1E test -- then a shared tail
   that fans the same 0x1F-masked counter byte into whichever of the record's
   +0xC/+0xD/+0xE slots +0x18's low three bits select.

   `*(s16 *)(a + 0x1A) * 16` must be a MULTIPLY: spelled `<< 4` the mask
   propagates back through the chain into the load and the target's
   `lh 0x1A` comes out `lhu` -- 102 words, 1 differing. The entry's
   `addu $a1, $a0, $zero` is gcc's own: an explicit `u8 *a = p;` second
   pseudo, the func_800D1E34 idiom, measures the same 102 / 0 differing, so
   the copy is not source evidence here.

   102 words, 0 differing. */
void func_800BB14C(u8 *p, UnkVec *b, u8 *c)
{
    s32 t;

    switch (*(s16 *)(p + 0x14))
    {
    case 0:
        *(u16 *)(c + 0x1E) += *(s16 *)(p + 0x1A) * 16;
        if (*(u16 *)(c + 0x1E) <= 0x4000)
        {
            *(u16 *)(c + 0x1E) = 0x4000;
            (*(s16 *)(p + 0x14))++;
        }
        /* fallthrough */
    case 1:
        if (*(u16 *)(*(u8 **)p + 0x1E) & 0x8000)
        {
            *(s16 *)(p + 0x16) = 4;
            *(s16 *)(p + 0x14) = 2;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x16) != 0)
        {
            *(u16 *)(c + 0x1E) -= *(u16 *)(c + 0x1E) / *(s16 *)(p + 0x16);
        }
        if (--*(s16 *)(p + 0x16) < 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        break;
    }
    (*(u16 *)(p + 0x1A))++;
    p[0xD] = (p[0xD] - 4) & 0xBF;
    t = p[0x1A] & 0x1F;
    if (*(u16 *)(p + 0x1A) & 0x20)
    {
        t = 0x30 - t;
    }
    else
    {
        t += 0x10;
    }
    if (*(u16 *)(p + 0x18) & 0x1)
    {
        c[0xC] = t;
    }
    if (*(u16 *)(p + 0x18) & 0x2)
    {
        c[0xD] = t;
    }
    if (*(u16 *)(p + 0x18) & 0x4)
    {
        c[0xE] = t;
    }
}

/* 158 words, 0 differing. The three-way overlay dispatcher: with a fifth
   argument it brackets func_800B8FC8's output in a mask primitive; without
   one, and only when the fourth argument's low halfword is zero, it emits a
   single 0x10-byte primitive from the two words at *a and *b; and whenever
   that halfword is NOT zero it falls through to the 0x60000000 tint
   rectangle, a second mask primitive and SetDrawArea's draw-area
   primitive.

   THE ONE SPELLING FACT: `s16 dd = d;' AT THE TOP, WITH THE TAIL TEST
   READING `dd' AND THE else-ARM TEST READING `d'. The target tests the
   fourth argument twice: `sll $v0, $a3, 16' in the else arm, out of the
   INCOMING ARGUMENT REGISTER, and `sll $v0, $s6, 16' at the tail, out of a
   callee-saved copy. That is two pseudos, and with one `s32' variable it is
   one: `d' then has three references across 178 insns, global.c's
   allocno_compare scores it 3*10000/178 = 168 against `lean''s
   2*10000/93 = 215, `lean' sorts first and takes $s6, and the function comes
   out 158 words / 7 differing -- every row the $s6/$s7 swap plus the else
   arm reading the copy.

   A PLAIN `s32 dd = d;' DOES NOT SPLIT THEM (158/7, byte-identical to the
   one-variable body): cse.c's make_regs_eqv makes whichever pseudo has the
   LATER last use the canonical one, so a copy read late swallows the
   parameter's early use and a copy read early is swallowed by it. An `s16'
   copy is a different mode, so insert_regs finds no same-mode REG in the
   equivalence class, `dd' gets a quantity of its own and both pseudos
   survive. `d' then crosses no call on the entry->else-arm path, local-alloc
   leaves it in $a3, and `dd' is 2 refs / live length shorter than `lean''s,
   so it scores above `lean' and takes $s6. `u16 dd' and `s16 dd' with the
   tail test written `(s16)dd == 0' are identical; `s32 dd = (s16)d;' puts
   the copy in $s5 and moves two other allocnos.

   THIS IS NOT THE "FOURTH REFERENCE" TWO EARLIER ATTEMPTS WENT LOOKING FOR.
   One of them concluded the window was unreachable by sweeping the reference
   count at a fixed live length; the correction to that was that a reference
   placed at the END lengthens the range too and does reach it. Both were
   reasoning about ONE pseudo. The target has two, and no reference count on
   a single variable produces that.

   Also measured: the scissor-off block written once after the join instead
   of in both arms is 153 words (cross-jumping leaves each arm its own
   D_80083160 reload); `s32 lean' is 158/11 (the target copies the boolean
   to $v0 in each arm, which an s32 tested straight out of $s6 does not);
   `lean' computed before `r' is 158/11 and before `g' 158/15; the two arms
   exchanged is 69 differing. The first argument's halfword arithmetic is a
   SIGNED divide `((s16) * ((u16 *)c + 2) / 2)', not the `>> 1' the sibling
   func_800B835C uses at the same slot. */
void func_800BB2E4(void *a, void *b, s32 *c, s32 d, void *e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;
    s16 dd;

    dd = d;
    g = &D_80083160;
    r = D_80083160.unk_00 + 0x8B0;
    lean = D_80083160.unk_00 != D_801C9E40;
    if (e != NULL)
    {
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 0, 0);
        AddPrim(r, q);
        w[0] = *(u16 *)c + ((s16) * ((u16 *)c + 2) / 2);
        w[1] = *((u16 *)c + 1) + *((u16 *)c + 3);
        func_800B8FC8((u8 *)e, c, (s32 *)w, 0, 0);
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 1, 0);
        AddPrim(r, q);
    }
    else
    {
        if ((s16)d != 0)
        {
            goto rect;
        }
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x8) = *(u32 *)a;
        *(u32 *)(u + 0xC) = *(u32 *)b;
        if (lean)
        {
            *(u16 *)(u + 0xA) -= 0xE0;
            *(u16 *)(u + 0xE) -= 0xE0;
        }
        u[0x3] = 3;
        *(u32 *)(u + 0x4) = 0;
        u[0x7] = 0x40;
        AddPrim(r, u);
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 0, 0);
        AddPrim(r, q);
    }
    if (dd == 0)
    {
        return;
    }
rect:
    u = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(u + 0x10);
    *(u32 *)(u + 0x4) = 0x60000000;
    u[0x3] = 3;
    *(u16 *)(u + 0x8) = *(u16 *)c;
    t = *((s16 *)c + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    *(s16 *)(u + 0xA) = t;
    *(u16 *)(u + 0xC) = *((u16 *)c + 2);
    *(u16 *)(u + 0xE) = *((u16 *)c + 3);
    AddPrim(r, u);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    func_80067EF4(q, 0, 1);
    AddPrim(r, q);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, c);
    AddPrim(r, q);
}

/* The dungeon's five-spoke ring effect: prime the VRAM pair, lay the five
   spokes' endpoints with func_80064584/func_800644B8 and func_800A07D0, then
   walk each spoke outward once per frame, growing +0x30 up to 0x1F and firing
   func_800A56E0(0x603) through the actor's +0x60 predicate when the ring
   completes, and finally spawn four func_800BC0A8 sparks per frame until the
   +0x64 countdown runs out. 305 words, 0 differing.

   THE WALKED POINTER MUST BE `r = a + i * 2;` AS THE BODY'S FIRST STATEMENT.
   Written `r = a; ... r += 2;` the pointer is a biv, loop.c strength-reduces
   the two MEM addresses into ONE giv and BIASES the register by the larger
   displacement -- `addiu $s1, $s4, 60` with `lh -0xC($s1)` where the target
   has `addu $s2, $s4, $zero` with `lh 0x30($s2)`/`sh 0x3C($s2)`. 51 aligned
   differences against 46.

   AND IN THE SECOND LOOP THE INDEX GOES INLINE AT EVERY SITE. The target
   eliminates that loop's counter and tests the pointer against `a + 0xA` with
   a SIGNED `slt`, which is biv elimination rewriting `i < 5`; a pointer
   comparison would be `sltu`. Keeping `u` as a named local leaves the counter
   and costs a second register (22 aligned differences); writing
   `*(s16 *)(a + i * 2 + 0x30)` at all four sites is 5.

   THE THIRD PARAMETER IS COPIED INTO A LOCAL. The target holds it in BOTH
   $s1 and $fp -- `addu $s1, $a2, $zero` at entry and `addu $fp, $s1, $zero`
   before the first func_800BB2E4 call -- which is a source-level `d = c;`.
   Without it the parameter lives in one register and the two words are
   missing: 39 aligned differences against 22.

   THE TWO `ang` INCREMENTS ARE POSITION-PINNED. `ang += 0x999` belongs ABOVE
   the func_800BB2E4 call it precedes and `ang += 0x333` immediately after the
   inner loop, so each lands in the basic block whose schedule slot the target
   fills; written at the end of their loop bodies instead it is 5 aligned
   differences at the same 305 words. */
void func_800BB55C(u8 *a, u8 *b, u8 *c)
{
    s32 v[2];
    s16 p[2];
    s16 q[2];
    s16 ang;
    s16 t;
    s32 i;
    s32 j;
    u8 *r;
    u8 *u;
    u8 *w;
    u8 *g;
    u8 *d;

    v[0] = 0x1000340;
    v[1] = 0x460044;
    func_800478B8(a + 0xC);
    *(u8 **)(a - 0x14) = a + 0xC;
    d = c;
    func_800BB2E4(NULL, NULL, v, 0, a);
    ang = -0x400;
    for (i = 0; i < 5; i++)
    {
        r = a + i * 2;
        p[0] = (func_80064584(ang) >> 7) + 0x362;
        p[1] = (func_800644B8(ang) >> 7) + 0x122;
        q[0] = (func_80064584(ang + 0x999) >> 7) + 0x362;
        q[1] = (func_800644B8(ang + 0x999) >> 7) + 0x122;
        t = func_800A07D0(p[0], p[1], q[0], q[1]);
        q[0] = p[0] + ((func_80064584(t) * *(s16 *)(r + 0x30)) >> 11);
        q[1] = p[1] + ((func_800644B8(t) * *(s16 *)(r + 0x30)) >> 11);
        *(s16 *)(r + 0x3C) = ang;
        ang += 0x999;
        func_800BB2E4(p, q, v, 0, NULL);
    }
    ang = -0x400;
    for (i = 0; i < 5; i++)
    {
        p[0] = (func_80064584(ang) >> 7) + 0x362;
        p[1] = (func_800644B8(ang) >> 7) + 0x122;
        t = ang;
        for (j = 0; j < *(s16 *)(a + i * 2 + 0x30); j++)
        {
            t += 0x19;
            q[0] = (func_80064584(t) >> 7) + 0x362;
            q[1] = (func_800644B8(t) >> 7) + 0x122;
            func_800BB2E4(p, q, v, 0, NULL);
            p[0] = q[0];
            p[1] = q[1];
        }
        ang += 0x333;
        *(s16 *)(a + i * 2 + 0x30) = *(u16 *)(a + i * 2 + 0x30) + 1;
        if (*(s16 *)(a + i * 2 + 0x30) >= 0x1F)
        {
            *(s16 *)(a + i * 2 + 0x30) = 0x1F;
            if (*(s16 *)(a + 0x4A) == 0)
            {
                if ((*(s32(**)(void))(a + 0x60))() != 0)
                {
                    *(s16 *)(a + 0x50) = 0x120;
                    *(s16 *)(a + 0x4A) = *(u16 *)(a + 0x4A) + 1;
                    func_800A56E0(0x603);
                }
            }
            *(s16 *)(a + 0x4C) = 0x20;
        }
    }
    func_800BB2E4(NULL, NULL, v, 1, NULL);
    *(u8 **)(a - 0x14) = d;
    if (*(s16 *)(a + 0x4A) != 0)
    {
        *(s16 *)(a + 0x4E) = *(u16 *)(a + 0x4E) + 0x100;
        for (i = 0; i < 4; i++)
        {
            *(s16 *)(a + 0x4E) = *(u16 *)(a + 0x4E) + 0x400;
            func_800BC0A8(*(s16 *)(b + 0x2), *(s16 *)(b + 0x6), *(s16 *)(b + 0xA), *(s16 *)(a + 0x4E),
                          *(s16 *)(a + 0x50), *(s16 *)(a + 0x58), *(u32 *)(a + 0x5C), 6);
        }
        if (*(s16 *)(a + 0x58) < 0x20)
        {
            *(s16 *)(a + 0x58) = *(u16 *)(a + 0x58) + 1;
        }
        if (*(s16 *)(a + 0x4A) == 1)
        {
            *(s16 *)(a + 0x64) = *(u16 *)(a + 0x64) + 1;
            if (*(s16 *)(a + 0x64) >= 0x21)
            {
                *(s16 *)(a + 0x64) = 0x10;
                *(s16 *)(a + 0x4A) = *(u16 *)(a + 0x4A) + 1;
            }
        }
        else
        {
            w = a + 0x5C;
            w[0] -= w[0] / *(s16 *)(a + 0x64);
            w[1] -= w[1] / *(s16 *)(a + 0x64);
            w[2] -= w[2] / *(s16 *)(a + 0x64);
            *(s16 *)(a + 0x50) = *(u16 *)(a + 0x50) - *(s16 *)(a + 0x50) / *(s16 *)(a + 0x64);
            *(s16 *)(a + 0x64) = *(u16 *)(a + 0x64) - 1;
            if (*(s16 *)(a + 0x64) <= 0)
            {
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) -= 1;
                *(u16 *)(a - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
    }
}

/* 8 words, 0 differing. */
s32 func_800BBA20(void)
{
    func_80045C34();
    return 0;
}

/* The 0x12 marker spawner for the three kill handlers' out-of-range arm:
   func_800BB2E4 over {0x1000340, 0x1000080}, then func_800C1A44's spawner
   with func_800BB55C as the node's +0x10 and a five-slot angle fan.

   THE FAN IS AN INDEXED GIV, `r = q + i * 2;' AS THE BODY'S FIRST STATEMENT --
   func_800BB55C records the same rule. Walking the pointer instead
   (`q += 2' at the bottom) makes `i' dead in the body, loop.c REVERSES the
   loop into a countdown and the preheader loses the `addu $v1, $v0, $zero'
   copy the giv's init gives: 102 words against 104, 48 differing.

   THE FIFTH PARAMETER IS `s16', AND THAT IS THE WHOLE REGISTER ROTATION.
   As `s32' it is 104 words and 13 differing -- every difference a callee-saved
   register, with the four register parameters one slot too low and the stack
   parameter in $s6 where the target has $s3. The narrow declaration's extra
   truncation reference is what lifts it past them. `u16' measures the same;
   `u32' is 13 differing again. 104 words, 0 differing. */
s32 func_800BBA40(s16 x, s16 y, s16 ang, void *d, s16 e, s32 g, void *h)
{
    s32 v[2];
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    s16 a;
    s32 i;

    v[0] = 0x1000340;
    v[1] = 0x1000080;
    func_800BB2E4(NULL, NULL, v, 1, NULL);
    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        func_800A56E0(0x704);
        n->unk_10 = (u32)func_800BB55C;
        func_8004491C(n, (u32)func_800BBA20);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (u8 *)n->unk_0C;
        *(u8 **)(r + 0x8) = D_800DF3C0;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x6) = 8;
        func_8003DB94((u8 *)n + 0x2C, d, 0);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x2A) = e;
        *(s16 *)(q + 0x28) = e;
        *(u32 *)(q + 0x18) = 0x808080;
        *(u32 *)(q + 0x5C) = g;
        *(u32 *)(q + 0x60) = (u32)h;
        a = -0x400;
        for (i = 0; i < 5; i++)
        {
            r = q + i * 2;
            *(s16 *)(r + 0x3C) = a;
            a += 0x999;
        }
        func_800C77D0(D_80083498, D_80083780, 8, 0x300);
    }
    return (s32)n;
}

/* func_800BC0A8's node handler, installed at UnkNode +0x10 and therefore
   reached through func_80040190's three-argument cast: the view at node
   + 0x20, the mover, and the renderable. Swing the mover's two integer
   halves by rcos/rsin of +0x4E shifted right by +0x48, advance the angle by
   +0x58 and hand it to the renderable's +0x1A with a 0x400 bias, then run a
   three-state fade on +0x4A: state 0 eases the three colour bytes toward
   p[0..2] over +0x4C ticks, state 1 just counts +0x4C down, and the last
   state bleeds the colours away and kills the view when +0x4C runs out.

   IT IS AN `else if` CHAIN, NOT A `switch`. As `switch (*(s16 *)(p + 0x4A))`
   with `case 0`, `case 1` and `default`, gcc puts the DEFAULT body inline
   after the tests and branches to the two case bodies -- `beqz $v1` where the
   target has `bnez $v1` -- and the extra layout costs two words: 133 against
   131, 99 differing. The chain gives the target's order, case 0 inline after
   the head. Both case-0 and case-1 tails are written out in full; cross-
   jumping merges the shared `+0x4A` increment at the end of case 1.
   131 words, 0 differing. */
void func_800BBBE0(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(p + 0x4) + (func_80064584(*(s16 *)(p + 0x4E)) >> *(s16 *)(p + 0x48));
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(p + 0x6) + (func_800644B8(*(s16 *)(p + 0x4E)) >> *(s16 *)(p + 0x48));
    *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) + *(u16 *)(p + 0x58);
    *(s16 *)(c + 0x1A) = *(u16 *)(p + 0x4E) + 0x400;
    if (*(s16 *)(p + 0x4A) == 0)
    {
        c[0xC] += (p[0x0] - c[0xC]) / *(s16 *)(p + 0x4C);
        c[0xD] += (p[0x1] - c[0xD]) / *(s16 *)(p + 0x4C);
        c[0xE] += (p[0x2] - c[0xE]) / *(s16 *)(p + 0x4C);
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) <= 0)
        {
            *(s16 *)(p + 0x4C) = 8;
            *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
        }
    }
    else if (*(s16 *)(p + 0x4A) == 1)
    {
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) <= 0)
        {
            *(s16 *)(p + 0x4C) = 4;
            *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
        }
    }
    else
    {
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x4C);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x4C);
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x4C);
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* The shadow quad: compose the renderable's +0x16 rotation with the current
   view matrix in the 0x1F800000 scratchpad, project a 0x10 x -(p+0x50) strip
   with RotAverage4 into one POLY_G4, tint it from the renderable's +0xC
   colour at a quarter strength on the second corner, and -- when the
   renderable carries bit 2 -- follow it with a DR_MODE. 154 words,
   0 differing.

   THREE SPELLINGS ARE LOAD-BEARING, each measured:
   (1) THE COLOUR WORD GOES THROUGH A LOCAL, read BEFORE the two zeroing
   stores. Written inline after them the `lw` cannot be hoisted over them and
   the load-delay `nop` the target does not have costs a word (155).
   (2) `h = -*(u16 *)(p + 0x50);` IS ITS OWN STATEMENT, sitting between the
   first and second chained vertex assignments. Inline in the last assignment
   the `lhu` is scheduled after all ten stores (13 differing); at the top of
   the group it is 7; one statement later, 7 again.
   (3) THE ROTATION COPY IS `((s16 *)sp)[0x80..0x82]`, AN ARRAY REF, and the
   three colour bytes are stored 0xC, 0xD, 0xE. Through `*(s16 *)((u8 *)sp +
   0x100)` the three `r->unk_1x` loads are all hoisted above the stores (9
   differing), because a cast store and a MEM_IN_STRUCT_P load record no
   dependence in either direction.
   The twelve vertex stores are five chained assignments in descending-offset
   runs, the same rule func_800BA384 records for the same four SVECTORs.
   The scratchpad is func_800BA414's UnkDungeonSP3 -- this is its sibling, and
   the two share every offset except that this one writes a real rotation at
   +0x100 where func_800BA414 writes zero. */
void func_800BBDEC(u8 *p, UnkVec *v, UnkSlot *r, s16 d)
{
    UnkDungeonSP3 *sp;
    u8 *g;
    u8 *q;
    u32 t;
    s32 h;

    sp = (UnkDungeonSP3 *)0x1F800000;
    g = (u8 *)&D_80083160;
    sp->unk_20 = *(u8 **)&D_80083160 + 0xB0;
    sp->unk_D0.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_D0.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_D0.t[2] = *(s16 *)((u8 *)v + 0xA);
    q = *(u8 **)(*(u8 **)&D_80083160 + 0x8D0);
    PushMatrix();
    ((s16 *)sp)[0x80] = r->unk_16;
    ((s16 *)sp)[0x82] = r->unk_1A;
    ((s16 *)sp)[0x81] = r->unk_18;
    RotMatrix((SVECTOR *)&sp->unk_100, &sp->unk_D0);
    ReadRotMatrix(&sp->unk_11C);
    CompMatrix(&sp->unk_11C, &sp->unk_D0, &sp->unk_50);
    SetTransMatrix(&sp->unk_50);
    SetRotMatrix(&sp->unk_50);
    sp->unk_70[0].vz = sp->unk_70[1].vz = 0;
    h = -*(u16 *)(p + 0x50);
    sp->unk_70[1].vx = sp->unk_70[3].vx = 0x10;
    sp->unk_70[0].vx = sp->unk_70[2].vx = 0;
    sp->unk_70[0].vy = sp->unk_70[1].vy = 0;
    sp->unk_70[2].vy = sp->unk_70[3].vy = 0;
    sp->unk_70[2].vz = sp->unk_70[3].vz = h;
    sp->unk_C0 = RotAverage4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                             (s32 *)(q + 0x8), (s32 *)(q + 0x10), (s32 *)(q + 0x18),
                             (s32 *)(q + 0x20), &sp->unk_90, &sp->unk_94);
    if ((u32)sp->unk_C0 < 0x1E0)
    {
        t = *(u32 *)((u8 *)r + 0xC);
        ((u32 *)q)[5] = 0;
        ((u32 *)q)[7] = 0;
        ((u32 *)q)[1] = t;
        q[0xC] = q[0x4] >> 2;
        q[0xD] = q[0x5] >> 2;
        q[0xE] = q[0x6] >> 2;
        SetPolyG4(q);
        if ((r->unk_14 & 4) != 0)
        {
            q[0x7] |= 2;
        }
        AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
        q += 0x24;
        if ((r->unk_14 & 4) != 0)
        {
            func_80067F20((u32 *)q, 0, 0, 0x20, 0);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
            q += 0xC;
        }
    }
    PopMatrix();
    *(u32 *)(*(u8 **)g + 0x8D0) = (u32)q;
}

/* The node walk, seven instances, in three variants. This is func_80046398
   of csrc/unk_19434.c -- the engine's own walk -- recompiled once per
   visitor: `p - 8` is the node's own back-pointer read out of its payload,
   so the loop steps along a list whose links sit 8 bytes below each payload
   base.

   Variant A passes the slot's unk_06 as the visitor's fourth argument,
   exactly as the engine's does: func_800BC054, func_800C55A0, func_800C9034
   and func_800D0774. 21 words each, 0 differing. */
s32 func_800BC054(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800BBDEC(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* An eight-argument 0x202 spawner on the engine's own record. A POINTER
   LOCAL for `n + 0x20` is what gives the target's `addiu $a1, $s0, 0x20`;
   cse rewrites `*p` back to `0x20($s0)` and leaves the rest on the
   materialised base. 67 words, 0 differing. */
UnkNode *func_800BC0A8(s16 x, s16 y, s16 ang, s16 d, s16 e, s16 f, u32 g, s16 h)
{
    UnkNode *n;
    UnkVec *m;
    u8 *p;
    u8 *r;

    n = func_8003FD64(0x202, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BBBE0;
        func_8004491C(n, (u32)func_800BC054);
        m = n->unk_08;
        p = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)(p + 0x4) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)(p + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = ang;
        *(s16 *)(p + 0x8) = ang;
        r = (u8 *)n->unk_0C;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x1A) = d + 0x400;
        *(s16 *)(p + 0x4E) = d;
        *(s16 *)(p + 0x4C) = 4;
        *(s16 *)(p + 0x50) = e;
        *(u32 *)p = g;
        *(s16 *)(p + 0x58) = f;
        *(s16 *)(p + 0x48) = h;
    }
    return n;
}

/* Wrap the record's +0x02 counter into 0..0x1F by its +0x0C span and blit the
   two 0x10 x 0x20 cells. The rectangle is the 4-halfword UnkDungeonRect
   MoveImage takes; its first halfword reuses the 0x270 already in $a1
   for the call. 29 words, 0 differing. */
void func_800BC1B4(u8 *p)
{
    UnkDungeonRect r;

    *(u16 *)(p + 0x2) = (*(u16 *)(p + 0x2) - *(u16 *)(p + 0xC)) & 0x1F;
    r.v[0] = 0x270;
    r.v[1] = *(u16 *)(p + 0x2) + 0x140;
    r.v[2] = 0x10;
    r.v[3] = 0x20;
    MoveImage(r.v, 0x270, 0x100);
    MoveImage(r.v, 0x270, 0x120);
}

/* 17 words, 0 differing. */
void func_800BC228(s16 arg0)
{
    UnkNode *n;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BC1B4;
        *(s16 *)((u8 *)n + 0x2C) = arg0;
    }
}

/* The complement of func_800A7700 above: here the `s16` second parameter IS
   required. It is both the ternary's test and the +0x0E store, and that pair
   is where the `addu $s3, $s1, $zero` in the first branch's delay slot comes
   from -- the HImode pseudo is a second quantity, not a redundant copy.
   43 words, 0 differing. */
UnkNode *func_800BC26C(void *list, s16 flag, s32 val, s32 d)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x110, list);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BC388;
        func_8004491C(n, (u32)func_800BC3E4);
        p = (u8 *)n + 0x20;
        *(void **)p = list;
        *(s32 *)(p + 4) = val;
        *(s16 *)(p + 0xC) = flag ? 0x7F80 : 0x7FC0;
        *(s16 *)(p + 0xE) = flag;
    }
    return n;
}

/* Walk the engine's node list for the func_800BC388 node whose payload word
   is `key` and mark it for redraw. 28 words, 0 differing. */
void func_800BC318(u8 *key)
{
    UnkNode *n;

    for (n = D_80081498; n != NULL; n = n->unk_00)
    {
        if (n->unk_10 == (u32)func_800BC388 && *(u8 **)((u8 *)n + 0x20) == key)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
    }
}

/* Raise 0x8000 on the node header's +0x1E and on D_800814A0 when either the
   referenced record's +0x1E already carries it or the second pointer's first
   word is zero. The two tests are one `||`: the first `bnez` jumps TO the
   body and the second jumps PAST it. Note this target WANTS the folded
   `-0x2($a0)` that func_8009A078 above had to avoid. 23 words,
   0 differing. */
void func_800BC388(u8 *p)
{
    if ((*(u16 *)(*(u8 **)p + 0x1E) & 0x8000) || *(s32 *)*(u8 **)(p + 0x4) == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* ONE POINTER LOCAL FOR THE SCRATCHPAD, not three constant addresses:
   `*(s16 *)0x1F800100 = ...` three times gives three `lui $at, 0x1f80` and
   the function is 62 words / 57 differing, where 0x1F800000 -- a constant
   with a ZERO LOW HALF -- is a one-word base the three stores share as
   displacements. splat renders these as raw immediates rather than %lo
   symbols, so there is no relocation here. 60 words, 0 differing. */
s32 func_800BC3E4(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *s = (u8 *)0x1F800000;
    UnkNode *n;
    UnkNode *q;
    UnkVec *m;
    u8 *r;
    s32 t;

    *(s16 *)(s + 0x100) = -g->unk_C4;
    *(s16 *)(s + 0x102) = -g->unk_C6;
    *(s16 *)(s + 0x104) = g->unk_B8;
    while (1)
    {
        q = *(UnkNode **)p;
        r = (u8 *)q->unk_0C;
        if ((*(u16 *)(r + 0x14) & 0x8000) == 0)
        {
            m = q->unk_08;
            t = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                              *(s16 *)((u8 *)m + 0xA));
            func_800BC4D4(m, r, (s16)t, 0);
            if (func_80045310(*(u32 *)(g->unk_00 + 0x8D0)) != 0)
            {
                break;
            }
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

void func_800BC4D4(UnkVec *v, UnkSlot *s, s16 arg2, s32 arg3)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSP9 *p = (UnkDungeonSP9 *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    u8 *q;
    u8 *e;
    s32 w;
    s32 h;

    q = *(u8 **)(base + 0x8D0);
    p->unk_20 = base + 0xB0;
    p->unk_EC = 0;
    p->unk_E8 = 0;
    p->unk_E4 = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    e = (u8 *)s->unk_08;
    p->unk_24 = s->unk_14;
    p->unk_00 = ((u16 *)v)[1];
    p->unk_02 = ((u16 *)v)[3];
    p->unk_04 = arg2;
    p->unk_C0 = RotTransPers((void *)0x1F800000, (void *)0x1F8000B8, (void *)0x1F800090,
                             (void *)0x1F800094);
    w = p->unk_C0 - 0x7;
    D_8006CD30.unk_1C = p->unk_C0 * 4;
    p->unk_C0 = w - arg3;
    if ((u32)p->unk_C0 < 0x1E0)
    {
        PushMatrix();
        p->unk_B8 = p->unk_B8 - 0xA0;
        p->unk_BA = p->unk_BA - 0x78;
        h = (((s16 *)v)[5] - (s16)p->unk_04) * 0x20 + 0x1000;
        p->unk_38.w = h;
        if (h < 0)
        {
            p->unk_38.w = 0;
        }
        p->unk_30.w = p->unk_38.w;
        p->unk_34.w = p->unk_38.w / 2;
        RotMatrix((void *)0x1F800100, (void *)0x1F8000D0);
        CompMatrix(&D_8006CD30, (void *)0x1F8000D0, (void *)0x1F800050);
        ScaleMatrix((void *)0x1F800050, (void *)0x1F800030);
        p->unk_30.w = s->unk_1C;
        p->unk_34.w = s->unk_1E;
        p->unk_38.w = 0x1000;
        ScaleMatrix((void *)0x1F800050, (void *)0x1F800030);
        SetTransMatrix((void *)0x1F800050);
        SetRotMatrix((void *)0x1F800050);
        do
        {
            if ((e[0x0] & 0x20) == 0)
            {
                s16 a1;
                s16 a2;
                s16 b1;
                s16 b2;

                p->unk_08.w = e[0x8];
                p->unk_0C.w = e[0x9];
                p->unk_10.w = e[0xA];
                p->unk_14.w = e[0xB];
                func_80067EF4(q, 0, 0);
                AddPrim(p->unk_20 + p->unk_C0 * 4, q);
                q += 0xC;
                if ((e[0x0] ^ p->unk_24) & 1)
                {
                    a1 = -(s8)e[0x2];
                    p->unk_80 = a1;
                    p->unk_70 = a1;
                    a1 = a1 - p->unk_10.h;
                    p->unk_88 = a1;
                    p->unk_78 = a1;
                }
                else
                {
                    a2 = (s8)e[0x2];
                    p->unk_80 = a2;
                    p->unk_70 = a2;
                    a2 = a2 + p->unk_10.h;
                    p->unk_88 = a2;
                    p->unk_78 = a2;
                }
                if ((e[0x0] ^ p->unk_24) & 2)
                {
                    b1 = -(s8)e[0x3];
                    p->unk_7A = b1;
                    p->unk_72 = b1;
                    b1 = b1 - p->unk_14.h;
                    p->unk_8A = b1;
                    p->unk_82 = b1;
                }
                else
                {
                    b2 = (s8)e[0x3];
                    p->unk_7A = b2;
                    p->unk_72 = b2;
                    b2 = b2 + p->unk_14.h;
                    p->unk_8A = b2;
                    p->unk_82 = b2;
                }
                p->unk_70 = p->unk_70 + 6;
                p->unk_78 = p->unk_78 + 6;
                RotTransPers4(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88, &p->unk_F0,
                              &p->unk_F4, &p->unk_F8, &p->unk_FC, &p->unk_90, &p->unk_94);
                *(s16 *)(q + 0x8) = p->unk_F0 + p->unk_B8;
                *(s16 *)(q + 0xA) = p->unk_F2 + p->unk_BA;
                *(s16 *)(q + 0x10) = p->unk_F4 + p->unk_B8;
                *(s16 *)(q + 0x12) = p->unk_F6 + p->unk_BA;
                *(s16 *)(q + 0x18) = p->unk_F8 + p->unk_B8;
                *(s16 *)(q + 0x1A) = p->unk_FA + p->unk_BA;
                *(s16 *)(q + 0x20) = p->unk_FC + p->unk_B8;
                *(s16 *)(q + 0x22) = p->unk_FE + p->unk_BA;
                q[0x3] = 9;
                p->unk_10.w = p->unk_10.w + p->unk_08.w;
                p->unk_14.w = (p->unk_0C.w + p->unk_14.w) << 8;
                p->unk_0C.w = p->unk_0C.w << 8;
                *(u32 *)(q + 0xC) = p->unk_0C.w + p->unk_08.w +
                                    ((*(u16 *)(e + 0x4) & 0x80) ? 0x7F800000 : 0x7FC00000);
                *(u32 *)(q + 0x14) = p->unk_0C.w + p->unk_10.w + (*(s16 *)(e + 0x4) << 16);
                *(s16 *)(q + 0x1C) = p->unk_14.h + p->unk_08.h;
                *(s16 *)(q + 0x24) = p->unk_14.h + p->unk_10.h;
                if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x20))
                {
                    q[0x14]--;
                    q[0x24]--;
                }
                if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x22))
                {
                    q[0x1D]--;
                    q[0x25]--;
                }
                *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
                q[0x7] = 0x2E;
                AddPrim(p->unk_20 + p->unk_C0 * 4, q);
                q += 0x28;
                func_80067EF4(q, 1, 0);
                AddPrim(p->unk_20 + p->unk_C0 * 4, q);
                q += 0xC;
            }
            if (*(s8 *)e < 0)
            {
                break;
            }
            e += 0xC;
        } while (1);
        PopMatrix();
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* Look a tile up in the 6-byte map rows at D_80083160 + 0x1DC and return the
   negated height at +0x2, or 0x400 where the row's first halfword is 0.

   THREE THINGS, ALL MEASURED. (1) The row address is INTEGER arithmetic: read
   the base as `*(u32 *)` and write the index first and the `addu` comes out
   `addu $v1, $v0, $v1`; through `*(u8 **)` it is base-first and wrong by a
   word. (2) The ZERO arm has to be written first -- with the negate arm first
   the 0x400 block is laid out out of line and the function is 28 words.
   (3) The narrowing belongs in the negate arm on an s32 result, not on an s16
   result variable: an s16 `r` shares one `sll`/`sra` between both arms, which
   is 7 differing, and the target narrows only the negation while 0x400 goes
   straight to the epilogue in the `beqz`'s delay slot.
   26 words, 0 differing. */
s16 func_800BCA68(u16 x, u16 y)
{
    u8 *g;
    u16 i;
    u16 *e;
    s32 r;

    g = (u8 *)&D_80083160;
    i = (y >> 6) << *(s16 *)(g + 0x1F0);
    e = (u16 *)((i + (x >> 6)) * 6 + *(u32 *)(g + 0x1DC));
    if (*e == 0)
    {
        r = 0x400;
    }
    else
    {
        r = (s16)(0 - e[1]);
    }
    return r;
}

/* The record's (x, z) and facing into func_800BCB04; the result is
   re-extended, so the return is s16. 13 words, 0 differing. */
s16 func_800BCAD0(u8 *p)
{
    return (s16)func_800BCB04(*(u16 *)(p + 0x2), *(u16 *)(p + 0x6), *(s16 *)(p + 0xA));
}

/* The dungeon's height cast: clamp (x, z) to the 0x40-square cell grid, read
   the 6-byte row at D_8008333C[0], and walk that cell's 0x18-byte polygon
   records keeping the lowest plane height above the caller's floor. This is
   csrc/ovl_town.c's func_8008D0B8 with the cell sweep removed -- the record
   layout, the winding (unk_00, unk_02, unk_06, unk_04), the +0x16/+0x17
   advance and the `(s16)SP->p140` re-read all come from there -- and the
   scratchpad prefix at +0x140..+0x152 sits under UnkDungeonCastSP's
   pad_00[0x154], whose unk_154..unk_162 this fills for func_800BCE7C.

   THE RECORD ADVANCE IS `n * 0x18 + 0x18`, NOT `(n + 1) * 0x18`: the target
   multiplies the nibble by 24 and then adds 24 (`sll`/`addu`/`sll`/`addiu`),
   where the +1-first form increments the nibble and multiplies
   (`addiu`/`sll`/`addu`/`sll`) -- same word count, 4 differing, and the only
   4 this function ever had.

   Everything else follows the two spellings func_800BCA68 above already
   measured: the bases are INTEGER (`*(u32 *)`), so every `addu` is
   index-first, and the row address is written out at each of its three uses
   because the `sh` to +0x152 invalidates cse's load of +0x150 through the
   same `u8 *`. 222 words, 0 differing. */
s32 func_800BCB04(s32 x, s32 z, s16 c)
{
    u8 *sp = (u8 *)0x1F800000;
    u8 *m = (u8 *)D_8008333C;
    u32 verts;
    u32 cells;
    u32 norms;
    u8 *p;
    SVECTOR *n;
    SVECTOR *v;
    s32 ix;
    s32 iy;
    u16 px;
    u16 py;

    *(u16 *)(sp + 0x140) = x & 0x3F;
    *(u16 *)(sp + 0x142) = z & 0x3F;
    *(s32 *)(sp + 0x14C) = c;
    ix = (u16)x >> 6;
    *(u16 *)(sp + 0x150) = ix;
    verts = *(u32 *)(m + 0x8);
    cells = D_8008333C[0];
    norms = *(u32 *)(m + 0xC);
    if (ix < 0)
    {
        *(u16 *)(sp + 0x150) = 0;
    }
    else if (ix >= 0x40)
    {
        *(u16 *)(sp + 0x150) = 0x3F;
    }
    iy = (u16)z >> 6;
    *(s32 *)(sp + 0x148) = iy;
    if (iy < 0)
    {
        *(s32 *)(sp + 0x148) = 0;
    }
    else if (iy >= 0x40)
    {
        *(s32 *)(sp + 0x148) = 0x3F;
    }
    *(u16 *)(sp + 0x150) =
        *(u16 *)(sp + 0x150) + (*(s32 *)(sp + 0x148) << *(s16 *)(m + 0x14));
    *(s32 *)(sp + 0x148) = 0x400;
    if (*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells) == 0)
    {
        return (s16) * (u16 *)(sp + 0x148);
    }
    *(u16 *)(sp + 0x152) = -*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells + 2);
    p = *(u8 **)(*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells) * 4 +
                 *(u32 *)(m + 0x4));
    while (*(s16 *)(*(u16 *)(p + 0x10) * 8 + norms + 4) < -0x7FF)
    {
        if (!(*(u8 *)(p + 0x17) & 1))
        {
            px = *(u16 *)(sp + 0x140);
            py = *(u16 *)(sp + 0x142);
            *(s32 *)(sp + 0x154) = *(s32 *)(*(u16 *)(p + 0x0) * 8 + verts);
            *(u16 *)(sp + 0x154) = *(u16 *)(sp + 0x154) - px;
            *(u16 *)(sp + 0x156) = *(u16 *)(sp + 0x156) - py;
            *(s32 *)(sp + 0x158) = *(s32 *)(*(u16 *)(p + 0x2) * 8 + verts);
            *(u16 *)(sp + 0x158) = *(u16 *)(sp + 0x158) - px;
            *(u16 *)(sp + 0x15A) = *(u16 *)(sp + 0x15A) - py;
            *(s32 *)(sp + 0x15C) = *(s32 *)(*(u16 *)(p + 0x6) * 8 + verts);
            *(u16 *)(sp + 0x15C) = *(u16 *)(sp + 0x15C) - px;
            *(u16 *)(sp + 0x15E) = *(u16 *)(sp + 0x15E) - py;
            *(s32 *)(sp + 0x160) = *(s32 *)(*(u16 *)(p + 0x4) * 8 + verts);
            *(u16 *)(sp + 0x160) = *(u16 *)(sp + 0x160) - px;
            *(u16 *)(sp + 0x162) = *(u16 *)(sp + 0x162) - py;
            if (func_800BCE7C((UnkDungeonCastSP *)sp) != 0)
            {
                n = (SVECTOR *)(*(u16 *)(p + 0x10) * 8 + norms);
                v = (SVECTOR *)(*(u16 *)(p + 0x0) * 8 + verts);
                *(s32 *)(sp + 0x144) =
                    (n->vx * (v->vx - (s16) * (u16 *)(sp + 0x140)) +
                     n->vy * (v->vy - (s16) * (u16 *)(sp + 0x142)) + n->vz * v->vz) /
                    n->vz;
                *(s32 *)(sp + 0x144) =
                    *(s32 *)(sp + 0x144) + (s16) * (u16 *)(sp + 0x152);
                if (*(s32 *)(sp + 0x144) >= *(s32 *)(sp + 0x14C))
                {
                    if (*(s32 *)(sp + 0x144) < *(s32 *)(sp + 0x148))
                    {
                        *(s32 *)(sp + 0x148) = *(s32 *)(sp + 0x144);
                    }
                }
            }
        }
        if ((*(u8 *)(p + 0x16) & 0xF) == 1)
        {
            if (*(s8 *)(p + 0x17) < 0)
            {
                break;
            }
            if ((*(u8 *)(p + 0x16) & 0xF0) != 0)
            {
                p = p + ((*(u8 *)(p + 0x16) >> 4) * 0x18 + 0x18);
                continue;
            }
        }
        p = p + 0x18;
    }
    return (s16) * (u16 *)(sp + 0x148);
}

/* The angular-span test on the cast scratchpad: four corner (x, z) deltas
   through ratan2, three gaps between consecutive angles, and a pass only if
   every gap is under 0x801. This is csrc/ovl_town.c's func_8008CE08 with the
   record 0xE8 longer -- see UnkDungeonCastSP above -- and the shape notes
   there apply unchanged: a corner's two halfwords are tested as ONE WORD,
   unk_170 and unk_16C are stored unmasked while unk_168 is masked, and the
   result is the last comparison rather than a branch. 80 words,
   0 differing. */
s32 func_800BCE7C(UnkDungeonCastSP *p)
{
    if (*(s32 *)&p->unk_154 == 0)
    {
        return 1;
    }
    if (*(s32 *)&p->unk_160 == 0)
    {
        return 1;
    }
    p->unk_164 = func_80065F90(p->unk_156, p->unk_154);
    p->unk_170 = func_80065F90(p->unk_162, p->unk_160);
    p->unk_170 = p->unk_170 - p->unk_164;
    p->unk_180 = (0x1000 - p->unk_170) & 0xFFF;
    if (p->unk_180 >= 0x801)
    {
        return 0;
    }
    if (*(s32 *)&p->unk_15C == 0)
    {
        return 1;
    }
    p->unk_16C = func_80065F90(p->unk_15E, p->unk_15C);
    p->unk_16C = p->unk_16C - p->unk_164;
    p->unk_17C = (p->unk_170 - p->unk_16C) & 0xFFF;
    if (p->unk_17C >= 0x801)
    {
        return 0;
    }
    if (*(s32 *)&p->unk_158 == 0)
    {
        return 1;
    }
    p->unk_168 = func_80065F90(p->unk_15A, p->unk_158);
    p->unk_168 = (p->unk_168 - p->unk_164) & 0xFFF;
    if (p->unk_168 >= 0x801)
    {
        return 0;
    }
    p->unk_178 = (p->unk_16C - p->unk_168) & 0xFFF;
    return p->unk_178 < 0x801;
}

/* Widened from `(void)`: func_80091A38 calls it with the high byte of its
   handler word minus one. An unread parameter costs it nothing. */
void func_800BCFBC(s32 a)
{
}

/* THE TWO WORDS FOUR BYTES APART ARE NOT A SPLIT ADDRESS. $s0 holds the
   CONSTANT 0x80010000, which mips_move_1word materialises with `lui` alone
   because its low half is zero, and 0x2090/0x208C ride in the two loads as
   ordinary displacements -- splat renders those as `%lo(SYM)`, which is what
   made this look like a `%hi` kept in a register. See
   docs/matching-levers.md, "A CONSTANT ADDRESS WITH A ZERO LOW HALF IS A
   ONE-WORD BASE".

   So the question this was parked on -- can this cc1psx emit
   `(lo_sum (high X) X+k)`? -- is answered: it does in `.rtl`, and it never
   survives. cse folds `(lo_sum (high X) X)` back to `X` and then prefers
   reg+displacement over a constant address on `mips_address_cost`, which is
   the `addiu $s0, $v0, %lo` word that every symbol spelling produced.

   `b` is ASSIGNED INSIDE the first arm. As an initialiser on the
   declaration it is 44 words and 7 differing. 44 words, 0 differing. */
void func_800BCFC4(void)
{
    u8 *b;

    if (D_80082E6B == 0x11)
    {
        b = (u8 *)0x80010000;
        if (*(s32 *)(b + 0x2090) == 0)
        {
            if (D_8008146C == 0x28)
            {
                if (func_80033BC0(0xA2) == 0)
                {
                    if (*(s32 *)(b + 0x208C) == 0)
                    {
                        *(s32 *)(b + 0x208C) = 1;
                        func_80040AA0(0x21);
                        D_800DCF4E = 0;
                        return;
                    }
                }
            }
        }
    }
    func_8003E4FC(0x6, D_800DF3CC, 0);
    func_8003F320();
}

/* func_800BCFC4 with D_800DF3D4. 44 words, 0 differing. */
void func_800BD074(void)
{
    u8 *b;

    if (D_80082E6B == 0x11)
    {
        b = (u8 *)0x80010000;
        if (*(s32 *)(b + 0x2090) == 0)
        {
            if (D_8008146C == 0x28)
            {
                if (func_80033BC0(0xA2) == 0)
                {
                    if (*(s32 *)(b + 0x208C) == 0)
                    {
                        *(s32 *)(b + 0x208C) = 1;
                        func_80040AA0(0x21);
                        D_800DCF4E = 0;
                        return;
                    }
                }
            }
        }
    }
    func_8003E4FC(0x6, D_800DF3D4, 0);
    func_8003F320();
}

/* 13 words, 0 differing. */
void func_800BD124(void)
{
    func_8003E4FC(0x6, D_800DF3DC, 0);
    D_800DCF4D = -1;
}

/* 11 words, 0 differing. */
void func_800BD158(void)
{
    func_8003E4FC(6, D_800DF3E4, 0);
}

/* 16 words, 0 differing. */
void func_800BD184(s16 i)
{
    func_8003E4FC(6, &D_800DF3EC[i * 8], 0);
    func_8003F320();
}

/* 166 words, 0 differing. */
s32 func_800BD1C4(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s32 n;
    s32 t;
    s16 d;
    u8 *g;

    d = 0;
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x20E020, p);
    n = (func_800A6D30() & 7) + 0x20;
    if (n >= 0x100)
    {
        n = 0xFF;
    }
    *(s16 *)(p + 0x64) = n;
    if (p[0x28] == p[0x29])
    {
        if (q[0] == 0xF)
        {
            if (p[5] < 0xFF)
            {
                p[5] = p[5] + 1;
                func_80041E70(p);
            }
            *(s16 *)(p + 0x64) = 1;
            d = 1;
        }
        else
        {
            d = -1;
        }
    }
    else
    {
        if (p[0x29] - p[0x28] < n)
        {
            n = p[0x29] - p[0x28];
        }
        *(s16 *)(p + 0x64) = n;
    }
    if (func_800AD568(p) != 0)
    {
        if (d > 0)
        {
            t = 0x8010;
        }
        else
        {
            t = 0x10;
        }
        func_800B4C7C(t, p, *(s16 *)(p + 0x64), 1);
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        m = func_800990FC();
        if (d > 0)
        {
            h = func_80099734(p, m);
            h = func_80099194(D_800E0E14, h);
            h = func_8003AD08(*(s16 *)(p + 0x64), h);
            h = func_80099194(D_80089354, h);
        }
        else if (d == 0)
        {
            h = func_80099734(p, m);
            h = func_80099194(D_800E0E34, h);
            h = func_8003AD08(*(s16 *)(p + 0x64), h);
            h = func_80099194(D_80089354, h);
        }
        else
        {
            h = func_80099194(D_800E0E50, m);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The kill skeleton plus a spoken report, and the one that was parked for a
   day over two words.

   THE CALL VALUE GOES INTO THE NAME THAT IS WRITTEN AGAIN, and the
   long-lived handle is a copy taken off it:

       h = func_800990FC();
       m = h;
       h = func_80099194(MSG, h);
       ...
       func_800A5720(m);

   `make_regs_eqv` (cse.c:836-890) puts the two in one quantity with `h`
   canonical -- which is what the target wants, because the argument insn
   then keeps `h`, the SHORT pseudo, and local-alloc can give it $a1, making
   the argument move a no-op. The only damage cse does is rewriting `m`'s
   later use back to `h`, and reassigning `h` immediately after the copy
   calls `delete_reg_equiv` on it, so `m` becomes the class head and
   `func_800A5720(m)` is left alone. A third variable for func_80099194's
   result, or the copy written the other way round (`m = f(); h = m;`), is
   90 words and 2 differing -- which is what twelve spellings in the park
   file measured.

   This is a 19-INSTANCE IDIOM in this overlay. `addu $aN, $vM, $zero` …
   `jal` … `addu $sK, $aN, $zero` matches nineteen functions here and
   nowhere else in the project. 90 words, 0 differing. */
s32 func_800BD45C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x802080, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (!(*(u32 *)(p + 0x1C) & 0x40))
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_800E0E69, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    func_80042B68((s8 *)p, 0x5);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800BD45C with a different tint, mask, string and sound.
   90 words, 0 differing. */
s32 func_800BD5C4(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0xC02020, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (!(*(u32 *)(p + 0x1C) & 0x400))
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_800E0E82, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    func_80042B68((s8 *)p, 0x2);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* THE KILL HANDLERS: one skeleton, five landed copies and two parked. Every
   member opens the same way -- if the record IS the engine's current one
   (D_800E3D7C), park the caller's node at +0x110, reset the actor through
   func_8008D330 and report 0; otherwise, for a record below 0xA0000000, run
   func_800A63B8 and consult func_800AD6FC with a two-bit field pulled out of
   D_800DDE84[record[0x13]], and hand the whole thing to func_800A5F38 if
   that says no. What follows the gate is the per-entity part, and it is all
   that differs between the seven. They close alike: func_80098B38 on the
   node and the engine's +0x0A population counter down by one.

   The pointer gate is `(u32)p <= 0x9FFFFFFF`, which is the `sltu` against
   the constant read the way the listing has it -- the comparison is
   unsigned and the constant is the last KSEG0 address, so this is "is the
   record in cached RAM".

   func_8008D330 IS CALLED WITH FOUR ARGUMENTS HERE, which is the witness
   for the signature it now carries; see its own comment above.

   func_800BD45C and func_800BD5C4 are the two that did not land: they are
   this skeleton plus the message chain, and they sit at 90 of 90 words with
   2 differing. docs/near-misses/func_800BD45C.c has the analysis and the
   twelve spellings that were measured.

   80 words, 0 differing. */
s32 func_800BD72C(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0xC0C0C0, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (!(*(u32 *)(p + 0x1C) & 0x200))
        {
            func_800997FC(D_800E0E9B);
        }
    }
    func_80042B68((s8 *)p, 0x1);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The first kill skeleton with the message chain unconditional inside the
   0x4000 gate -- no inner +0x1C test as func_800BD45C has -- and a long tail.
   118 words, 0 differing. */
s32 func_800BD86C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s32 i;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x20C0C0, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        h = func_800990FC();
        m = h;
        h = func_80099194(D_800E0EB4, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800E0EC9, h);
        func_80099290(h);
        func_800A5720(m);
    }
    if (*(u32 *)(p + 0x1C) & 0x4000000)
    {
        p[0x28] = (p[0x29] >> 1) + 1;
    }
    p[0x26] = p[0x68];
    p[0x27] = p[0x69];
    for (i = 1; i < 0x21; i++)
    {
        func_80042B68((s8 *)p, i);
    }
    *(u32 *)(p + 0x1C) &= 0xFBFFE10F;
    func_80041E70(p);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same skeleton with a 0x2020C0 tint, the record's +0x68 copied into
   +0x26 and one message. 76 words, 0 differing. */
s32 func_800BDA44(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x2020C0, p);
    p[0x26] = p[0x68];
    func_80041E70(p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E0EE1);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same skeleton with a 0x40C040 tint, the record's +0x69 copied into
   +0x27 and no func_80041E70. 73 words, 0 differing. */
s32 func_800BDB74(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x40C040, p);
    p[0x27] = p[0x69];
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E0F07);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The first kill skeleton with a THREE-WAY flag and the D_800DDE84 field
   taken as bits 8-9 (`srl 8` / `andi 3`), the only member that reads that
   half; the func_800AD6FC gate is tested as a HALFWORD here (`sll 16` with
   no `sra`), which is the (s16) cast func_800C4490 also needs.

   `s16 f` CARRIES -1, 0 AND 1, and the three message arms are spelled
   `if (f < 0) ... else if (f != 0) ... else ...`. Written as the reading
   order `else if (f == 0) {0F65} else {0F44}` the second test comes out
   `bnez` where the target has `beqz` -- 1 differing at the right length,
   and invisible everywhere else because both strings are relocations.
   136 words, 0 differing. */
s32 func_800BDC98(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s16 f;
    u8 *g;

    f = 0;
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (p[0x25] == p[0x66])
        {
            f = 1;
        }
        if ((s16)func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 8) & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        if (f != 0)
        {
            if (p[0x4] < 0xFF)
            {
                p[0x4]++;
                func_80041E70(p);
            }
        }
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            if (f == 0)
            {
                if (p[0x25] != p[0x66])
                {
                    f = -1;
                }
            }
            h = func_800990FC();
            m = h;
            if (f < 0)
            {
                h = func_80099734(p, h);
                h = func_80099194(D_800E0F2D, h);
            }
            else if (f != 0)
            {
                h = func_80099734(p, h);
                h = func_80099194(D_800E0F44, h);
            }
            else
            {
                h = func_80099734(p, h);
                h = func_80099194(D_800E0F65, h);
            }
            func_80099290(h);
            func_800A5720(m);
        }
        func_800D5460((UnkNode *)(p - 0x20), 0x20A0A0, 0x702);
        func_80098B38(q);
    }
    else
    {
        func_800997FC(D_800E0F85);
        func_800A56E0(0x506);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* THE KILL HANDLER WITH A FLAG, and four spellings had to be right at once.

   - `t = f` IS A NARROW COPY (u8 off a u16). Both tests read `t`, not `f`,
     and cse only leaves the copy alone because the modes differ; same-width
     and the two fuse into one pseudo and the three `addu $v0, $s3, $zero` the
     listing has disappear. `u16 f` rather than `s32` is what keeps the first
     gate's `return 0` as `addu $v0, $zero, $zero`.
   - THE TWO `goto`s ARE THE BRANCH TARGETS. Written as the plain
     `if (t == 0)` / `if (t != 0)` pair the first branch's target is the
     second test's own copy, reorg steals it into the delay slot and the
     function is a word long; jump threading cannot save it because
     `thread_jumps` refuses a target whose compared register is written
     between the label and the branch (jump.c's `modified_regs`).
   - `f = 1` IS WRITTEN TWICE in the 0x1E arm. The `||` spelling is one
     assignment and leaves `f` at five references against `old`'s two, which
     hands `old` the lower register. The if/else-if pair is a sixth reference
     that costs nothing: flow counts both insns and jump2's cross-jumping,
     which runs after it, puts them back into one `addiu $s3, $zero, 0x1`.

   154 words, 0 differing. */
s32 func_800BDEB8(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    void *m2;
    void *h2;
    u16 f;
    u8 t;
    u8 old;
    u8 cur;
    u8 *g;

    f = 0;
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x13] < 0x2E)
    {
        if (p[0x13] == 0x1E)
        {
            if (*(s32 *)(p + 0xA4) != 0)
            {
                f = 1;
            }
            else if (*(s16 *)(p + 0xAA) != 0)
            {
                f = 1;
            }
        }
        t = f;
        if (t != 0)
        {
            goto msg;
        }
        old = p[0x13];
        if (*(u32 *)(p + 0x1C) & 0x80000)
        {
            *(s16 *)(*(u8 **)(p - 0x14) + 6) = 0;
        }
        if ((s16)func_800A48F0((s8 *)p, 0xA, 0x20) >= 0)
        {
            cur = p[0x13];
            m = func_800990FC();
            p[0x13] = old;
            h = func_80099734(p, m);
            p[0x13] = cur;
            h = func_80099194(D_800E0FA4, h);
        }
        else
        {
            m = func_800990FC();
            h = func_80099194(D_800E0FB9, m);
        }
        func_80099290(h);
        func_800A5720(m);
        func_800D5460((UnkNode *)(p - 0x20), 0x20A0A0, 0x613);
    }
    else
    {
        f = 1;
    }
    t = f;
    if (t == 0)
    {
        goto tail;
    }
msg:
    h2 = func_800990FC();
    m2 = h2;
    h2 = func_80099194(D_800E0FD7, h2);
    h2 = func_80099734(p, h2);
    h2 = func_80099194(D_80089358, h2);
    func_80099290(h2);
    func_800A5720(m2);
tail:
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same `u8 t = f` flag as func_800BDEB8, but with ONE test, so the
   `goto`s are not needed: the copy at the head of the test block is
   duplicated into the four gates' delay slots by reorg. 144 words,
   0 differing. */
s32 func_800BE120(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u16 f;
    u8 t;
    u8 *g;

    f = 0;
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (p[0x13] >= 2 && !(*(u32 *)(p + 0x54) & 0x800000) && p[0x49] == 0)
        {
            if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
            {
                func_800A5F38(p, q);
                return 1;
            }
            *(s32 *)(p + 0x48) = *(s32 *)q;
            p[0x48] = q[0];
            p[0x49] = q[1];
            p[0x4A] = q[2];
            p[0x4B] = q[3];
            *(u32 *)(p + 0x54) |= 0x800000;
            func_800D5460((UnkNode *)(p - 0x20), 0x20A0A0, 0x702);
            f = 1;
            if (*(u32 *)(p + 0x14) & 0x4000)
            {
                func_80099844(p, D_800E0FF4);
            }
        }
    }
    t = f;
    if (t == 0)
    {
        if (p[0x13] < 2 || (*(u32 *)(p + 0x54) & 0x800000) || p[0x49] != 0)
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E100F, h);
            h = func_80099368(q, h);
            h = func_80099194(D_8008935C, h);
            func_80099290(h);
            func_800A5720(m);
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

s32 func_800BE360(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    s32 f;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if ((*(s16 *)(D_80073414[q[1]].unk_0C + q[0] * 0x14) & 0x8000) == 0)
        {
            f = D_800DDE84[p[0x13]] & 3;
        }
        else
        {
            f = (D_800DDE84[p[0x13]] >> 4) & 3;
        }
        if (func_800AD6FC(p, f, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_80098B38(q);
    }
    else
    {
        func_800997FC(D_800E101C);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The kill skeleton's short member: no D_800E3D7C arm, func_800A6480 for the
   report and the 0x2-bit field of D_800DDE84's entry. THE THIRD PARAMETER IS
   NOT READ AND HAS NO WITNESS -- there is no `jal` to this address and no
   address-take anywhere under asm/nonmatchings/ovl_dungeon, so whatever
   dispatches it lives in the overlay's unsplit data. `s16 k` is carried over
   from the eleven landed members of the same family; the bytes are identical
   without it. 48 words, 0 differing. */
s32 func_800BE4E8(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 2) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_80098B38(q);
    }
    else
    {
        func_800997FC(D_800E1035);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* A kill handler with an OUT-OF-RANGE arm: where the other six fall through
   the pointer gate into shared code, these two branch to func_800997FC with
   a string of their own, and the `lui $a0, %hi(...)` for it sits in the
   gate's delay slot. The in-range arm applies an effect through
   func_800A48F0, announces it, and tints the node through func_800D5460.
   82 words, 0 differing. */
s32 func_800BE5A8(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        if ((s16)func_800A48F0((s8 *)p, 0x20, 0x10) != 0)
        {
            if (*(u32 *)(p + 0x14) & 0x4000)
            {
                func_80099844(p, D_800E104E);
            }
        }
        func_800D5460((UnkNode *)(p - 0x20), 0xE0E0E0, 0x702);
    }
    else
    {
        func_800997FC(D_800E107C);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The kill skeleton plus the chest/item teardown. Three spelling facts, each
   measured here:

   - THREE SEPARATE POINTER LOCALS, not one reused `b'. `*(u8 **)(p - 0x18)',
     `*(u8 **)(p - 0x14)' and the 0x80010000 constant base are three different
     values; sharing one local is 19 differing -- it ties all three to one
     pseudo ($a3), and it also lets cse common the 0x80010000 high into the
     `ori' for 0x80010980, costing a `move' and the target's second `lui'.
     Three locals is 2 differing.
   - D_80083460 IS TAKEN TWICE, SO IT IS TWO LOCALS (`g', `g2'). One local
     reassigned puts the `%hi' in a different register from the `%lo' at the
     tail site -- `lui $v0' / `addiu $a0, $v0' against the target's
     `lui $a0' / `addiu $a0, $a0'. 2 differing -> 0. This is
     docs/matching-levers.md's "THE NUMBER OF SOURCE LOCALS HOLDING ONE
     GLOBAL'S ADDRESS DECIDES WHETHER ITS %hi AND %lo SHARE A REGISTER".
   - THE TWO 0x80010000 TABLES KEEP THE TWO SPELLINGS csrc/ovl_town.c's
     func_8009EF88 records: a pointer LOCAL whose low half is zero for the
     0x54-stride byte (one `lui', the whole offset in the displacement), and
     the WHOLE constant for the word table, because `((s32 *)0x80010980)[r]'
     makes the address plus(ashift, CONST_INT) and force_operand materialises
     it entire. Here they are in ONE block rather than town's two arms, and
     they still do not common once the base local is block-scoped.

   The `p[0x13]' range gate is written twice on purpose -- the message arm and
   the teardown arm each test it -- and the target reads p[0x13] a third time
   for the odd-state chain because the `s[0] = p[0x13]' store into an
   address-taken local kills the cse. 189 words, 0 differing. */
s32 func_800BE6F0(u8 *p, u8 *q, s16 k)
{
    u8 s[4];
    u8 *b;
    u8 *w;
    u8 *e;
    u8 *g;
    u8 *g2;
    void *h;
    void *m;
    s32 r;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        h = func_800990FC();
        m = h;
        if ((u32)(p[0x13] - 3) < 0x2B)
        {
            h = func_80099734(p, h);
            h = func_80099194(D_800E1095, h);
        }
        else
        {
            h = func_80099734(p, h);
            h = func_80099194(D_800E10B3, h);
            h = func_80099368(q, h);
            h = func_80099194(D_80089360, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    if ((u32)(p[0x13] - 3) < 0x2B)
    {
        s[0] = p[0x13];
        if (p[0x13] == 3 || p[0x13] == 5 || p[0x13] == 7 || p[0x13] == 9 || p[0x13] == 0xB ||
            p[0x13] == 0xD || p[0x13] == 0xF || p[0x13] == 0x11 || p[0x13] == 0x13)
        {
            s[0]++;
        }
        s[1] = 0x12;
        s[2] = (func_800A6D30() & 0xF) + 0xA;
        s[3] = 0;
        func_800A90E8(s);
        b = *(u8 **)(p - 0x18);
        func_800B8228(*(s16 *)(b + 0x2), *(s16 *)(b + 0x6), *(s16 *)(b + 0xA), s);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(p - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        e = func_800A32A4(p);
        if (e != NULL)
        {
            r = func_800A6620(e, 0);
            if (r < 0x40)
            {
                u8 *z = (u8 *)0x80010000;

                *(u8 *)(z + r * 0x54 + 0xA93) = 0;
                ((s32 *)0x80010980)[r] = 0;
            }
        }
        if ((s16)func_80042900((s8 *)p, 0x1B) == 0)
        {
            w = *(u8 **)(p - 0x14);
            func_8009A3D0(w[0x24], w[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(p);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        func_800A5F38(p, q);
        return 1;
    }
    func_80098B38(q);
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0xA) -= 1;
    return 1;
}

/* func_800BE5A8 with a RANDOM effect value -- `func_800A6D30() & 0x1F` with
   bit 5 forced on -- whose result is not tested, a different tint and a
   different pair of strings. 83 words, 0 differing. */
s32 func_800BE9E4(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_800A48F0((s8 *)p, 0xF, (func_800A6D30() & 0x1F) | 0x20);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E10C0);
        }
        func_800D5460((UnkNode *)(p - 0x20), 0x804040, 0x612);
    }
    else
    {
        func_800997FC(D_800E10F2);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* THE KILL HANDLER THAT BUILDS A SENTENCE. The second skeleton's gate, then
   an out-of-range arm that reports what happened in words: take a message
   buffer from func_800990FC and run it through the overlay's
   func_80099194 / func_80099368 / func_8009955C / func_80099978 /
   func_800998C0 chain, each returning the handle the next takes, and hand
   the result to func_80099290. D_80089364 and D_80089368 are two of the
   three fragments this overlay keeps in its own rodata.

   Two spellings cost words here:

   - `h` IS REUSED for func_80099368's result; a second variable for it puts
     that result in the temporary's register instead and moves four words.
     The OLD handle still has to reach func_800998C0, so it goes into its own
     variable first: `e2 = h; h = func_80099368(e, h); func_800998C0(t, e2);`
   - `t` HOLDS THE BIASED POINTER, `h - 2`, not `h`. Written as `t = h` with
     the `- 2` at the call site the subtraction moves to the call and the
     function is 1 word differing.

   The arms are in the target's physical order, which is the reverse of the
   natural reading twice over: the `e == NULL` report is the LAST arm and the
   `e[3] & 0x40` short form is the SECOND, so both are written as the `else`.
   175 words, 0 differing. */
s32 func_800BEB30(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    void *t;
    void *e2;
    u8 *e;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(D_800E3D7C + 0x110) = q;
        func_8008D344(D_800E3D7C, D_80083780, D_80082E80, D_800E3D7C);
        q[3] &= 0xDF;
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = *(u8 **)(D_800E3D7C + 0x4C);
        if (e != NULL)
        {
            if (!(q[3] & 0x20))
            {
                *(s32 *)(D_800E3D7C + 0x114) = 0x202080;
                func_8008D368(D_800E3D7C, D_80083780, D_80082E80, D_800DD148, 0x2);
                q[3] |= 0x20;
                return 0;
            }
            m = func_800990FC();
            h = m;
            if (e[1] == 0xF || (e[1] == 0x10 && e[0] == 2))
            {
                if (!(e[3] & 0x40))
                {
                    h = func_8009955C(e, m);
                    if (*(s8 *)(e + 2) < 0x63)
                    {
                        e[2]++;
                    }
                    h = func_80099978(func_80099194(D_800E110B, h));
                    t = (u8 *)h - 2;
                    h = func_80099194(D_80089364, h);
                    e2 = h;
                    h = func_80099368(e, h);
                    func_800998C0(t, e2);
                    h = func_80099194(D_80089368, h);
                }
                else
                {
                    h = func_80099194(D_800E112A, func_80099368(e, m));
                }
            }
            else
            {
                h = func_80099194(D_800E1156, func_80099368(e, h));
            }
            func_80099290(h);
            func_800A5720(m);
        }
        else
        {
            func_800997FC(D_800E116E);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BEB30's shorter sibling: the record comes from +0x50 rather than
   +0x4C, there is no +0x01/+0x00 dispatch at all, and func_800990FC is
   called INSIDE each arm rather than once above them -- which is also why
   the final func_80099194 result goes straight into func_80099290 here
   instead of through a variable. 162 words, 0 differing. */
s32 func_800BEDEC(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    void *t;
    void *e2;
    u8 *e;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(D_800E3D7C + 0x110) = q;
        func_8008D344(D_800E3D7C, D_80083780, D_80082E80, D_800E3D7C);
        q[3] &= 0xDF;
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = *(u8 **)(D_800E3D7C + 0x50);
        if (e != NULL)
        {
            if (!(q[3] & 0x20))
            {
                *(s32 *)(D_800E3D7C + 0x114) = 0x802020;
                func_8008D368(D_800E3D7C, D_80083780, D_80082E80, D_800DD158, 0);
                q[3] |= 0x20;
                return 0;
            }
            if (!(e[3] & 0x40))
            {
                m = func_800990FC();
                h = func_8009955C(e, m);
                if (*(s8 *)(e + 2) < 0x63)
                {
                    e[2]++;
                }
                h = func_80099978(func_80099194(D_800E118C, h));
                t = (u8 *)h - 2;
                h = func_80099194(D_8008936C, h);
                e2 = h;
                h = func_80099368(e, h);
                func_800998C0(t, e2);
                func_80099290(func_80099194(D_80089370, h));
            }
            else
            {
                m = func_800990FC();
                func_80099290(func_80099194(D_800E11AB, func_80099368(e, m)));
            }
            func_800A5720(m);
        }
        else
        {
            func_800997FC(D_800E11D7);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* 103 words, 0 differing. */
s32 func_800BF074(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *g2;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        h = func_800990FC();
        m = h;
        g2 = (u8 *)&D_80082EB0;
        h = func_80099368(*(u8 **)(g2 + 4), h);
        h = func_80099194(D_800E11F5, h);
        (*(u8 **)(g2 + 4))[3] &= 0x7F;
        func_800A90E8(*(u8 **)(g2 + 4));
        h = func_80099368(*(u8 **)(g2 + 4), h);
        h = func_80099194(D_80089374, h);
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BF34C with D_800E11FD and bit 0x800000, plus func_8009D8A4 and
   sound 0x80F on the out-of-range arm. 79 words, 0 differing. */
s32 func_800BF210(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E11FD);
        func_8009D8A4();
        D_800E296C |= 0x800000;
        func_800A56E0(0x80F);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* THE SECOND KILL SKELETON, in five copies. It is not the func_800BD72C one:
   four parameters rather than three, a 0xD short-circuit at the top that
   hands the node straight to func_80098864, func_800A6480 in place of
   func_800A63B8, func_8008D344 in place of func_8008D330, the D_800DDE84
   field taken as the LOW two bits rather than bits 6-7, the node passed to
   func_800AD6FC as its third argument instead of zero, and an out-of-range
   arm that reports a string and raises a bit in D_800E296C. Only that
   string and that bit differ between the five -- D_800E1224/0x100,
   D_800E1258/0x800, D_800E1279/0x400, D_800E12A9/0x200 -- except
   func_800BF210, which also rings func_8009D8A4 and sound 0x80F.

   The 0xD arm RETURNS func_80098864's value: the branch leaves $v0 alone
   and jumps to the epilogue, where the other arms set it explicitly.

   75 words, 0 differing. */
s32 func_800BF34C(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E1224);
        D_800E296C |= 0x100;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BF34C with D_800E1258 and bit 0x800. 75 words, 0 differing. */
s32 func_800BF478(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E1258);
        D_800E296C |= 0x800;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BF34C with D_800E1279 and bit 0x400. 75 words, 0 differing. */
s32 func_800BF5A4(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E1279);
        D_800E296C |= 0x400;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BF34C with D_800E12A9 and bit 0x200. 75 words, 0 differing. */
s32 func_800BF6D0(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E12A9);
        D_800E296C |= 0x200;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The second kill skeleton with an out-of-range arm that reaches the
   D_800E2970 room table through the record's -0x14 node: the 0x14-byte
   stride and the +0x0C flag halfword, with `b = D_800E2970;` as its own
   statement exactly as func_8009FCAC needs it. The +0x26 index is RE-READ
   after func_800C4EB4, which is the second `lb`. 124 words, 0 differing. */
s32 func_800BF7FC(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
    u8 *b;
    s32 i;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = *(u8 **)(p - 0x14);
        i = *(s8 *)(e + 0x26);
        if (i >= 0)
        {
            b = D_800E2970;
            if (!(*(u16 *)(b + i * 0x14 + 0xC) & 2))
            {
                if (func_800C4EB4(e[0x24], e[0x25], *(s16 *)(p + 0x88), q[0], i) == 0)
                {
                    return 0;
                }
                i = *(s8 *)(e + 0x26);
                *(u16 *)(b + i * 0x14 + 0xC) |= 2;
                func_800C4D78((u8 *)0x20C0C0, 1);
                h = func_800990FC();
                m = h;
                h = func_80099194(D_800E12D6, h);
                func_80099290(h);
                func_800A5720(m);
            }
            else
            {
                func_800997FC(D_800E1303);
            }
        }
        else
        {
            func_800997FC(D_800E1303);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* 133 words, 0 differing. */
s32 func_800BF9EC(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *h;
    u8 *e;
    s32 t;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800C4D78((u8 *)0x2020C0, 1);
        func_800997FC(D_800E131C);
        h = (u8 *)D_800814A8;
        for (e = *(u8 **)(h + 0x5C) + 0x20; e != h; e = *(u8 **)(e + 0x5C) + 0x20)
        {
            if ((*(u32 *)(e + 0x1C) & 0x2400) != 0x2000 ||
                (s16)func_80042900((s8 *)e, 0xC) != 0)
            {
                if (p[0x11] < 0x63)
                {
                    *(u32 *)(e + 0x18) = D_800835E8[e[0x11]];
                    func_800A1D4C(e, 0);
                    if (!(*(u16 *)(*(u8 **)(e - 0x14) + 0x14) & 0x8000))
                    {
                        if (*(u32 *)(e + 0x14) & 0x2000)
                        {
                            t = 0x8002;
                        }
                        else
                        {
                            t = 0x8003;
                        }
                        func_800B4C7C(t, e, -2, 1);
                    }
                }
            }
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The first kill skeleton with a two-stage counter: when the record's +0x26
   has caught up with its +0x68 cap, bump +0x00 and notify func_80041E70;
   then raise +0x26 itself while it is below both 0xFF and the cap.
   96 words, 0 differing. */
s32 func_800BFC00(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x26] == p[0x68])
    {
        if (p[0x0] < 0xFF)
        {
            p[0x0]++;
            func_80041E70(p);
        }
    }
    if (p[0x26] < 0xFF && p[0x26] < p[0x68])
    {
        p[0x26]++;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E1350);
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x2020F0, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800BFC00 one byte over -- +0x27/+0x69/+0x01 -- with its own string
   and tint. 96 words, 0 differing. */
s32 func_800BFD80(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x27] == p[0x69])
    {
        if (p[0x1] < 0xFF)
        {
            p[0x1]++;
            func_80041E70(p);
        }
    }
    if (p[0x27] < 0xFF && p[0x27] < p[0x69])
    {
        p[0x27]++;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E2056);
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x20F020, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The func_800BD72C kill skeleton with this entity's middle: below a
   population of 0x63 the record takes a fresh func_8004383C index, drops its
   two +0x41/+0x42 bytes and raises D_800E296C's 0x100000. First compile.
   79 words, 0 differing. */
s32 func_800BFF00(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x11] < 0x63)
    {
        *(u32 *)(p + 0x18) = func_8004383C(p, 0);
        p[0x42] = 0;
        p[0x41] = 0;
        D_800E296C |= 0x100000;
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x20F0F0, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800C3C08 on the +0x02 counter, with the effect applied
   unconditionally through func_800A48F0 and a different tint.
   81 words, 0 differing. */
s32 func_800C003C(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x2] < 0xFF)
    {
        p[0x2]++;
    }
    func_800A48F0((s8 *)p, 0x7, 0x8);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E206A);
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0xF02020, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* 44 words, 0 differing. The +0xAC slot table walked DOWNWARD from 1, with
   D_800E3D7C re-read every turn -- the inline `i * 4 + D_800E3D7C + 0xAC`
   spelling, not the pointer-local hoist. */
s32 func_800C0180(void)
{
    s32 i;
    u8 *p;

    if (func_800C07AC(D_80083498) == NULL)
    {
        return 0;
    }
    for (i = 1; i >= 0; i--)
    {
        p = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
        if (p != NULL)
        {
            func_800C542C(p, D_800DCED4[func_800429E4(p)], (s16)i, 0);
        }
    }
    return 1;
}

/* The kill skeleton whose out-of-range arm refuses while the engine's two
   state gates are up, otherwise runs func_800BBA40's effect and BUMPS the
   counter rather than dropping it -- the only member that does.

   THE PARAMETER IS COPIED THROUGH A DUPLICATED `if` ARM, and that is a
   device: the whole residue is one delay slot. Written plainly, reorg fills
   the cached-RAM gate's `bnez` with `lui %hi(D_80012090)` stolen from the
   head of the out-of-range block and retargets the branch past it; the
   target fills it from the fall-through with func_800A6480's first argument
   and leaves the `lui` where it is. 26 differing, every one of them that
   one-instruction shift, with no register anywhere wrong. Twenty-odd hand
   spellings sat flat on it; decomp-permuter went from base 60 to 0 in four
   minutes, and this is its idea minimised. A plain `pp = p;` does NOT work
   (26 differing) -- the two arms are what put a CODE_LABEL in reorg's way,
   and the guard is irrelevant (`q != NULL` and `k != 0` both reach 0).
   jump2 cross-jumps the arms back together, so the emitted code carries no
   trace of the device. Same mechanism as func_8009704C's duplicated arm.

   And the lever that got it to the right LENGTH, which is not a device:
   with three counter sites and one shared tail, exactly ONE arm
   early-returns. All three sharing the tail is 114 words of 117; all three
   carrying their own copy is 115. 117 words, 0 differing. */
s32 func_800C0230(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *e;
    u8 *pp;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, NULL);
        return 0;
    }
    if (q != NULL)
    {
        pp = p;
    }
    else
    {
        pp = p;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(pp, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
    else
    {
        if (D_80012090 == 0 && D_8008146C == 0x28 && func_80033BC0(0xA2) == 0)
        {
            func_800997FC(D_800E1375);
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
            func_80098B38(q);
            return 1;
        }
        e = *(u8 **)(p - 0x14);
        if (func_800BBA40(e[0x24], e[0x25], *(s16 *)(p + 0x88), D_800DF45C, 0x2800, 0x208020,
                          func_800C0180) == 0)
        {
            return 0;
        }
        func_800997FC(D_800E13B7);
        g3 = (u8 *)&D_80083460;
        *(u16 *)(g3 + 0xA) += 1;
    }
    func_80098B38(q);
    return 1;
}

void func_800C0404(u8 *p, UnkVec *v, UnkSlot *r)
{
    u32 w[2];
    UnkDungeonRect q;
    Unk80083160 *g;
    u8 *tag;
    u8 *e;
    u8 *c;
    u8 *t;
    u8 *z;
    s32 lean;
    s32 i;
    s32 n;
    s32 k;
    s32 m;
    s16 h;

    w[0] = 0x1800340;
    w[1] = 0x400040;
    g = &D_80083160;
    lean = g->unk_00 != D_801C9E40;
    e = *(u8 **)(g->unk_00 + 0x8D0);
    tag = g->unk_00 + 0x8B0;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0xC);
    SetDrawArea(e, g->unk_00);
    AddPrim(tag, e);
    for (i = 0; i < *(s16 *)(p + 0x52); i++)
    {
        e = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0x10);
        n = (rand() & 0x3F) + 0x340;
        *(s16 *)(e + 0xC) = n;
        *(s16 *)(e + 0x8) = n;
        k = rand() & 0x3F;
        *(s16 *)(e + 0xA) = 0x180 - k;
        *(s16 *)(e + 0xE) = 0x1BF - k;
        do
        {
            if (lean)
            {
                *(u16 *)(e + 0xA) -= 0xE0;
                *(u16 *)(e + 0xE) -= 0xE0;
            }
        } while (0);
        *(u32 *)(e + 0x4) = 0;
        SetLineF2(e);
        AddPrim(tag, e);
    }
    if (*(s16 *)(p + 0x52) < 0x60)
    {
        *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) + 1;
    }
    e = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0xC);
    SetDrawArea(e, w);
    AddPrim(tag, e);
    q.v[0] = 0x360;
    q.v[1] = 0x1B8;
    func_800B8FC8(*(u8 **)(p + 0x68), w, q.v, 1, 0);
    *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
    m = *(s16 *)(p + 0x4A);
    if (m == 0)
    {
        if (*(s16 *)(p + 0x4C) <= 0x20)
        {
            return;
        }
        c = *(u8 **)(p + 0x68);
        if (c[0x13] == 0)
        {
            func_80091934((UnkDungeonActor *)c, D_80083780, D_80082E80, NULL);
            D_800E3D7C[0x9B] = 0x11;
            z = D_800E3D7C;
            t = D_800DD0F8;
            e = D_80082E80;
            *(u8 **)(e + 0x2C) = t;
            func_80048A44(e, t[((D_80083228 + *(s16 *)(z + 0x2A) + 0x100) >> 9) & 7],
                          1, 1);
        }
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
    }
    else if (m == 1)
    {
        r->unk_0C[0] -= r->unk_0C[0] >> 3;
        r->unk_0C[1] -= r->unk_0C[1] >> 3;
        r->unk_0C[2] -= r->unk_0C[2] >> 3;
        r->unk_1C += (0x400 - r->unk_1C) >> 2;
        c = *(u8 **)(p - 0x18);
        v->unk_08 += v->unk_14;
        *(s32 *)(c + 0x8) = v->unk_08;
        v->unk_14 -= 0x40000;
        if ((*(u8 **)(p + 0x68))[0x13] != 0)
        {
            return;
        }
        if (!(*(u16 *)(*(u8 **)(D_800E3D7C - 0x14) + 0x14) & 0x6000))
        {
            return;
        }
        h = func_8003F794(6, 0x20);
        D_800814E8 = h;
        D_80083120[h].unk_06 = 1;
        func_800945E8(D_800E3D7C);
        func_800948BC();
        D_80082E76 = 0x8000;
        func_80041094(6, 0, 0, 0, 0x8000);
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
    }
}

/* Re-home the engine node that carries `p`. THE COLOUR WORD IS STORED BEFORE
   THE TWO 0x1000 SCALES: that one statement swap is the whole residue -- with
   the colour last the 0x1000 constant and the +0x14 read-modify-write
   exchange $v0 and $a0 and the payload base moves, 62 words / 10 differing.
   All 120 orderings of the six +0x0C-record statements were measured; only
   this one reaches 0. 62 words, 0 differing. */
void *func_800C07AC(void *p)
{
    UnkNode *n;
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *q;
    u8 *r;

    n = func_8003FD64(2, p);
    if (n != NULL)
    {
        func_80044A50(p);
        func_800BC318(p);
        n->unk_10 = (u32)func_800C0404;
        func_8004491C(n, (u32)D_80045340);
        b = *(u8 **)((u8 *)p + 0x8);
        a = *(u8 **)((u8 *)n + 0x8);
        *(s16 *)(a + 0x2) = *(u16 *)(b + 0x2);
        *(s16 *)(a + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)(a + 0xA) = *(u16 *)(b + 0xA);
        c = (u8 *)n->unk_0C;
        *(s16 *)(c + 0x10) = 0x20;
        *(u8 **)(c + 0x8) = D_800DF4A4;
        *(s32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        *(u16 *)(c + 0x14) |= 0xC;
        q = (u8 *)p + 0x20;
        r = (u8 *)n + 0x20;
        *(s16 *)(r + 0x4C) = 0x10;
        *(u8 **)(r + 0x68) = q;
        *(s32 *)(q + 0x14) |= 0x100000;
    }
    return n;
}

/* The second kill skeleton with a 0x16 ENTITY ARM that runs the same
   func_80099978 / func_800998C0 sentence the out-of-range arm does, and a
   0xD short-circuit that is itself conditional:
   `if (k == 0xD) { if (p[0x13] != 0x16 || (func_800A6D30() & 1)) return
   func_80098864(q, r); }`.

   The chain is func_800BEB30's, including its two measured facts -- `h` is
   REUSED for func_80099368's result with the old handle parked in its own
   variable first, and the biased pointer holds `h - 2` rather than taking
   the `- 2` at the call site. A second variable for func_80099368's result
   (`b = func_80099368(q, h); func_800998C0(a, h);`) is 177 words.

   THE TWO CHAINS SHARE `m` AND `h` BUT NOT THE BIASED POINTER AND THE PARKED
   HANDLE. With `a`/`b` shared as well neither `b` coalesces, the function
   takes a fifth callee-saved register and comes out 179 words; with all four
   split per arm it is the right length but `m` and `h` swap $s1 and $s2,
   24 differing. Only this split reaches 0 -- the other half of
   func_800C0B60's "two pairs of handle variables" lesson.
   175 words, 0 differing. */
s32 func_800C08A4(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    void *a;
    void *b;
    void *a2;
    void *b2;
    u8 *n;
    u8 *g;

    if (k == 0xD)
    {
        if (p[0x13] != 0x16 || (func_800A6D30() & 1))
        {
            return func_80098864(q, r);
        }
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        if (p[0x13] == 0x16)
        {
            n = *(u8 **)(p - 0x18);
            p[0x4B] &= ~0x20;
            func_800A7A7C(*(u16 *)(n + 0x2) >> 6, *(u16 *)(n + 0x6) >> 6, *(s16 *)(n + 0xA),
                          func_8004A658(p[0x49], p[0x48]), p + 0x48);
            p[0x48] = q[0];
            p[0x4B] |= 0x20;
            m = func_800990FC();
            h = func_80099978(func_80099194(D_800E13DA, func_80099734(p, m)));
            a = (u8 *)h - 2;
            h = func_80099194(D_80089378, h);
            b = h;
            h = func_80099368(q, h);
            func_800998C0(a, b);
            h = func_80099194(D_8008937C, h);
            func_80099290(h);
            func_800A5720(m);
        }
        else
        {
            func_800A6480(p, q);
            if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 4) & 3, 0) == 0)
            {
                func_800A5F38(p, q);
                return 1;
            }
        }
        func_80098B38(q);
    }
    else
    {
        m = func_800990FC();
        h = func_80099978(func_80099194(D_800E13E9, func_80099734(p, m)));
        a2 = (u8 *)h - 2;
        h = func_80099194(D_80089378, h);
        b2 = h;
        h = func_80099368(q, h);
        func_800998C0(a2, b2);
        h = func_80099194(D_8008937C, h);
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The elemental-brand handler. TWO PAIRS OF HANDLE VARIABLES, not one. The
   in-range chain and the out-of-range chain share `m`/`h` at 2 differing:
   `m` becomes a global allocno, the in-range chain loses the
   reused-temporary shape and the record takes $s0 away from the handle.
   `c = p + 8` BEFORE `i = 0` in the for-init is the last word.
   202 words, 0 differing. */
s32 func_800C0B60(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    void *m2;
    void *h2;
    u8 *s;
    u8 *c;
    s32 i;
    s32 n;
    u32 f;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        *(u32 *)(p + 0x14) &= ~7;
        *(u32 *)(p + 0x1C) &= ~7;
        if (q[0] == 5)
        {
            s = D_800E13FB;
            *(u32 *)(p + 0x14) |= 1;
            *(u32 *)(p + 0x1C) |= 1;
            func_800D4FC8((UnkNode *)(p - 0x20), 0xF0, 0x81C);
        }
        else if (q[0] == 6)
        {
            s = D_800E1402;
            *(u32 *)(p + 0x14) |= 2;
            *(u32 *)(p + 0x1C) |= 2;
            func_800D4FC8((UnkNode *)(p - 0x20), 0xF00000, 0x81C);
        }
        else
        {
            s = D_800E140A;
            *(u32 *)(p + 0x14) |= 4;
            *(u32 *)(p + 0x1C) |= 4;
            func_800D4FC8((UnkNode *)(p - 0x20), 0xF000, 0x81C);
        }
        func_80042984(p);
        for (c = p + 8, i = 0; i < 3; i++, c += 3)
        {
            if (c[0] != 0)
            {
                n = c[0];
                f = *(u32 *)(p + 0x14);
                if (((u8)D_8006DE24[n].unk_10 & 7) & f)
                {
                    continue;
                }
                if (f & 1)
                {
                    c[0] = (n - 1) / 3 * 3 + 1;
                }
                else if (f & 2)
                {
                    c[0] = (n - 1) / 3 * 3 + 2;
                }
                else if (f & 4)
                {
                    c[0] = (n - 1) / 3 * 3 + 3;
                }
            }
        }
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E1411, h);
            h = func_80099194(s, h);
            h = func_80099194(D_80089380, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    else
    {
        h2 = func_800990FC();
        m2 = h2;
        h2 = func_80099194(D_800E1426, h2);
        h2 = func_80099734(p, h2);
        h2 = func_80099194(D_80089380, h2);
        func_80099290(h2);
        func_800A5720(m2);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* 309 words, 0 differing. */
s32 func_800C0E88(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    UnkDungeonName0 *t;
    s32 i;
    s32 d;
    u8 *e;
    s32 f;
    s32 n;
    s32 w;
    s32 a;
    s32 b;
    u16 v;
    u8 *g;
    u8 *g2;
    u8 *g3;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        q[3] |= 0x20;
        *(u8 **)(D_800E3D7C + 0x110) = q;
        func_8008D344(D_800E3D7C, D_80083780, D_80082E80, D_800E3D7C);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if ((s8)q[2] >= 0xA && q[0] < 0x10)
        {
            if (func_800A57B4(p, (q[0] - 1) * 3 + 1) < 0)
            {
                t = (UnkDungeonName0 *)D_8006DE24;
                for (i = 0; i < 3; i++)
                {
                    if (p[i * 3 + 8] == 0)
                    {
                        f = *(u32 *)(p + 0x14);
                        if (f & 1)
                        {
                            p[i * 3 + 8] = (q[0] - 1) * 3 + 1;
                        }
                        else if (f & 2)
                        {
                            p[i * 3 + 8] = (q[0] - 1) * 3 + 2;
                        }
                        else if (f & 4)
                        {
                            p[i * 3 + 8] = (q[0] - 1) * 3 + 3;
                        }
                        d = (s8)q[2];
                        d = d / 10;
                        p[i * 3 + 0xA] = d;
                        p[i * 3 + 9] = d;
                        if (*(u32 *)(p + 0x14) & 0x4000)
                        {
                            h = func_800990FC();
                            m = h;
                            h = func_80099734(p, h);
                            h = func_80099194(D_800E1440, h);
                            h = func_80099194(t[p[i * 3 + 8]].unk_00, h);
                            h = func_80099194(D_800E144C, h);
                            func_80099290(h);
                            func_800A5720(m);
                        }
                        break;
                    }
                }
            }
        }
        else if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_80098B38(q);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
    else if (*(s8 *)(q + 2) == 0)
    {
        func_800997FC(D_800E1456);
        q[3] &= 0xDF;
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0xA) -= 1;
    }
    else
    {
        *(u16 *)(D_800E3D7C + 0x98) &= 0xFF7F;
        p[9] = 0xFF;
        n = (q[0] - 1) * 3;
        p[8] = n + 1;
        v = *(u16 *)(D_80073470 + q[0] * 0x14);
        if (v & 2)
        {
            p[8] = n + 2;
        }
        else if (v & 4)
        {
            p[8] = n + 3;
        }
        e = *(u8 **)(p - 0x14);
        p = (u8 *)((u32)p & 0xDFFFFFFF);
        w = 0x10;
        *(s32 *)(p + 0x60) = func_800A05A4(p, e[0x24], e[0x25], *(s16 *)(p + 0x2A), w);
        a = *(s8 *)(p + 0x72);
        b = *(s8 *)(p + 0x73);
        p[0x72] = __builtin_abs(a);
        p[0x73] = __builtin_abs(b);
        if (q[3] & 0x20)
        {
            if (func_800A94A0(p, p + 8, 0, D_800E3D7C + 0x98) == 0)
            {
                return 0;
            }
            D_800DF4B0 = w;
            q[3] &= 0xDF;
        }
        D_800DF4B0 -= 1;
        if (D_800DF4B0 >= 0)
        {
            return 0;
        }
        D_800DF4B0 = 0;
        if (func_8003F270() != 0)
        {
            return 0;
        }
        g3 = (u8 *)&D_80083460;
        *(u8 **)(g3 + 0xC) = p;
        *(u16 *)(D_800814A8 + 0x98) |= 0x80;
        q[2] -= 1;
        h = func_800990FC();
        m = h;
        h = func_800992E8(q, h);
        h = func_80099194(D_800E146F, h);
        func_80099290(h);
        func_800A5720(m);
        *(u16 *)(g3 + 0xA) -= 1;
    }
    return 1;
}

/* THE `p == NULL` ARM IS AN EARLY RETURN and is laid out at the END. Spelled
   `if (p != NULL) { ...; return 1; } return 0;` the `return 0` collapses into
   the `beqz`'s delay slot and the function is 25 words against 27.
   27 words, 0 differing. */
s32 func_800C135C(void)
{
    u8 *p;

    p = func_800C9850(D_80082E80[0x24], D_80082E80[0x25], *(s16 *)(D_800E3D7C + 0x88));
    if (p == NULL)
    {
        return 0;
    }
    *(s32 *)(*(u8 **)(p - 0x14) + 0xC) = 0;
    func_80042640((Unk42710 *)p, p[0x13]);
    return 1;
}

/* WIP C13C8 */
s32 func_800C13C8(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *e;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, NULL);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
    else
    {
        e = *(u8 **)(p - 0x14);
        if (func_800BBA40(e[0x24], e[0x25], *(s16 *)(p + 0x88), D_800DEAE0, 0x5000, 0x202080, func_800C135C) == 0)
        {
            return 0;
        }
        func_800997FC(D_800E1482);
    }
    func_80098B38(q);
    return 1;
}

/* func_8003FA44's gate, then one func_800C1A44 per occupied engine slot.
   34 words, 0 differing. */
s32 func_800C152C(void)
{
    s32 i;
    u8 *p;
    u8 *e;

    if (func_8003FA44(2) == 0)
    {
        return 0;
    }
    for (i = 0; i < 2; i++)
    {
        p = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
        if (p != NULL)
        {
            e = *(u8 **)(p - 0x14);
            func_800C1A44(e[0x24], e[0x25], *(s16 *)(p + 0x88));
        }
    }
    return 1;
}

/* WIP C15B4 */
s32 func_800C15B4(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *e;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, NULL);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
    else
    {
        e = *(u8 **)(p - 0x14);
        if (func_800BBA40(e[0x24], e[0x25], *(s16 *)(p + 0x88), D_800DF514, 0x2800, 0x802020, func_800C152C) == 0)
        {
            return 0;
        }
        func_800997FC(D_800E14A6);
    }
    func_80098B38(q);
    return 1;
}

/* The engine-slot marker's node handler, installed at UnkNode +0x10 by
   func_800C1A44 and reached through func_80040190's three-argument cast:
   the view at node + 0x20, the mover and the renderable. A three-state
   switch on +0x4A. State 0 eases the renderable's +0x1C/+0x1E toward 0x800
   over +0x4C ticks and, when the count runs out, resets both engine-slot
   records in the +0xAC table; state 1 eases +0x50 toward 0xC0 and state 2
   bleeds +0x50 and the three colour bytes away, each spawning four 0x202
   nodes per tick off a 0x100 angle step.

   THE SLOT IS READ TWICE, in the test and again into the local. Written
   once -- `e = *(u8 **)(i * 4 + D_800E3D7C + 0xAC); if (e != NULL)` -- the
   load lands straight in `e`'s own register and reorg fills the `beqz`
   delay slot with the `addu $a0, $s0, $zero` the inner loop needs, which is
   one word short (202, 156 differing from the shift). Read twice, cse turns
   the second read into the copy `addu $s0, $a0, $zero`, that copy takes the
   delay slot, and the `$a0` setup stands on its own ahead of the loop.
   203 words, 0 differing. */
void func_800C1718(u8 *p, UnkVec *b, u8 *c)
{
    s32 i;
    s32 j;
    u8 *e;

    switch (*(s16 *)(p + 0x4A))
    {
    case 0:
        *(s16 *)(c + 0x1E) = *(s16 *)(c + 0x1C) =
            *(u16 *)(c + 0x1C) + (0x800 - *(u16 *)(c + 0x1C)) / *(s16 *)(p + 0x4C);
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) <= 0)
        {
            for (i = 0; i < 2; i++)
            {
                if (*(u8 **)(i * 4 + D_800E3D7C + 0xAC) != NULL)
                {
                    e = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
                    e[0x26] = e[0x68];
                    e[0x27] = e[0x69];
                    for (j = 1; j < 0x21; j++)
                    {
                        func_80042B68((s8 *)e, j);
                    }
                    *(s16 *)(e + 0x64) = 0xFF;
                    *(u32 *)(e + 0x1C) &= 0xFBFFE107;
                    func_800AD568(e);
                    e[0x24] = 0xFF;
                    e[0x25] = e[0x66];
                    func_80041E70(e);
                }
            }
            *(s16 *)(p + 0x4C) = 8;
            *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
        }
        break;
    case 1:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) + (0xC0 - *(s16 *)(p + 0x50)) / *(s16 *)(p + 0x4C);
        for (j = 0; j < 4; j++)
        {
            *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) + 0x100;
            func_800BC0A8(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA),
                          *(s16 *)(p + 0x4E), *(s16 *)(p + 0x50), *(s16 *)(p + 0x58), 0x802020,
                          *(s16 *)(p + 0x48));
        }
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) <= 0)
        {
            *(s16 *)(p + 0x4C) = 4;
            *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
        }
        break;
    case 2:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - *(s16 *)(p + 0x50) / *(s16 *)(p + 0x4C);
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x4C);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x4C);
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x4C);
        for (j = 0; j < 4; j++)
        {
            *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) + 0x100;
            func_800BC0A8(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA),
                          *(s16 *)(p + 0x4E), *(s16 *)(p + 0x50), *(s16 *)(p + 0x58), 0x802020,
                          *(s16 *)(p + 0x48));
        }
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The 0x12 spawner for the two engine-slot markers func_800C152C places. The
   two position arguments are s16: the target sign-extends each with
   `sll 16`/`sra 10`, which is the narrow parameter's extension folded into
   the `* 64`. No prototype is added -- its only caller sits ABOVE it and
   rides the implicit declaration, and a prototype would narrow that caller's
   three promoted arguments. 54 words, 0 differing. */
UnkNode *func_800C1A44(s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800C1718;
        func_8004491C(n, (u32)func_80045C34);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (u8 *)n->unk_0C;
        *(u8 **)(r + 0x8) = D_800DF3C0;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x6) = 8;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x4C) = 8;
        *(s16 *)(p + 0x48) = 7;
    }
    return n;
}

/* 113 words, 0 differing. */
s32 func_800C1B1C(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
    u8 *g2;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        m = func_800990FC();
        g2 = (u8 *)&D_80082EB0;
        if ((*(u8 **)(g2 + 4))[1] == 4)
        {
            h = func_80099194(D_800E14D8, m);
            h = func_800992E8(*(u8 **)(g2 + 4), h);
            h = func_80099194(D_800E14EB, h);
            func_800A56E0(0x80F);
            e = *(u8 **)(g2 + 4);
            if (*(s8 *)(e + 2) < 0x63)
            {
                e[2] = e[2] + 1;
            }
        }
        else
        {
            h = func_80099194(D_800E1514, m);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The second kill skeleton whose out-of-range arm takes the message handle
   FIRST and only then decides what to say: with either +0xAC or +0xB0 of
   D_800E3D7C occupied it runs func_800C1F28 over both slots behind
   func_8003FA44's gate, otherwise it reports D_800E1549. The slot walk is
   func_800C0180's inline `i * 4 + D_800E3D7C + 0xAC` spelling, upward from 0.
   121 words, 0 differing. */
s32 func_800C1CE0(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
    s32 i;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        h = func_800990FC();
        m = h;
        if (*(u8 **)(D_800E3D7C + 0xAC) != NULL || *(u8 **)(D_800E3D7C + 0xB0) != NULL)
        {
            if (func_8003FA44(4) == 0)
            {
                return 0;
            }
            for (i = 0; i < 2; i++)
            {
                e = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
                if (e != NULL)
                {
                    func_800C1F28(e, (s16)i);
                }
            }
            h = func_80099194(D_800E1532, h);
            func_800A56E0(0x501);
        }
        else
        {
            h = func_80099194(D_800E1549, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* 25 words, 0 differing. */
void func_800C1EC4(u8 *p)
{
    u8 *g;

    if (--*(s16 *)(p + 0x2) <= 0)
    {
        *(s16 *)(*(s32 *)(p + 0xC) + 0x88) = *(u16 *)(p + 0x8);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
}

/* Spawn the "tile marker" node for slot k: claim a D_80083460 slot, wire the
   node's +0x20 view to the actor and park func_800C1EC4 on it as the
   per-frame handler. When func_8009B88C hands back a cursor the marker goes
   to that tile's projected depth (and to -0x400 if that came out above
   0x200); otherwise it goes to the actor's own +0x24/+0x25 tile and +0x88
   depth. func_800C542C's argument expression is func_800C0180's (:15545)
   verbatim.

   THREE SPELLING FACTS, all measured, and none of them a device:

   1. `((s16 *)m)[4] = ((u16 *)p)[0x44];` -- BOTH SIDES AS ARRAY REFERENCES.
      As `*(s16 *)(m + 0x8) = *(u16 *)(p + 0x88);` the arm is 131 words: the
      `y` load cannot be scheduled into the +0x88 load's delay slot and the
      extra `nop` is the whole excess. sched.c's `true_dependence` /
      `output_dependence` clause only drops a dependence when one MEM is
      MEM_IN_STRUCT_P with a varying address and the other is neither
      in-struct nor varying -- the `y` slot is sp-based, so marking BOTH the
      +0x88 read and the +0x8 store in-struct is what lets the load and the
      store cross it. The +0x88 read alone is 130 words / 6 differing, the
      +0x8 store alone 131 / 76, and neither at all 131 / 76. The statement
      order must be x, y, then the depth: x, depth, y is 131 / 74.
   2. ONE SHARED `return e`, with the whole body under `if (e != NULL)`.
      As an early `return e;` (or `return NULL;`) the null exit's delay slot
      is `move $v0, $zero` where the target has `addu $v0, $s3, $zero` --
      gcc knows the value is zero there, so only reorg COPYING the shared
      block's own insn produces it.
   3. THE SECOND `+0x1C` READ GOES THROUGH THE POINTER READ BACK OUT OF
      `m + 0xC`. Read off `p` again it is 2 differing: the target reads
      `0x1C($v1)` off a register copy of the actor pointer and fills a
      load-delay slot with it. `q = p;` in any of three positions is
      copy-propagated away (same md5), and an `__asm__("" : "=r"(q) : "0"(q))`
      copy is 131 words / 21. cse forwards the `(u32)p` store, so the read-back
      IS that copy.
   130 words, 0 differing. */
UnkNode *func_800C1F28(u8 *p, s16 k)
{
    UnkNode *e;
    u8 *m;
    u8 *r;
    u8 *g;
    u8 *v;
    s16 x;
    s16 y;

    e = func_8003FC64(2);
    if (e != NULL)
    {
        func_800C542C(p, D_800DCED4[func_800429E4(p)], k, 3);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        m = (u8 *)e + 0x20;
        r = *(u8 **)(p - 0x14);
        func_8009A3D0(r[0x24], r[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        if ((s16)func_8009B88C(NULL, D_80082E80[0x24], D_80082E80[0x25], &x, &y) == 0)
        {
            x = r[0x24];
            y = r[0x25];
            ((s16 *)m)[4] = ((u16 *)p)[0x44];
        }
        else
        {
            *(s16 *)(m + 0x8) = func_800BCB04(((x << 6) + 0x20) & 0xFFE0, ((y << 6) + 0x20) & 0xFFE0,
                                              (s16)(*(u16 *)(p + 0x88) - 0x20));
            if (*(s16 *)(m + 0x8) > 0x200)
            {
                *(s16 *)(m + 0x8) =
                    func_800BCB04(((x << 6) + 0x20) & 0xFFE0, ((y << 6) + 0x20) & 0xFFE0, -0x400);
            }
        }
        r[0x24] = x;
        r[0x25] = y;
        *(s16 *)(p + 0x88) = *(u16 *)(m + 0x8);
        e->unk_10 = (u32)func_800C1EC4;
        *(u32 *)(m + 0xC) = (u32)p;
        *(s16 *)(m + 0x2) = 0x20;
        *(s16 *)(m + 0x4) = r[0x24];
        *(s16 *)(m + 0x6) = r[0x25];
        v = *(u8 **)(m + 0xC);
        func_8009A21C(r[0x24], r[0x25], (*(u32 *)(v + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    }
    return e;
}

/* 111 words, 0 differing. */
s32 func_800C2130(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    s32 i;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800C4D78((u8 *)0xC02020, 1);
        h = func_800990FC();
        m = h;
        if (D_80082EA6 >= 0)
        {
            for (i = 0; i < 0x20; i++)
            {
                if (D_800E3648_s[i].unk_01 != 0 && *(u8 *)&D_800E3648_s[i] != 0 &&
                    (D_800E3648_s[i].unk_03 & 0x40) == 0)
                {
                    D_800E3648_s[i].unk_03 |= 0x80;
                }
            }
        }
        else
        {
            h = func_80099194(D_800E1567, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The kill skeleton's out-of-range arm reaches the record D_80082EB4 names,
   and `p` ITSELF is reused for it -- the target keeps everything in $s0.
   THE FLAG IS SET LAST IN THE ARM: written first its `li $s2, 1` lands
   before the mask's `ori` and the function is 3 differing; one statement
   later, 2; last, 0. 111 words, 0 differing. */
s32 func_800C22EC(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *e;
    s32 f;
    s32 i;

    f = 0;
    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = D_80082EB4;
        if (e[1] == 0x13)
        {
            i = e[3] & 0x1F;
            if (i < 0x14)
            {
                p = (u8 *)D_800E3DF0[i];
                if ((s16)func_80042900((s8 *)p, 0xE) != 0)
                {
                    p[0x28] = (p[0x29] >> 1) + 1;
                    *(s32 *)(p + 0x1C) &= ~0x4000000;
                    func_80041E70(p);
                    func_80099844(p, D_800E1580);
                    f = 1;
                }
            }
        }
        if (f == 0)
        {
            func_800997FC(D_800E15A2);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* 93 words, 0 differing. */
s32 func_800C24A8(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if ((s16)func_800A48F0((s8 *)p, 0x9, 0x60) != 0)
    {
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E15BB, h);
        }
        else
        {
            m = func_800990FC();
            h = func_80099194(D_800E15E5, m);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The first kill skeleton WITHOUT the pointer gate. TWO SCRATCH LOCALS FOR
   D_80083460, one per site. With one `g` the symbol becomes a global
   allocno, both pairs come out as `lui $v0` / `addiu $a1` split across two
   registers, and the function is 10 differing at the right length.
   117 words, 0 differing. */
s32 func_800C261C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;
    u8 *g2;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (p[0x13] >= 3 && p[0x13] < 0x2E)
        {
            if (D_80012090 == 0 && D_8008146C == 0x28 && func_80033BC0(0xA2) == 0)
            {
                func_800997FC(D_800E15FE);
                g2 = (u8 *)&D_80083460;
                *(u16 *)(g2 + 0xA) -= 1;
                func_80098B38(q);
                return 1;
            }
            if (func_800C2C7C(p) == 0)
            {
                return 0;
            }
            func_80098B38(q);
        }
        else
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E1677, h);
            h = func_80099368(q, h);
            h = func_80099194(D_80089384, h);
            func_80099290(h);
            func_800A5720(m);
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        if (p[0x13] >= 3 && p[0x13] < 0x2E)
        {
            if (func_800C2C7C(p) == 0)
            {
                return 0;
            }
        }
        else
        {
            func_800A63B8(p, q, k);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* THE CALL GETS ITS OWN STATEMENT, which is func_800A0E44's lever: written
   as `D_800DF560 = func_800C07AC(...)` the global's `lui` expands ahead of
   the call. 13 words, 0 differing. */
s32 func_800C27F0(void)
{
    void *p;

    p = func_800C07AC(D_800DF55C - 0x20);
    D_800DF560 = p;

    return p != NULL;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089354);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089358);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_8008935C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089360);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089364);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089368);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_8008936C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089370);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089374);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089378);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_8008937C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089380);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B6AC4", D_80089384);
