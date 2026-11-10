#include "common.h"
#include "main.h"
#include "strings.h"

/* cardslot.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 172..205. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine". */

void func_800261C0(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_800261F0(s32 *p)
{
    func_800261C0(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_800265B8;
    }
}

void func_80026240(s32 *p)
{
    func_80026920(p - 8);
    func_80027BF4();
    func_80040AA0(3);
}

void func_80026270(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_80024298((s32 *)p[3]);
        p++;
    } while (++i < 5);
}

/* The `p[-4] = ...` store is written out in all three arms, as in the twin
   func_80025230, and gcc's cross-jumping merges them into the tail: that is
   where the target's `sw $v0, -0x10($s0)` in recheck_card_slot's delay slot
   comes from. Spelling the merge by hand -- one `s32 h` assigned in each arm and
   stored once after the if -- lands 55 words with 3 DIFF, because then the
   `p[13]` store and the `p - 8` argument are the last two insns of the fail
   block and sched2, which is a REVERSE list scheduler whose only live
   tiebreak on r3000 is descending INSN_LUID (every latency is 1, so every
   priority is 1), puts the argument in the jal's delay slot instead of the
   store. Measured on that spelling: all six permutations of the fail block's
   three statements (parked order 55w/3, next best 56w/44, anything with the
   `h` assignment before the call 57-58w), the argument in a local at block
   and at function scope (both 55w/3), `(s32)p - 32` and `(s32)&p[-8]` as the
   argument (both 55w/3), and the store re-based on the argument local as
   `q[21]` with `q = p - 8`, which fixes the order and leaves only the base
   register wrong (55w/1). */
void func_800262BC(s32 *p)
{
    init_save_file_header((u8 *)0x80010000, p[11], p[9]);
    if (write_save_file(p[11], (u8 *)0x80010000) == 0)
    {
        p[13] = (s32)func_800265B8;
        msg_overwrite_failed((s32)(p - 8));
        p[-4] = (s32)func_800261C0;
    }
    else
    {
        func_80024F3C((s32 *)p[1], p[11], 3);
        func_80026270((s32 **)p);
        func_800241D4((s32 *)p[p[11] + 3], 1);
        if (((s16 *)0x80010000)[0x104] != 0)
        {
            func_800A6104();
            p[-4] = (s32)func_80026240;
        }
        else
        {
            func_80053DA8(0x503);
            p[-4] = (s32)func_800265B8;
        }
    }
    recheck_card_slot();
    p[16] = 0;
}

void func_80026398(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_800262BC;
    }
}

