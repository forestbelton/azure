/* The remainder of unk_102A8 after the gprel_10308 carve-out. Named for its
 * start offset. */
#include "common.h"

void func_8003D7DC(void);
void func_8003E758(void);
void func_80040AA0(u16);

/* Per-frame billboard emitter. Sets up a translate+rotate matrix in the
 * scratchpad, then walks D_800777AC -- 0xC-byte entries, ending when the sign
 * bit of unk_00 is set -- building one 0x28-byte textured quad per entry into
 * the OT buffer at D_80083160.unk_00 + 0x8D0.
 *
 * Load-bearing shapes, with the cost of reverting each:
 *
 *  - **`s16 a; s16 b;` for the two corner accumulators, not `s32`** (8 words).
 *    This is what the function was parked on for two sessions. The target's
 *    `lbu` / `sll 24` / `sra 24` is the NATURAL expansion of a byte-to-signed
 *    conversion and `lb` is the optimised one, so the job is to stop combine's
 *    3-insn refold into extendqisi2_internal -- and a HImode consumer does it
 *    for free, because combine then runs force_to_mode with a 16-bit mask down
 *    the chain instead. Both other ways of blocking that refold cost a `move`
 *    this arm has no room for, which is why it looked unreachable. See
 *    docs/matching-levers.md section 7.
 *  - **the two join stores duplicated into all four arms**, with nothing after
 *    the `if` (91 differing words). It costs no words -- jump2 cross-jumps the
 *    identical `sh`/`sh` pair back out -- and it is what keeps the whole chain
 *    in one pseudo. With the join after the `if`, `a`'s HImode assignment
 *    leaves an SImode expression temp, the two `sh` read the temp while the sum
 *    writes `a`'s own pseudo, so there is no anti-dependence and sched1 puts
 *    the `subu` before the stores instead of last, where reorg wants it for the
 *    `j`'s delay slot.
 *  - **the 0xC value re-read from memory** for its own shift rather than taken
 *    from the `v` local (2 words). cse rewrites the redundant load's SET_SRC to
 *    the pseudo that already holds it, and because the shift then writes that
 *    same pseudo it has two sets, so combine will not substitute and flow will
 *    not delete -- which is exactly the target's `addu $v0,$a1,$zero` /
 *    `sll $v0,$v0,8` pair. Taking it from the local collapses to one `sll`.
 *  - **the three scratchpad stores in the order 0x10, 0x14, 0xC.** Only this
 *    permutation produces the copy above; the other five scatter the four `lw`
 *    (measured 220, 220, 220, 221, 223 words).
 *  - **`*(u16 *) ((u8 *) e + 6)` as the +0xE store's source**, not `e->unk_06`
 *    (2 words). Spelled as a struct field, both sides of the dependence are not
 *    in-struct, so true_dependence's differing-flags exception applies and
 *    sched1 lifts the `lhu` up between the `sw 0x10` and the `sll`, filling its
 *    load-delay slot. The cast makes the dependence real and the target's `nop`
 *    appears. Per-site: the neighbouring `e->unk_04` read does not need the
 *    cast, because by then nothing is left in the block to hoist it past.
 *  - **`p->u1 = p->u3--;`** rather than a `u8` temp and `t - 1`
 *    (2 words). For a `u8` lvalue, `x--` builds the decrement constant in the
 *    lvalue's own type and emits `addiu rd,rs,0xFF`; through an int-promoted
 *    temporary it is `addiu rd,rs,-1`.
 *  - **both clamp tests spelled `field > field`** in this polarity (6 and 2
 *    words): one fixes the order of the two `lh` loads, the other a local-alloc
 *    $v0/$v1 assignment between the sum and the comparison operand.
 *  - **`e` declared uninitialised and assigned after `PushMatrix()`** (45
 *    words). A call is a sched1 block boundary, so initialising `e` at its
 *    declaration strands the `lui %hi` / `addiu` pair and the giv init ahead of
 *    the call, where sched1 hoists them to the top of the prologue; assigning
 *    after the call also re-orders the seven register saves to match.
 *  - **separate locals `q`/`r` for the post-call re-loads**, not the clamp
 *    locals (14 words). Reusing `u`/`w` makes one global allocno each and they
 *    get one hard register for the whole body; the target gives the clamp pair
 *    $v0/$v1 and the post-call quartet $v0/$a0/$v1/$a1, which needs separate
 *    pseudos.
 *  - one scratchpad pointer local for the region but **literal constants** for
 *    the libgte matrix arguments, a separate `sz` for 0x1F800094, the four `vz`
 *    stores in descending index order, store-then-overwrite for the two
 *    clamps, the `u`/`w` clamp locals interleaved rather than batched, and the
 *    loop testing before advancing. All measured earlier; see the commit that
 *    landed this. */
