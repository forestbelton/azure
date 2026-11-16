/* LANDED on 2026-11-16 (batch j52): 398 words, 0 differing, verified with
   tools/decomp/ovlverify.py.  This file is the body as it should go into
   csrc/ovl_dungeon.c (it replaces that function's INCLUDE_ASM), with the
   declarations it needs at the top.  It is NOT a park: it is here only
   because /tmp is not durable. */

/* --- declarations, for the block at the top of the file --- */
extern u8 D_800DD260;
extern s16 D_800DD262;
extern u8 D_8008149C[];
extern u8 D_800E06FA[];
extern u8 D_800E0714[];
extern s32 D_800E3D74[];
/* The same word as D_80082EB0, read as a STRUCT MEMBER. */
typedef struct UnkDungeon82EB0
{
    u8 *unk_00;
} UnkDungeon82EB0;
extern UnkDungeon82EB0 D_80082EB0_m __asm__("D_80082EB0");
u8 *func_80048118(s16, u8 *);
void func_80047FF4(s16, u32);
s32 func_800A31D0(u8 *);

/* The ten-state spawn/handover machine over +0x9B: state 0 drops the
   D_800DD138 skin, runs func_80093C70/func_80093D8C and latches the view's
   +0x8A slot in D_800DD262, state 1 runs a 0x10 countdown and either retires
   the slot through func_80093E74 or skips two states, state 2 announces
   D_800E06FA, clears the +0xAC slot and either closes (D_80082EB0 == NULL)
   or carries on, states 3 and 4 load a resource through func_800A1618 /
   func_800A0B94 and wait on the D_800E3E40 handshake, state 5 builds the
   entity, dresses it, announces D_800E0714 and arms a 0x10 countdown, state
   7 re-skins from D_800DD140 and state 8 parks func_8008ACDC.  jtbl_80088A58
   is 0 mod 8, so cc1psx's `.align 3` is a no-op.  398 words, 0 differing.

   EVERY CASE ADVANCES +0x9B ITSELF.  Written as one `p[0x9B]++` after the
   switch the out-of-range arm has to fall into it; written per case,
   cross-jumping merges the three tails into the listing's
   `lbu` / `addiu 1` / `sb` ladder, which is how states 1 and 2 can reach it
   with +2 and +4 and state 5 with the +0x96 store already done.

   D_80082EB0 IS READ AS A STRUCT MEMBER HERE, AND IT IS WORTH THREE WORDS.
   As the file's plain `extern u8 *D_80082EB0` the target's two re-reads in
   state 5 do not appear: cse.c's `note_mem_written` leaves `all` clear for a
   store through `(plus (reg) (const_int))`, so the fixed-address scalar
   survives the +0xD0 store and the `[3] |= 0x20` read is forwarded (2 words,
   with the target's `nop` after it), and sched.c's `true_dependence` excuses
   a fixed-address load against a varying-address store unless exactly one of
   them is a struct reference, so the load after `D_800E3D74[...] = 0` is
   hoisted into the `lh`'s load-delay slot (1 word).  Measured: plain, 397
   words / 70 differing; `(*(u8 *volatile *)&D_80082EB0)[3]` at the one site
   restores the reload only, 399 / 70; the one-member struct alias restores
   both, 400 / 48.  Same lever as docs/matching-levers.md's "A FIXED-ADDRESS
   SCALAR DOES NOT CONFLICT WITH A VARYING-ADDRESS STRUCT".

   THE +0x1C FLAG IS A STRUCT MEMBER FOR THE SAME REASON, worth the last 9
   differing.  As `*(s32 *)(q2 + 0x1C) |= 0x2000000;` the store is not
   MEM_IN_STRUCT_P, so true_dependence refuses to let the fixed-address
   `lh %lo(D_800DD262)` of the next statement move across it and the index
   arithmetic cannot fill the store's slots.  `((Unk42710 *)q2)->unk_1C` is
   the same instruction and 0 differing.

   ONE `h` LOCAL FOR BOTH func_800990FC RESULTS, and that is the function's
   register allocation.  With a local per case the state-5 one is 4 refs over
   29 insns, does NOT conflict with `d`, and shares $s1 with it; the state-5
   `%hi(D_80082EB0)` (8 refs / 132 insns, priority 3*8/132 = 0.182 against
   `d`'s 4*18/308 = 0.234) is then allocated after `d`, takes $s3, and the
   `%hi(D_800DD262)` is pushed to a SIXTH callee-saved register -- 400 words
   of 398, 48 differing, all of them renames.  One local spans state 2 as
   well, where it DOES conflict with `d`; `d` loses $s1 to the `%hi`, and the
   `%hi(D_800DD262)` then shares `d`'s $s3 exactly as the target does.
   398 words, 9 differing.

   THREE SPLIT STATEMENTS, ONE WORD EACH, all the same shape -- a global
   store wants to sit between a load and its use:
   `fl = *(u16 *)(c + 0x14); D_800DD260 = 0; *(u16 *)(c + 0x14) = fl | 0x200;`
   (either order of a plain `|= 0x200` and the `sb` leaves the `lhu`'s
   load-delay slot a `nop`), the same split for the +0xAC word against
   `D_800DD262 =`, and `r = *(u8 **)(d + 0x60);` ahead of `D_800DD260 = 1;`
   so that the `sb` is the insn in front of the `jal` reorg takes its delay
   slot from.  Inlined, func_800A31D0's call gets a `nop`.

   `k = h; if (D_800DD260 == 0) { k = func_8009929C(8, h); }` and not a
   ternary: the ternary builds both arms with a `j` and a second copy, 3
   words. */
