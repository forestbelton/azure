#include "common.h"

s32 rand(void);

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;

typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_s42_800243A4(u8 *p, UnkVec *v);

UnkNode *func_8003FD64(s32, void *);

s32 func_800BCB04(s32, s32, s16);
extern s16 D_80025630;
typedef struct UnkDng24094SP
{
    u8 pad_00[0x8];
    Unk3DBD0W unk_08;
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    u8 pad_18[0x8];
    u8 *unk_20;
    u16 unk_24;
    u8 pad_26[0xA];
    Unk3DBD0W unk_30;
    Unk3DBD0W unk_34;
    Unk3DBD0W unk_38;
    u8 pad_3C[0x34];
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    u8 pad_76[0x2];
    s16 unk_78;
    s16 unk_7A;
    s16 unk_7C;
    u8 pad_7E[0x2];
    s16 unk_80;
    s16 unk_82;
    s16 unk_84;
    u8 pad_86[0x2];
    s16 unk_88;
    s16 unk_8A;
    s16 unk_8C;
    u8 pad_8E[0x2];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u16 unk_F0;
    u16 unk_F2;
    u16 unk_F4;
    u16 unk_F6;
    u16 unk_F8;
    u16 unk_FA;
    u16 unk_FC;
    u16 unk_FE;
    u16 unk_100;
    u16 unk_102;
    u16 unk_104;
    u16 unk_106;
    u16 unk_108;
    u16 unk_10A;
} UnkDng24094SP;