void func_8003D0F0(void)
{
    s32 *sz = (s32 *)0x1F800094;
    Unk80083160 *s = &D_80083160;
    POLY_FT4 *p = *(POLY_FT4 **)(D_80083160.unk_00 + 0x8D0);
    u8 *sp = (u8 *)0x1F800000;
    Unk800777AC *e;

    *(s32 *)(sp + 0x48) = 0;
    ((SVECTOR *)(sp + 0x70))[3].vz = 0;
    ((SVECTOR *)(sp + 0x70))[2].vz = 0;
    ((SVECTOR *)(sp + 0x70))[1].vz = 0;
    ((SVECTOR *)(sp + 0x70))[0].vz = 0;
    PushMatrix();
    e = &D_800777AC;
    *(s32 *)(sp + 0x40) = 0x100;
    *(s32 *)(sp + 0x44) = 0xB0;
    ((SVECTOR *)(sp + 0x28))->vx = 0;
    ((SVECTOR *)(sp + 0x28))->vy = 0;
    ((SVECTOR *)(sp + 0x28))->vz = 0;
    TransMatrix((void *)0x1F800050, (void *)0x1F800040);
    RotMatrix((void *)0x1F800028, (void *)0x1F800050);
    SetRotMatrix((void *)0x1F800050);
    SetTransMatrix((void *)0x1F800050);
    for (;;)
    {
        s16 a;
        s16 b;
        s32 u;
        s32 w;
        s32 q;
        s32 r;
        s32 h;
        s32 v;

        u = e->unk_08;
        *(s32 *)(sp + 0x8) = u;
        w = e->unk_0A;
        *(s32 *)(sp + 0x10) = w;
        if (u + w >= 0x100)
        {
            *(s32 *)(sp + 0x10) = w - 1;
        }
        u = e->unk_09;
        *(s32 *)(sp + 0xC) = u;
        w = e->unk_0B;
        *(s32 *)(sp + 0x14) = w;
        if (u + w >= 0x100)
        {
            *(s32 *)(sp + 0x14) = w - 1;
        }
        if (e->unk_00 & 1)
        {
            a = -(s8)e->unk_02;
            ((SVECTOR *)(sp + 0x70))[2].vx = a;
            ((SVECTOR *)(sp + 0x70))[0].vx = a;
            a = a - *(u16 *)(sp + 0x10);
            ((SVECTOR *)(sp + 0x70))[3].vx = a;
            ((SVECTOR *)(sp + 0x70))[1].vx = a;
        }
        else
        {
            a = (s8)e->unk_02;
            ((SVECTOR *)(sp + 0x70))[2].vx = a;
            ((SVECTOR *)(sp + 0x70))[0].vx = a;
            a = a + *(u16 *)(sp + 0x10);
            ((SVECTOR *)(sp + 0x70))[3].vx = a;
            ((SVECTOR *)(sp + 0x70))[1].vx = a;
        }
        if (e->unk_00 & 2)
        {
            b = -(s8)e->unk_03;
            ((SVECTOR *)(sp + 0x70))[1].vy = b;
            ((SVECTOR *)(sp + 0x70))[0].vy = b;
            b = b - *(u16 *)(sp + 0x14);
            ((SVECTOR *)(sp + 0x70))[3].vy = b;
            ((SVECTOR *)(sp + 0x70))[2].vy = b;
        }
        else
        {
            b = (s8)e->unk_03;
            ((SVECTOR *)(sp + 0x70))[1].vy = b;
            ((SVECTOR *)(sp + 0x70))[0].vy = b;
            b = b + *(u16 *)(sp + 0x14);
            ((SVECTOR *)(sp + 0x70))[3].vy = b;
            ((SVECTOR *)(sp + 0x70))[2].vy = b;
        }
        RotTransSV((void *)(sp + 0x70), &p->x0, sz);
        RotTransSV((void *)(sp + 0x78), &p->x1, sz);
        RotTransSV((void *)(sp + 0x80), &p->x2, sz);
        RotTransSV((void *)(sp + 0x88), &p->x3, sz);
        q = *(s32 *)(sp + 0x10);
        r = *(s32 *)(sp + 0x8);
        h = *(s32 *)(sp + 0x14);
        v = *(s32 *)(sp + 0xC);
        *(s32 *)(sp + 0x10) = q + r;
        *(s32 *)(sp + 0x14) = (h + v) << 8;
        *(s32 *)(sp + 0xC) = *(s32 *)(sp + 0xC) << 8;
        *(u16 *)((u8 *)p + 0xE) = *(u16 *)((u8 *)e + 6);
        *(u16 *)((u8 *)p + 0xC) = *(u16 *)(sp + 0xC) + *(u16 *)(sp + 0x8);
        *(u16 *)((u8 *)p + 0x14) = *(u16 *)(sp + 0xC) + *(u16 *)(sp + 0x10);
        *(u16 *)((u8 *)p + 0x16) = e->unk_04;
        *(u16 *)((u8 *)p + 0x1C) = *(u16 *)(sp + 0x14) + *(u16 *)(sp + 0x8);
        *(u16 *)((u8 *)p + 0x24) = *(u16 *)(sp + 0x14) + *(u16 *)(sp + 0x10);
        if (p->x0 > p->x3)
        {
            p->u1 = p->u3--;
        }
        if (p->y0 > p->y3)
        {
            p->v2 = p->v3--;
        }
        setPolyFT4(p);
        /* Not setRGB0: the target writes b0, g0, r0 in that order, and the
         * macro's r0-first order moves $s1's base from p+4 to p+6 and costs
         * 0x17 bytes. */
        p->b0 = 0x80;
        p->g0 = 0x80;
        p->r0 = 0x80;
        DrawPrim(p);
        p++;
        if ((s8)e->unk_00 < 0)
        {
            break;
        }
        e++;
    }
    PopMatrix();
    *(POLY_FT4 **)(s->unk_00 + 0x8D0) = p;
}

