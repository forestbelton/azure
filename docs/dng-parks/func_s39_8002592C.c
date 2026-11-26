/* PARK: func_s39_8002592C, csrc/ovl_dng_s39.c, agent-h238, 2026-11-26.

   TARGET 847 words. BEST BUILT 847 words, 2 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports the same 2 masked, in the
   unit AND in the onefunc standalone -- the two agree exactly, so the
   standalone below is a valid probe for this body.
   1 member, 847 words (tools/decomp/dngfam.py) -- no replication value.

   VERDICT: ALLOC, and it is ONE INSTRUCTION IN THE WRONG SLOT. Every
   opcode, immediate, displacement AND REGISTER in all 847 words matches.
   The whole residue is a transposed pair of loads in case 1's out[] arm:

     target   151 sh $v0,0x6($fp)  152 lhu $v0,0xA($fp)  153 lhu $v1,0x24($sp)
     built    151 sh $v0,0x6($fp)  152 lhu $v1,0x24($sp)  153 lhu $v0,0xA($fp)

   Both registers are already the target's ($v0 for the v+0xA value, $v1 for
   out[2]) and the following `j' / `addu $v0,$v0,$v1' match. The 2 differing
   is that one transposition counted at both words. It is a sched1
   ready-list tie between two loads with equal depth, and I did NOT isolate
   which input breaks it -- THAT MUCH IS A HYPOTHESIS. Everything else in
   this header is measured.

   IT OWNS A JUMP TABLE AND THE ARM ORDER IS MEASURED, NOT ASSUMED.
   jtbl_s39_80024038, 9 words = 9 cases + 0 pad, dispatch bound
   `sltiu $v0, $v1, 0x9' with NO minval subtraction, 9 distinct targets
   (tools/decomp/jtcheck.py). A standalone `tools/cdkgcc -S' of this body
   emits the table in .rdata as

     $L134 $L135 $L146 $L168 $L177 $L186 $L188 $L133 $L190

   whose emitted addresses run 0 < 1 < 2 < 3 < 4 < 5 < 6 < 8 < 7, with the
   case-7 entry ($L133) pointing at the FUNCTION'S EPILOGUE. That is the
   listing's order exactly:
     case 0 .Ls39_80025A24   case 5 .Ls39_80026578
     case 1 .Ls39_80025AEC   case 6 .Ls39_8002659C
     case 2 .Ls39_80025DC0   case 7 .Ls39_80026634  (THE EPILOGUE, NO ARM)
     case 3 .Ls39_80026200   case 8 .Ls39_800265D0
     case 4 .Ls39_80026354
   So there is no `default:' statement and no `case 7:' label -- case 7 falls
   out of the switch -- and the eight real arms are written 0,1,2,3,4,5,6,8
   in ascending order, which is the address order. Case 0 FALLS THROUGH into
   case 1. Cases 4 and 5 share a two-store tail that jump2 cross-joins at
   .Ls39_80026588; both arms write it out below.
   The emitted asm contains ZERO `$LC' constant pools and its only .rdata is
   this table, so the `u16 t[8] = {...}' container refusal does not apply.

   THE DONOR is the landed func_s38_80024FA4 (csrc/ovl_dng_s38.c, 627 words,
   also a 9-entry table with case 7 absent). dngtwin found NOTHING at 0.70;
   dngcallee named it alone at 17 of 24 symbols with a clean gap at 14.
   Its head (`o'/`n'/`m', the 0x20-byte D_s39_80024014 copy into
   `UnkDngS58Rec tbl[8]'), its case 0 renderable-and-ring opening, its case 1
   func_8003DF74 hand read with the same `u16 out[3]'/`s32 arr[2]' stack
   locals, its D_80045340 tag block, its `D_8006CCD8[f]'/`D_8006CCE8[f]'
   corner pair, its `__builtin_abs(...)' step count, its case 2 arrival
   block, its case 6 and its case 8 are this body's verbatim. The deltas are
   this bank's own: FOUR func_8003FC64 spawns against s38's one, three
   `rand()' * func_s39_8002512C velocity terms in case 4, a 0x60-byte
   zero-fill loop in two of the spawns, and a whole spawn block inside
   case 3 where s38 only advances state.
   s38's OWN measured facts all HOLD here and were re-measured as part of
   the table below: the table copy before `n'/`m', case 0's +0xA bump as the
   array ref `((s16 *)p)[0x5]', `z' for m+0xA assigned after the +0x6 store,
   the 0x8000 gate with the out[] arm as the `then', `k' for o+0x88 in the
   t == NULL arm, case 1's +0xA store before the +0x88 store, the 0x7B gate
   as `<= 0' with the integrate block as the ELSE, and case 4's argument as
   TWO locals `j' and `h'.

   THE PRICED TABLE. Each row is the body as it then stood, measured with
   aligndiff; the row counts INCLUDE register-only (R) rows, with the R
   share given where it dominates.

   | state                                                  | words | rows |
   |--------------------------------------------------------|-------|------|
   | first compile, s38 adapted                             |  857  | 355  |
   |   (310 of those 355 are register-only R rows)          |       |      |
   | + three word-count fixes, below                        |  869  | 364  |
   | + case 4 re-reads `r = (u8 *)n3->unk_0C' after the     |       |      |
   |   rand/func_s39_8002512C calls                   KEPT  |  848  |  83  |
   |   (68 R; without it `r' spans calls, needs a tenth     |       |      |
   |    callee-saved register and SPILLS `v' to the stack,  |       |      |
   |    which is the 0x80 frame against the target's 0x78)  |       |      |
   | + case 1's `c[0xD] = 0xE0' moved below the             |       |      |
   |   `*(u16 *)(c + 0x14) |= 0xC'                    KEPT  |  847  |  61  |
   | + the 0x1A wrap's `y' split into a load then a         |       |      |
   |   separate `y += 0x400'                          KEPT  |  847  |  58  |
   |   (as one expression the load and the add are two      |       |      |
   |    quantities: `lhu $a0' / `addiu $v1,$a0' against     |       |      |
   |    the target's `lhu $v1' / `addiu $v1,$v1')           |       |      |
   | + the v+0xA store written ONCE after the if/else       |       |      |
   |   merge, into `yv'                               KEPT  |  847  |   7  |
   | + `out[2]' read into the local `kv' so the v+0xA       |       |      |
   |   load is the plus's FIRST operand              KEPT  |  847  |   2  |

   THE THREE WORD-COUNT FIXES, each measured as a single ablation off the
   847-word body:

   * `(D_800DDC40[...] << 15)', NOT `(D_800DDC40[...] >> 1) << 16'. The
     shift pair is `srl 1' + `sll 16' = 2 words where the target has one
     `sll $v1, $v1, 15'; gcc will not fuse them because the operand is a
     zero-extended QImode and bit 0 survives `<< 15' but not `>> 1 << 16',
     so the two spellings are DIFFERENT VALUES. s38 writes the `>> 1 << 16'
     form at its own site; it does not transfer. Worth 1 word.
   * case 4's `*(u8 **)(*(u8 **)(o + 0x60) - 0x14)' must be read into the
     local `x'. Spelled inline at both the +0x1C and the +0x1E store it is
     4 words of re-loads, because the `*(s16 *)(r + 0x1C)' store BETWEEN
     them is through a pointer gcc cannot disambiguate from `o + 0x60'.
   * the 0x1A wrap must go through the local `y' with ONE store after the
     if/else. s38's two-store spelling re-reads `*(u16 *)(c + 0x1A)' and
     gives `sh` + `andi 0xffff` + `sltiu 0x1001' where the target has a bare
     signed `slti $v0, $v1, 0x1001' on the unstored sum: 1 word and the
     wrong comparison.

   WHY THE v+0xA STORE IS THE WHOLE ALLOCATION STORY, AND IT IS ARITHMETIC.
   `global.c:allocno_compare' ranks allocnos by
   `floor_log2(n_refs) * n_refs / live_length * 10000', ties broken by
   allocno number (so a parameter, whose pseudo is lower, wins a tie), and
   `qsort' at global.c:546 is followed by `dump_conflicts' at :551, so the
   `;; N regs to allocate:' line IS the allocation order. MIPS defines no
   REG_ALLOC_ORDER, so `find_reg' takes the lowest-numbered free suitable
   register and $s7 (23) is tried before $fp (30). The `.lreg' numbers for
   the two-store body were

     80 = p   88 refs / 1152 insns          81 = v   33 refs /  756 -> 2182
     82 = c   31 refs /  642               86 = o   24 refs /  442 -> 2171

   so v outranked o BY 11 PARTS IN 2180, took $s7, pushed o to $fp and
   inverted 56 of the 58 rows. Ten values cross calls and there are nine
   callee-saved registers, so `c' spills in both the target and this body.
   Writing the two arms' v+0xA stores as ONE store after the merge takes v
   to 32 refs / 754 insns -> 2122, below o's 2176, and o takes $s7. THE
   EMITTED CODE IS UNCHANGED EITHER WAY, because jump2 cross-joins the two
   arms' stores into the one store at .Ls39_80025BA0 regardless -- flow
   runs long before jump2 and counts BOTH.

   AND `set_preference' READS THE FIRST OPERAND OF THE EXPRESSION. Its first
   two lines are `if (GET_RTX_FORMAT (GET_CODE (src))[0] == 'e') src = XEXP
   (src, 0)', so for `(set yv (plus A B))' the preference is taken from A's
   hard register (via `reg_renumber', i.e. local-alloc's choice). That is
   exactly why `yv = out[2] + *(u16 *)((u8 *)v + 0xA)' gives
   `;; 83 preferences: 3' and yv lands in $v1, while reading out[2] into
   `kv' first and writing `yv = *(u16 *)((u8 *)v + 0xA) + kv' puts the
   v+0xA load first and yv in $v0. 5 rows.

   MEASURED AND FREE -- these are NOT the reason anything here lands:
   * `*(s16 *)(r + 0x10) = i;' against `= 0x60;' in case 2's second spawn.
     Identical (847 words / 61 rows both) at the point they were compared.
     The listing's `addiu $s0, $zero, 0x60' right after the zero-fill loop
     is produced by both, so the loop variable's reuse is NOT witnessed.
     `0x60' is kept below as the less speculative spelling.
   * where `s32 yv;' sits in the declaration block. 847 / 5 either way.

   MEASURED AND WORSE, so a later attempt starts here:
   * `*(s16 *)(p + 0x8A) = ...' moved ABOVE case 1's v->unk_0C/10/14 group:
     this DOES flip the allocation the right way (v 33 refs / 768 insns ->
     2148, below o's 2171, 26 rows) but it is 848 WORDS -- an extra `nop'
     and a lost `lbu 0x7B' reuse in the mult group. One word long.
   * `o = *(u8 **)p' moved BELOW the table copy: shortens o to 441 insns,
     not the 439 needed, and costs the `lw $s7, 0x0($s6)' row. 847 / 62.
   * `do { } while (0)' around case 1's first two out[] groups: 847 / 61.
     Around the third group alone: 847 / 61. Both re-break the v/o order.
   * case 1's third group as the array ref `((u16 *)v)[0x5] + out[2]':
     847 / 7. Its first two groups as array refs: 847 / 7.
   * `kv = *(u16 *)((u8 *)v + 0xA); k2 = out[2]; yv = kv + k2;' -- both
     operands named, in the target's birth order: 847 / 7.
   * `yv = *(u16 *)((u8 *)v + 0xA); kv = out[2]; yv += kv;': 846 words, 9
     rows. One word SHORT.
   * `kv = out[2];' hoisted to the top of the out[] arm: 843 words, 15 rows.

   NO PROTOTYPE OF A LANDED BODY IS CHANGED BY THIS and no rodata object is
   hand-placed. The declarations this body needs are already in the unit's
   one block (agent-h238 added them while landing func_s39_8002405C's
   neighbour); func_s39_80025228/80025348/80025398/8002558C/80025648 and
   func_s39_8002512C are all LANDED in the unit and all defined above this
   function, so no forward declaration is needed. 0x80069EF8 is `rand' in
   splat/common_syms.txt and the unit already declares `s32 rand(void);'.

   DECLARATIONS
#include "common.h"

UnkNode *func_8003FC64(s32);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
extern u8 D_80026674[];
extern u8 D_80026680[];
extern u8 D_80026698[];
extern u8 D_800266A4[];
extern u8 D_80045340[];
extern u8 D_800DDC40[];
extern u8 D_800E3D68;
extern u8 D_s39_80024014[];
extern u8 D_s39_80026668[];
s32 func_8004491C(UnkNode *, u32);
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
s32 func_800A6D30(void);
s32 func_800C8A3C(u8 *, s16, s16);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
void func_800B835C(u8 *, s32 *, s32, s32);
   END DECLARATIONS
*/
void func_s39_8002592C(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    s32 arr[2];
    UnkDngS58Rec tbl[8];
    s32 yv;
    s32 kv;
    UnkNode *n;
    UnkNode *n2;
    UnkNode *n3;
    u8 *o;
    u8 *m;
    u8 *e;
    u8 *t;
    u8 *u;
    u8 *q;
    u8 *r;
    u8 *w;
    u8 *x;
    s32 i;
    s32 k;
    s32 z;
    s32 j;
    s32 h;
    s32 y;

    o = *(u8 **)p;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s39_80024014;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(UnkDngS58Name12 *)(p + 0xA2) = *(UnkDngS58Name12 *)D_80026698;
        *(u32 *)(c + 0x8) = (u32)(p + 0xA2);
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        D_800266A4[0x10] = 0x80;
        D_800266A4[0x11] = 0x80;
        D_800266A4[0x12] = 0x80;
        D_800266A4[0x14] = 4;
        D_800266A4[0x15] = 4;
        D_800266A4[0x16] = 4;
        arr[0] = 0x1400340;
        arr[1] = 0x400040;
        func_800B835C(D_800266A4, arr, 1, 0);
        D_800266BC = 1;
        ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out, 0) ==
            NULL)
        {
            if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
            {
                break;
            }
        }
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)v + 0xA) = z;
        if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)v + 0x2) + out[0];
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)v + 0x6) + out[1];
            kv = out[2];
            yv = *(u16 *)((u8 *)v + 0xA) + kv;
        }
        else
        {
            yv = z - 0x40;
        }
        *(s16 *)((u8 *)v + 0xA) = yv;
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if ((p[0x7A] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(c + 0x10) = 0x20;
            c[0xE] = 0x20;
            c[0xC] = 0x20;
            *(u16 *)(c + 0x14) |= 0xC;
            c[0xD] = 0xE0;
            *(s16 *)(c + 0x1E) = 0x7D0;
            *(s16 *)(c + 0x1C) = 0x7D0;
            *(u16 *)(c + 0x14) &= 0xFFFC;
            p[0x7A] |= 0x4;
        }
        t = *(u8 **)(o + 0x60);
        if (t != NULL)
        {
            u = *(u8 **)(t - 0x18);
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - (D_800DDC40[t[0x13]] >> 1);
            e = *(u8 **)(o - 0x14);
            p[0xBA] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xBB] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            p[0x7B] = __builtin_abs(*(s8 *)(o + 0x72) == e[0x24] ? *(s8 *)(o + 0x73) - e[0x25]
                                                                 : *(s8 *)(o + 0x72) - e[0x24]) *
                          2 -
                      1;
        }
        else
        {
            k = *(u16 *)(o + 0x88) - 0x50;
            p[0x7B] = 0x20;
            *(s16 *)(p + 0x78) = k;
        }
        v->unk_0C = tbl[*(s16 *)(p + 0x7E)].a << 16;
        v->unk_10 = tbl[*(s16 *)(p + 0x7E)].b << 16;
        v->unk_14 = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x8A) = *(s8 *)(p + 0x7B) * *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x82) = 0;
        *(s16 *)(p + 0x90) = 0;
        *(s16 *)(p + 0x92) = 0;
        *(s16 *)(p + 0x94) = 5;
        *(s16 *)(p + 0x96) = 0xC8;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x88) = *(s8 *)(p + 0x7B);
        break;
    case 2:
        for (i = 0; i < 8; i++)
        {
            func_s39_80025228((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0x40E020,
                              (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(o + 0x60)) != 0)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0;
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
            break;
        }
        y = *(u16 *)(c + 0x1A);
        y += 0x400;
        if (y > 0x1000)
        {
            y -= 0x1000;
        }
        *(s16 *)(c + 0x1A) = y;
        if (*(s16 *)(p + 0x90) == 0)
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + *(u16 *)(p + 0x96);
        }
        else
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - *(u16 *)(p + 0x96);
        }
        *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 1;
        if (*(s16 *)(p + 0x92) >= *(s16 *)(p + 0x94))
        {
            *(s16 *)(p + 0x92) = 0;
            *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
        }
        p[0x7B] = p[0x7B] - 1;
        if (*(s8 *)(p + 0x7B) <= 0)
        {
            if (*(u8 **)(o + 0x60) != NULL)
            {
                *(s16 *)(p + 0xA) = 3;
                *(s16 *)(p + 0x82) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                *(s16 *)((u8 *)v + 0xA) =
                    *(u16 *)(u + 0xA) - D_800DDC40[(*(u8 **)(o + 0x60))[0x13]];
                func_800A56E0(0x300);
                n2 = func_8003FC64(0x212);
                if (n2 != NULL)
                {
                    *(s16 *)((u8 *)n2 + 0x22) = 0x78;
                    n2->unk_10 = (u32)func_s39_80025348;
                    func_8004491C(n2, (u32)D_80045340);
                    r = (u8 *)n2->unk_0C;
                    *(u16 *)(r + 0x14) |= 0xC;
                    *(s16 *)(r + 0x10) = 0;
                    *(s16 *)(r + 0x6) = 0;
                    *(u16 *)(r + 0x14) |= 0x80;
                    u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                    w = (u8 *)n2->unk_08;
                    *(s32 *)(w + 0x0) = *(s32 *)(u + 0x0);
                    *(s32 *)(w + 0x4) = *(s32 *)(u + 0x4);
                    *(s32 *)(w + 0x8) = *(s32 *)(u + 0x8);
                    r = (u8 *)n2->unk_0C;
                    *(s16 *)(r + 0x1E) = 0x1000;
                    *(s16 *)(r + 0x1C) = 0x1000;
                    r[0xE] = 0x80;
                    r[0xD] = 0x80;
                    r[0xC] = 0x80;
                    *(UnkDngS58Name12 *)((u8 *)n2 + 0x3A) = *(UnkDngS58Name12 *)D_80026680;
                    *(u32 *)(r + 0x8) = (u32)((u8 *)n2 + 0x3A);
                }
                n3 = func_8003FC64(0x212);
                if (n3 != NULL)
                {
                    q = (u8 *)n3 + 0x20;
                    *(s16 *)(q + 0x2) = 0x50;
                    *(s16 *)(q + 0xA) = 0x14;
                    *(s16 *)(q + 0x4) = 0;
                    *(u32 *)(q + 0x28) = (u32)o;
                    *(u32 *)(q + 0x2C) = *(u32 *)(o + 0x60);
                    *(u32 *)(q + 0x30) = (u32)p;
                    *(u32 *)(q + 0x34) = (u32)((u8 *)n2 + 0x20);
                    for (i = 0; i < 0x60; i++)
                    {
                        q[0x38 + i] = 0;
                    }
                    *(s16 *)(q + 0x9A) = 0;
                    n3->unk_10 = (u32)func_s39_80025398;
                    func_8004491C(n3, (u32)D_80045340);
                    r = (u8 *)n3->unk_0C;
                    *(u16 *)(r + 0x14) = 0xC;
                    *(s16 *)(r + 0x10) = 0x60;
                    *(s16 *)(r + 0x6) = 6;
                    *(u16 *)(r + 0x14) |= 0x80;
                    w = (u8 *)n3->unk_08;
                    *(s32 *)(w + 0x0) = v->unk_00;
                    *(s32 *)(w + 0x4) = v->unk_04;
                    *(s32 *)(w + 0x8) = v->unk_08 + 0x400000;
                    r = (u8 *)n3->unk_0C;
                    *(s16 *)(r + 0x1C) = 0x2000;
                    *(s16 *)(r + 0x1E) = 0x1800;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    r[0xC] = 0;
                    *(UnkDngS58Name12 *)(q + 0x1A) = *(UnkDngS58Name12 *)D_80026674;
                    *(u32 *)(r + 0x8) = (u32)(q + 0x1A);
                }
            }
            else
            {
                *(s16 *)(p + 0xA) = 8;
                *(s16 *)(p + 0x82) = 0;
            }
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
            *(s16 *)(c + 0x1E) = 0;
            *(s16 *)(c + 0x1C) = 0;
        }
        else
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
        break;
    case 3:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) == 4)
        {
            n3 = func_8003FC64(0x212);
            if (n3 != NULL)
            {
                q = (u8 *)n3 + 0x20;
                *(u32 *)(q + 0x28) = (u32)o;
                *(u32 *)(q + 0x2C) = *(u32 *)(o + 0x60);
                *(u32 *)(q + 0x30) = (u32)p;
                for (i = 0; i < 0x60; i++)
                {
                    q[0x38 + i] = 0;
                }
                *(s16 *)(q + 0x9A) = 0;
                n3->unk_10 = (u32)func_s39_80025648;
                func_8004491C(n3, (u32)D_80045340);
                r = (u8 *)n3->unk_0C;
                *(u16 *)(r + 0x14) &= 0xFFF3;
                *(s16 *)(r + 0x10) = 0x20;
                *(u16 *)(r + 0x14) |= 0x80;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                w = (u8 *)n3->unk_08;
                *(s32 *)(w + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(w + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(w + 0x8) = *(s32 *)(u + 0x8);
                r = (u8 *)n3->unk_0C;
                *(s16 *)(r + 0x1E) = 0x1000;
                *(s16 *)(r + 0x1C) = 0x1000;
                r[0xE] = 0x80;
                r[0xD] = 0x80;
                r[0xC] = 0x80;
                *(UnkDngS58Name12 *)(q + 0x1A) = *(UnkDngS58Name12 *)D_s39_80026668;
                *(u32 *)(r + 0x8) = (u32)(q + 0x1A);
            }
        }
        if (*(s16 *)(p + 0x82) == 4)
        {
            *(s16 *)(p + 0xA) = 4;
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0x9C) = 0;
        }
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if ((*(s16 *)(p + 0x82) & 3) == 0)
        {
            if (*(s16 *)(p + 0x82) < 0x50)
            {
                n3 = func_8003FC64(0x212);
                if (n3 != NULL)
                {
                    q = (u8 *)n3 + 0x20;
                    *(s16 *)(q + 0x2) = 0x14;
                    *(s16 *)(q + 0xA) = 0xA;
                    *(s16 *)(q + 0x4) = 0;
                    n3->unk_10 = (u32)func_s39_8002558C;
                    func_8004491C(n3, (u32)D_80045340);
                    r = (u8 *)n3->unk_0C;
                    *(s16 *)(r + 0x10) = 0x20;
                    *(u16 *)(r + 0x14) |= 0xC;
                    u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                    w = (u8 *)n3->unk_08;
                    *(s32 *)(w + 0x0) =
                        *(s32 *)(u + 0x0) + (((rand() & 0x1F) + 0x10) * func_s39_8002512C() << 16);
                    *(s32 *)(w + 0x4) =
                        *(s32 *)(u + 0x4) + (((rand() & 0x1F) + 0x10) * func_s39_8002512C() << 16);
                    *(s32 *)(w + 0x8) = *(s32 *)(u + 0x8) -
                                        (D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] << 15) -
                                        ((rand() & 0x1F) << 16);
                    x = *(u8 **)(*(u8 **)(o + 0x60) - 0x14);
                    r = (u8 *)n3->unk_0C;
                    *(s16 *)(r + 0x1C) = *(u16 *)(x + 0x1C) >> 1;
                    *(s16 *)(r + 0x1E) = *(u16 *)(x + 0x1E) >> 1;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    r[0xC] = 0;
                    *(UnkDngS58Name12 *)((u8 *)n3 + 0x3A) = *(UnkDngS58Name12 *)D_s39_80026668;
                    *(u32 *)(r + 0x8) = (u32)((u8 *)n3 + 0x3A);
                }
            }
        }
        if (*(s16 *)(p + 0x82) == 0x50)
        {
            if (func_8009D218(*(u8 **)(o + 0x60), 4, o) == 0)
            {
                j = (func_800A6D30() & 3) + 4;
                h = (p[0x9] >> 2) + j;
                func_800C8A3C(*(u8 **)(o + 0x60), D_800E3D68 == 0xFF ? 0xFF : 0x10, h);
            }
        }
        if (*(s16 *)(p + 0x82) >= 0x78)
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x9C) != 0)
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0xB)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            ((s16 *)p)[0x41] = ((u16 *)p)[0x41] - 1;
            if (D_800266BC == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_800266BC = 0;
            }
        }
        break;
    }
}