void func_8009345C(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u8 *q;
    u8 *q2;
    u8 *r;
    s32 fl;
    s32 v;
    u8 *h;
    u8 *t;
    u8 *t2;
    u8 *w;
    u8 *g;
    u8 *k;
    s16 n;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            t = D_800DD138;
            *(u8 **)(c + 0x2C) = t;
            func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            func_80093C70(p, b, c);
            func_80093D8C(p, b, c);
            fl = *(u16 *)(c + 0x14);
            D_800DD260 = 0;
            *(u16 *)(c + 0x14) = fl | 0x200;
            *(s16 *)(p + 0x96) = 0x10;
            v = *(s32 *)(p + *(s16 *)(d + 0x8A) * 4 + 0xAC);
            D_800DD262 = *(u16 *)(d + 0x8A);
            *(s32 *)(d + 0x60) = v;
            p[0x9B]++;
        }
        break;
    case 1:
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0)
        {
            if (*(s32 *)(d + 0x60) != 0)
            {
                func_80093E74(p, b, c, d);
                p[0x9B]++;
            }
            else
            {
                p[0x9B] += 2;
            }
        }
        break;
    case 2:
        if ((*(s32 *)(*(u8 **)(d + 0x60) + 0x1C) & 0x800000) != 0)
        {
            h = func_800990FC();
            func_80099290(func_80099194(
                D_800E06FA, func_80099734(*(u8 **)(d + 0x60),
                                          func_80099194(D_800DD262 ? D_800E05F0 : D_800E05E1,
                                                        func_8009929C(8, h)))));
            func_800A5720(h);
            r = *(u8 **)(d + 0x60);
            D_800DD260 = 1;
            func_800A31D0(r);
            *(s32 *)(p + D_800DD262 * 4 + 0xAC) = 0;
            func_800A18E8((*(u8 **)(d + 0x60))[0x13], 3);
            func_8009A028(*(u8 **)(d + 0x60));
            *(u16 *)(*(u8 **)(d + 0x60) - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            if (D_80082EB0_m.unk_00 == NULL)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] += 4;
            }
            else
            {
                p[0x9B]++;
            }
        }
        break;
    case 3:
        if ((s16)func_800A2BDC(NULL) == 0)
        {
            q = func_800A1618(*D_80082EB0_m.unk_00, 3);
            if (q != NULL)
            {
                D_80081488 = (u32)func_80048118(*D_80082EB0_m.unk_00, D_8008149C);
                func_800A0B94(*D_80082EB0_m.unk_00, q, 0);
                D_800E3E40 = 0;
                func_8003E4FC(0xFF, D_8003E140, (s32)&D_800E3E40);
            }
            p[0x9B]++;
        }
        break;
    case 4:
        if (D_800E3E40 != 0)
        {
            if (D_80081488 != 0)
            {
                func_80047FF4(*D_80082EB0_m.unk_00, D_80081488);
            }
            p[0x9B]++;
        }
        break;
    case 5:
        q2 = func_800A1618(*D_80082EB0_m.unk_00, 3);
        if (q2 != NULL)
        {
            func_80043914(D_80082EB0_m.unk_00);
            q2 = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(*D_80082EB0_m.unk_00, q2, 1))(
                1, *(s8 *)(d + 0x72), *(s8 *)(d + 0x73), (s16)(*(u16 *)(d + 0x88) - 0x20));
            if (q2 != NULL)
            {
                *(s32 *)(p + D_800DD262 * 4 + 0xAC) = (s32)q2;
                D_800E3DF0[D_80082EB0_m.unk_00[3] & 0x1F] = (s32)q2;
                w = *(u8 **)(q2 - 0x14);
                *(s16 *)(w + 0x1E) = 0x800;
                *(s16 *)(w + 0x1C) = 0x800;
                w = *(u8 **)(q2 - 0x18);
                *(s16 *)(w + 0x2) = *(u16 *)(b + 0x2);
                *(s16 *)(w + 0x6) = *(u16 *)(b + 0x6);
                *(s16 *)(w + 0xA) = *(u16 *)(b + 0xA) - 0x10;
                func_80042640((Unk42710 *)q2, q2[0x13]);
                func_800424E0(q2, q2[0x13], D_80082EB0_m.unk_00);
                ((Unk42710 *)q2)->unk_1C |= 0x2000000;
                *(s32 *)(p + D_800DD262 * 4 + 0xD0) = (s32)D_80082EB0_m.unk_00;
                D_80082EB0_m.unk_00[3] |= 0x20;
                h = func_800990FC();
                k = h;
                if (D_800DD260 == 0)
                {
                    k = func_8009929C(8, h);
                }
                func_80099290(func_80099194(
                    D_800E0714, func_80099734(q2, func_80099194(D_800DD262 ? D_800E05F0
                                                                           : D_800E05E1,
                                                                k))));
                func_800A5720(h);
                D_800E3D74[D_800DD262] = 0;
                func_800A152C(*D_80082EB0_m.unk_00, 3);
                func_80042560((s8 *)q2);
                func_800A56E0(0x704);
            }
        }
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B]++;
        break;
    case 6:
        p[0x9B]++;
        break;
    case 7:
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0)
        {
            t2 = D_800DD140;
            *(u8 **)(c + 0x2C) = t2;
            func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 8:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0x2) |= 0x812;
            *(u16 *)(c + 0x14) &= 0xFDFF;
            p[0x9B]++;
        }
        break;
    case 9:
        break;
    }
}
