/* PARKED 2026-11-16, j50.  target 358 words, candidate 358 words,
   228 differing (ovlverify masked); aligndiff: 165 of the 220 rows are
   REGISTER-NAME-ONLY, every instruction is positionally aligned, and the
   whole residue is ONE callee-saved permutation plus its scheduling ripple.

   mine:   $s0=e  $s1=s  $s2=p  $s3=flag $s4=sp $s5=q  $s6=b $s7=&D_80083160
   target: $s0=q  $s1=e  $s2=s  $s3=flag $s4=p  $s5=sp $s6=b $s7=&D_80083160

   i.e. the target gives `q` -- the 0x8D0 primitive cursor -- the TOP allocno
   priority and everything else shifts down one; in mine `q` comes sixth and
   e/s/p take $s0..$s2.  Nothing I measured moves it (see below).  The four
   remaining non-register clusters are all downstream of that: the two sign
   blocks' `addu` is scheduled before its two `sh`es instead of between them
   (the same $t0-vs-$v0 pathology as func_800BC4D4's park); the
   `*(u16 *)(r + 0x16) |= 0x100` pair lands after the +0x25 byte update
   instead of between the two; the POLY_F4 copy loads three source words
   before the first store where the target loads four (the target's base is
   `$s1 = r + 0x28` with NEGATIVE displacements, which only exists because of
   the register assignment); and the second AddPrim passes `q` where the
   target rematerialises `r - 0x28`.

   DECLARATIONS THIS BODY NEEDS: the UnkDungeonSPB typedef (in the agent's
   report) and a PROTOTYPE CHANGE -- csrc/ovl_dungeon.c declares
   `void func_8009E798();`, but the function returns 0 and takes three
   arguments, so that line has to become
   `s32 func_8009E798(u8 *, UnkVec *, UnkSlot *);`.  Its only use is
   `func_8004491C(n, (u32)func_8009E798);`, which is unaffected.

   MEASURED AND REJECTED:
     rc.y written as `*(u16 *)(p + 0x6) + 0xE0` in the arm      361 words
       (the three stores to the address-taken `rc` invalidate cse's entry and
       the load comes back with its own `nop`); an `ry` local fixes it, but
       `ry` must be assigned AFTER `rc.x`, not before, or its load fills the
       first `lhu`'s delay slot and the function is 357.
     `sp->unk_24 = s->unk_14;` before `e = s->unk_08;`          359 / 309
       -- THE SINGLE BIGGEST LEVER HERE, worth a word and 81 differing: with
       `e` first the `lw` fills the `lhu 0x14`'s load delay and PushMatrix's
       delay slot takes the `sh 0x24`; the other way round the `sh` is
       emitted before the call and `q += 0xC` -- a callee-saved pseudo add,
       which sched2 may move freely across a call -- takes the delay slot.
     the four `unk_8C/84/7C/74 = 0` before the sign blocks      same count
     s32 a                                                      356 words
       (the byte reads fold to `lb`)
     chained `sp->unk_80 = sp->unk_70 = a = (s8)e[0x2];`        228 (no change)
     ditto with a chained tail                                  228 (no change)
     a `z = r + 0x28` local for the POLY_F4                     362 words
     one rolling `c` cursor for the four tail primitives        361 words
     second AddPrim written `r - 0x28`                          228 (no change)
     dropping the `base` local                                  228 (no change)
     declaration orders q/e/r, e/q/r, q/r/e                     228 (no change)
     `q += 0xC` moved later, or just before the e[] reads       359 words */