extern u8 D_s42_80024004[];
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800A6DA4(u32, u32);
s32 func_800A2424(u8 *, s16);
void func_80099844(u8 *, u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
extern u8 D_s42_80024038[];
extern u8 D_s42_80024040[];
extern s32 D_80025638[];
extern u8 D_80025648[];
extern u8 D_80025678[];
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
s32 func_s42_80024590(s32 n);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;
extern const u8 D_s42_80024064[];
extern u8 D_80025624[];
extern u8 D_s42_80025618[];
UnkNode *func_8003FC64(s32);
extern u8 D_80045340[];
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800E3D68;
extern s32 D_8008346C;
void func_s42_800246B4(u8 *p, UnkVec *q, u8 *r);

/* The slot's "use the item on me" responder: refuse if func_8009D218 does,
   then if func_800A6DA4(0, p[0x3])'s roll less 0x10 falls under the caller's
   strength `b' -- or `b' is 0xFF -- give func_800A2424 a veto, and otherwise
   print D_s42_80024004 and tell func_800B4C7C 0x53 with -1. 42 words, 0
   differing, 0 aligned differences, on the FIRST COMPILE. raw 8 == the
   relocation count: 5 R_MIPS_26 and 2 HI16 + 1 LO16 on D_s42_80024004.

   THE ROLL IS TRUNCATED TO `s16' AND THAT IS THE ONE LOAD-BEARING SPELLING.
   `s32 x' is 40 words and 29 differing: the `sll'/`sra' pair after the
   `addiu $v0, -0x10' is the HImode truncation of the subtraction, and
   without it the comparison runs on the full word and the whole tail
   re-schedules. func_800A6DA4 is declared `s32' project-wide, so the
   narrowing has to be written here.

   TWO SPELLINGS MEASURED AND FREE, so neither is the reason this lands:
   * the local -- `if ((s16)(func_800A6DA4(0, p[0x3]) - 0x10) < b || ...)'
     inlined is a tie at 42 words / 0 differing;
   * the ARITY. $a2 is never set in this body, so the third parameter is
     passed straight through to func_8009D218 as in csrc/ovl_dng_s41.c's
     func_s41_8002405C -- but a two-argument alias also measures 42 / 0 here,
     where s41's header prices its own two-argument form at 1 word short.
     The three-parameter form is kept because the callee's arity really is
     three, not because it is worth a word. A twin's arity fact does not
     transfer.

   The donor is func_s41_8002405C / func_s40_8002407C (dngcallee, 3 of 6
   symbols, clean gap); dngtwin found nothing at 0.70. Both donors save
   s2,s0,ra,s1 at -0x20 where this body saves only s0,s1,ra, which is the
   whole delta: they compute a modulus into a fourth value and this one does
   not. */
void func_s42_800240AC(u8 *p, u8 b, u8 *q)
{
    s16 x;

    if (func_8009D218(p, 4, q) != 0)
    {
        return;
    }
    x = func_800A6DA4(0, p[0x3]) - 0x10;
    if (x < b || b == 0xFF)
    {
        if (func_800A2424(p, 1) != 0)
        {
            return;
        }
    }
    func_80099844(p, D_s42_80024004);
    func_800B4C7C(0x53, p, -1, 1);
}

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with the tile's colour word
   copied off the payload at +0x8 and then faded: each of the three bytes is
   scaled by the payload's s16 life counter at +0x32 over 0x100. The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D5AC0 (120 words, resident), which is
   this function without the fade; the relink moves every scratchpad slot down
   by 4 -- the input SVECTOR to +0x00, the OT cursor to +0x18, the OT base to
   +0x20, RotTransPers' two out-parameters to +0x90/+0x94 and the OT index to
   +0xC0 -- so the struct is this unit's own, and the input vector is the
   scratchpad base itself, which is why $a0 is a bare register copy.

   THE FADE REPLACES THE TWIN'S DEAD setRGB0 ECHO. func_800D5AC0 needs
   setRGB0(t, t->r0, t->g0, t->b0) to keep three loads whose stores cse
   strips; here the three lbu/mult/sb runs are the real work and the echo must
   NOT be written -- added in front of the tag setup it is 151 words and 76
   differing.

   `/ 0x100' AND NOT `>> 8': the bgez/addiu 0xFF pair ahead of each sra is the
   signed division's correction, and `>> 8' drops it -- 139 words, 102
   differing.

   THE SCALE FACTOR IS RE-READ AT ALL THREE CHANNELS. A local for
   *(s16 *)(p + 0x32) is 145 words and 109 differing; the target has three
   separate `lh 0x32($s3)'.

   Three orderings, every one re-measured here rather than taken from the
   twin: the colour word and its three scalings come BEFORE the tag setup
   (setTile1 first is 147 words and 111 differing); `sp->unk_18' is assigned
   BEFORE `sp->unk_20' even though the target STORES them the other way round
   (swapped, 11 differing, against the twin's 20 for the same pair); and
   `p = (u8 *)n + 0x20;' comes before `v = n->unk_08;' (swapped, 17
   differing, against the twin's 4).

   setTile1(t) plus setSemiTrans(t, 1) is the whole `sb 2, 0x3' /
   `sb 0x6A, 0x7' pair: cse folds the getcode back to the 0x68 setTile1 just
   stored and drops the dead store. `(SVECTOR *)&sp->unk_00' written for
   `(SVECTOR *)sp' is a tie at 0.

   148 words, 0 differing. raw 7 == relocdump's 7, none on a function symbol:
   1 HI16 + 2 LO16 on D_80083160, R_MIPS_26 on RotTransPers, GetTPage and
   func_80067F20, and one on .text for the loop's own backward `j'. The
   +0x90/+0x94 addresses that splat auto-names D_1F800090/D_1F800094 carry no
   relocation on this side and come out byte-identical, so seqcheck counts one
   extra entry in the target and reports seq=FAIL on that bookkeeping alone. */
s32 func_s42_80024154(u8 *p, UnkVec *v)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_18;
        sp->unk_18 = (u8 *)(t + 1);
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            t->r0 = t->r0 * *(s16 *)(p + 0x32) / 0x100;
            t->g0 = t->g0 * *(s16 *)(p + 0x32) / 0x100;
            t->b0 = t->b0 * *(s16 *)(p + 0x32) / 0x100;
            setTile1(t);
            setSemiTrans(t, 1);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* One frame of the 0x211 spark func_s37_80024FA0 spawns: pin D_80025630 to 1,
   and while the mover's +0xA is still below the ground height func_800BCB04
   reports two units above it, climb +0x8 by 0x20000 plus a fresh
   rand() & 0xFFF; the payload's +0x32 countdown falls by 8 a frame and raises
   the node's redraw bit and D_800814A0's at zero. 51 words, 0 differing.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_80025630
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written with the loads inside the
   call it is 51 words and 6 differing, every row the `li 1' / `sh' pair and
   the three `lhu's trading places: gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_80025630)' store from `mem:HI ($s0 + 10)', so each
   argument load carries the store in its dependency list, and calls.c emits
   a call's argument register loads at the very END of its expansion -- the
   only way to get them above the store is to evaluate them in an earlier
   statement. The shape is csrc/ovl_dng_s18.c's func_s18_80024FF0, which
   documents it for the 48-word relative of this family; the bearing test
   itself is csrc/ovl_dungeon_800CABA8.c's func_800CAED0 (67 words) with a
   one-axis rand() climb in place of its three-axis glide and 8 for its 4.
   raw 7 == relocdump's 7: 2 on D_80025630, 3 on D_800814A0, 2 R_MIPS_26. */
void func_s42_800243A4(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80025630 = 1;
    r = func_800BCB04(x, y, z + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_08 += 0x20000 + (rand() & 0xFFF);
    }
    if ((*(s16 *)(p + 0x32) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset
   less 0x10, then park func_s42_800243A4 as the step and
   func_s42_80024154 as the draw, with `b` at the payload's +0x14, `d` at
   +0x32 and `c` at +0x8.  72 words, 0 differing.  The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D6AD4 -- the same 0x211 spawner
   without the jitter.

   THE THREE STACK OFFSETS ARE `s16` AND THE SUBTRACTION WEARS AN `(s16)`
   CAST, and both halves of that are load-bearing:

   * `s32 dx/dy/dz` is 66 words and 71 differing.  An `s32` stack parameter
     keeps its incoming MEM as its rtl and is rematerialised with a
     `lw 0x48($sp)` at its single use; the promoted `s16` goes through a
     pseudo, which register allocation then parks in $s2/$s3/$s4.  The six
     missing words are those three registers' save/restore pairs.  (Same
     mechanism, opposite sign, as func_800D6AD4's own note.)
   * `s16` with a bare `(dx - 0x10)` is 74 words and 20 aligned differences:
     fold's associate step splits `(X + jitter) + (dx + -0x10)` into
     `(X + jitter + dx) + -0x10` and the `addiu` lands on the running sum
     instead of on the parameter.  The NOP_EXPR the cast leaves blocks that
     split, which is the whole of the remaining 20 rows -- with it the
     `andi` falls into the `lhu`'s load-delay slot and the destination's
     `lw 0x8($s0)` moves ahead of the first `addu` on its own. */
void func_s42_80024470(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s42_800243A4;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s42_80024154);
        *(u32 *)(p + 0x8) = c;
    }
}

/* The bank's "random value below n": six masked draws from rand,
   picked by the size of n, each retried until it falls under n. The mask and
   shift pairs are read straight off the listing -- 0xC >> 2, 0xF0 >> 4,
   0xFC >> 2, 0xFF0 >> 4, then 0xFFF and 0xFFFF unshifted -- and the six
   bounds are the `slti' constants 5, 0x11, 0x41, 0x101, 0x1001 and the
   fall-through. xsibs found no twin at ratio 0.70 and nothing in the project
   reads rand this way, so this is plain decompilation.

   Two shape facts read off the listing rather than guessed. The n == 0 test
   shares its `addu $v0, $v1, $zero' with the LAST arm's fall-through at
   .Lb25_80171768, so `r = 0' is initialised before the test and the last
   `do' loop has NO `return' of its own -- the five earlier arms do, which is
   what puts the return-value copy in each of their `beqz' delay slots and a
   bare `nop' in the following `j'. The retry test is a signed `slt', so n and
   r are both s32.

   73 words, 0 differing, on the first compile; no alternative spelling was
   measured, so nothing here is claimed to be load-bearing. No jump table and
   no rodata of its own -- the listing opens at `glabel'. */
s32 func_s42_80024590(s32 n)
{
    s32 r;

    r = 0;
    if (n != 0)
    {
        if (n < 5)
        {
            do
            {
                r = (rand() & 0xC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x11)
        {
            do
            {
                r = (rand() & 0xF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x41)
        {
            do
            {
                r = (rand() & 0xFC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x101)
        {
            do
            {
                r = (rand() & 0xFF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x1001)
        {
            do
            {
                r = rand() & 0xFFF;
            } while (r >= n);
            return r;
        }
        do
        {
            r = rand() & 0xFFFF;
        } while (r >= n);
    }
    return r;
}

/* The bank's seven-state shutter/shuffle step, on a JUMP TABLE
   (jtbl_s42_80024048, 7 `.word's, 7 distinct targets, bound `sltiu 0x7'):
   state 0 parks the func_800B8FC8 renderable off r1 and copies the +0x30
   object's -0x18 mover into `q', states 1..4 walk `q->unk_08' and r's
   +0x1C/+0x1E a step at a time, state 5 re-seeds and Fisher-Yates-shuffles
   the two byte decks D_80025648[0x30] and D_80025678[0x2C] and FALLS THROUGH
   into state 6, which LoadImages 0x30 2x2 tiles off the two decks and kills
   the node at 0x2C. 263 words, 0 differing, 0 aligned differences.

   THE TWO DECK-INIT LOOPS COUNT UP IN THE SOURCE AND DOWN IN THE OUTPUT, AND
   THAT IS THE WHOLE RESIDUE OF THE FIRST COMPILE.

       for (i = 0; i < 0x30; i++) D_80025648[i] = i;   263 words / 0 rows
       for (i = 0x2F; i >= 0; i--) D_80025648[i] = i;  263 words / 6 rows
       d = &D_80025648[0x2F]; ... *d-- = i;            261 words / 7 rows

   `check_dbra_loop' reverses the up-counting loop and strength-reduces the
   store address into a decrementing pointer, so the emitted shape is
   `li $s1, 0x2F' / `lui' / `addiu %lo' / `addiu 0x2F' with `addiu -0x1' at
   the bottom. Writing the source already reversed leaves the address as
   `addu $v0, $s1, $v1' recomputed every iteration -- same 263 words, 6 rows,
   which is the misleading part: the count agrees and only the addressing
   differs. Writing the decrementing POINTER by hand is two words SHORT,
   because `&D_80025648[0x2F]' is a constant address and folds into one
   `addiu $v0, $v0, %lo(D_80025648+0x2F)' where loop.c's own initialisation
   keeps the symbol and the offset in separate insns. Only the up-counting
   source produces the split pair. The four loops BELOW these two -- the two
   shuffles and state 6's LoadImage loop -- are genuinely up-counting and
   recompute `base + i'; they matched from the first compile, so this is not
   a blanket rule about the unit, it is about a loop whose index is also its
   stored value.

   TWO MORE SPELLINGS, PRICED SEPARATELY AND ADDITIVE:
   * `t = *(s16 *)p;' BEFORE `D_80025630 = 1;' is worth 1 word / 5 rows --
     inline as `switch (*(s16 *)p)' it is 264 / 5. Same mechanism as
     func_s42_800243A4 above and as csrc/ovl_dng_s40.c's func_s40_8002499C:
     gcc 2.7.2 cannot disambiguate the `mem:HI (lo_sum D_80025630)' store
     from `mem:HI ($s4)', so the switch index cannot be loaded above the
     store unless an earlier statement evaluates it.
   * `(r1.w >> 1)' and NOT `/ 2' is worth 3 words / 12 rows (266 / 12) --
     the signed-division correction. Both wrong together is 267 / 17.

   The twin is csrc/ovl_dng_s40.c's func_s40_8002499C (also 263 words), which
   has the same `RECT r1; RECT r2; DVECTOR w;' trio copied off this bank's own
   rodata with `lwl'/`lwr' (the source arrays are `u8 []', so alignment 1
   forces the unaligned pair on both sides), the same `w.vx = r1.x +
   (r1.w >> 1)' and the same four-statement kill tail at +0x34/+0x90,
   p-0x2 and D_800814A0. Its `s16 u' counter fact holds here too. dngtwin
   found nothing at 0.70; dngcallee named it at 3 of 9 symbols.

   ARM ORDER CHECKED BY STANDALONE COMPILE, not inferred from the count: the
   emitted .rdata table is seven labels at strictly increasing text
   positions, matching the target's seven increasing addresses .Ls42_80024764,
   80024820, 80024880, 800248A4, 800248C8, 80024904, 800249E4. */
void func_s42_800246B4(u8 *p, UnkVec *q, u8 *r)
{
    RECT r1;
    RECT r2;
    DVECTOR w;
    UnkVec *v;
    s32 i;
    s32 k;
    s32 t;
    s16 u;
    u8 c;

    r1 = *(RECT *)D_s42_80024038;
    r2 = *(RECT *)D_s42_80024040;
    t = *(s16 *)p;
    D_80025630 = 1;
    switch (t)
    {
    case 0:
        w.vx = r1.x + (r1.w >> 1);
        w.vy = r1.y + 0x48;
        func_800B8FC8(*(u8 **)(p + 0x30), (s32 *)&r1, (s32 *)&w, 1, 1);
        v = *(UnkVec **)(*(u8 **)(p + 0x30) - 0x18);
        q->unk_00 = v->unk_00;
        q->unk_04 = v->unk_04;
        q->unk_08 = v->unk_08;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1C) - 0x20;
        *(s16 *)(r + 0x1E) = *(u16 *)(r + 0x1E) + 0x80;
        u = *(u16 *)(p + 0x2) + 1;
        *(s16 *)(p + 0x2) = u;
        if (u >= 0x28)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p = *(u16 *)p + 1;
        }
        break;
    case 1:
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1C) + 0x140;
        q->unk_08 -= 0x140000;
        *(s16 *)(r + 0x1E) = *(u16 *)(r + 0x1E) - 0x500;
        u = *(u16 *)(p + 0x2) + 1;
        *(s16 *)(p + 0x2) = u;
        if (u >= 0x4)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p = *(u16 *)p + 1;
        }
        break;
    case 2:
        q->unk_08 -= 0x100000;
        *(s16 *)(r + 0x1E) = *(u16 *)(r + 0x1E) - 0x200;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1C) + 0x200;
        *(s16 *)p = *(u16 *)p + 1;
        break;
    case 3:
        q->unk_08 += 0x180000;
        *(s16 *)(r + 0x1E) = *(u16 *)(r + 0x1E) + 0x300;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1C) - 0x300;
        *(s16 *)p = *(u16 *)p + 1;
        break;
    case 4:
        q->unk_08 -= 0x80000;
        *(s16 *)(r + 0x1E) = *(u16 *)(r + 0x1E) - 0x100;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1C) + 0x100;
        *(s16 *)p = *(u16 *)p + 1;
        break;
    case 5:
        for (i = 0; i < 0x30; i++)
        {
            D_80025648[i] = i;
        }
        for (i = 0; i < 0x2C; i++)
        {
            D_80025678[i] = i;
        }
        for (i = 0; i < 0x30; i++)
        {
            c = D_80025648[i];
            k = func_s42_80024590(0x30);
            D_80025648[i] = D_80025648[k];
            D_80025648[k] = c;
        }
        for (i = 0; i < 0x2C; i++)
        {
            c = D_80025678[i];
            k = func_s42_80024590(0x2C);
            D_80025678[i] = D_80025678[k];
            D_80025678[k] = c;
        }
        D_80025638[0] = 0;
        D_80025638[1] = 0;
        D_80025638[2] = 0;
        D_80025638[3] = 0;
        *(s16 *)(p + 0x2) = 0;
        *(s16 *)p = *(u16 *)p + 1;
    case 6:
        for (i = 0; i < 0x30; i++)
        {
            r2.x = D_80025648[i] * 2 + 0x340;
            k = i + *(s16 *)(p + 0x2);
            if (k >= 0x2C)
            {
                k -= 0x2C;
            }
            r2.y = D_80025678[k] * 2 + 0x100;
            LoadImage(&r2, (u_long *)D_80025638);
        }
        u = *(u16 *)(p + 0x2) + 1;
        *(s16 *)(p + 0x2) = u;
        if (u >= 0x2C)
        {
            *(s16 *)(*(u8 **)(p + 0x34) + 0x90) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

const u8 D_s42_80024064[] = {
    0x20,
    0x00,
    0x00,
    0x00,
    0x20,
    0x00,
    0x20,
    0x00,
    0x00,
    0x00,
    0x20,
    0x00,
    0xE0,
    0xFF,
    0x20,
    0x00,
    0xE0,
    0xFF,
    0x00,
    0x00,
    0xE0,
    0xFF,
    0xE0,
    0xFF,
    0x00,
    0x00,
    0xE0,
    0xFF,
    0x20,
    0x00,
    0xE0,
    0xFF,
};

/* THE SHUTTER SCRIPT'S MASTER STEP, AND IT OWNS A JUMP TABLE AND THE 0x20-BYTE
   D_s42_80024064 TABLE ABOVE IT. jtbl_s42_80024088: 9 `.word's = 9 cases + 0
   pad, dispatch `sltiu $v0, $v1, 0x9' with no minval subtraction, so the case
   set is 0..8. The entry list in table order:

     case 0  .Ls42_80024BE4      case 5  .Ls42_8002552C
     case 1  .Ls42_80024C58      case 6  .Ls42_800255F0  (NO ARM)
     case 2  .Ls42_80024F08      case 7  .Ls42_800255F0  (NO ARM)
     case 3  .Ls42_80025148      case 8  .Ls42_8002558C
     case 4  .Ls42_80025308

   `.Ls42_800255F0' is both the range-check `beqz' target and the first word of
   the epilogue, so there is no `default:', no `case 6:' and no `case 7:'.
   Seven real arms written 0 1 2 3 4 5 8 -- ascending, which here IS
   entry-address order, case 8's block sitting just above the epilogue -- with
   case 0 falling through into case 1. 722 words, 0 differing, 0 aligned
   differences. raw 56 == the listing's own relocated-field count of 56, so
   every raw difference is an unresolved relocation and none sits on an
   unexpected field.

   DONOR: csrc/ovl_dng_s40.c's LANDED func_s40_80024F10 (966 words), which
   dngtwin ranks at 0.709 and dngcallee names ALONE at 14-of-20 symbols with a
   clean gap. Same `(u8 *p, UnkVec *v, u8 *c)' signature, same
   `u16 out[3]; RECT r; DVECTOR w; UnkDngS58Rec tbl[8];' stack quartet at
   +0x20/+0x28/+0x30/+0x38, same `o'/`n'/`m' head with the tbl copy BEFORE
   `n' and `m', same 9-entry table with cases absent rather than empty, same
   D_80045340 tag block, same `D_8006CCD8[facing]'/`D_8006CCE8[facing]' corner
   pair, same `__builtin_abs(... ? ... : ...) * 2 - 1' stride, and
   D_s42_80024064 is BYTE-IDENTICAL to its D_s40_80024038. Mine is 244 words
   shorter because it has no func_s40_80024DB8-style redraw call in any arm
   and only one spawn in case 3 where the donor has four.

   FOUR FACTS, EACH RE-PRICED ONE AT A TIME FROM THE MATCHING BODY. All four
   are pure schedule/allocation: NONE of them costs a word, so the word count
   was 722 through the entire search and only aligndiff could see them.

   | spelling                                        | words | rows |
   |-------------------------------------------------|-------|------|
   | as kept                                         |  722  |   0  |
   |  case 4's +0x2A gate written `>= 0x2A'          |  722  |   1  |
   |  case 4's g[] triples written 0xE, 0xC, 0xD     |  722  |  20  |
   |  case 5's D_800E3D68 read inline in the `?:'    |  722  |  20  |
   |  case 4's two pointer locals merged into one    |  722  |  31  |

   1. `if (*(s16 *)(p + 0x82) < 0x2A) { v->unk_08 -= 0x20000; }' -- the gate
      runs the subtraction BELOW 0x2A, not above it. One row, the branch
      opcode alone, and the only one of the four that is a logic error rather
      than a spelling.

   2. CASE 4's THREE g[] BYTE BUMPS ARE WRITTEN 0xE, 0xD, 0xC AND EMIT
      0xE, 0xC, 0xD. sched1 schedules the block backward and exchanges the
      last two independent read-modify-write statements, so the source order
      is the REVERSE of the emitted order for the trailing pair -- and
      because jump2 cross-joins the two arms on their last statement, the
      shared `sb' after the `j' tells you which statement is last in the
      SOURCE: it is 0xC in the emitted stream, hence 0xC is written second.
      Transcribing the emitted order is 20 rows. Both arms must agree; they
      share the tail.

   3. CASE 5's `D_800E3D68' MUST BE READ INTO A LOCAL BEFORE THE `&=' STORE.
      `d = D_800E3D68;' ahead of
      `*(u32 *)(*(u8 **)(o + 0x60) + 0x14) &= 0xFFEFFFFF;' is worth 20 rows.
      A load cannot be scheduled above a store it cannot be disambiguated
      from, and the fixed-address `lbu %lo(D_800E3D68)' against
      `0x14($a0)' is exactly that case; the target has the `lbu' ABOVE the
      `sw', which only an earlier statement can produce. Two consequences
      follow and both are visible: the `sw' becomes the last insn before the
      `beq' so dbr steals it into the delay slot (inline, the delay slot is a
      `nop'), and the unified `0xFF' pseudo -- one value serving both the
      comparison operand and the `then' arm -- is pushed off $a1 by the
      0xFFEFFFFF mask and reaches the argument through the target's
      `addu $a1, $a2, $zero'. Inline, that copy does not exist at all.
      NOT the cure, measured: computing the whole ternary into a local ahead
      of the `&=' is 721 words / 49 rows -- one word SHORT, because the
      ternary then ends its own basic block before the `&=' and nothing can
      interleave; and a local for case 5's `*(u8 **)(o + 0x60)' is 722 / 43.

   4. CASE 4's TWO `*(u8 **)(o + 0x60)' POINTERS ARE TWO LOCALS, NOT ONE.
      The `|= 0x10000000' block and the `&= 0xEFFFFFFF' block each need a
      local -- with NEITHER, written as a double dereference at both sites,
      the body is 726 words / 33 rows, because the store to
      `*(u8 **)(o + 0x60) + 0x1C' stops cse reusing the loaded pointer for
      the `- 0x14' read and each block pays a second `lw' and a `nop' -- but
      sharing ONE local between them is 31 rows: the shared pseudo also gets
      more references than case 1's `t', and that rotates case 1's own
      `lw -0x18' / `lbu 0x13' pair as well as both case 4 blocks. In the
      target the two live in $a0 and $v1 while `g', which IS shared, lives in
      $a1 in both. Three locals, not one and not four.

   AND A WORKED INSTANCE OF THE INTERACTION TRAP, since this body is the
   evidence for it: fact 4 measured 22 rows while fact 2 was still wrong and
   31 rows once fact 2 was right. Facts 2 and 3 are additive (20 + 20 = the
   40 measured with both wrong) and fact 4 is not additive with fact 2. Only
   the one-at-a-time prices above, taken against the matching body, are
   quotable. */
void func_s42_80024AD0(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    RECT r;
    DVECTOR w;
    UnkDngS58Rec tbl[8];
    UnkNode *n;
    UnkNode *n2;
    UnkSlot *s;
    u8 *o;
    u8 *m;
    u8 *m2;
    u8 *e;
    u8 *t;
    u8 *t2;
    u8 *t3;
    u8 *u;
    u8 *g;
    u8 *q;
    s32 i;
    s32 k;
    s32 z;
    s32 d;
    s16 j;

    o = *(u8 **)p;
    r = *(RECT *)D_s42_80024038;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s42_80024064;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(UnkDngS58Name12 *)(p + 0x96) = *(UnkDngS58Name12 *)D_80025624;
        *(u32 *)(c + 0x8) = (u32)(p + 0x96);
        D_80025630 = 1;
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out,
                          0) == NULL)
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
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)v + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if ((p[0x7A] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            c[0xE] = 0x14;
            c[0xD] = 0x14;
            c[0xC] = 0x14;
            *(s16 *)(c + 0x10) = 0;
            *(s16 *)(c + 0x1E) = 0x800;
            *(s16 *)(c + 0x1C) = 0x800;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x7A] |= 0x4;
        }
        t = *(u8 **)(o + 0x60);
        if (t != NULL)
        {
            u = *(u8 **)(t - 0x18);
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - (D_800DDC40[t[0x13]] + 0x20);
            e = *(u8 **)(o - 0x14);
            p[0xA2] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xA3] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
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
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x88) = *(s8 *)(p + 0x7B);
        break;
    case 2:
        for (i = 0; i < 4; i++)
        {
            func_s42_80024470((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0x202020,
                              (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        c[0xC] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) * 0x6C / *(s16 *)(p + 0x8A) +
                 0x14;
        c[0xD] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) * 0x6C / *(s16 *)(p + 0x8A) +
                 0x14;
        c[0xE] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) * 0x6C / *(s16 *)(p + 0x8A) +
                 0x14;
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(o + 0x60)) != 0)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0;
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
            *(s16 *)(c + 0x1E) = 0;
            *(s16 *)(c + 0x1C) = 0;
            break;
        }
        p[0x7B] = p[0x7B] - 1;
        if (*(s8 *)(p + 0x7B) <= 0)
        {
            if (*(u8 **)(o + 0x60) != NULL)
            {
                *(s16 *)(p + 0xA) = 3;
                *(s16 *)(p + 0x82) = 0;
                func_800A56E0(0x300);
                *(u32 *)(*(u8 **)(o + 0x60) + 0x14) |= 0x100000;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
            }
            else
            {
                *(s16 *)(p + 0xA) = 8;
                *(s16 *)(p + 0x82) = 0;
                c[0xE] = 0;
                c[0xD] = 0;
                c[0xC] = 0;
                *(s16 *)(c + 0x1E) = 0;
                *(s16 *)(c + 0x1C) = 0;
            }
        }
        else
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
        break;
    case 3:
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        j = *(u16 *)(p + 0x82) + 1;
        *(s16 *)(p + 0x82) = j;
        if (j == 1)
        {
            w.vx = r.x + (r.w >> 1);
            w.vy = r.y + 0x48;
            func_800B8FC8(*(u8 **)(o + 0x60), (s32 *)&r, (s32 *)&w, 1, j);
        }
        if (*(s16 *)(p + 0x82) == 4)
        {
            n2 = func_8003FC64(0x212);
            if (n2 != NULL)
            {
                q = (u8 *)n2 + 0x20;
                *(u32 *)(q + 0x2C) = (u32)o;
                *(u32 *)(q + 0x30) = *(u32 *)(o + 0x60);
                *(u32 *)(q + 0x34) = (u32)p;
                n2->unk_10 = (u32)func_s42_800246B4;
                func_8004491C(n2, (u32)D_80045340);
                s = (UnkSlot *)n2->unk_0C;
                s->unk_10 = 0x20;
                s->unk_06 = 6;
                s->unk_14 |= 0xC;
                m2 = (u8 *)n2->unk_08;
                *(s16 *)(q + 0xA) = 0;
                *(s16 *)(q + 0xC) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(m2 + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(m2 + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(m2 + 0x8) = *(s32 *)(u + 0x8);
                s = (UnkSlot *)n2->unk_0C;
                s->unk_1E = 0x1000;
                s->unk_1C = 0x1000;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n2 + 0x40) = *(UnkDngS58Name12 *)D_s42_80025618;
                s->unk_08 = (u32)((u8 *)n2 + 0x40);
                *(s16 *)(s->unk_08 + 0x4) = GetTPage(2, 1, 0x340, 0x100);
            }
            if (*(s16 *)(p + 0x82) == 4)
            {
                *(s16 *)(p + 0xA) = 4;
                *(s16 *)(p + 0x82) = 0;
                *(s16 *)(p + 0x90) = 0;
            }
        }
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        if (*(s16 *)(p + 0x82) < 0x2A)
        {
            v->unk_08 -= 0x20000;
        }
        if (*(s16 *)(p + 0x82) == 0x51)
        {
            *(s16 *)(c + 0x10) = 0x20;
            c[0xC] = 0x36;
            c[0xD] = 0x36;
            c[0xE] = 0x36;
        }
        if (*(s16 *)(p + 0x82) >= 0x51)
        {
            c[0xC] = (0x64 - *(s16 *)(p + 0x82)) * 0x36 / 0x14;
            c[0xD] = (0x64 - *(s16 *)(p + 0x82)) * 0x36 / 0x14;
            c[0xE] = (0x64 - *(s16 *)(p + 0x82)) * 0x36 / 0x14;
        }
        if (*(s16 *)(p + 0x82) >= 0)
        {
            t2 = *(u8 **)(o + 0x60);
            *(u32 *)(t2 + 0x1C) |= 0x10000000;
            g = *(u8 **)(t2 - 0x14);
            if (*(s16 *)(p + 0x82) >= 0x2B)
            {
                if (*(s16 *)(p + 0x82) < 0x40)
                {
                    g[0xE] = g[0xE] + 0x3;
                    g[0xD] = g[0xD] + 0x3;
                    g[0xC] = g[0xC] + 0x3;
                }
            }
            else if (*(s16 *)(p + 0x82) == 0x29)
            {
                g[0xE] = g[0xE] - 0x3C;
                g[0xD] = g[0xD] - 0x3C;
                g[0xC] = g[0xC] - 0x3C;
            }
        }
        if (*(s16 *)(p + 0x82) >= 0x64)
        {
            t3 = *(u8 **)(o + 0x60);
            *(u32 *)(t3 + 0x1C) &= 0xEFFFFFFF;
            g = *(u8 **)(t3 - 0x14);
            g[0xE] = 0x80;
            g[0xD] = 0x80;
            g[0xC] = 0x80;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x90) != 0)
        {
            d = D_800E3D68;
            *(u32 *)(*(u8 **)(o + 0x60) + 0x14) &= 0xFFEFFFFF;
            func_s42_800240AC(*(u8 **)(o + 0x60), d == 0xFF ? 0xFF : p[0x9], o);
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            ((s16 *)p)[0x41] = ((u16 *)p)[0x41] - 1;
            if (D_80025630 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80025630 = 0;
            }
        }
        break;
    }
}
