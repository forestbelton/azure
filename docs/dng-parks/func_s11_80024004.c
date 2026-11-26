/* PARK: func_s11_80024004, csrc/ovl_dng_s11.c, agent-h263, 2026-11-26.

   TARGET 502 words. BEST BUILT 481 words, 227 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py, which counts register-only R rows in that
   total); ovlverify reports 425 masked on the same body, which is the
   position-by-position inflation of a 21-word deficit -- use the aligndiff
   figure.
   1 member, 502 words (tools/decomp/dngfam.py) -- no replication value.
   This unit owns NO jump table (JTCHECK.txt: 0 tables); the four-arm switch
   on p + 0xA is a COMPARE CHAIN in the listing (beq 1, slti 2, beqz 0, then
   beq 2 / beq 3), and this body reproduces that chain exactly.

   VERDICT: LENGTH, and the whole deficit is ONE allocation decision.
   The target SPILLS the `b' parameter to its incoming home slot and this
   body keeps it in $s8. The target prologue has `sw $a1, 0x54($sp)' and
   twelve `lw $t0, 0x54($sp)' reloads (13 words touching 0x54($sp), plus four
   load-delay `nop's the reloads force) -- 17 of the 21 missing words. The
   remaining four sit in the b + 0x2 / 0xE / 0x6 / 0x12 publish block, where
   fold reassociates the +0x20 differently (see the table).
   Everything else is in place: the diff is 33 delete rows against 12 insert
   rows, and every structural row -- opcodes, immediates, displacements,
   branch polarities, call targets, relocated symbols -- already agrees. The
   register renaming through the body (this body p/s/o = $s7/$s5/$s6 against
   the target fp/$s6/$s7) is the downstream wake of that one spill, not an
   independent fault.

   THE MECHANISM, as far as it was established.
   In case 0 the target uses NINE callee-saved registers inside the two-call
   loop: $s0 uy, $s1 ux, $s2 i, $s3 the x sum, $s4 ax, $s5 ay, $s6 s, $s7 o
   and $fp p. `b' conflicts with all nine, there are only nine callee-saved
   registers, so `b' loses and reload homes it. In this body the x sum shares
   a register with ux (they do not conflict -- the sum dies at the mask), so
   the loop needs only EIGHT and $s8 is left over for `b'. Read off -da:
   `;; Register dispositions' for this body is 80 in 23, 81 in 30, 82 in 6,
   83 in 21, 84 in 22, 85/86/90/94/102 in 16, 87/93/101 in 17, 88/98 in 18,
   89 in 19, 97 in 20 -- nine registers, nothing spilled but dx and dy.
   WHAT WAS NOT ESTABLISHED: why the target gives the x sum its own register
   when the two allocnos do not conflict. That is a find_reg scan-order
   question in global.c and it was not resolved. The claim that the x sum is
   the ninth value is an INFERENCE from the target listing, not a measurement.

   WHAT WAS RULED OUT, all measured, all at 481 words:
   * allocation PRIORITY via declaration order. Fifteen permutations of the
     twenty-local declaration block (14 random plus full reverse) all build
     481 words; rows move 227/233/237/240/241/243 but the spill never
     appears. Per docs/matching-levers.md that rules declaration-order
     ranking out as the cause here.
   * loop SHAPE. for/while/do forms, the increment order, the func_800BCB04
     result in a local, the mask inline at the call sites, the mask as a u16
     local: 227-230 rows, all 481 words.
   * the x/y sum variable. Twenty permutations of which scratch local holds
     the two sums (j, gx, gy, h, k crossed pairwise): 224-257 rows, every one
     481 words. `j' for x and `gy' for y is the best at 224 but it is NOT
     banked below, because 224 was measured against a body with the live
     21-word fault and this project has retracted four facts of exactly that
     shape. The body as parked uses gx/gy and reads 227.
   * one scratch local per axis doing all six jobs (loop sum, the dx*i/q[]
     difference, and the h/k multipliers), which is what the target register
     pattern suggests: 227 rows, 481 words.
   * a ninth call-crossing value in the loop, added deliberately as a
     diagnostic: *(s16 *)(s + 0x2A) hoisted into a local is 483/265,
     *(s32 *)(s + 0x60) hoisted is 480/226, both hoisted is 482/271. NONE of
     them spills `b', which is why the mechanism above is marked an
     inference: the simple count does not predict the spill.
   * preamble order and the D_8006CCD8/D_8006CCE8 index spelling: a separate
     local for the >>8 index 241, the mask folded into the index 240, the
     p + 0x50 decrement first 263 (482 words), w computed last 227, the
     array-ref form D_8006CCD8[i * 2] 227.

   MEASURED SPELLING FACTS THAT DID MOVE THIS BODY, in the order they landed,
   each one priced against the body as it stood at that moment -- so each
   figure prices that change PLUS the live spill fault, and only the
   DIRECTION should be trusted:
   * the two loop x/y values must be TWO locals each -- an s32 sum and a
     masked value -- not one. One local is `ori $sN, $sN, 0x20' where the
     target has `addiu $sM, $vN, 0x20', because combine rewrites
     (plus (ashift X 6) 32) into an IOR once nonzero_bits proves the low six
     bits clear (combine.c, the PLUS arm of simplify_rtx, "If we are adding
     two things that have no bits in common"). Storing the sum into a local
     that is referenced in another basic block defeats both the IOR rewrite
     and local-alloc's tie: combine_regs refuses to tie when the SET reg has
     reg_qty == -1, which is what a multi-block local has. 249 rows one local,
     243 two.
   * __builtin_abs on the y value gets its register-to-register copy back for
     free once the sums are split -- the same change, same 243 figure.
   * the inner switch case 1 must assign a LOCAL in both arms and store it
     once, not store in both arms: 240 against 243. jump.c then hoists one
     constant set above the comparison, which is what the target emits
     (`addu $v1, $a1, $zero' in the beqz delay slot).
   * and the arms of that if must be written != / == rather than == / !=:
     238 against 240. Same program, and gcc hoists the OTHER arm each way.
   * the two *(s8 *)(s + 0x72) / 0x73 reads must go through an s32 local:
     228 against 238, and TWO WORDS, which is the one real word fault this
     body had. Written inline the s16 destination narrows the computation and
     combine merges the sign-extension into the shift, giving
     lbu + sll 0x18 + sra 0x12 where the target has lb + sll 6.
   * spawn B e + 0x48 first instead of last: 227 against 228.

   SPELLINGS MEASURED AND REJECTED:
   * the q publish block in the listing emission order (1C/1E, 0x10, 0x0,
     0x14, 0x4, 0x5, 0xC, 0x8) instead of this body order: 241, WORSE by 13.
     Keep 0xC before 0x0 and 0x8 before 0x4/0x5 as parked.
   * the b + 0x2 / 0x6 publish with the +0x20 pinned in a local
     (j = dx * (k * 16) + 0x20): 484 words / 255 rows. THREE WORDS NEARER the
     target and 28 rows worse. k = k * 16 as its own statement is 484 / 230,
     also three words nearer. Both move the word count the right way and the
     rows the wrong way, which is the signature docs/matching-levers.md calls
     a fix that is worse alone and may be necessary in combination -- the
     combination with a spilled `b' was never reachable, so this is the first
     thing to retry if the spill is solved.
   * the trailing form ... + dx * (k * 16) + 0x20 (no parentheses): 481 / 254.
   * the two b + 0xE / 0x12 stores after both position stores: 482 / 231.

   THINGS A LATER READER SHOULD NOT RE-DERIVE.
   * THE DONOR is func_s06_80024004 (csrc/ovl_dng_s06.c, 503 words), found by
     dngcallee.py at 10 of 13 symbols with a clean gap to the 8/13 tier.
     dngtwin found NOTHING for either stub in this unit at ratio 0.70 and
     xsibs was not needed. The donor is the same (u8 *p, UnkVec *b, u8 *c)
     0x312 spawner: same preamble (s, o, w, the D_8006CCD8/D_8006CCE8 facing
     pair out of *(u16 *)(s + 0x2A) >> 8, the p + 0x50 countdown), the same
     __builtin_abs / "if (i < j) i = j" / "p + 0x50 = i * 4" block, the same
     q publish block, and the same case-0xFF teardown (D_8008346C = 0,
     *(u16 *)(p - 0x2) |= 0x8000, D_800814A0 |= 0x8000) which is case 3 here.
     Its frame is -0x58 with ten saves against this body -0x50 with ten, and
     the donor spills `c' where this one spills `b', so NO allocation fact
     transfers from it. Every shape fact did.
   * func_800A4688 has a real definition at csrc/ovl_dungeon_8009F644.c:4167,
     `s32 func_800A4688(u16 px, u16 py, u16 face, u16 ang, s32 flag)', and it
     is declared NOWHERE in include/. That prototype is WRONG for this call
     site: arguments 3 and 4 arrive sign-extended (sll/sra on the
     func_800BCB04 result, lh on *(s16 *)(s + 0x2A)), where u16 parameters
     would emit andi and lhu. The unit-local alias in the DECLARATIONS block
     below is the project fix and it is load-bearing.
   * the three arms of the inner switch on i are 0, 1 and default with
     h/k = 0/2, 0/3 and 1/0; the default also re-reads p + 0x50 and
     subtracts 2, which is why *(s16 *)(p + 0x50) = i * 4 has to be written
     BEFORE the switch even though case 0 overwrites it with 0.
   * the 7-argument func_8009CE1C call in case 2 passes 0x13 and 2 where the
     donor passes 8 and 4; the 7th argument is 2 and the target reuses the
     register that holds *(s16 *)(p + 0xA), which is 2 on that path, so the
     literal and the field read are indistinguishable here.

   relcand.py reports ok on this body: every %hi data symbol and every jal
   target matches the listing. relocheck needs a build and was NOT RUN.

   DECLARATIONS
   s32 rand(void);
   UnkNode *func_8003FD64(s32, void *);
   void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
   s32 func_800A56E0(s32);
   s32 func_800BCB04(s32, s32, s16);
   s32 func_800A4688_s(s32, s32, s16, s32, s32) __asm__("func_800A4688");
   void func_s11_800247DC(u8 *p, UnkVec *b, u8 *c);
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s32 D_8008346C;
   extern u8 D_80083498[];
   extern u32 D_800814A0;
   extern u8 D_800DE9D0[];
   extern u8 D_800DEC28[];
   END DECLARATIONS */