s32 func_8009E798(u8 *p, UnkVec *b, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPB *sp = (UnkDungeonSPB *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    u8 *q;
    u8 *r;
    u8 *e;
    s32 flag;
    s32 ry;
    s16 a;
    RECT rc;

    q = *(u8 **)(base + 0x8D0);
    sp->unk_20 = base + 0xB0;
    sp->unk_38 = 0x1000;
    flag = base != D_801C9E40;
    sp->unk_48 = 0;
    func_80067E2C((DR_AREA *)q, (RECT *)base);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, q);
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_00 = *(u16 *)((u8 *)b + 0x2);
    sp->unk_02 = *(u16 *)((u8 *)b + 0x6);
    e = (u8 *)s->unk_08;
    sp->unk_24 = s->unk_14;
    PushMatrix();
    sp->unk_98 = 0;
    sp->unk_9A = 0;
    sp->unk_9C = g->unk_C8;
    RotMatrix((SVECTOR *)0x1F800098, (MATRIX *)0x1F8000D0);
    ScaleMatrix((MATRIX *)0x1F8000D0, (VECTOR *)0x1F800030);
    sp->unk_E4 = sp->unk_00;
    sp->unk_E8 = (s16)sp->unk_02;
    sp->unk_EC = 0;
    func_80064CF0((MATRIX *)0x1F8000D0);
    func_80064D80((MATRIX *)0x1F8000D0);
    sp->unk_28 = 0x20 - s->unk_20;
    sp->unk_2A = 0x20 - s->unk_22;
    sp->unk_2C = 0;
    func_80065320((SVECTOR *)0x1F800028, (s32 *)0x1F800100, (s32 *)0x1F800094);
    q += 0xC;
    sp->unk_100 = sp->unk_100 - sp->unk_00;
    sp->unk_102 = sp->unk_102 - sp->unk_02;
    sp->unk_08.w = e[0x8];
    sp->unk_0C.w = e[0x9];
    sp->unk_10.w = e[0xA];
    sp->unk_14.w = e[0xB];
    a = (s8)e[0x2];
    sp->unk_80 = a;
    sp->unk_70 = a;
    a = a + sp->unk_10.h;
    sp->unk_88 = a;
    sp->unk_78 = a;
    a = (s8)e[0x3];
    sp->unk_7A = a;
    sp->unk_72 = a;
    a = a + sp->unk_14.h;
    sp->unk_8A = a;
    sp->unk_82 = a;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    func_80065320((SVECTOR *)0x1F800070, (s32 *)(q + 0x8), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800078, (s32 *)(q + 0x10), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800080, (s32 *)(q + 0x18), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800088, (s32 *)(q + 0x20), (s32 *)0x1F800094);
    *(s16 *)(q + 0x8) = *(u16 *)(q + 0x8) + sp->unk_100;
    *(s16 *)(q + 0x10) = *(u16 *)(q + 0x10) + sp->unk_100;
    *(s16 *)(q + 0x18) = *(u16 *)(q + 0x18) + sp->unk_100;
    *(s16 *)(q + 0x20) = *(u16 *)(q + 0x20) + sp->unk_100;
    *(s16 *)(q + 0xA) = *(u16 *)(q + 0xA) + sp->unk_102;
    *(s16 *)(q + 0x12) = *(u16 *)(q + 0x12) + sp->unk_102;
    *(s16 *)(q + 0x1A) = *(u16 *)(q + 0x1A) + sp->unk_102;
    *(s16 *)(q + 0x22) = *(u16 *)(q + 0x22) + sp->unk_102;
    sp->unk_10.w = sp->unk_10.w + sp->unk_08.w;
    sp->unk_14.w = (sp->unk_14.w + sp->unk_0C.w) << 8;
    sp->unk_0C.w = sp->unk_0C.w << 8;
    *(s16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
    *(s16 *)(q + 0xC) = sp->unk_0C.h + sp->unk_08.h;
    *(u32 *)(q + 0x14) =
        sp->unk_0C.w + sp->unk_10.w + ((s->unk_10 | *(s16 *)(e + 0x4)) << 16);
    *(s16 *)(q + 0x1C) = sp->unk_14.h + sp->unk_08.h;
    *(s16 *)(q + 0x24) = sp->unk_14.h + sp->unk_10.h;
    *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
    func_800666F4((POLY_FT4 *)q);
    r = q + 0x28;
    func_8003DB6C((int *)r, (int *)q, 0xA);
    q[0x7] |= 2;
    r[0xD] = r[0x15] += 0x40;
    *(u16 *)(r + 0x16) |= 0x100;
    r[0x1D] = r[0x25] += 0x40;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, q);
    *(u32 *)(r + 0x2C) = 0x606060;
    r[0x2C] >>= 1;
    r[0x2D] >>= 1;
    r[0x2E] >>= 1;
    *(s32 *)(r + 0x30) = *(s32 *)(r + 0x8);
    *(s32 *)(r + 0x34) = *(s32 *)(r + 0x10);
    *(s32 *)(r + 0x38) = *(s32 *)(r + 0x18);
    *(s32 *)(r + 0x3C) = *(s32 *)(r + 0x20);
    func_800666E0((POLY_F4 *)(r + 0x28));
    r[0x2F] |= 2;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r + 0x28);
    func_80067F20((u32 *)(r + 0x40), 0, 0, 0x40, 0);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r + 0x40);
    rc.x = *(u16 *)(p + 0x4);
    ry = *(u16 *)(p + 0x6);
    rc.y = ry;
    rc.w = *(u16 *)(p + 0x8);
    rc.h = *(u16 *)(p + 0xA);
    if (flag != 0)
    {
        rc.y = ry + 0xE0;
    }
    func_80067E2C((DR_AREA *)(r + 0x4C), &rc);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r + 0x4C);
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x58);
    return 0;
}
