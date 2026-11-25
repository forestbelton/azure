/* PARK -- func_s63_80024D58, 339 words target, 333 words built, 109 aligned
   differences. Batch 33, agent h142. ovl_dng_s63; one-member family. It is
   the renderer of the 0x302 node that func_s63_8002401C (LANDED) spawns in
   its state 3, and func_s63_80024B20 (LANDED) is that nodes step, so the
   record this reads at p+0x10..p+0x3F is the EIGHT SIX-BYTE TRAIL ROWS that
   func_s63_80024B20 shifts and settles every frame. Read that function first;
   it fixes the record layout and the row stride.

   WHAT IT DOES, derived entirely from the listing. A tag-chain walk, same
   shape as the landed func_s63_8002441C: the body runs on p, then
   `n = *(UnkNode **)(p - 8)` and `p = (u8 *)n + 0x20` until the chain ends,
   returning 0. Each node is drawn as a 15x15 mesh of semi-transparent
   POLY_FT4 quads whose screen vertices are built in three phases.

   PHASE A seeds the input vector arrays y column, `a[i].vy = i * 0x60 -
   0x2D0` for sixteen i, and fills the four invariant fields of the
   func_800DBA90 descriptor.

   PHASE B is the eight-step ring. For i = 7 down to 0 it points the
   descriptor at `&a[i]` and `&b[i]`, sets its count field to `(8 - i) * 2`
   and its angle field to the trail rows `*(u16 *)(p + i * 6 + 0x12)`, fills
   `a[k].vx`/`a[k].vz` for the counts worth of entries, and then runs the
   descriptor through func_800DBA90 FOUR TIMES, adding 0x400 to the angle
   field after each, projecting the rotated `b[k]` into a different EDGE of
   the 16x16 screen-vertex grid each time:

       j == 0   c[i][k]
       j == 1   c[i * 2 + r - (k + 1)][i]
       j == 2   c[i + r - 1][i * 2 + r - (k + 1)]
       j == 3   c[k][i + r - 1]

   with `r` the descriptors count field read back as
   `(s16) *(u16 *)(rec + 0x18)`. The four arms are a COMPARE CHAIN over
   {0,1,2,3} -- this function owns no jump table and the listing has no
   .rodata block.

   PHASE C walks j and k from 0 to 14 and emits one POLY_FT4 per cell off
   D_80083160s +0x8D0 cursor, 0x28 bytes of cursor each, with the four
   corners `c[k][j]`, `c[k][j+1]`, `c[k+1][j]`, `c[k+1][j+1]` written as WORDS
   into +0x8/+0x10/+0x18/+0x20, a fixed 0xC0/0xDF u and 0/0x1F v box, and
   `addPrim((u32 *)(g->unk_00 + 0xB0), t)` -- OT row 0, no depth index.

   THE FRAME IS THE MOST USEFUL THING HERE. -0x570 with s0..s7, fp and ra,
   and gcc has run out of registers: four values live in SPILL SLOTS
   (sp+0x538 = p, sp+0x53C = the `a` base, sp+0x540 = `&c[i][0]`, sp+0x544 =
   the `i * 0x60 - 0x2D0` giv), and `p` itself is reloaded from its incoming
   home slot at sp+0x570 at the top of every walk iteration. The local layout
   that produces it, in declaration order:

       SVECTOR a[16];        sp+0x010 .. 0x08F
       SVECTOR b[16];        sp+0x090 .. 0x10F
       s32     c[16][16];    sp+0x110 .. 0x50F
       u8      rec[0x20];    sp+0x510 .. 0x52F
       s32     w0;           sp+0x530
       s32     w1;           sp+0x534

   THE DESCRIPTOR IS func_800DBA90s AND ITS LAYOUT IS ALREADY LANDED, which
   is the donor nothing in LEADS.txt, CALLEE.txt or NEAR.txt points at. Three
   landed bodies call func_800DBA90 -- csrc/ovl_dng_b04.c func_b04_801743EC,
   csrc/ovl_dng_b38.c and csrc/ovl_dng_b33.c -- and b04s comment states the
   type outright: `u8 rec[0x20]`, +0x0 the input vector pointer, +0x4 the
   output vector pointer, two eight-byte `memcpy` payloads, and two s16 tail
   fields. It also states WHY the type matters: a `u8` array is alignment 1,
   so an eight-byte `memcpy(rec + 0x10, p + 0x4, 8)` with both pointers `u8 *`
   expands to one unaligned load pair and one unaligned store pair per word,
   which is the `lwl`/`lwr` and `swl`/`swr` run at 0x80024DDC..0x80024DF8.
   No other spelling reaches it.

   MEASURED, the one fact that is priced:

     | fact                                                  | words | rows |
     | PHASE C REUSES PHASE Bs TWO COUNTERS -- its outer     |  333  |  109 |
     |   loop is the same local as the four-way switchs      |       |      |
     |   counter and its inner loop the same local as the    |       |      |
     |   `a[k]` fill loops (as written)                      |       |      |
     |   ... phase C given two counters of its own           |  334  |  194 |

   That is 85 aligned rows for two variable names and it is the brief s
   shared-register rule paying off: the target holds the outer counter in $s4
   and the inner in $s1 across BOTH phases, so they are two locals for the
   whole function, not four. Count the callee-saved saves against the number
   of long-lived values before writing the locals -- ten saves minus one
   parameter is nine, and four counters do not fit.

   THE THREE FAULTS I DID NOT CLOSE, in the order I would attack them.

   1. THE `rec + 0x18` COUNT FIELD IS RE-READ BY THE TARGET INSIDE THE `a[k]`
      FILL LOOP (`lh $v0, 0x528($sp)` at 0x80024E98, inside the loop) and is
      HOISTED in this body. As a plain cast off a `u8` array the read is a
      fixed-address scalar, the `a[k]` stores are varying struct references,
      and `fixed_scalar_and_varying_struct_p` excuses the pair, so nothing
      invalidates it. The untried fix is to make the read a COMPONENT_REF --
      give the descriptor a struct typedef whose +0x18 is an `s16` member and
      whose +0x10 is a `u8 [8]` member, so the memcpys pointer stays
      alignment 1 while the field read becomes in_struct and is killed by the
      stores. That is the MEM_IN_STRUCT_P lever in the brief, used for its
      DEPENDENCE half rather than its allocation half.

   2. THE -0x2D0 ADDITIVE TERM IS A HOISTED REGISTER IN THE TARGET AND A
      FOLDED CONSTANT HERE. The target materialises it with
      `addiu $t0, $zero, -0x2D0` and then derives BOTH giv initialisers from
      it (`addiu $v1, $t0, 0x5A0` for the phase A loop at i = 15, and
      `addiu $t0, $t0, 0x2A0` for the phase B giv at i = 7); this body folds
      each to a single `li`. `emit_iv_add_mult` only fails to fold when
      `add_val` reaches it as a REGISTER, so the constant is a movable that
      `scan_loop` hoisted out of the WALK loop -- which needs the two giv
      expressions to share it. Worth about four words of the six-word
      deficit. Try writing the term so the two sites are textually one value.

   3. EVERYTHING ELSE IS THE PHASE B HEAD`S STATEMENT ORDER. The target
      computes `&a[i]`, `&b[i]`, the count field, the angle field and
      `a[0].vx = (*(s16 *)(p + 0x4) + i) * 0x40` in one straight-line run
      ending with the `a[k]` loops entry test, with the `a[0].vx` store in
      the `beqz`s delay slot. This body emits the same insns in a different
      order and the register names rotate through the rest of the arm.
      Re-derive that order against 0x80024E20..0x80024E7C before touching
      anything else; the four switch arms and all of phase C are already
      register-rename-only against the target.

   NOTE ON `a[0].vx`. The store at 0x80024E78 is to a FIXED frame address,
   sp+0x10, which is `a[0]` -- not `a[i]` -- while the descriptor is handed
   `&a[i]`. I could find no reading that makes it `a[i]`, and the listing is
   the authority, so it is written as `a[0].vx` here. If the next agent finds
   a layout in which sp+0x10 is something other than `a[0]`, that changes the
   whole local map above and should be checked first.

   DECLARATIONS
   void func_800DBA90(u8 *);
   END DECLARATIONS

   D_80083160, Unk80083160, UnkNode and memcpy need no declaration; all four
   are reachable from common.h and include/structs.h. RotTransPers,
   SetPolyFT4, SetSemiTrans, GetTPage, GetClut and addPrim are the psyq
   names of the listings func_80065420, func_800666F4, func_80066640,
   func_80066460 and func_8006649C -- splat/common_syms.txt names all five
   and none needs declaring. */
