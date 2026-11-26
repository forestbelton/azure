/* PARK: func_b05_80174228, csrc/ovl_dng_b05.c, agent-h202, 2026-11-25.

   TARGET 352 words. BEST BUILT 355 words, 122 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports 291 differing unaligned,
   which is the transposition inflation the brief warns about -- read the
   aligned figure.
   1 member, 352 words (tools/decomp/dngfam.py) -- no replication value.

   VERDICT: ALLOC with a 3-word surplus. The shape, every loop bound, every
   constant and every store site below came off the listing and are believed
   right; what is left is ONE allocation decision that cascades (see THE
   REMAINING FAULT) plus a handful of schedule rows downstream of it.

   NO TWIN. dngtwin finds nothing at 0.70; xsibs lists this stub not at all.
   dngcallee.py gives func_s68_80024B20 and func_s36_80024798 at 5 of 7
   symbols; neither is this shape, but func_s36_80024798
   (csrc/ovl_dng_s36.c, `s32 f(u8 *p, UnkVec *v)') is the right SKELETON --
   the `while (1) { q = p; ... n = *(UnkNode **)(p - 0x8); if (!n) break;
   p = (u8 *)n + 0x20; }' chain walk, the +0x8D0 prim cursor and the inlined
   addPrim all come from it. func_800DBA90's record layout came from
   csrc/ovl_dungeon_800D50FC.c's definition and from the landed
   func_b04_801743EC (csrc/ovl_dng_b04.c), which is where the
   `u8 rec[0x20]' alignment-1 type and the `memcpy(rec + k, p + n, 8)'
   spelling come from.

   WHAT IT DOES. A 23-segment semi-transparent beam, drawn once per node of
   the tag chain off p - 8. Per node: seed four SVECTORs as a 0x80-wide,
   0xB0-deep quad (vx +/-0x40, vy 0, vz 0x10 or -0xA0 by parity), rotate them
   in place through func_800DBA90 with the view's -0x80083160+0xC8 yaw and
   the node's first 8 bytes as the translation, RotTransPers all four while
   accumulating (z - 8) and tracking a packed min/max xy box, average the
   depth by 4, build a 23-entry symmetric half-width taper from the node's
   +0xE halfword (each step half the previous, mirrored about index 0xB), and
   if the averaged depth is under 0x1E0 emit 23 POLY_FT4s whose grey ramps
   with the taper, whose u-span is i/23 of the texture width and whose x-span
   interpolates the box, inserting each on OT row `depth'.

   THE KEY TYPE FACT, and it is the whole reason this body did not fall out
   of a plain read: THE MIN/MAX BOX IS NOT A DVECTOR. The target keeps both
   halves of each corner in ONE REGISTER ($s2 = min, $s3 = max) and inserts
   the halves with `and'/`or'. A psyq DVECTOR (two shorts, align 2) is
   BLKmode on this target -- gcc 2.7.2's layout_type refuses a non-BLK mode
   for a RECORD whose TYPE_ALIGN is under its size when STRICT_ALIGNMENT is
   on -- so `DVECTOR mn' goes on the STACK and the body is 354 words / 198
   aligned differences. Written as two `s32' with explicit half masks it is
   in registers and the body is 354 / 177. The same fact puts `xy' (the
   RotTransPers output) at a 4-byte slot: as a `DVECTOR' it is a BLKmode
   temp, which assign_stack_temp rounds to BIGGEST_ALIGNMENT = 8 bytes, so
   the frame comes out 0xD8 instead of 0xC8 and every later local is off by
   4. As a plain `s32' read through `*(s16 *)&xy' it lands at 0x88 with `w'
   at 0x8C, which is the target's frame exactly.

   FRAME, read off the listing and reproduced: 0x10 outgoing args, then
   `u16 tex[4]' at 0x10 (the D_b05_8017087C copy), `SVECTOR sv[4]' at 0x18,
   `u8 rec[0x20]' at 0x38, `u16 arr[0x18]' at 0x58, `s32 xy' at 0x88,
   `s32 w' at 0x8C -- `w' and `xy' are address-taken (RotTransPers' sxy, p
   and flag) which is why they are in memory -- then THREE SPILL SLOTS at
   0x90 (the depth accumulator), 0x94 (the `SVECTOR *svp' cursor) and 0x98
   (the hoisted `(s16)max.x'), and ten saves at 0xA0..0xC4.

   MEASURED FACTS (each one re-measured on this body):

   | spelling                                                    | words | aligned |
   | as written below                                             |  355  |   122   |
   | `DVECTOR mn/mx' instead of two `s32' with half masks         |  354  |   198   |
   | `DVECTOR xy' instead of `s32 xy'                             |  354  |   168   |
   | the half masks as `mn = (mn & M) | V' instead of `&=' / `|=' |  354  |   194   |
   | the mn/mx seeding ABOVE the func_800DBA90 call               |  354  |   177   |
   | `sv[i]' indexed off the array instead of the `svp' pointer   |  360  |   150   |
   | `memcpy(tex, D_b05_8017087C, 8)' with the array operand      |  332  |   259   |
   | no `e = p' cursor copy at the top of the `while'             |  341  |   237   |
   | `D_80083160.unk_00' direct instead of through `g'            |  360  |   150   |
   | the +0x25 gate as `if/else' instead of assign-then-overwrite |  360  |   131   |
   | `xy''s halves re-read inside each min/max arm                |  360  |   131   |

   FIVE OF THOSE ARE WORTH RESTATING BECAUSE THEY GENERALISE:

   (1) AN 8-BYTE `memcpy' FROM A NAMED ARRAY COMES OUT ALIGNED. `memcpy(dst,
   D_b05_8017087C, 8)' emits `lw'/`sw', not the target's `lwl'/`lwr' pair,
   while the SAME call at 0x20 bytes emits `lwl'/`lwr'. The discriminator is
   expr.c's `emit_block_move', which takes the `move_by_pieces' path when
   `move_by_pieces_ninsns(size, align) < MOVE_RATIO' and that function
   RAISES align to MOVE_MAX; at 8 bytes the count is small enough to take
   that path and at 0x20 it is not. Routing the source through a `u8 *'
   LOCAL (`s = (u8 *)D_b05_8017087C; memcpy(tex, s, 8);') forces
   get_pointer_alignment to 1 and restores the unaligned pair. The
   0x20-byte copy in this unit's func_b05_80172288 needs no such local.

   (2) THE `while' BODY MUST READ THE PARAMETER INTO ITS OWN CURSOR. The
   target spills `p' to its incoming slot at 0xC8($sp) and reloads it into
   $s7 at the top of every iteration; that is func_s36_80024798's `q = p;'
   and without it `p' simply stays in a callee-saved register and the body
   is 341 / 237.

   (3) THE SVECTOR ARRAY IS REACHED THROUGH ONE POINTER LOCAL. The target
   has a single spilled pseudo at 0x94 serving both `*(u8 **)rec = svp' and
   `&svp[i]'; indexing the array directly makes two pseudos and two spill
   slots (360 / 150).

   (4) THE mn/mx SEEDING IS AFTER func_800DBA90(rec) EVEN THOUGH ITS `andi'
   HALVES ARE EMITTED BEFORE THE CALL. $s2/$s3 are callee-saved, so the
   `andi $s2, $s2, 0xFFFF' has no dependence on the call and sched hoists it
   across; the `lui'/`or' cannot follow because $v0 is call-clobbered.
   Seeding before the call puts all six words before it: 354 / 177.

   (5) THE +0x25 V GATE IS ASSIGN-THEN-OVERWRITE, not if/else. The target's
   `addiu $a0, $v1, 0x2' sits in the `bnez' DELAY SLOT, so `m = t->v0 + 2' is
   unconditional and the `>= 2' case overwrites it. Written as if/else it is
   360 / 131.

   THE REMAINING FAULT, named and priced but not fixed: the depth
   accumulator. The target SPILLS it to 0x90 and keeps `z * 4' (the OT byte
   offset, loop-invariant in the 23-iteration loop) in $fp; this body does
   the opposite -- `z' in $s8 and `z * 4' spilled to 0x98. Every `z'
   reference then differs, the three spill slots rotate (svp lands at 0x90
   instead of 0x94), and the body is 3 words short because the spilled form
   costs a `lw'/`sw' pair at the `+=' site. Priced by global.c's
   allocno_compare with flow's loop weighting, `z' scores roughly
   3*8/350 = 0.069 against `z * 4''s 2*5/170 = 0.059, so `z' wins the
   register here; the target needs the reverse, which means its `z' has
   fewer weighted references or a shorter range than this spelling gives it.
   That is the next thing to attack and it is probably one statement.

   ALSO STILL OPEN, all downstream of the same block: the two loop-invariant
   constants -0xA0 and 0x10 are hoisted ABOVE the seeding loop in the target
   and rematerialised inside it here (1 word); the `rec + 0xC' store is
   scheduled before the `rec + 0x0/0x4' pair here and after it in the target;
   the second `(s16)tex[2] / 0x17' gains a redundant `sll 16'/`sra 16' pair.

   DECLARATIONS
   void func_800DBA90(u8 *);
   extern const u8 D_b05_8017087C[];
   END DECLARATIONS
*/
s32 func_b05_80174228(u8 *p, UnkVec *vv)
{
    Unk80083160 *g;
    u16 tex[4];
    SVECTOR sv[4];
    u8 rec[0x20];
    u16 arr[0x18];
    s32 xy;
    s32 w;
    POLY_FT4 *t;
    UnkNode *n;
    SVECTOR *svp;
    u8 *e;
    u8 *s;
    s32 mn;
    s32 mx;
    s32 z;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 ax;
    s32 ux;
    s32 ay;
    s32 uy;
    s32 span;
    s32 q;

    s = (u8 *)D_b05_8017087C;
    memcpy(tex, s, 8);
    g = &D_80083160;
    z = 0;
    svp = sv;
    while (1)
    {
        e = p;
        w = 0x40;
        for (i = 3; i >= 0; i--)
        {
            svp[i].vx = w;
            if (i < 2)
            {
                svp[i].vx = -w;
            }
            svp[i].vy = 0;
            if ((i & 1) != 0)
            {
                svp[i].vz = -0xA0;
            }
            else
            {
                svp[i].vz = 0x10;
            }
        }
        *(s16 *)(rec + 0x8) = *(s16 *)(rec + 0xA) = 0;
        *(u8 **)rec = (u8 *)svp;
        *(u8 **)(rec + 0x4) = (u8 *)svp;
        *(s16 *)(rec + 0xC) = -*(u16 *)((u8 *)g + 0xC8);
        memcpy(rec + 0x10, e, 8);
        *(s16 *)(rec + 0x18) = 4;
        *(s16 *)(rec + 0x1A) = 0;
        func_800DBA90(rec);
        mn &= 0xFFFF;
        mn |= 0x75300000;
        mn &= 0xFFFF0000;
        mn |= 0x7530;
        mx &= 0xFFFF;
        mx |= 0x8AD00000;
        mx &= 0xFFFF0000;
        mx |= 0x8AD0;
        for (i = 3; i >= 0; i--)
        {
            z += RotTransPers(&svp[i], &xy, (long *)&w, (long *)&w) - 8;
            ax = *(s16 *)&xy;
            ux = *(u16 *)&xy;
            if ((s16)mx < ax)
            {
                mx &= 0xFFFF0000;
                mx = ux | mx;
            }
            else if (ax < (s16)mn)
            {
                mn &= 0xFFFF0000;
                mn = ux | mn;
            }
            ay = *(s16 *)((u8 *)&xy + 0x2);
            uy = *(u16 *)((u8 *)&xy + 0x2);
            if ((mx >> 16) < ay)
            {
                mx &= 0xFFFF;
                mx |= uy << 16;
            }
            else if (ay < (mn >> 16))
            {
                mn &= 0xFFFF;
                mn |= uy << 16;
            }
        }
        z >>= 2;
        arr[0xB] = *(u16 *)(e + 0xE);
        for (i = 1; i < 0xC; i++)
        {
            j = (s16)arr[0xC - i] >> 1;
            arr[0xB - i] = arr[0xB + i] = j;
        }
        if ((u32)z < 0x1E0)
        {
            for (i = 0; i < 0x17; i++)
            {
                t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
                w = (*(s16 *)&arr[i] - __builtin_abs(0xB - i)) * 2 +
                    ((*(s16 *)(e + 0x18) << 2) + 0x5C);
                if (w < 0)
                {
                    w = 0;
                }
                t->r0 = t->g0 = t->b0 = *(u8 *)&w;
                SetPolyFT4(t);
                SetSemiTrans((u8 *)t, 1);
                t->tpage = GetTPage(2, 1, 0x340, 0x100);
                k = i * (s16)tex[2] / 0x17;
                t->u0 = t->u1 = k;
                t->v0 = t->v2 = 0;
                k += (s16)tex[2] / 0x17;
                t->u2 = t->u3 = k;
                t->v3 = *(u8 *)&tex[3];
                m = t->v0 + 2;
                if (*(u8 *)&tex[3] >= 2)
                {
                    m = t->v0 + t->v3;
                }
                t->v1 = t->v3 = m;
                span = (s16)mx - (s16)mn;
                q = span * i / 0x17;
                w = q;
                if (i < 0xB)
                {
                    w = q + *(s16 *)(e + 0xA);
                    if ((span >> 1) < w)
                    {
                        w = span >> 1;
                    }
                }
                else if (i >= 0xC)
                {
                    w = q - *(s16 *)(e + 0xA);
                    if (w < (span >> 1))
                    {
                        w = span >> 1;
                    }
                }
                j = w + mn;
                t->x0 = t->x1 = j;
                j += span / 0x17;
                t->x2 = t->x3 = j;
                t->y0 = t->y2 = arr[i] + (mn >> 16);
                t->y1 = t->y3 = arr[i] + (mx >> 16);
                addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
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