void func_800263C0(s32 *p)
{
    p[15] = 1;
    msg_saving((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80026398;
}

void func_80026410(s32 *p)
{
    if (p[14] != 0)
    {
        p[-4] = (s32)func_800265B8;
    }
    else
    {
        func_800263C0(p);
    }
}

void func_8002644C(s32 *p)
{
    p[13] = (s32)func_80026410;
    msg_overwrite_confirm((s32)(p - 8), (s32)(p + 14));
    p[-4] = (s32)func_800261F0;
}

void func_80026494(s32 *p)
{
    p[13] = (s32)func_800265B8;
    msg_not_enough_blocks_overwrite((s32)(p - 8));
    p[-4] = (s32)func_800261F0;
}

void func_800264D8(s32 *p)
{
    p[13] = (s32)func_800265B8;
    msg_not_enough_blocks((s32)(p - 8));
    p[-4] = (s32)func_800261F0;
}

void func_8002651C(s32 *p)
{
    if (D_80083E98[p[11]][0] != 0)
    {
        p[-4] = (s32)func_8002644C;
    }
    else if (get_free_blocks() >= 3)
    {
        p[-4] = (s32)func_800263C0;
    }
    else if (count_used_save_slots() != 0)
    {
        func_80026494(p);
    }
    else
    {
        func_800264D8(p);
    }
}

/* The card-list input handler. Three things are load-bearing:

   The `p[12] >= 0xD` arm has to come FIRST, with the plain increment as the
   `else`: the target's `slti`/`bnez` jumps forward to the increment, so the
   increment is the block placed after the chain, and writing it the natural
   way round costs a `j` over it.

   The two exit tests must be two separate `if`s. As `r == 0 || r == 1` gcc
   folds them into `sltiu $s0, $s0, 2`, where the target keeps `beqz` plus a
   `beq` against a materialised 1.

   The 0x20 case leaves through a `goto` into the tail -- it does NOT fall
   into the func_8002168C/print_card_status pair the way every other path does.

   D_80083160 is the Unk80083160 struct from include/structs.h, read as
   .unk_08 and .unk_10; the same pair appears in func_800277BC. */
void func_800265B8(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            goto close;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_8002651C(p);
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        if (dir != 0)
        {
            func_80053DA8(0x502);
            p[11] = func_80049DE8(p[11], dir, 5);
            func_80025DC8(p);
        }
    }
    r = func_8002168C();
    print_card_status();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
close:
    func_80026920(p - 8);
    func_80027AFC(p[8], p[9]);
}

void func_80026720(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80026728(s32 *p, s32 v)
{
    func_80026720(p, v);
}

void func_80026748(s32 *p)
{
    func_80025DC8(p);
    recheck_card_slot();
    p[-4] = (s32)func_800265B8;
}

void func_80026784(s32 *p)
{
    func_80026920(p - 8);
    func_80027AFC(p[8], p[9]);
}

void func_800267C0(s32 *p)
{
    s32 n = p[10];

    if (n < MEM_CARD_FILE_ID_TMP)
    {
        if (load_save_slot_info(n) != 0)
        {
            p[n + 3] = func_80023FF0((s32)(p - 8), n);
            p[10] = p[10] + 1;
        }
        else
        {
            p[13] = (s32)func_80026784;
            msg_load_failed((s32)(p - 8));
            p[-4] = (s32)func_800261C0;
        }
    }
    if (n == MEM_CARD_FILE_ID_TMP)
    {
        p[-4] = (s32)func_80026748;
    }
}

s32 *func_80026868(s32 *p, s32 a)
{
    s32 *q = func_8003FC64(0);
    s32 *r;

    if (q != 0)
    {
        r = q + 8;
        r[9] = a;
        func_80026728(r, (s32)p);
        erase_tmp_file();
        refresh_card_info();
        q[8] = func_8004F418(q, 0);
        r[1] = (s32)func_80024D58(q, r[11], 3);
        r[2] = (s32)func_80027364(q);
        r[10] = 0;
        q[4] = (s32)func_800267C0;
    }
    return q;
}

/* Same destructor family as func_80025D34, which is still a near miss; this
   one lands because only the last of its four calls is inside the loop, so
   there is no delay-slot nop to account for. gcc folds the 0xC into the
   memory displacement and strength-reduces the index to the q cursor, so
   `q[3]` with `q++` and `q[3 + i]` in a `for` both give the same 37 words. */
void func_80026920(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_8004F52C(q[0]);
        func_80024EEC((s32 *)q[1]);
        func_80027458((s32 *)q[2]);
        i = 0;
        do
        {
            func_80024184((s32 *)q[3]);
            q++;
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Two slots, each selected through the D_80028068 pick table. Four spellings
   were load-bearing here, and three of them are about the ORDER gcc emits a
   two-register `addu` in:

   - D_80028068 has to be declared `[][2]` and indexed `[p[9]][i]`. As a flat
     `u8 []` with `[p[9] * 2 + i]` gcc adds `i` before the array base, which
     is six words wrong across the three evaluations.
   - The func_8004CBFC pointer needs its offset parenthesised,
     `(u8 *)p + (k * 12 + 4)`, so the `+ 4` lands in the scaled value rather
     than after the base is added.
   - The tested pointer has to come off a BYTE cursor whose scale is already
     applied (`(u8 *)p + k * 4`, then `0x68` as the displacement). Written
     `p[26 + k]` the offset is still a shift when the address is formed, and
     gcc canonicalises `(plus (mult ...) (reg))` with the shift first, giving
     `addu $v0, $v0, $s2` where the target has `addu $v0, $s2, $v0`. Four
     base-first spellings of the same index all leave that one word wrong.

   func_8004CBFC takes THREE arguments (see csrc/unk_1C540.c:2791); the
   declaration here had two, and the third is this same pointer, which is why
   the target loads it into $a2 before the test. */
void func_800269B4(s32 *p)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        s32 k = D_80028068[p[9]][i];
        u8 *q = (u8 *)p + k * 4;
        u32 *e = *(u32 **)(q + 0x68);

        if (e[0] != 0)
        {
            func_8004CBFC((u32 *)((u8 *)p + (k * 12 + 4)), D_80028074[i], e);
        }
        ((UnkCardGrp044 *)p)->ptrs[i]->unk_04->unk_00 = D_8002806C[D_80028068[p[9]][i]];
        ((UnkCardGrp044 *)p)->ptrs[2 + i]->unk_04->unk_00 = D_8002806C[D_80028068[p[9]][i]];
    }
}

void func_80026AE4(s32 *p)
{
    func_8004CB2C(p + 1);
    func_8004CB2C(p + 4);
}

/* The two 0xC-stride induction variables are kept apart by WHERE the constant
   offset is parenthesised, the same lever as func_800269B4 above. The call's
   pointer takes `(i * 0xC + 4)` as one scaled value, so loop.c reduces it to a
   giv with add_val 4 and adds `p` at the use (`addu $a0, $s7, $s3`); the
   store's address leaves `+ 8` outside, so its giv is `p + i * 0xC` with
   add_val `p` and 8 as the displacement. Two givs whose add_vals are a
   register and a constant cannot differ by a constant, so combine_givs_p
   declines to merge them -- write both offsets the same way and gcc folds them
   into a single giv, which is two words short and also hoists
   `%hi(D_80027E68)` out of the loop, taking $s7 away from `p`. */
void func_80026B18(u8 *p, s32 *b, s32 *c)
{
    s32 any = 0;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (c[i] != 0)
        {
            UnkCardPair *q = ((UnkCardGrp044 *)p)->ptrs[2 + i];

            any = 1;
            if (b[i] != 0)
            {
                func_8004CBFC((u32 *)(p + (i * 0xC + 4)), (u8 *)D_80027E68, (u32 *)q);
            }
            else
            {
                q->unk_00 = 0;
                *(s32 *)(p + i * 0xC + 8) = 0;
            }
        }
    }
    if (any != 0)
    {
        func_800269B4((s32 *)p);
    }
}

void func_80026C00(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -120;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

void func_80026C94(UnkCardGrp044 *c)
{
    UnkCardPair *q;

    q = c->ptrs[0];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80028418;
    q = c->ptrs[1];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80028430;
}

/* func_8004CBFC takes THREE arguments (csrc/unk_1C540.c:2791) and the third
   is the UnkCardPair this function just wrote through -- which is why the
   target loads it into $a2, uses $a2 for the two field stores, and then
   leaves it there for the call. With two arguments the pointer goes to a
   different register and $s0/$s1 come out swapped for the whole function.
   Same discovery as func_800269B4, from the other side. */
void func_80026CF0(UnkCardGrp044 *c)
{
    UnkCardPair *q;

    q = c->ptrs[2];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    func_8004CBFC((u32 *)((s32 *)c + 1), (u8 *)D_80027E68, (u32 *)q);
    q = c->ptrs[3];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    func_8004CBFC((u32 *)((s32 *)c + 4), (u8 *)D_80027E68, (u32 *)q);
}

void func_80026D78(UnkCardGrp044 *c)
{
    func_80026C94(c);
    func_80026CF0(c);
    func_80026B18((u8 *)c, (s32 *)D_8002807C, (s32 *)D_80028084);
    func_800269B4((s32 *)c);
}

s32 func_80026DC8(s32 a)
{
    return func_800491F4(a, D_8002808C, 4);
}

void func_80026DF0(UnkCardGrp044 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_80026E90(UnkCardGrp044 *c, s32 v)
{
    *(s32 *)c = v;
    func_80026C00(&c->root, c->root_prim, c->root_shared);
    func_80026D78(c);
}

s32 *func_80026ED0(s32 *p, s32 a)
{
    s32 *q = (s32 *)D_8002B850;
    s32 *r = q + 8;
    UnkCardGrp044 *c;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002B850, 0x45);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80026FB4((u16 *)q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0xF0);
    }
    c = (UnkCardGrp044 *)r;
    func_80026DF0(c, 4);
    q[3] = (s32)&c->root;
    c->root.unk_0C = func_80026DC8((s32)c->ptrs);
    r[9] = a;
    func_80026E90(c, (s32)p);
    q[4] = (s32)func_80026AE4;
    return q;
}

void func_80026FB4(u16 *p)
{
    p[15] |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80026FD4(s32 *p, s32 v)
{
    s32 *q = p + 8;

    q[9] = v;
    func_800269B4(q);
}

void func_80026FF8(s32 *p, s32 *b, s32 *c)
{
    func_80026B18((u8 *)(p + 8), b, c);
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S .data, moved here from csrc/ovl_main_80020000.c on 2026-11-10.
   The overlay's first data run, 0x80027E20..0x800280B4, partitions over the
   nine units with zero order violations; docs/overlays.md, "Rodata is per
   unit; so is data", has the argument. Definition order is address order, and
   the block sits at the END of the file on purpose: every function above it
   reaches these through main.h's declaration, and a definition earlier in the
   file would complete the type at those sites and could move codegen.
   --------------------------------------------------------------------------- */

/* 0x80028068 */
/* The two-slot pick table, indexed [p[9]][i] in func_800269B4. */
u8 D_80028068[2][2] = {{0x01, 0x00},
                       {0x00, 0x01}};

/* 0x8002806C */
/* The two colours those two slots select. */
s32 D_8002806C[2] = {
    0x00404040,
    0x00808080,
};

/* 0x80028074 */
/* The two animation scripts those two slots select. */
u8 *D_80028074[2] = {
    (u8 *)D_80027E68,
    (u8 *)D_80027E20,
};

/* 0x8002807C */
/* Read as `(s32 *)` by func_80026D78 -- a pair of zeros. */
u8 D_8002807C[0x8] = {0};

/* 0x80028084 */
/* The matching pair of ones. */
u8 D_80028084[0x8] = {0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

/* 0x8002808C */
/* func_800491F4 index list for func_80026DC8, 4 entries then the 0x04
   terminator; three more bytes follow it inside the object. */
u8 D_8002808C[0x8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x00, 0x0C};
