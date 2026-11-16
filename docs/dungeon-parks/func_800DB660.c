/* ===== LANDED, NOT A PARK =====
   func_800DB660, 268 of 268 words, 0 differing, measured against
   csrc/ovl_dungeon.c as of 2026-11-16 by agent j47.  Here only so the body
   survives a /tmp cleanup.  Needs no new declarations and no prototype
   change.  Do not queue it for the permuter. */

/* One semi-transparent textured quad per step of a straight segment, for
   every node of the tag chain. The segment runs from the record's
   +0x08/+0x0A/+0x0C to its +0x10/+0x12/+0x14 in `*(s16 *)(p + 0x36)` steps;
   each step builds func_800DBA90's 0x20-byte matrix record, rotates the
   +0x26-scaled unit vector through it, widens the result by the +0x2E/+0x30
   half-extents into the four corners, projects them with RotTransPers4 and
   hangs the POLY_FT4 on the OT at the returned depth less 8. The colour at
   +0x18 steps down by 0x080808 per quad until it reaches 0x080807, and the
   two facing angles at +0x28/+0x22 advance by +0x2A/+0x24. 268 words,
   0 differing.

   THE THREE INTERPOLATED AXES ARE `d * i / n`, NOT ACCUMULATORS, AND THAT IS
   THREE WORDS. loop.c strength-reduces each product into a giv whose `= 0`
   lands in the loop PREHEADER -- after the `bltz` guard -- and whose `+= d`
   lands at the loop bottom, in the reverse order the three were created.
   Hand-written accumulators zeroed in the `for` init are 262 words of 268
   with 49 aligned differences: the init is then in the GUARD's own basic
   block, where sched1 spends the three `addu`s on load-delay slots that the
   target leaves as `nop`s, and the three givs' registers rotate. The copy
   chain `addu $s6, $zero, $zero` / `addu $s5, $s6` / `addu $s4, $s5` is what
   a giv init looks like after cse unifies three zeros.

   THE LOOP BOUND IS READ OFF `q`, NOT `p`. Both are the same pointer -- `q`
   is a plain copy and the two live in different callee-saved registers all
   the way through -- but cse substitutes `p` into the duplicated top test,
   which is why the guard is `lh $v1, 0x36($s2)` and the bottom test is
   `lh $v0, 0x36($s1)`. Spelling the bound on `p` is 1 differing, exactly
   that word.

   `ang += *(s16 *)(q + 0x2A)` GOES BEFORE THE PRIMITIVE FETCH. After it,
   the `lh`/`addu` pair falls into the RotTransPers4 call's delay slot and
   the `sw $v0, 0x24($sp)` that belongs there moves out: 268 words,
   21 aligned differences.

   D_80083160 IS A POINTER LOCAL AND REMATERIALISES. All nine callee-saved
   registers are taken ($s0..$s7 and $fp), so `g` gets none and reload emits
   its `lui`/`addiu` at each of the three uses, which is the shape
   func_800A7CC4's note records for the same symbol. The 0xFFFFFF addPrim
   mask is loop-hoisted into a pseudo and spilled to 0xA8($sp); the
   0xFF000000 half is rematerialised with a bare `lui`.

   The primitive cursor advances by 0x34, not sizeof(POLY_FT4): the stride is
   written as a byte count on the u32, the way this unit's other +0x8D0 users
   do it. `SVECTOR vec` and `u16 vec[3]` are byte-identical here, because
   every read of it feeds an `sh` and combine picks `lhu` either way. */
s32 func_800DB660(u8 *p)
{
    SVECTOR v[4];
    SVECTOR v2[4];
    SVECTOR vec;
    u8 rec[0x20];
    s32 w;
    Unk80083160 *g = &D_80083160;
    POLY_FT4 *t;
    UnkNode *n;
    u8 *q;
    s32 i;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 ang;
    s32 zz;
    s32 col;
    s32 z;

    while (1)
    {
        v[0].vx = v[1].vx = -*(s16 *)(p + 0x2E) >> 1;
        v[0].vz = v[2].vz = -*(s16 *)(p + 0x30) >> 1;
        v[2].vx = v[3].vx = v[0].vx + *(u16 *)(p + 0x2E);
        v[1].vz = v[3].vz = v[0].vz + *(u16 *)(p + 0x30);
        v[0].vy = v[1].vy = v[2].vy = v[3].vy = 0;
        i = 0;
        zz = *(s16 *)(p + 0x22);
        q = p;
        ang = *(s16 *)(p + 0x28);
        col = *(s32 *)(p + 0x18);
        dx = *(s16 *)(p + 0x10) - *(s16 *)(p + 0x8);
        dy = *(s16 *)(p + 0x12) - *(s16 *)(p + 0xA);
        dz = *(s16 *)(p + 0x14) - *(s16 *)(p + 0xC);
        for (; i <= *(s16 *)(q + 0x36); i++)
        {
            vec.vx = (func_800644B8(ang) * *(s16 *)(q + 0x26)) >> 12;
            vec.vy = vec.vz = 0;
            *(u8 **)rec = (u8 *)&vec;
            *(u8 **)(rec + 0x4) = (u8 *)&vec;
            *(s16 *)(rec + 0xC) = *(u16 *)(q + 0x2C);
            *(s16 *)(rec + 0x10) = *(u16 *)(q + 0x8) + dx * i / *(s16 *)(q + 0x36);
            *(s16 *)(rec + 0x12) = *(u16 *)(q + 0xA) + dy * i / *(s16 *)(q + 0x36);
            *(s16 *)(rec + 0x14) = *(u16 *)(q + 0xC) + dz * i / *(s16 *)(q + 0x36) +
                                   ((func_800644B8(zz) * *(s16 *)(q + 0x20)) >> 12);
            *(s16 *)(rec + 0x18) = 1;
            *(s16 *)(rec + 0x1A) = 0;
            func_800DBA90(rec);
            v2[0].vx = v2[1].vx = vec.vx - ((s16) * (u16 *)(q + 0x2E) >> 1);
            v2[2].vx = v2[3].vx = v2[0].vx + *(u16 *)(q + 0x2E);
            v2[0].vz = v2[2].vz = vec.vz - ((s16) * (u16 *)(q + 0x30) >> 1);
            v2[1].vz = v2[3].vz = v2[0].vz + *(u16 *)(q + 0x30);
            v2[0].vy = v2[1].vy = v2[2].vy = v2[3].vy = vec.vy;
            zz += *(s16 *)(q + 0x24);
            ang += *(s16 *)(q + 0x2A);
            t = *(POLY_FT4 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x34;
            z = RotTransPers4(&v2[0], &v2[1], &v2[2], &v2[3], (s32 *)&t->x0, (s32 *)&t->x1,
                              (s32 *)&t->x2, (s32 *)&t->x3, &w, &w) -
                8;
            if ((u32)z < 0x1E0)
            {
                *(u32 *)&t->r0 = col;
                SetPolyFT4(t);
                SetSemiTrans((u8 *)t, 1);
                t->tpage = *(u16 *)(q + 0x32);
                t->clut = *(u16 *)(q + 0x34);
                t->u0 = t->u1 = q[0x0];
                t->u2 = t->u3 = q[0x0] + q[0x4];
                t->v0 = t->v2 = q[0x2];
                t->v1 = t->v3 = q[0x2] + q[0x6];
                addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
            }
            if (col > 0x80807)
            {
                col += 0xFFF7F7F8;
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
