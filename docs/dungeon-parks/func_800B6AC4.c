/* PARK -- func_800B6AC4, the 12-case +0x9B state machine over jtbl_80089310.
   NOT LANDED.  Best measured: built 811 words, target 812 words,
   46 aligned differences (tools/decomp/aligndiff.py).  ovlverify's positional
   count reads 598/643 only because the candidate is one word short at
   aligned index 203 and everything after it shifts.

   The register allocation, the switch dispatch, the epilogue, the shared
   `p[0x9B]++` tail (.L800B7718/.L800B771C), the shared redraw tail
   (.L800B770C), and cases 0, 1, 3, 4, 5, 6, 8 and 11 are now EXACT.
   What is left, in aligned-index order:

   (1) 2 words, case 2's `k` flag.  The target materialises k in $v1 and
       tests a COPY in $v0: `addu $v0, $v1, $zero` twice, once in the
       delay slot of `beqz $v0, .L800B6DFC` and once at the .L800B6DF8
       cross-jump join.  Every spelling tried tests $v1 directly and loses
       both copies.  Measured and rejected: `if (k == 1)` (808/64),
       `if (k != 0)` (807/61 -- identical to `if (k)`), an explicit
       `else { k = 0; }` in the g+8 arm (805/64), and declaring k first
       (no change).

   (2) 1 row, case 7: the target emits `lui $a1, %hi(D_800DCF4F)` BEFORE
       `addiu $a0, $a0, %lo(D_80083460)`; the candidate emits it after.
       Zero words, pure order.

   (3) 5 rows, case 9: the target leaves a load-delay `nop` between
       `lw $v1, 0xC0($s2)` and `lbu $v1, 0x0($v1)` and only materialises
       %hi/%lo(D_800E3DF0) after the three `sb`s.  The candidate fills that
       stall with the D_800E3DF0 `lui`, which costs $v0 and forces an extra
       `move $a1, $v0` for the func_80099090 result.  Net zero words.
       Measured and rejected: `*(u8 *)(p + 0xC0)` in place of the
       `((u8 **)p)[0x30]` array form (no change), hoisting `base9` above the
       func_80099090 call (no change), dropping the `base9` local and writing
       `(u8 *)0x80010000` inline (808/62).

   (4) ~6 rows, case 10: the target's `sw $v1, %lo(D_800814A0)($t0)` is the
       LAST insn before .L800B770C, with `sw $a1, 0x2C($a0)` and
       `lh $a3, 0x2A($s3)` scheduled between the load and the store.  The
       candidate puts the store four insns earlier and the table store one
       insn earlier.

   WHAT THE STATEMENT ORDER IN CASE 10 IS WORTH -- this is the single most
   expensive thing measured on this function, because it decides whether
   case 8's redraw cross-jumps into case 10's (the target shares only
   .L800B770C; a candidate whose case-10 tail ends in the same `addu`
   merges ten more words and comes out short).  With the four statements
   labelled B = the D_8001029C/D_800E3DF0 clear, C = the `-0x2 |= 0x8000`
   store, D = the `((u8 **)c)[0xB]` table store, and the D_800814A0
   read-modify-write split as Y = `y = D_800814A0 | 0x8000;` and
   S = `D_800814A0 = y;`, all 24 orders of {B,C,D} x Y-position were
   measured.  B,Y,D,C,S is 811/46; B,Y,D,S,C is 811/46 as well;
   B,C,Y,D,S is 814/65; B,D,C,Y,S is 814/65; Y,B,C,D,S is 805/58;
   the unsplit `D_800814A0 |= 0x8000;` in its best position is 807/61 and
   in its worst 816/68.

   LEVERS THAT WERE WORTH A LOT AND ARE ALREADY IN THE BODY:

   * EVERY D_80083460 BASE AND EVERY SCRATCH POINTER NEEDS ITS OWN LOCAL.
     One `h` shared by cases 2, 3 and 7 crosses a call in case 3, so global
     alloc gives the single pseudo a CALLEE-SAVED register everywhere and
     case 2's `lui/addiu D_80083460` comes out in $s0 where the target has
     $v1.  Splitting h/h2/h3/h4 (and r/e2/e7/e9/e10/n5/base9/base10) took
     the function from 280 aligned differences to 96 IN ONE STEP and, with
     it, fixed the whole parameter register map: before the split the
     allocation order was b, g, c, p, d and after it g, c, p, d, b, which
     is the target's.  Nothing else moved that map.

   * THE SIX FACING TABLES ARE NAMED TWICE, NOT BOUND TO A `u8 *t`.
     func_8008E264's rule transfers verbatim: with the local, the
     `addu $a0, $s1, $zero` the target emits BEFORE the table store is
     pushed past it at all six sites.  325 aligned differences with the
     local, 280 without.

   * CASE 2's FIRST D_80013714 READ MUST BE THE CONSTANT ADDRESS
     `*(u16 *)0x80013714`; the other four must be structs.h's SYMBOL.
     As the symbol, the `lui` is two insns and reorg steals it into the
     delay slot of the preceding `bnez $v0, .L800B7750`, where the target
     has a `nop` -- func_8008E0C4 records exactly this.  As the symbol in
     case 7 the `%hi` is CSE'd into $s3 and serves three reads, which the
     assembler macro cannot do, so that site must NOT be the constant.

   * CASE 1's THREE STORES ARE 0xC0, D_80081484, 0x8C -- in that order,
     with the +0x8C one a COMPONENT_REF (`((UnkDungeonActor *)p)->unk_8C`).
     The component ref is what lets sched sink the D_80081484 store past it
     (true_dependence's in-struct/varying exception); written
     `*(u32 *)(p + 0x8C)`, or with the 0x8C store ahead of the D_80081484
     one, the `lui %hi(func_800B69DC)` is stolen into the delay slot of the
     `bnez $v0, .L800B6C80` above and the block comes out in the wrong
     order.  Worth 10 aligned differences (75 -> 65).

   * CASE 4's +0x8C STORE IS ALSO A COMPONENT_REF, for func_800969B8's
     reason: through `*(u32 *)(p + 0x8C)` the store may alias D_800E296C
     and sched1 cannot lift that global's `lw` above it.

   * EVERY p+0xC0 ACCESS IS `((u8 **)p)[0x30]`, NOT A CAST.  The array form
     sets MEM_IN_STRUCT_P, which is what lets case 7's `lw $a0, 0xC0($s2)`
     move above the `sb` to D_800E2968.

   * CASE 9's e9[0] SOURCE BYTE IS READ INTO A TEMP BEFORE THE e9[3]/e9[2]
     STORES.  The target's `lw`/`nop`/`lbu` pair sits between `sb $v1, 0x1`
     and `sb $v0, 0x3`, and a byte store is QImode, which true_dependence
     never disambiguates -- so no spelling moves the load across it.  Only
     writing it out in that order puts it there.  Worth 10 (85 -> 75).

   * CASE 2's FIRST TEST IS WRITTEN `== 0` SO THE D_80083460 ARM IS THE
     FALL-THROUGH; written the other way round the branch is inverted.

   * D_80083228 IS D_80083160 + 0xC8, AND BOTH SPELLINGS APPEAR.  Case 0
     reads it as `q->unk_C8` off its own `Unk80083160 *q` (func_800927E4's
     measured shape, which this arm is a line-for-line copy of); every other
     arm reads the absolute `D_80083228`.

   * D_8001029C IS REACHED AS func_80098FF8 DOES IT: a `u8 *` local holding
     0x80010000 with 0x29C inside the index.

   Target 812 words = 0xCB0 bytes.  jtbl_80089310 is at rodata 0x89310,
   0 mod 8, so this function is safe to land (func_80095538 is the
   precedent). 
   DECLARATIONS (added 2026-11-16: this park did not compile from the repo
   alone -- seven symbols it uses were declared only in its author's
   transcript, so parkresidue could not measure it at all.)
     extern u32 D_800835E4[];
     extern u8 D_800DD108[];
     extern u8 D_800DD110[];
     extern u8 D_800DD118[];
     extern u8 D_800E0A2A[];
     extern u8 D_800E0A33[];
     extern u8 D_800E2968;
     typedef u8 *(*UnkDungeonSpawner)(s32, u8, u8, s16);
   END DECLARATIONS
   UnkDungeonSpawner was used by the body and defined nowhere -- reconstructed
   2026-11-16 from the `jalr $v0` at 800B7140: four arguments in $a0..$a3 as
   int / lbu c[0x24] / lbu c[0x25] / a halfword sign-extended by the sll+sra
   pair at 800B713C, and $v0 used as a pointer immediately after
   (`lbu 0x13($s0)`, `lw -0x14($s0)`).
   Shapes read off asm/nonmatchings/ovl_dungeon/func_800B6AC4.s: the six
   arrays are lui/addiu address pairs, D_800E2968 is lui + `sb %lo(..)` and
   so is a scalar byte, not an array.
   ====================================== 2026-11-20: 46 rows at 811/812 words
                                          -> 39 at 812/812

   ONE DECLARATION: `s32 k' became `s16 k'.  That is the whole change -- it
   supplies the missing word AND seven rows.  A permuter find, and the kind
   that is worth running the permuter for: nothing in the park's diagnosis
   pointed at `k''s width, and no amount of reading the residue would have.

   The park could not be measured AT ALL until today, for two reasons that had
   nothing to do with the body: csrc/ovl_dungeon_decls.h declared this
   function `void' while it returns s32 (so no parksplice placement compiled),
   and func_800B69DC -- defined in the PREVIOUS carved unit and called only
   from here -- had no prototype in this unit, because the carve generated
   cross-unit prototypes only for callees that LANDED code mentions.
*/
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
    u8 *w;
    void *s;
    s16 k;
    s32 x;
    u32 y;
    s32 v;
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
            v = (func_800A6D30() & 3) + 1;
            (*(u8 **)(p + 0xBC))[2] -= v;
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
        func_80069E78(n + 0x34, func_8003C06C(n[0x13]));
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
            h3 = (u8 *)&D_80083460;
            *(u16 *)(h3 + 0xA) += 1;
            D_800DCF4F = 1;
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
            *(u32 *)((u8 *)D_800E3DF0[e10[3] & 0x1F] + 0x14) &= ~0x4000;
            y = D_800814A0 | 0x8000;
            ((u8 **)c)[0xB] = D_800DD140;
            *(u16 *)(*(u8 **)(d + 0x60) - 0x2) |= 0x8000;
            D_800814A0 = y;
            func_80048A44(c, D_800DD140[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
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