void func_8003D468(void)
{
    u8 *base = (u8 *)0x80010000;
    u8 a = base[0x3186];
    u32 b = *(u32 *)(base + 0x3180);
    u8 c = base[0x3184];
    u8 d = base[0x2D6E];
    u16 e = *(u16 *)(base + 0x2094);
    u16 f = *(u16 *)(base + 0x2098);

    bzero(base, 0x5C84);
    /* Both assignments are byte-aligned block moves: 6 bytes comes out as one
     * lwl/lwr pair plus two lb/sb, 10 bytes as two pairs plus two lb/sb. The
     * cast supplies the size and the alignment; D_80080AB4's own declaration
     * is an incomplete array so that its address is compiler-split. */
    *(Unk80080AB4 *)0x8001020C = *(Unk80080AB4 *)D_80080AB4;
    *(Unk8002D5B4 *)0x80010238 = D_8002D5B4;
    base[0x3186] = a;
    *(u32 *)(base + 0x3180) = b;
    base[0x3184] = c;
    base[0x2D6E] = d;
    *(u16 *)(base + 0x2094) = e;
    *(u16 *)(base + 0x2098) = f;
}

/* arg0 is unused; it exists because func_8003D5A4's call site passes 1 and an
   unused register parameter generates no code. */
void func_8003D56C(s32 arg0)
{
    func_80069C90();
    func_80069CE8();
    _bu_init();
    ChangeClearPAD(0);
}

/* Boot-time graphics/pad/CD setup.
 *
 * Load-bearing shapes:
 *  - `Unk80083160 *s = &D_80083160;` as the first statement. The whole address
 *    is materialised up front into a callee-saved register and carried across
 *    every call, even though its first use is 30 instructions from the end.
 *    Reaching the fields as `D_80083160.unk_00` instead costs 105 words: it
 *    rebuilds a lui/%lo pair per access.
 *  - but D_80083160.unk_A8 stays a plain symbol reference. Through the pointer
 *    it becomes 0xA8($s0); the target has its own compiler-split lui/%lo pair,
 *    which splat renders as the known label D_80083208 at that address. So the
 *    function really does mix the two spellings for one object, which is
 *    evidence D_80083208 was a separate global in the original. `s->unk_A8`
 *    costs 16 words.
 *  - the two buffer addresses go in as `(u32) D_801D8D7A`, with those symbols
 *    declared as incomplete arrays. The scalar-plus-& spelling emits the same
 *    eight instructions in the same order but with $v0 and $v1 swapped: a
 *    scalar is one `(set reg (symbol_ref))` the assembler expands to `la`,
 *    while the array gives the compiler-split high/lo_sum pair, which adds a
 *    pseudo per value and reshuffles local-alloc.
 *  - D_80082E60.unk_0B, not a scalar D_80082E6B: the struct is 0x19 bytes,
 *    past -G8, so the store is compiler-split where a scalar gets the
 *    assembler-expanded $at form.
 *  - the 0x80010000 accesses use the out-of-image constant-base-pointer idiom,
 *    as func_8003D468 does three functions earlier. */
