/* PARK: func_800C0404, 234 of 234 words, 2 aligned differences / 3 masked.
   ovl_dungeon.  Re-measured 2026-11-16.  THIS BODY WAS RECOVERED OUT OF THE
   GITIGNORED `nonmatchings/func_800C0404/base.c` -- the 2026-11-15 permuter
   run's scaffold -- which was the only copy of it anywhere.  It is written
   here so that a `nonmatchings/` clean does not take it.

   It is the kind-? node's +0x10 handler (`n->unk_10 = (u32)func_800C0404`),
   so the three parameters are the engine's usual (node + 0x20, n->unk_08,
   n->unk_0C).  It draws the flash quads out of D_80083160.unk_00's +0x8D0
   cursor, then runs a three-state tail on +0x4A.

   NO NEW DECLARATIONS and NO PROTOTYPE CHANGE: every symbol it names is
   already declared in csrc/ovl_dungeon.c, and `void func_800C0404();` at
   :1046 fits the definition below.

   ------------------------------------------------------------------ residue

   WORD COUNTS MATCH.  `aligndiff` reports TWO rows and they are the SAME
   INSTRUCTION in two places -- a pure transposition inside one basic block:

     target    132 lw   $v1, %lo(D_800E3D7C)($a2)
               133 lui  $v0, %hi(D_80083228)
               134 sw   $a1, 0x2C($a0)
     candidate 132 lui  $v0, %hi(D_80083228)
               133 sw   $a1, 0x2C($a0)
               134 lw   $v1, %lo(D_800E3D7C)($a2)

   Everything before 132 and from 135 on is byte-identical, registers
   included.  The three insns are mutually independent; both schedules
   satisfy the load delay of the two `lh`s at 135/136.  So the residue is
   sched1 choosing a different member of the ready list, which is what
   docs/matching-levers.md's "THE PERMUTER DOES NOT CRACK A SMALL
   PURE-ALLOCATION RESIDUE" table already records for this function:
   **base 60 -> best 60 over 132,425 candidates, 30 minutes, found=none.**
   Do not queue it again.

   THE `do { if (lean) { ... } } while (0)` IN THE LOOP IS LOAD-BEARING AND
   ALREADY PAID FOR.  It is worth 28 of 31 differing -- `flow.c:2122` adds
   `loop_depth` to `reg_n_refs` at every reference, so the zero-trip loop
   multiplies the reference counts inside it and flips `allocno_compare`.
   The `if` MUST be nested inside the loop: `while (x) { ...; break; }` and
   `do { if (!x) break; ... } while (0)` are both 40 differing.  See
   docs/matching-levers.md, "A ZERO-TRIP `do { } while (0)` AROUND AN `if` IS
   A REGISTER-ALLOCATION DEVICE".

   MEASURED AND REJECTED on 2026-11-16, all against this body, all at 234
   words (so none of them is a length regression -- they only move the
   transposition around or break something downstream of it):

     this body                                                3 masked
     `((u8 **)D_80082E80)[0xB] = t;` (ARRAY_REF store)         7   -- it frees
         the store to float ABOVE the `sb $v0, 0x9B($v1)` as well, 10 aligned
     `((s16 *)D_800E3D7C)[0x15]` for the 0x2A read            11
     both of the above                                         7
     `u8 *d = D_800E3D7C;` before the store, 0x2A read off d   3   -- the `lw`
         is STILL scheduled after the `sw`, which is what rules the source
         ORDER out as the cause and leaves the ready-list tie as the cause
     the same with `d` assigned before `t`                     3
     `*(u8 **)(D_80082E80 + 0x2C) = D_800DD0F8;` (no `t`)      3
     the store written before `t = D_800DD0F8;`                3
     no `t` local at all, D_800DD0F8 named twice               3
     `*(u8 *)(D_800E3D7C + 0x9B) = 0x11;` (INDIRECT_REF store) 3
     `*(s16 *)&D_800E3D7C[0x2A]`                               3
     the store moved ABOVE `D_800E3D7C[0x9B] = 0x11;`          8
     `(*(s16 *)(D_800E3D7C + 0x2A) + D_80083228 + 0x100)`      5   -- swaps
         which of $v0/$v1 holds which half; the ORDER is unchanged
     `((D_80083228 + 0x100) + *(s16 *)(...))`                 15
     the index hoisted into an `s32` local before the call   235 words / 181

   WHAT THIS CANDIDATE HOLDS FIXED: the `u32 w[2]` initialised aggregate and
   the `UnkDungeonRect q` with its two `.v[]` stores, the `lean` boolean
   computed once before the loop, `s16 h` for func_8003F794's result, and the
   three-way `m` chain written as `if (m == 0) ... else if (m == 1)`.  None of
   those was re-measured this session.

   NEXT MOVE, if anyone spends more on it: `tools/decomp/sched.py <file>
   func_800C0404 --sched1` prints cc1psx's own ready lists.  The block is
   number 9 (insns 307..373 in that dump) and the insn that keeps losing the
   tie shows as priority 1 / ref_count 1, sitting in the ready list from T-2
   to T-21.  Solve for the reference count rather than sweeping spellings --
   that is the route docs/matching-levers.md prescribes for exactly this
   class. */

void func_800C0404(u8 *p, UnkVec *v, UnkSlot *r)
{
    u32 w[2];
    UnkDungeonRect q;
    Unk80083160 *g;
    u8 *tag;
    u8 *e;
    u8 *c;
    u8 *t;
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
    func_80067E2C(e, g->unk_00);
    func_8006658C(tag, e);
    for (i = 0; i < *(s16 *)(p + 0x52); i++)
    {
        e = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0x10);
        n = (func_80069EF8() & 0x3F) + 0x340;
        *(s16 *)(e + 0xC) = n;
        *(s16 *)(e + 0x8) = n;
        k = func_80069EF8() & 0x3F;
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
        func_800667BC(e);
        func_8006658C(tag, e);
    }
    if (*(s16 *)(p + 0x52) < 0x60)
    {
        *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) + 1;
    }
    e = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0xC);
    func_80067E2C(e, w);
    func_8006658C(tag, e);
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
            t = D_800DD0F8;
            *(u8 **)(D_80082E80 + 0x2C) = t;
            func_80048A44(D_80082E80, t[((D_80083228 + *(s16 *)(D_800E3D7C + 0x2A) + 0x100) >> 9) & 7],
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