s32 func_s63_80024D58(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    POLY_FT4 *t;
    s32 i;
    s32 j;
    s32 k;
    s32 r;
    SVECTOR a[16];
    SVECTOR b[16];
    s32 c[16][16];
    u8 rec[0x20];
    s32 w0;
    s32 w1;

    while (1)
    {
        for (i = 15; i >= 0; i--)
        {
            a[i].vy = i * 0x60 - 0x2D0;
        }
        *(s16 *)(rec + 0x8) = *(s16 *)(rec + 0xA) = 0;
        memcpy(rec + 0x10, p + 0x4, 8);
        *(s16 *)(rec + 0x1A) = 0;
        for (i = 7; i >= 0; i--)
        {
            *(u8 **)(rec + 0x4) = (u8 *)&b[i];
            *(u8 **)rec = (u8 *)&a[i];
            *(s16 *)(rec + 0x18) = (8 - i) * 2;
            *(s16 *)(rec + 0xC) = *(u16 *)(p + i * 6 + 0x12);
            a[0].vx = (*(s16 *)(p + 0x4) + i) * 0x40;
            for (k = i; k < i + *(s16 *)(rec + 0x18); k++)
            {
                a[k].vx = i * 0x60 - 0x2D0;
                a[k].vz = *(u16 *)(p + i * 6 + 0x10);
            }
            for (j = 0; j < 4; j++)
            {
                func_800DBA90(rec);
                switch (j)
                {
                case 0:
                    for (k = i; k < i + *(s16 *)(rec + 0x18); k++)
                    {
                        RotTransPers(&b[k], &c[i][k], &w0, &w1);
                    }
                    break;
                case 1:
                    for (k = i; k < i + *(s16 *)(rec + 0x18); k++)
                    {
                        r = (s16)*(u16 *)(rec + 0x18);
                        RotTransPers(&b[k], &c[i * 2 + r - (k + 1)][i], &w0, &w1);
                    }
                    break;
                case 2:
                    for (k = i; k < i + *(s16 *)(rec + 0x18); k++)
                    {
                        r = (s16)*(u16 *)(rec + 0x18);
                        RotTransPers(&b[k], &c[i + r - 1][i * 2 + r - (k + 1)], &w0, &w1);
                    }
                    break;
                case 3:
                    for (k = i; k < i + *(s16 *)(rec + 0x18); k++)
                    {
                        r = (s16)*(u16 *)(rec + 0x18);
                        RotTransPers(&b[k], &c[k][i + r - 1], &w0, &w1);
                    }
                    break;
                }
                *(s16 *)(rec + 0xC) = *(u16 *)(rec + 0xC) + 0x400;
            }
        }
        for (j = 0; j < 15; j++)
        {
            for (k = 0; k < 15; k++)
            {
                t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
                *(u32 *)&t->r0 = *(s32 *)(p + 0xC);
                SetPolyFT4(t);
                SetSemiTrans((u8 *)t, 1);
                t->tpage = GetTPage(0, 3, 0x300, 0x100);
                t->clut = GetClut(0x10, 0x1F8);
                *(s32 *)&t->x0 = c[k][j];
                *(s32 *)&t->x1 = c[k][j + 1];
                *(s32 *)&t->x2 = c[k + 1][j];
                t->u0 = t->u1 = 0xC0;
                t->u2 = t->u3 = 0xDF;
                t->v0 = t->v2 = 0;
                t->v1 = t->v3 = 0x1F;
                *(s32 *)&t->x3 = c[k + 1][j + 1];
                addPrim((u32 *)(g->unk_00 + 0xB0), t);
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
