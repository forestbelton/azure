/* PARK: func_s55_80025C80, csrc/ovl_dng_s55.c, agent-h101, 2026-11-24.

   TARGET 449 words. BEST BUILT 449 words, 2 differing (ovlverify) and
   2 ALIGNED DIFFERENCES (tools/decomp/aligndiff.py). 1 member only
   (tools/decomp/dngfam.py: no family), so this is 449 words of its own.
   tools/decomp/relcand.py: ok, 11 data symbols and 10 calls all agreeing
   with the listing.

   THE WHOLE RESIDUE IS ONE TRANSPOSED PAIR, in case 0's tail block, at the
   delay slot of the second gate:

       target   beqz $v0, .Ls55_8002633C / addiu $a1, $zero, 0x8 /
                lui $v0, %hi(D_800E3D7C) / lw $v1, %lo(D_800E3D7C)($v0)
       built    beqz $v0, .Ls55_8002633C / lui $v0, 0x0 /
                addiu $a1, $zero, 0x8    / lw $v1, 0x0($v0)

   Every other word of the 449, every register, every relocation and both
   divisions agree. reorg fills the beqz's delay slot from the TARGET thread
   (nothing before the branch is eligible: the two insns ahead of it both set
   $v0, which the branch reads), so it takes whichever insn sched2 put first
   in the block. The target put the `addiu $a1, $zero, 0x8' there, i.e. the
   constant 8 for the p+0x38 store; this build puts the head of the
   D_800E3D7C load chain there, which is the insn with the LONGER critical
   path and so the higher sched2 priority. Both destinations are one-set
   pseudos, so both are eligible for LAUNCH_PRIORITY through
   birthing_insn_p, and the tie then falls to INSN_LUID -- which is sched1's
   output order and not reachable from the statement order, as the
   measurements below show: putting the 0x38 store FIRST in the block does
   not move it.

   WHAT IT DOES. The 0x212 actor's root step, five states on
   *(s16 *)(p + 0x2C), under a top-level gate on the node pointer stashed at
   the payload's +0x20. With no stashed node it bumps this bank's
   D_800281F8, runs func_s55_8002660C over the +0x32 facing and
   func_s55_80026548 over the slot's element, then dispatches:
   state 0 drags the mover's +0x08 down by 0x10000 a frame, fades the slot's
   three colour bytes toward 0x80 by 1/count, flips the +0x32 parity and on
   the count reaching zero -- and only if the slot raised 0x4000 -- seeds the
   whole flight from D_800E3D7C (+0x88 ceiling, +0x2A direction bits 9..11)
   and the party position bytes at D_80082E80's +0x24/+0x25, and advances;
   state 1 projects D_80082E80's own slot through func_8003DE58 and either
   offsets the mover by the payload's +0x24 triple or snaps it to
   D_80083780, advancing when the count runs out or the projection fails;
   state 2 walks one tile at a time, asking func_800A44E0 whether the next
   tile along the +0x40 heading is passable, easing the mover toward the
   tile centre by 1/count on each axis, clamping the height to the ceiling
   less 0x10, and at count 2 asking func_8009B4B0 for the tile's actor and
   running func_8009CE1C on it; state 0x10 is the ballistic fall -- mover
   plus velocity, velocity bled by a quarter, the colour divided down, and
   at zero it clears D_8002966C and retires. Every arm ends in
   func_s55_8002589C, and with a stashed node the step just mirrors that
   node's mover and slot instead. The tail installs func_800C9034 once.

   NO TWIN. dngtwin.py finds no carved sibling at 0.70 for this stub; xsibs
   is useless on this family (four consecutive s-unit agents, the last
   measuring 0 of 9 stubs at 0.70 against 3,366 landed functions). What
   produced this body is the LISTING plus the callee-set / data-symbol grep:
   func_800A44E0 + func_8009B4B0 + D_800E3D7C + D_80083780 + D_80082E80
   intersect on csrc/ovl_dungeon_8009F644.c and csrc/ovl_dungeon_800C2824.c,
   which carry every prototype used here, and csrc/ovl_dng_s25.c's landed
   656-word func_s25_800247D4 carries the func_800A44E0 argument spellings.

   FACTS MEASURED ON THIS BODY, both spellings priced (aligned differences
   unless the word count is given):

   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | the clamp in case 2 as `b+0xA > ceiling - 0x10'           |  449  |   2  |
   |   ... as `ceiling - 0x10 < b+0xA', the listing's own slt  |  449  |  16  |
   | the D_8006CCD8/E8 reads in the FBC arm as `(u16)t[i]'     |  449  |   2  |
   |   ... as `t[i] & 0xFFFF', s25's own measured spelling     |  449  |  16  |
   |   ... as `*(u16 *)((u8 *)t + i * 2)'                      |  449  |  16  |
   | the D_80082E80 pointer as a SEPARATE local per switch arm |  449  |   2  |
   |   ... one local shared by case 0 and case 1               |  449  |  94  |
   | the two case-2 dividends with BOTH the multiply and the   |       |      |
   |   `- 0x20' hoisted into locals, multiply first            |  449  |   2  |
   |   ... only the `- 0x20' local, multiply inline            |  449  |  10  |
   |   ... neither local, one expression                       |  449  |  12  |
   | func_800A44E0's two coordinates as `tile * 64 + 0x20'     |  449  |   2  |
   |   ... as `(tile << 6) + 0x20'                             |  449  |   4  |
   |   ... with the mask as `& 0xFFE0' not `& 0xFFFFFFE0'      |  449  |   4  |
   | the two F68 velocity stores as `table[i] * 8'             |  449  |   2  |
   |   ... as `table[i] << 3'                                  |  449  |   4  |
   | `*(s16 *)(b+0x16) = 8' AFTER the b+0x12 store             |  449  |   2  |
   |   ... before it                                           |  450  |  14  |
   | `D_800281F8 += 1' AFTER the p+0x20 load                   |  449  |   2  |
   |   ... before it                                           |  450  |  12  |

   Every row above is a SINGLE change against the body below, so the prices
   are independent; the first compile of this body, with the shared local,
   the counter first, the shifts and no dividend locals, was 450 words / 146.

   The last row is this family's split-decrement/global-store ORDER fact and
   it lands on the AFTER side here: the counter's lhu still comes out first
   either way, but only with the statement after the pointer load does the
   `sh %lo(D_800281F8)' sink into the gate's delay slot. There is no
   intervening call.

   MEASURED AND FREE at 449 words / 2 rows, so none of these is why the rest
   of the body agrees: the 0x4000 gate as `s->unk_14' or as a plain cast;
   the +0x32 parity as `(x ^ 1) & 1' or `!(x & 1)'; case 2's b+0x2/0x6/0xA
   accesses as `((s16 *)b)[N]' array refs instead of plain casts; the new
   b+0xA value held in an s16 or an s32 local instead of re-read; case 0's
   two early exits written as two `break's or as one nested `&&' gate; the
   FBC additions with the table read as the first operand; case 0's
   p+0x38 store ahead of the D_800E3D7C read.

   MEASURED AND REJECTED for the transposition specifically, all at 449
   words: `if (1)' round the p+0x38 store 2 (i.e. no effect),
   `do { } while (0)' round it 7, round the three constant stores 450/17,
   round both 8-stores 5, both 8-stores hoisted first 5, the 0x42 store
   first 7, the two 8-stores adjacent 6, a chained
   `p+0x42 = p+0x38 = 8' 6, p+0x42 moved to the end of the block 18,
   D_800E3D7C read inline at both uses 450/27, and the 0x3A/0x40 pair
   emitted after the D_80082E80 pair 39.

   NOT TRIED: the permuter, which this agent is not allowed to run and which
   is exactly the tool for a sched2/sched1 LUID tie -- this is the same class
   as docs/dng-parks/func_s69_80024C38.c's residue and the two should be
   spent together if they are spent at all. And `-da' on this body, to read
   the .sched dumps and confirm which of the two insns is actually promoted;
   the argument above is from gcc 2.7.2's sched.c, not from a dump.

   DECLARATIONS
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   void func_s55_8002660C(u8 *, s16 *, s16, s16);
   void func_s55_800266D0(u8 *, s16);
   void func_s55_80026548(u8 *, UnkElem *);
   void func_s55_8002589C(UnkNode *, s16, s32);
   s32 func_8004491C(UnkNode *, u32);
   s32 func_800A44E0(u16, u16, s16, u32);
   u8 *func_8009B4B0(u8 *, u16, u16);
   void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
   s32 func_800C9034(u8 *, UnkVec *, UnkSlot *);
   extern s16 D_800281F8;
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern u8 *D_800E3D7C;
   extern u8 D_80082E80[];
   extern u16 D_80082E94;
   extern u8 D_80083780[];
   extern s16 D_8002966C;
   extern u8 D_8002966E;
   extern u32 D_80029670;
   END DECLARATIONS
*/
void func_s55_80025C80(u8 *p, UnkVec *b, UnkSlot *s)
{
    UnkNode *o;
    UnkSlot *q;
    UnkVec *m;
    u8 *g;
    u8 *g2;
    u8 *r;
    u8 *r2;
    u8 *r3;
    u8 *e;
    u8 c;
    s32 t;
    s32 t2;
    s32 u;
    s32 u2;

    o = *(UnkNode **)(p + 0x20);
    D_800281F8 += 1;
    if (o == NULL)
    {
        func_s55_8002660C((u8 *)s, (s16 *)(p + 0x32), *(s16 *)(p + 0x38), *(s16 *)(p + 0x36));
        func_s55_80026548(p, s->unk_00);
        switch (*(s16 *)(p + 0x2C))
        {
        case 0:
            b->unk_08 -= 0x10000;
            c = *(u8 *)((u8 *)s + 0xC);
            c = c + (0x80 - c) / *(s16 *)(p + 0x30);
            *(u8 *)((u8 *)s + 0xC) = c;
            *(u8 *)((u8 *)s + 0xE) = c;
            *(u8 *)((u8 *)s + 0xD) = c;
            *(s16 *)(p + 0x32) = (*(u16 *)(p + 0x30) ^ 1) & 1;
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) > 0)
            {
                break;
            }
            if ((s->unk_14 & 0x4000) == 0)
            {
                break;
            }
            r = D_800E3D7C;
            *(s16 *)(p + 0x38) = 8;
            *(s16 *)(p + 0x36) = 0xF;
            *(s16 *)(p + 0x32) = 1;
            *(s16 *)(p + 0x3A) = *(u16 *)(r + 0x88);
            *(s16 *)(p + 0x40) = (*(u16 *)(r + 0x2A) >> 9) & 7;
            g = D_80082E80;
            *(s16 *)(p + 0x3C) = *(u8 *)(g + 0x24);
            *(s16 *)(p + 0x3E) = *(u8 *)(g + 0x25);
            *(s16 *)(p + 0x42) = 8;
            *(s16 *)(p + 0x30) = 3;
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            break;
        case 1:
            g2 = D_80082E80;
            e = func_8003DE58(*(u8 **)(g2 + 0x8), (UnkSlot *)g2, (s16 *)(p + 0x24), 0);
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) <= 0 || e == NULL || (*(u16 *)(g2 + 0x14) & 0x8000) != 0)
            {
                if ((D_80082E94 & 0x8000) != 0)
                {
                    r2 = D_80083780;
                    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(r2 + 0x2);
                    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(r2 + 0x6);
                    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(r2 + 0xA) - 0x10;
                }
                *(s16 *)(p + 0x30) = 0;
                *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            }
            else
            {
                r3 = D_80083780;
                *(s16 *)((u8 *)b + 0x2) = *(u16 *)(r3 + 0x2) + *(u16 *)(p + 0x24);
                *(s16 *)((u8 *)b + 0x6) = *(u16 *)(r3 + 0x6) + *(u16 *)(p + 0x26);
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(r3 + 0xA) + *(u16 *)(p + 0x28);
            }
            break;
        case 2:
            if (*(s16 *)(p + 0x30) == 0)
            {
                if ((s16)func_800A44E0((*(s16 *)(p + 0x3C) * 64 + 0x20) & 0xFFFFFFE0,
                                       (*(s16 *)(p + 0x3E) * 64 + 0x20) & 0xFFFFFFE0,
                                       (s16)(*(u16 *)(p + 0x3A) - 0x20),
                                       (s16)(*(u16 *)(p + 0x40) << 9)) != 0 ||
                    (*(s16 *)(p + 0x42) = *(u16 *)(p + 0x42) - 1) <= 0)
                {
                    *(s16 *)(p + 0x2C) = 0x10;
                    *(s16 *)((u8 *)b + 0xE) = D_8006CCD8[*(s16 *)(p + 0x40)] * 8;
                    *(s16 *)((u8 *)b + 0x12) = D_8006CCE8[*(s16 *)(p + 0x40)] * 8;
                    *(s16 *)((u8 *)b + 0x16) = 8;
                    *(s16 *)(p + 0x30) = 8;
                }
                else
                {
                    *(s16 *)(p + 0x3C) = *(u16 *)(p + 0x3C) + (u16)D_8006CCD8[*(s16 *)(p + 0x40)];
                    *(s16 *)(p + 0x3E) = *(u16 *)(p + 0x3E) + (u16)D_8006CCE8[*(s16 *)(p + 0x40)];
                    *(s16 *)(p + 0x30) = 3;
                }
            }
            u = *(s16 *)(p + 0x3C) * 64;
            t = *(s16 *)((u8 *)b + 0x2) - 0x20;
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u - t) / *(s16 *)(p + 0x30);
            u2 = *(s16 *)(p + 0x3E) * 64;
            t2 = *(s16 *)((u8 *)b + 0x6) - 0x20;
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u2 - t2) / *(s16 *)(p + 0x30);
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + 5;
            if (*(s16 *)((u8 *)b + 0xA) > *(s16 *)(p + 0x3A) - 0x10)
            {
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0x3A) - 0x10;
            }
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) == 2)
            {
                e = func_8009B4B0(D_800E3D7C, *(u16 *)(p + 0x3C), *(u16 *)(p + 0x3E));
                if (e != NULL)
                {
                    func_8009CE1C(e, 0xC, D_8002966E, 9, (s16)(*(u16 *)(p + 0x40) << 9), D_80029670, 5);
                }
            }
            func_s55_8002589C((UnkNode *)(p - 0x20), *(s16 *)(p + 0x40), 0xF0);
            break;
        case 0x10:
            b->unk_00 += b->unk_0C;
            b->unk_04 += b->unk_10;
            b->unk_08 += b->unk_14;
            b->unk_0C -= b->unk_0C >> 2;
            b->unk_10 -= b->unk_0C >> 2;
            if (*(s16 *)(p + 0x3A) - 0x10 < *(s16 *)((u8 *)b + 0xA))
            {
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0x3A) - 0x10;
            }
            *(u8 *)((u8 *)s + 0xC) = *(u8 *)((u8 *)s + 0xC) - *(u8 *)((u8 *)s + 0xC) / *(s16 *)(p + 0x30);
            *(u8 *)((u8 *)s + 0xD) = *(u8 *)((u8 *)s + 0xD) - *(u8 *)((u8 *)s + 0xD) / *(s16 *)(p + 0x30);
            *(u8 *)((u8 *)s + 0xE) = *(u8 *)((u8 *)s + 0xE) - *(u8 *)((u8 *)s + 0xE) / *(s16 *)(p + 0x30);
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) <= 0)
            {
                D_8002966C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
                return;
            }
            if (*(s8 *)((u8 *)s + 0x4) == *(s16 *)(p + 0x36))
            {
                *(s16 *)(p + 0x32) = -1;
            }
            else if (*(s8 *)((u8 *)s + 0x4) == *(s16 *)(p + 0x38))
            {
                *(s16 *)(p + 0x32) = 1;
            }
            func_s55_8002589C((UnkNode *)(p - 0x20), *(s16 *)(p + 0x40), 0xF0);
            break;
        }
    }
    else if ((o->unk_1E & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    else
    {
        m = o->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA);
        q = (UnkSlot *)(*(UnkNode **)(p + 0x20))->unk_0C;
        func_s55_800266D0((u8 *)s, *(s8 *)((u8 *)q + 0x4));
        func_s55_80026548(p, s->unk_00);
        *(s16 *)((u8 *)s + 0x1C) = *(u16 *)((u8 *)q + 0x1C);
        *(s16 *)((u8 *)s + 0x1E) = *(u16 *)((u8 *)q + 0x1E);
        *(s16 *)((u8 *)s + 0x20) = *(u16 *)((u8 *)q + 0x20);
        *(s32 *)((u8 *)s + 0xC) = *(s32 *)((u8 *)q + 0xC);
        *(s16 *)((u8 *)s + 0x16) = *(u16 *)((u8 *)q + 0x16);
        *(s16 *)((u8 *)s + 0x18) = *(u16 *)((u8 *)q + 0x18);
        *(s16 *)((u8 *)s + 0x1A) = *(u16 *)((u8 *)q + 0x1A);
    }
    if (*(s16 *)(p + 0x2E) == 0)
    {
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_800C9034);
        *(s16 *)(p + 0x2E) = *(u16 *)(p + 0x2E) + 1;
    }
}