void func_8003D5A4(void)
{
    Unk80083160 *s = &D_80083160;
    u8 *base;
    u8 *p;
    s32 i;

    SetVideoMode(0);
    CdInit();
    func_8003E34C();
    ResetGraph(0);
    func_80053F98();
    func_80053DCC(D_80080A94);
    func_80053DF0(D_80080A98);
    func_80053E14(D_80080A9C);
    for (i = 0; i < 0x22; i++)
    {
        D_800830C0[i] = 0xFF;
        D_800830E8[i] = 0xFF;
    }
    InitPAD(D_800830C0, 0x22, D_800830E8, 0x22);
    func_8006A444();
    func_8003D56C(1);
    SetGraphDebug(0);
    func_80064688();
    D_80082E60.unk_0B = 0;
    bzero(D_80081468, 8);
    func_8003D468();
    base = (u8 *)0x80010000;
    *(u32 *)(base + 0x3180) = 0x2A080808;
    D_80081494 = 0x2A080808;
    base[0x3184] = 0x20;
    D_800814AC = 0x20;
    base[0x3186] = 3;
    D_800814A4 = 3;
    *(u16 *)(base + 0x2094) = 5;
    base[0x2D6E] = 0;
    *(u16 *)(base + 0x2098) = 2;
    func_80043674();
    p = D_801C9E40;
    if (s->unk_00 == p)
    {
        p += 0x108D4;
    }
    s->unk_00 = p;
    *(u8 **)(p + 0x8D0) = p + 0x8D4;
    D_80083160.unk_A8 = 0x2C808080;
    func_8003CAC4();
    D_80081478[0] = (u32)D_801D8D7A;
    D_80081478[1] = (u32)D_801E964E;
}

void func_8003D760(void)
{
    /* The rect goes out as two words: four halfword field assignments would
       give four `sh`s, and an aggregate initializer lands in .sdata, which
       this unit's linker script discards. */
    u32 r[2];

    func_8005FFFC();
    func_8004EB30();
    func_8003D7DC();
    r[0] = 0;
    r[1] = 0x2000400;
    ClearImage(r, 0, 0, 0);
    D_8008148C = (u32)&D_80126804;
    D_80081480 = (u32)&D_80126804;
    D_80080A7C = 0x38000;
}

void func_8003D7DC(void)
{
    /* The base has to come out as a lo_sum in a register: a constant address
     * or a scalar's `&` folds the +0x5C into a second `la` instead of the
     * target's single `addiu` off the base. */
    u8 *p = D_801C9E40;

    SetDefDrawEnv(p, 0, 0, 0x140, 0xE0);
    SetDefDrawEnv(&D_801DA714, 0, 0xE0, 0x140, 0xE0);
    SetDefDispEnv(p + 0x5C, 0, 0xE0, 0x140, 0xF0);
    SetDefDispEnv(&D_801DA770, 0, 0, 0x140, 0xF0);
    *(s16 *)(p + 0x1093A) = 0xC;
    *(s16 *)(p + 0x66) = 0xC;
    *(s16 *)(p + 0x1093E) = 0xE0;
    *(s16 *)(p + 0x6A) = 0xE0;
    D_801DA72C[0] = 1;
    *(u32 *)(p + 0x18) = 1;
}

void func_8003D8B0(void)
{
    s32 i;

    func_8003DB4C((int *)&D_80083120, 0x10);
    func_8003DB4C((int *)&D_80082EC0, 0x80);

    /* Must be an indexed store into a real array: only an array reference
     * forms the address as (lo_sum base) + i*4, which is what leaves the
     * strength-reduced +0xC as its own addiu. A pointer form folds it. */
    for (i = 3; i >= 0; i--)
    {
        D_80083110[i] = 0;
    }

    D_80013714 = 0;
    func_800499BC();
    func_8004437C(0x2A, 0);
    func_8004425C(0);
}

__asm__(".sdata\n\t.size D_80080ABC,2\n\t.size D_80080ABE,2\n\t.text");