void func_s11_80024004(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q;
    u8 *e;
    UnkNode *n;
    s32 i;
    s32 j;
    s32 dx;
    s32 dy;
    s32 gx;
    s32 gy;
    s32 h;
    s32 k;
    s32 ax;
    s32 ay;
    s32 tx;
    s32 ty;
    s32 ux;
    s32 uy;

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    i = *(u16 *)(s + 0x2A) >> 8;
    dx = *(s16 *)((i & 0xE) + (s32)D_8006CCD8);
    dy = *(s16 *)((i & 0xE) + (s32)D_8006CCE8);
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (*(s32 *)(s + 0x60) == 0)
        {
            ax = 0;
            ay = 0;
            for (i = 0; i < 2; i++)
            {
                gx = ((o[0x24] + ax) << 6) + 0x20;
                gy = ((o[0x25] + ay) << 6) + 0x20;
                ux = gx & 0xFFFF;
                uy = gy & 0xFFFF;
                if ((s16)func_800A4688_s(ux, uy, (s16)func_800BCB04(ux, uy, -0x400),
                                         *(s16 *)(s + 0x2A), *(s32 *)(s + 0x60)) != 0)
                {
                    break;
                }
                ay += dy;
                ax += dx;
            }
            gx = dx * i;
            gy = dy * i;
        }
        else
        {
            q = *(u8 **)(*(s32 *)(s + 0x60) - 0x14);
            gx = q[0x24] - o[0x24];
            gy = q[0x25] - o[0x25];
            if ((*(u16 *)(q + 0x14) & 0x8000) != 0 && (*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s16 *)(p + 0xA) = 2;
                *(s16 *)(p + 0x50) = 0;
                break;
            }
        }
        i = __builtin_abs(gx);
        j = __builtin_abs(gy);
        if (i < j)
        {
            i = j;
        }
        *(s16 *)(p + 0x50) = i * 4;
        switch (i)
        {
        case 0:
            h = 0;
            k = 2;
            *(s16 *)(p + 0x50) = 0;
            break;
        case 1:
            h = 0;
            k = 3;
            if (*(s32 *)(s + 0x60) != 0)
            {
                j = 1;
            }
            else
            {
                j = k;
            }
            *(s16 *)(p + 0x50) = j;
            break;
        default:
            h = 1;
            k = 0;
            *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 2;
            break;
        }
        *(s16 *)((u8 *)b + 0x2) = ((o[0x24] + dx * h) << 6) + (dx * (k * 16) + 0x20);
        *(s16 *)((u8 *)b + 0xE) = dx * 16;
        *(s16 *)((u8 *)b + 0x6) = ((o[0x25] + dy * h) << 6) + (dy * (k * 16) + 0x20);
        *(s16 *)((u8 *)b + 0x12) = dy * 16;
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        *(s16 *)((u8 *)b + 0xA) =
            func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30);
        if (*(s16 *)((u8 *)b + 0xA) >= 0x200)
        {
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA);
        }
        n = func_8003FD64(0x112, D_80083498);
        if (n != NULL)
        {
            q = (u8 *)n->unk_0C;
            n->unk_10 = (u32)func_s11_800247DC;
            e = (u8 *)n + 0x20;
            *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00;
            *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04;
            *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08;
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
            *(s16 *)(q + 0x10) = 0x60;
            *(s32 *)(q + 0xC) = 0xC08080;
            *(u8 **)(q + 0x0) = D_800DE9D0;
            *(u16 *)(q + 0x14) |= 0xC;
            *(s32 *)(q + 0x8) = *(s32 *)(D_800DE9D0 + 0x4);
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(u8 **)e = p;
            *(s16 *)(e + 0x48) = rand() & 3;
            *(s16 *)(e + 0x4E) = (rand() & 3) + 8;
            *(s16 *)(e + 0x4A) = (rand() & 7) + 0xC;
            *(s16 *)(e + 0x4C) = 0;
        }
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(s32 *)(s + 0x60) != 0)
            {
                *(s16 *)(p + 0x50) = 4;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
                j = *(s8 *)(s + 0x72);
                *(s16 *)((u8 *)b + 0x2) = (j << 6) + 0x20;
                b->unk_10 = 0;
                b->unk_0C = 0;
                j = *(s8 *)(s + 0x73);
                *(s16 *)((u8 *)b + 0x6) = (j << 6) + 0x20;
                n = func_8003FD64(0x112, D_80083498);
                if (n != NULL)
                {
                    q = (u8 *)n->unk_0C;
                    n->unk_10 = (u32)func_s11_800247DC;
                    e = (u8 *)n + 0x20;
                    *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00;
                    *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04;
                    *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08;
                    *(s16 *)(q + 0x1C) = 0x2000;
                    *(s16 *)(q + 0x1E) = 0x2800;
                    *(s16 *)(q + 0x10) = 0x20;
                    *(s32 *)(q + 0xC) = 0x606060;
                    *(u8 **)(q + 0x0) = D_800DE9D0;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DE9D0 + 0x4);
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(u8 **)e = p;
                    *(s16 *)(e + 0x48) = *(u16 *)(p + 0x50);
                    *(s16 *)(e + 0x4E) = 8;
                    *(s16 *)(e + 0x4A) = 8;
                    *(s16 *)(e + 0x4C) = 0;
                }
                for (i = 0x3C; i >= 0; i--)
                {
                    n = func_8003FD64(0x312, D_80083498);
                    if (n != NULL)
                    {
                        q = (u8 *)n->unk_0C;
                        n->unk_10 = (u32)func_s11_800247DC;
                        *(s16 *)((u8 *)n->unk_08 + 0x2) =
                            *(u16 *)((u8 *)b + 0x2) + (rand() & 0x3F) - 0x20;
                        *(s16 *)((u8 *)n->unk_08 + 0x6) =
                            *(u16 *)((u8 *)b + 0x6) + (rand() & 0x3F) - 0x20;
                        *(s16 *)((u8 *)n->unk_08 + 0xA) =
                            *(u16 *)((u8 *)b + 0xA) - (rand() & 0x1F);
                        *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x800;
                        *(s16 *)(q + 0x10) = 0x60;
                        *(s32 *)(q + 0xC) = 0x808080;
                        *(u8 **)(q + 0x0) = D_800DEC28;
                        *(u16 *)(q + 0x14) |= 0xC;
                        *(s32 *)(q + 0x8) = *(s32 *)(D_800DEC28 + 0x4);
                        *(u8 *)(q + 0x4) = 0;
                        *(u8 *)(q + 0x5) = 0;
                        *(s32 *)((u8 *)n->unk_08 + 0xC) = dx * (rand() << 3);
                        *(s32 *)((u8 *)n->unk_08 + 0x10) = dy * (rand() << 3);
                        *(s32 *)((u8 *)n->unk_08 + 0x14) = -0x80001 - rand();
                        e = (u8 *)n + 0x20;
                        *(u8 **)e = p;
                        *(s16 *)(e + 0x48) = 2;
                        *(s16 *)(e + 0x4E) = (rand() & 7) + 8;
                        *(s16 *)(e + 0x4A) = 0x10;
                        *(s16 *)(e + 0x4C) = 0;
                    }
                }
            }
            else
            {
                *(s16 *)(p + 0x50) = 4;
                *(s16 *)(p + 0xA) = 3;
            }
        }
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        break;
    case 2:
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(s32 *)(s + 0x60) != 0)
            {
                func_8009CE1C(*(u8 **)(s + 0x60), 0x13, *(u8 *)(p + 0x9), 2, *(s16 *)(s + 0x2A),
                              (u32)s, 2);
            }
            *(s16 *)(p + 0x50) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else if (*(s16 *)(p + 0x50) <= 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
