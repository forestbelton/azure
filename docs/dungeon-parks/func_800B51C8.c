/* PARK -- func_800B51C8, agent j59, 2026-11-16.  FRESH DECODE, no prior art
   (leverindex and a direct grep of docs/matching-levers.md both return
   nothing for this function).

   target 834 words, candidate 830 words, 262 aligned differences.
   THE BODY BELOW COMPILES AND MEASURES; the structure is believed right and
   the residue is mostly register assignment plus a handful of statement
   orderings.  It is a long way from 0 but it is a real starting point: four
   measured steps already took it from "no body at all" to this.

   WHAT IT IS.  The 32-slot overlay-sprite renderer.  It walks
   i = 0 .. 0x1F over the two parallel tables D_800E3648 (UnkDungeonSlot,
   4 bytes) and D_800E39C8 (UnkDungeonSpotB, 0x18 bytes) and, for each live
   slot, walks the slot's 0xC-byte part list at r->unk_08 emitting one
   POLY_FT4 per part into the scratchpad display list.

   ITS TEMPLATE IS THE LANDED func_800C6B40 (search csrc for
   `Unk453E0SP *p = (Unk453E0SP *)0x1F800000`).  That function is the same
   renderer for a node rather than a table slot: the 0x1F800070/78/80/88
   corner block, the 0x1F8000F0..FE projected-corner block, the four-corner
   `(u16)(x + 0x20) < 0x181 && (u16)(y + 0x20) < 0x121` visibility OR, the
   `p->unk_20 + p->unk_C0 * 4` AddPrim and the D_8006CD30.unk_1C store are
   all line-for-line the same idiom.  READ IT FIRST.

   PSY-Q IDENTIFICATIONS (all of these are called by their `func_` names in
   this unit already, and are used WITHOUT a declaration -- do not add
   prototypes, the landed func_800B7D74-era code calls func_8006658C bare):
     func_80064AE0(MATRIX *)                 saves the current matrix
     func_80065820(SVECTOR *, MATRIX *)      RotMatrix
     func_80064840(MATRIX *, MATRIX *, MATRIX *)   CompMatrix
     func_80064BC0(MATRIX *, VECTOR *)       ScaleMatrix
     func_80064D80(MATRIX *)                 SetTransMatrix
     func_80064CF0(MATRIX *)                 SetRotMatrix
     func_80065420(a,b,c,d)                  RotTransPers
     func_800654B0(10 args)                  RotTransPers4 -> 0x1F8000F0..FC
     func_80065590(10 args)                  RotTransPers4 straight into the
                                             primitive's 4 xy fields
     func_8006658C(ot, prim)                 AddPrim
     func_800666F4(prim)                     primitive init (sets code/len)

   PROTOTYPE CHANGE REQUIRED: the forward declaration at the top of the file
   is `void func_800B51C8();` and the function RETURNS 0
   (`addu $v0, $zero, $zero` at 800B5E94).  It must become
   `s32 func_800B51C8();` or the unit does not compile.  Its only reference
   is `func_8004491C(n, (u32)func_800B51C8);`, so nothing landed is affected.
   Its third parameter is the one it uses (UnkSlot *s, spilled to 0x90($sp));
   the first two are read only as the ABI requires them.

   FOUR MEASURED STEPS, in order, with their scores:

   (1) FRAME LAYOUT.  `MATRIX m; u16 cv[3]; VECTOR sv;` in that declaration
       order, giving m at sp+0x28, cv at sp+0x48 and sv at sp+0x50, frame
       0x88.  Three separate `u16 cx, cy, cz;` scalars instead get EIGHT
       bytes each (cx at 0x58, cy at 0x60) and the frame comes out 0x90:
       843 words / 327 differences against 846 / 267 with the array.
       `sv` is the ScaleMatrix VECTOR *and* the 0x58 scratch: the listing
       writes 0x58($sp) long before the ScaleMatrix call and then fills
       0x50/0x54 from it, so `sv.vz` is the scratch and
       `sv.vx = sv.vz; sv.vy = sv.vz / 2;` is the fill.

   (2) D_80083160 IS THE DECLARED STRUCT AT EVERY SITE, NOT A `g` POINTER
       LOCAL.  The listing recomputes `lui $t0 / addiu $t0, %lo` inside arm B
       and again in the tail, and loads unk_00 through its OWN
       `lui $v0 / lw %lo($v0)`.  With `Unk80083160 *g = &D_80083160;` the
       base takes a callee-saved register ($s8), which is one of the nine the
       scratchpad pointers need: 843 words / 327.

   (3) THE FOUR SCRATCHPAD CONSTANTS ARE POINTER LOCALS, assigned at the top
       in this order: 0x1F8000D0, 0x1F800050, 0x1F800090, 0x1F800094.  Each
       is a `lui`/`ori` pair in the target, held in $s6/$s4/$fp/$s7 for the
       whole function.  Written inline as `(void *)0x1F8000D0` at each call
       site: 846 words / 267.  As locals: 830 words / 262.
       THE NINE CALLEE-SAVED QUANTITIES THE TARGET WANTS ARE:
         $s0 primitive pointer   $s1 (Unk453E0SP *)0x1F800000
         $s2 part cursor (+4)    $s3 part cursor (+0)
         $s4 0x1F800050          $s5 the 0..0x1F slot index
         $s6 0x1F8000D0          $s7 0x1F800094      $fp 0x1F800090
       which is exactly nine, and it is why `s` is SPILLED to its incoming
       home slot 0x90($sp) and re-loaded with `lw $t0, 0x90($sp)` at each of
       its five uses.  The candidate still gives `s` $s8, so every arm is one
       register out of step -- THAT IS THE SINGLE BIGGEST REMAINING LEVER.

   (4) `*(u8 **)((u8 *)p + 0x18)` is the live primitive pointer: the
       scratchpad word at 0x1F800018 holds it across the whole function and
       it advances by 0x28 per quad.  structs.h's Unk453E0SP has `pad_18`
       there, so it has to be reached with the cast.  The tail writes it back
       to `*(u8 **)(D_80083160.unk_00 + 0x8D0)`.

   OTHER DECODED FACTS WORTH NOT RE-DERIVING:
     * p->unk_00/unk_02/unk_04 are read with `lhu` + `sll 16` + `sra 16`
       wherever a 32-bit value is wanted, i.e. as `(s16)*(u16 *)&p->unk_02`,
       even though structs.h declares them s16.  `p->unk_E4 = p->unk_00;`
       alone gives a plain `lh`, which is right for unk_00 and wrong for the
       other two.
     * `p->unk_100 = s->unk_16 + ((s16)cv[0] >> 1);` -- an ARITHMETIC SHIFT
       (`sra 17` fused with the lhu), not `/ 2`.  `sv.vy = sv.vz / 2;` in the
       shadow pass IS a divide (`srl 31 / addu / sra 1`).
     * The two arms are chosen by `r->unk_14 & 0x100`.  Arm A (set) builds
       the quad with func_80065590 straight into the primitive and gates on
       `(u32)p->unk_C0 < 0x1E0` AND the four-corner test; arm B (clear) does
       a RotTransPers first, `p->unk_C0 -= 4`, the same part loop with
       func_800654B0 into 0x1F8000F0, and then -- when the emitted code is
       not semi-transparent -- a SECOND shadow quad with its own
       func_800BCB04 height probe, a -3 OT bias and the 0x7FC00000 /
       `*(s16 *)c << 16` colour-and-tpage words written as 32-bit stores.
     * Both arms end at the shared `func_80064D80(&m); func_80064CF0(&m);`
       restore (label .L800B5E64), so it is written out in both.
     * The part cursor is TWO pointers 4 bytes apart: `e` reads e[0] for the
       0x20 skip and the sign test, `c = e + 4` reads c[-2], c[-1], c[0..7].
       Both step by 0xC.

   WHERE THE REMAINING 262 ROWS ARE: spread evenly over every block (5 rows
   in 800B51xx, 14 in 52xx, 13 in 53xx, 13 in 54xx, 2 in 55xx, 6 in 56xx,
   19 in 57xx, 5 in 58xx, 5 in 59xx, 10 in 5Axx, 5 in 5Bxx, 12 in 5Cxx,
   17 in 5Dxx, 16 in 5Exx, plus the insert/delete rows), which is the
   signature of the one-register-out-of-step problem in (3) rather than of
   wrong code.  FIX `s`'s SPILL FIRST.
*/
s32 func_800B51C8(u8 *arg0, UnkVec *v, UnkSlot *s)
{
    MATRIX m;
    u16 cv[3];
    VECTOR sv;
    Unk453E0SP *p = (Unk453E0SP *)0x1F800000;
    void *md = md;
    void *ms = ms;
    void *f0 = f0;
    void *f1 = f1;
    UnkDungeonSpotB *r;
    u8 *q;
    u8 *e;
    u8 *c;
    s32 i;
    s16 a;
    s16 b;

    p->unk_20 = D_80083160.unk_00 + 0xB0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    cv[0] = D_80083160.unk_C4;
    cv[1] = D_80083160.unk_C6;
    cv[2] = D_80083160.unk_C8;
    *(u8 **)((u8 *)p + 0x18) = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    func_80064AE0(&m);
    for (i = 0; i < 0x20; i++)
    {
        if ((D_800E3648_s[i].unk_03 & 0x80) != 0 && (D_800E296C & 8) == 0)
        {
            continue;
        }
        if (D_800E3648_s[i].unk_01 == 0)
        {
            continue;
        }
        if (D_800E3648_s[i].unk_00 == 0)
        {
            continue;
        }
        r = &D_800E39C8[i];
        if ((r->unk_14 & 0x40) != 0)
        {
            func_800478B8((u8 *)r);
        }
        p->unk_00 = (r->unk_06 << 6) + 0x20;
        p->unk_02 = (r->unk_07 << 6) + 0x20;
        p->unk_04 = r->unk_12;
        e = (u8 *)r->unk_08;
        if (e == NULL)
        {
            continue;
        }
        r->unk_10 = r->unk_12;
        if ((r->unk_14 & 0x100) != 0)
        {
            p->unk_30.w = 0x2000;
            p->unk_34.w = 0x2000;
            p->unk_38.w = 0x2000;
            p->unk_100 = 0;
            p->unk_102 = 0;
            p->unk_104 = 0;
            p->unk_E4 = p->unk_00;
            p->unk_E8 = (s16) * (u16 *)&p->unk_02;
            p->unk_EC = (s16) * (u16 *)&p->unk_04;
            c = e + 4;
            func_80065820(&p->unk_100, md);
            func_80064840(&m, md, ms);
            func_80064BC0(ms, &p->unk_30);
            func_80064D80(ms);
            func_80064CF0(ms);
            for (;;)
            {
                if ((e[0] & 0x20) == 0)
                {
                    p->unk_08.w = c[4];
                    p->unk_10.w = c[6];
                    if (c[4] + c[6] >= 0x100)
                    {
                        p->unk_10.w = c[6] - 1;
                    }
                    p->unk_0C.w = c[5];
                    p->unk_14.w = c[7];
                    if (c[5] + c[7] >= 0x100)
                    {
                        p->unk_14.w = c[7] - 1;
                    }
                    a = (s8)c[-2];
                    p->unk_80 = a;
                    p->unk_70 = a;
                    a = a + p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                    q = *(u8 **)((u8 *)p + 0x18);
                    *(u8 **)((u8 *)p + 0x18) = q + 0x28;
                    b = (s8)c[-1];
                    p->unk_7A = b;
                    p->unk_72 = b;
                    b = b + p->unk_14.h;
                    p->unk_8A = b;
                    p->unk_82 = b;
                    p->unk_C0 = func_80065590(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88,
                                              (void *)(q + 0x08), (void *)(q + 0x10),
                                              (void *)(q + 0x18), (void *)(q + 0x20),
                                              f0, f1);
                    if ((u32)p->unk_C0 < 0x1E0 &&
                        (((u16)(*(u16 *)(q + 0x08) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x0A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x18) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x1A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121)) != 0)
                    {
                        p->unk_10.w += p->unk_08.w;
                        p->unk_14.w += p->unk_0C.w;
                        p->unk_0C.w <<= 8;
                        p->unk_14.w <<= 8;
                        *(u16 *)(q + 0x0E) = *(u16 *)(c + 2);
                        *(u16 *)(q + 0x0C) = p->unk_0C.h + p->unk_08.h;
                        *(u16 *)(q + 0x14) = p->unk_0C.h + p->unk_10.h;
                        *(u16 *)(q + 0x16) = *(u16 *)c;
                        *(u16 *)(q + 0x1C) = p->unk_14.h + p->unk_08.h;
                        *(u16 *)(q + 0x24) = p->unk_14.h + p->unk_10.h;
                        *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
                        func_800666F4(q);
                        q[0x7] |= 2;
                        func_8006658C(p->unk_20 + p->unk_C0 * 4, q);
                    }
                }
                if (*(s8 *)e < 0)
                {
                    break;
                }
                c += 0xC;
                e += 0xC;
            }
            func_80064D80(&m);
            func_80064CF0(&m);
        }
        else
        {
            p->unk_E4 = 0;
            p->unk_E8 = 0;
            p->unk_EC = 0;
            p->unk_C0 = func_80065420(p, &p->unk_B8, &p->unk_90, &p->unk_94);
            D_8006CD30.unk_1C = p->unk_C0 * 4;
            p->unk_C0 = p->unk_C0 - 4;
            if ((u32)p->unk_C0 >= 0x1E0)
            {
                continue;
            }
            p->unk_B8 = p->unk_B8 - 0xA0;
            p->unk_BA = p->unk_BA - 0x78;
            p->unk_100 = s->unk_16 + ((s16)cv[0] >> 1);
            p->unk_104 = *(u16 *)((u8 *)&D_80083160 + 0xB8) + (s->unk_1A - cv[1]);
            c = e + 4;
            p->unk_102 = s->unk_18;
            func_80065820(&p->unk_100, md);
            func_80064840(&D_8006CD30, md, ms);
            func_80064D80(ms);
            func_80064CF0(ms);
            for (;;)
            {
                if ((e[0] & 0x20) == 0)
                {
                    p->unk_08.w = c[4];
                    p->unk_0C.w = c[5];
                    p->unk_10.w = c[6];
                    p->unk_14.w = c[7];
                    a = (s8)c[-2];
                    p->unk_80 = a;
                    p->unk_70 = a;
                    a = a + p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                    b = (s8)c[-1];
                    p->unk_7A = b;
                    p->unk_72 = b;
                    b = b + p->unk_14.h;
                    p->unk_8A = b;
                    p->unk_82 = b;
                    func_800654B0(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88, &p->unk_F0,
                                  &p->unk_F4, &p->unk_F8, &p->unk_FC, f0,
                                  f1);
                    q = *(u8 **)((u8 *)p + 0x18);
                    *(u8 **)((u8 *)p + 0x18) = q + 0x28;
                    *(u16 *)(q + 0x08) = p->unk_F0 + p->unk_B8;
                    *(u16 *)(q + 0x0A) = p->unk_F2 + p->unk_BA;
                    *(u16 *)(q + 0x10) = p->unk_F4 + p->unk_B8;
                    *(u16 *)(q + 0x12) = p->unk_F6 + p->unk_BA;
                    *(u16 *)(q + 0x18) = p->unk_F8 + p->unk_B8;
                    *(u16 *)(q + 0x1A) = p->unk_FA + p->unk_BA;
                    *(u16 *)(q + 0x20) = p->unk_FC + p->unk_B8;
                    *(u16 *)(q + 0x22) = p->unk_FE + p->unk_BA;
                    if ((((u16)(*(u16 *)(q + 0x08) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x0A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x18) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x1A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121)) != 0)
                    {
                        q[0x3] = 9;
                        p->unk_10.w = p->unk_10.w - 1 + p->unk_08.w;
                        if ((p->unk_10.w & 0x100) != 0)
                        {
                            p->unk_10.w -= 1;
                        }
                        p->unk_14.w = p->unk_14.w - 1 + p->unk_0C.w;
                        if ((p->unk_14.w & 0x100) != 0)
                        {
                            p->unk_14.w -= 1;
                        }
                        p->unk_0C.w <<= 8;
                        p->unk_14.w <<= 8;
                        *(u16 *)(q + 0x0C) = p->unk_0C.h + p->unk_08.h;
                        *(u16 *)(q + 0x0E) = *(u16 *)(c + 2);
                        *(u16 *)(q + 0x14) = p->unk_0C.h + p->unk_10.h;
                        *(u16 *)(q + 0x16) = *(u16 *)c;
                        *(u16 *)(q + 0x1C) = p->unk_14.h + p->unk_08.h;
                        *(u16 *)(q + 0x24) = p->unk_14.h + p->unk_10.h;
                        if (*(s16 *)(q + 0x20) < *(s16 *)(q + 0x8))
                        {
                            q[0x14] -= 1;
                            q[0x24] -= 1;
                        }
                        if (*(s16 *)(q + 0x22) < *(s16 *)(q + 0xA))
                        {
                            q[0x1D] -= 1;
                            q[0x25] -= 1;
                        }
                        *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
                        q[0x7] = 0x2C;
                        if ((D_800E296C & 8) != 0 && (D_800E3648_s[i].unk_03 & 0x80) != 0)
                        {
                            q[0x7] = 0x2E;
                        }
                        func_8006658C(p->unk_20 + p->unk_C0 * 4, q);
                        if ((q[0x7] & 2) == 0)
                        {
                            func_80064D80(&m);
                            func_80064CF0(&m);
                            sv.vz = (s16) * (u16 *)&p->unk_04 - 4;
                            p->unk_04 = func_800BCB04(*(u16 *)&p->unk_00, *(u16 *)&p->unk_02,
                                                      (s16)sv.vz);
                            p->unk_C0 = func_80065420(p, &p->unk_B8, &p->unk_90, &p->unk_94);
                            D_8006CD30.unk_1C = p->unk_C0 * 4;
                            p->unk_C0 = p->unk_C0 - 3;
                            p->unk_B8 = p->unk_B8 - 0xA0;
                            p->unk_BA = p->unk_BA - 0x78;
                            sv.vz = ((sv.vz - (s16) * (u16 *)&p->unk_04) << 5) + 0x1000;
                            if (sv.vz < 0)
                            {
                                sv.vz = 0;
                            }
                            p->unk_102 = 0;
                            sv.vx = sv.vz;
                            sv.vy = sv.vz / 2;
                            p->unk_100 = -(s16)cv[0];
                            p->unk_104 = *(u16 *)((u8 *)&D_80083160 + 0xB8) - cv[1];
                            func_80065820(&p->unk_100, md);
                            func_80064840(&D_8006CD30, md, ms);
                            func_80064BC0(ms, &sv);
                            func_80064D80(ms);
                            func_80064CF0(ms);
                            q = *(u8 **)((u8 *)p + 0x18);
                            *(u8 **)((u8 *)p + 0x18) = q + 0x28;
                            p->unk_78 = p->unk_78 + 6;
                            p->unk_70 = p->unk_70 + 6;
                            func_800654B0(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88,
                                          &p->unk_F0, &p->unk_F4, &p->unk_F8, &p->unk_FC,
                                          f0, f1);
                            *(u16 *)(q + 0x08) = p->unk_F0 + p->unk_B8;
                            *(u16 *)(q + 0x0A) = p->unk_F2 + p->unk_BA;
                            *(u16 *)(q + 0x10) = p->unk_F4 + p->unk_B8;
                            *(u16 *)(q + 0x12) = p->unk_F6 + p->unk_BA;
                            *(u16 *)(q + 0x18) = p->unk_F8 + p->unk_B8;
                            *(u16 *)(q + 0x1A) = p->unk_FA + p->unk_BA;
                            *(u16 *)(q + 0x20) = p->unk_FC + p->unk_B8;
                            q[0x3] = 9;
                            *(u16 *)(q + 0x22) = p->unk_FE + p->unk_BA;
                            *(u32 *)(q + 0x0C) = p->unk_0C.w + p->unk_08.w + 0x7FC00000;
                            *(u32 *)(q + 0x14) = p->unk_0C.w + p->unk_10.w + (*(s16 *)c << 16);
                            *(u16 *)(q + 0x1C) = p->unk_14.h + p->unk_08.h;
                            *(u16 *)(q + 0x24) = p->unk_14.h + p->unk_10.h;
                            if (*(s16 *)(q + 0x20) < *(s16 *)(q + 0x8))
                            {
                                q[0x14] -= 1;
                                q[0x24] -= 1;
                            }
                            if (*(s16 *)(q + 0x22) < *(s16 *)(q + 0xA))
                            {
                                q[0x1D] -= 1;
                                q[0x25] -= 1;
                            }
                            *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
                            q[0x7] = 0x2C;
                            func_8006658C(p->unk_20 + p->unk_C0 * 4, q);
                        }
                    }
                }
                if (*(s8 *)e < 0)
                {
                    break;
                }
                c += 0xC;
                e += 0xC;
            }
            func_80064D80(&m);
            func_80064CF0(&m);
        }
    }
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = *(u8 **)((u8 *)p + 0x18);
    return 0;
}