/* The "wait for the disc to be ready, then restart audio" tick. Returns 1 only
 * on the frame it actually performs the restart.
 *
 * The two $gp objects, D_80080ABC and D_80080ABE, are reached $gp-relatively
 * here while ten other symbols in the same function are absolute -- including
 * seven that sit inside gp +/- 0x8000. This unit is therefore NOT carved and
 * stays at the project default -G0, where everything is absolute, and the two
 * $gp accesses are asked for individually by the bare size record below. That
 * defines nothing: .sdata stays 0 bytes and both symbols stay UND, so the
 * linker script's own assignments resolve them. See the longer note on
 * func_8003F624 in csrc/unk_12DEC.c. Written as a -G8 carve instead, the seven
 * in-window absolutes would all have to be re-declared past -G8 first; the
 * directive is three lines against seven declaration changes.
 *
 * THE ONE SHAPE THAT DECIDED THE FUNCTION is that `g` is INITIALISED IN ITS
 * DECLARATION rather than assigned after the early return. The target
 * materialises `lui %hi(D_80083160)` / `addiu %lo(D_80083160)` in the PROLOGUE,
 * interleaved with the register saves and ahead of the D_80082E6F test; a
 * plain `g = &D_80083160;` after the `if` puts it inside the guarded region
 * instead, which costs 15 words and also exchanges `g` and the unk_10 value
 * between $v1 and $a0. This is matching-levers section 6's one-set pointer
 * local, with the detail that it keeps its `addiu` here rather than collapsing
 * to a bare `lui` as in func_8003CB08 -- because the accesses are at +0x8 and
 * +0x10, so cse keeps the lo_sum base rather than folding `%lo` into each MEM.
 *
 * Also load-bearing: `b = g->unk_08;` is cached for the last TWO tests (the
 * `& 0x100` guard and the `& 0x800` arm) while the three earlier reads of the
 * same field are each re-read -- the target reuses $v1 across exactly those
 * two and reloads for the others. `s16 t` holds the clamped sum so the compare
 * sign-extends the stored register instead of reloading it, which is the
 * `sll 16`/`sra 16` pair. D_80080ABC and D_80080ABE are declared s16 with a
 * `(u16)` cast at the single arithmetic site, section 7's documented pattern:
 * the `lhu` is the cast, the two `lh`s are the declaration.
 *
 * verify.py flags the D_80082E6F word, because `D_80082E60.unk_0F` emits
 * `%lo(D_80082E60)+0xF` where the listing prints `%lo(D_80082E6F)`. Same
 * address, same linked bytes; `make check` is the authority. */
s32 func_8003D92C(void)
{
    Unk80083160 *g = &D_80083160;
    s32 a;
    s32 b;
    s16 t;

    if (D_80082E60.unk_0F & 0x80)
    {
        return 0;
    }
    a = g->unk_10;
    if (a & 0x100)
    {
        if (!(g->unk_08 & 0x800))
        {
            D_80080ABE = 1;
        }
    }
    else if (a & 0x800)
    {
        if (g->unk_08 & 0x100)
        {
            D_80080ABE = 1;
        }
        else
        {
            D_80080ABE = 0;
            D_80080ABC = 0;
            return 0;
        }
    }
    b = g->unk_08;
    if (!(b & 0x100))
    {
        D_80080ABE = 0;
        D_80080ABC = 0;
        return 0;
    }
    if (D_80080ABE != 0)
    {
        if (b & 0x800)
        {
            t = (u16)D_80080ABC + D_80080A84;
            D_80080ABC = t;
            if (t >= 0x169)
            {
                D_80080ABC = 0x168;
            }
        }
        else
        {
            D_80080ABC = 0;
        }
    }
    if (0x78 / D_80080A84 < D_80080ABC)
    {
        D_80080ABE = 0;
        D_80080ABC = 0;
        func_8003F320();
        while (func_80053EF0(4) == 0x100)
        {
            func_8003E758();
            func_80044618(2);
        }
        func_80053DA8(0x74);
        while (func_80053EF0(4) != 0)
        {
            func_8003E758();
            func_80044618(2);
        }
        func_8003F320();
        func_80040AA0(3);
        D_80080A8A = 0;
        D_80080A88 = 0;
        D_80080AA0 = 0;
        D_80082E60.unk_00 &= ~2;
        func_80053DCC(D_80080A94);
        func_80053DF0(D_80080A98);
        func_80053E14(D_80080A9C);
        func_80053DA8(0x7F);
        func_8003D468();
        D_80080A84 = 2;
        return 1;
    }
    return 0;
}
