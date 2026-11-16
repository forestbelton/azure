/* PARKED 2026-11-16, j50.  target 357 words, candidate 357 words,
   21 differing (ovlverify masked); aligndiff 21 aligned differences.
   WORD COUNT AND SHAPE ARE EXACT.  The whole residue is ONE register
   decision, twice: the local `a` lands in $t0 where the target keeps it in
   $v0 -- the same register as the sign-extended byte -- and because mine
   writes a different register the WAR dependence that pins the target's
   `subu $v0, $v0, $v1` AFTER the two `sh` stores is absent, so sched2 lifts
   my `subu` above them and reorg then fills the `j`'s delay slot with a `sh`
   instead.  Target arm:  negu $v0 / sh 0x80 / sh 0x70 / j / subu $v0,$v0,$v1.
   Mine:                  negu $v0 / subu $t0 / sh 0x70 / j / sh 0x80.
   Diagnosis: combine propagates the SImode sra result into the two `sh`
   stores, which kills the HImode set of `a`, so `a` is set only by the
   subtraction and allocates independently.  The last two differing words are
   the ripple: my two tail `sh`es land after the RotTransPers4 argument
   `addiu`s instead of before them.

   DECLARATIONS THIS BODY NEEDS: the UnkDungeonSP9 typedef and the
   `void func_800BC4D4(UnkVec *, UnkSlot *, s16, s32);` forward declaration
   (func_800BC3E4 at csrc/ovl_dungeon.c:26313 calls it on an implicit one
   today), both in the agent's report.

   MEASURED AND REJECTED (all at 357 words unless stated):
     s16 arg3                                     359 words (+2 sll/sra)
     tail stores written inside BOTH arms         361 words (+4, no cross-jump)
     (p->unk_14.w + p->unk_0C.w) << 8             keeps unk_0C live in a 2nd
                                                  register; target reloads it
     q/unk_20 read after the zero stores          23 -> the prologue shifts 7
     plain `a = X; p->unk_80 = a; p->unk_70 = a;` 23
     p->unk_70 = p->unk_80 = a = X                23
     a -= p->unk_10.h                             23
     a = a - (s16)p->unk_10.h                     23
     a = (s16)(a - p->unk_10.h)                   23
     a = e[0x2]; a = -(s8)a;  (s16)               31
     s32 a                                        349 words (byte folds to lb)
     s32 a + volatile u8 *e                       361 words (+4 andi 0xFF on
                                                  the four widened byte reads)
     s32 a + (s8)(*(volatile u8 *)(e + 2))        357 / 42 -- sign blocks are
                                                  EXACT, but loop.c reverts the
                                                  dest_addr giv substitution on
                                                  a volatile MEM and $s2/$s3/$s4
                                                  permute (see the catalogue's
                                                  "volatile IS ONE CAUSE" entry)
     s32 a + s16 b intermediate                   365 words
     separate s16 a / s16 b per sign block        21 (same)
     a declared at function scope                 21 (same)
     chained tail p->unk_88 = p->unk_78 = a       21 (same) */
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
                s16 a;

                p->unk_08.w = e[0x8];
                p->unk_0C.w = e[0x9];
                p->unk_10.w = e[0xA];
                p->unk_14.w = e[0xB];
                func_80067EF4(q, 0, 0);
                AddPrim(p->unk_20 + p->unk_C0 * 4, q);
                q += 0xC;
                if ((e[0x0] ^ p->unk_24) & 1)
                {
                    p->unk_80 = p->unk_70 = a = -(s8)e[0x2];
                    a = a - p->unk_10.h;
                }
                else
                {
                    p->unk_80 = p->unk_70 = a = (s8)e[0x2];
                    a = a + p->unk_10.h;
                }
                p->unk_88 = a;
                p->unk_78 = a;
                if ((e[0x0] ^ p->unk_24) & 2)
                {
                    p->unk_7A = p->unk_72 = a = -(s8)e[0x3];
                    a = a - p->unk_14.h;
                }
                else
                {
                    p->unk_7A = p->unk_72 = a = (s8)e[0x3];
                    a = a + p->unk_14.h;
                }
                p->unk_8A = a;
                p->unk_82 = a;
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
