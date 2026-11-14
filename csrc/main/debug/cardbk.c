#include "common.h"
#include "main_twin.h"

/* THE 0x80400000 DEBUG TWIN -- cardbk.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x8040062C..0x80400738   .text from 0x804046C8

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main/debug/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */


/* --- cardbk.c's assert pair. --- */
/* 0x8040062C */
const char D_8040062C[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400654 */
const char D_80400654[12] __attribute__((section(".rodata"))) = "cardbk.c\0\0\0\xA2";

/* 0x80400660 " 名前 " */
const char D_80400660[8] __attribute__((section(".rodata"))) = "\x20\x96\xBC\x91\x4F\x20";

/* 0x80400668  the save-slot line template func_80404EA0 memcpy's,
   0x2A bytes of it; u32 because the copy is 4-aligned */
const u32 D_80400668[11] __attribute__((section(".rodata"))) = {
    0x99923909, 0x960AE08B, 0x20AF8C60, 0xF1893509, 0x8A9E8E20, 0x202020D4,
    0x46812020, 0x46812020, 0x0935090A, 0x959A8C39, 0x000000A8};

/* 0x80400694 "不思議な塔" -- D_80408CF8 points here */
const u8 D_80400694[12] __attribute__((section(".rodata"))) = "\x95\x73\x8E\x76\x8B\x63\x82\xC8\x93\x83";

/* 0x804006A0 "魔物の塔" -- D_80408CF4 points here */
const u8 D_804006A0[12] __attribute__((section(".rodata"))) = "\x96\x82\x95\xA8\x82\xCC\x93\x83";

/* 0x804006AC "Ｆから冒険再開" */
const char D_804006AC[16] __attribute__((section(".rodata"))) =
    "\x82\x65\x82\xA9\x82\xE7\x96\x60\x8C\xAF\x8D\xC4"
    "\x8A\x4A";

/* 0x804006BC */
const char D_804006BC[4] __attribute__((section(".rodata"))) = " ";

/* 0x804006C0 "10", three bytes and a pad; func_80404EA0 memcpy's 3 of them.
   u16 because that is what main_twin.h declares, and D_804006C2 -- splat's
   label on the `lb` half of that copy -- is the second halfword here. */
const u16 D_804006C0[2] __attribute__((section(".rodata"))) = {0x3031, 0x0000};

/* 0x804006C4 "メモリーカードに他のゲームのデータが" */
const u8 D_804006C4[40] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xC9\x91\xBC\x82\xCC\x83\x51\x81\x5B"
    "\x83\x80\x82\xCC\x83\x66\x81\x5B\x83\x5E\x82\xAA";

/* 0x804006EC "あります。これ以上セーブできません。" */
const u8 D_804006EC[40] __attribute__((section(".rodata"))) =
    "\x82\xA0\x82\xE8\x82\xDC\x82\xB7\x81\x42\x82\xB1"
    "\x82\xEA\x88\xC8\x8F\xE3\x83\x5A\x81\x5B\x83\x75"
    "\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81\x42";

/* 0x80400714 "あと" -- 82 A0 82 C6 00 and three bytes of pad, ONE object, the
   free-block line's prefix. s32 because func_804053A0's five-byte block move
   needs it 4-aligned; see csrc/main_twin.h and the device comment on
   func_804053A0 in cardinfo.c. D_80400718 is this
   object's tail, not a symbol. */
const s32 D_80400714[2] __attribute__((section(".rodata"))) = {(s32)0xC682A082, 0};

/* 0x8040071C "つデータがセーブできます。" */
const u8 D_8040071C[28] __attribute__((section(".rodata"))) =
    "\x82\xC2\x83\x66\x81\x5B\x83\x5E\x82\xAA\x83\x5A"
    "\x81\x5B\x83\x75\x82\xC5\x82\xAB\x82\xDC\x82\xB7"
    "\x81\x42";


/* func_8004EEBC, and it references nothing outside its own argument. */
void func_804046C8(u8 *p)
{
    u32 *q = *(u32 **) (p + 4);

    if (q != 0)
    {
        if (q[0] != 0)
        {
            *(u32 *) (p + 0x274) = 0;
            *(u32 *) (p + 0x284) = (u32) (p + 0xF8);
        }
        else
        {
            *(u32 *) (p + 0x274) = (u32) (p + 0x80);
            *(u32 *) (p + 0x284) = 0;
        }
    }
}

/* PARK: 42 words, and the closest spelling reaches 38. The body is
 * func_8004EEFC from csrc/unk_1C540.c, which copies the two GPU-primitive
 * templates into the caller's two primitives.
 *
 * What is settled: this block reads the templates as SEVEN SEPARATE SCALARS,
 * not through two array pointers. A pointer into an array keeps one base
 * register and displaces off it (37 words); seven scalars materialise a
 * `lui`/`%lo` pair each, which is what the listing shows, and that is worth
 * one word. splat named these words individually for the same reason, and the
 * shipped build's D_8002E5D8/D_8002E5E8 are this engine build's D_8002D690..
 * and D_8002D6A0..
 *
 * What is not: the remaining four words are LOAD-DELAY NOPS. This block emits
 * `lui`, `lw`, `nop`, `sw` per word; ours interleaves the next word's `lui`
 * into the delay slot. Tried and did not reach it -- `volatile` on the seven
 * sources (no change at 38, since it constrains the loads and not the stores)
 * and `volatile` on the destinations through a cast (overshoots to 44). What
 * is wanted is a spelling where the next load cannot be hoisted across the
 * previous store. */
void func_80404708(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -120;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* func_8004EF90. Its three globals are D_80408CC4, D_80408CCC and D_80408CC8,
 * in the order the body reads them -- the listing's first-use order is the
 * rename. */
u8 *func_804047B0(u8 *p)
{
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80408CC4;
    a = D_80408CCC;
    b = D_80408CC8;
    p[1] |= 2;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004EFF4, the same shape one template along. */
u8 *func_80404818(u8 *p)
{
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80408CD0;
    a = D_80408CD8;
    b = D_80408CD4;
    p[1] |= 2;
    p[0] |= 0x40;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004F068, the third of the template builders, with its three globals
 * as scalars for the same reason as func_804047B0's. */
u8 *func_80404888(u8 *p)
{
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80408CDC;
    a = D_80408CE4;
    b = D_80408CE0;
    p[1] |= 2;
    p[0] |= 0x41;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004F0DC, with ONE MORE INDIRECTION. Where the live structure has the
 * seven primitive pointers inline at p+0x220.., this build holds a POINTER
 * there to a table of them: it loads p[0x220], then reads the entry at 0x0,
 * 0x14, 0x18, 0x4, 0x8, 0xC, 0x10 off that -- the same seven fields, one
 * level down. It also RELOADS p[0x220] for every entry, which is not a
 * spelling choice: each block ends with a store through q, which may alias
 * p[0x220], so the load cannot be cached. Writing the dereference out each
 * time is what reproduces that; a local would not.
 */
void func_804048F8(u8 *p)
{
    u8 *q;

    q = (*(u8 ***) (p + 0x220))[0];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 6;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x10;
    *(u8 **) q = p + 8;
    q = (*(u8 ***) (p + 0x220))[5];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x80;
    q = (*(u8 ***) (p + 0x220))[6];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0x68;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0xF8;
    q = (*(u8 ***) (p + 0x220))[1];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = (*(u8 ***) (p + 0x220))[2];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = (*(u8 ***) (p + 0x220))[3];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
    q = (*(u8 ***) (p + 0x220))[4];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
}

/* func_8004F1E8, with its four primitive blocks ROTATED. The live body runs
 * 0x80/0xFF, then 0x2C with the GetTPage and GetClut pair, then the 0x70/0xE0
 * one, then 0x81 with p[0] = 0x80 inside it. This build runs the 0x81 block
 * FIRST -- without the p[0] = 0x80 -- then the other three in order, and sets
 * p[0] = 0x80 at the end. Reading it off the listing is unambiguous: the
 * entry sets 0x81/0x20/0 before it has touched anything else, and the final
 * `sb $s1, 0x0($s0)` comes after the 0x70/0xE0 stores. Its two calls are
 * GetTPage and GetClut in this engine build, func_80078570 and
 * func_80078640. */
u8 *func_80404A3C(u8 *p)
{
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p += 0xC;
    p[1] = 0x80;
    p[8] = 0;
    p[9] = 0;
    p[0xA] = 0xFF;
    p[0xB] = 0xFF;
    p += 0xC;
    p[1] = 0x2C;
    *(s16 *) (p + 4) = func_80078570(1, 0, 0x380, 0x100);
    *(s16 *) (p + 6) = func_80078640(0, 0x1F5);
    p[8] = 0;
    p[9] = 0;
    p[0xA] = 0xA0;
    p[0xB] = 0x70;
    p += 0xC;
    p[8] = 0x70;
    p[9] = 0xE0;
    p[1] = 0x80;
    p[0xA] = 8;
    p[0xB] = 8;
    p[0] = 0x80;
    return p + 0xC;
}

/* func_8004F2BC. Its five calls are this block's func_804047B0, func_80404818,
 * func_80404888, func_80404A3C and func_804048F8, in that order. */
void func_80404B10(u8 *p)
{
    func_804047B0(p + 0x8);
    func_80404818(p + 0x80);
    func_80404888(p + 0xF8);
    func_80404A3C(p + 0x170);
    func_804048F8(p);
}

/* func_80050B7C. */
/* RETURNS: func_80404C84 stores $v0 straight after the call. */
s32 func_80404B5C(s32 a)
{
    return func_80047358(a, D_80408CE8, 0x7);
}

/* It was parked here at 5 differing for a long time. What landed it was the
 * same thing that landed the other six builders: NO `u32 v` local for the
 * first template word, and the three trailing words read through the
 * one-member-struct spelling so that the store and the load conflict. The
 * park's structural findings stand: this block loads the ptrs POINTER from
 * c[0x88] where the live body has a seven-entry inline table, and every
 * field above it is 0x18 lower, which is exactly what a seven-entry table
 * costs when it becomes one pointer. */
/* The seventh of the template-builder family, and the second (after
 * func_804075D0) whose ptrs is a POINTER at c[0x88] rather than an inline
 * array. shared at c[0x81], ents at c[0x89], prims at c[0xA5]. */
void func_80404B84(s32 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        ((UnkCardPair **)c[0x88])[i] = (UnkCardPair *)&c[0x89 + i * 4];
        c[0xA5 + i * 4] = D_8002D690_a[0];
        c[0xA6 + i * 4] = D_8002D690_a[1];
        c[0xA7 + i * 4] = D_8002D690_a[2];
        c[0xA8 + i * 4] = D_8002D690_a[3];
        c[0x8A + i * 4] = (s32)&c[0xA5 + i * 4];
        c[0x8B + i * 4] = (s32)&c[0x81];
    }
    c[0x81] = D_8002D6A0.unk_00;
    c[0x82] = D_8002D6A4.unk_00;
    c[0x83] = D_8002D6A8.unk_00;
}

/* From the RESIDENT engine, not the live overlay: this debug build linked
 * libcard's callers statically where retail calls them in the executable, so
 * these bodies come from csrc/unk_1C540.c. Every rename below was read off
 * this block's own listing positionally -- the Nth call in the resident body
 * is the Nth jal here -- which is firmer than the similarity match that found
 * the counterpart. func_8004F3D0. */
void func_80404C3C(u32 *p, u32 v)
{
    *p = v;
}

/* func_8004F3D8. */
void func_80404C44(u8 *p, u32 v)
{
    func_80404C3C((u32 *) p, v);
    func_80404708(p + 0x210, (u32 *) (p + 0x1E8), (u32 *) (p + 0x1F8));
    func_80404B10(p);
}

s32 *func_80404C84(s32 *a, s32 *b)
{
    s32 *q = (s32 *)D_8040B1D0;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040B1D0, 0xC9);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_8040062C, D_80400654, 0x17A);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80404DBC((u8 *)q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x304);
    }
    D_804F281B = 8;
    D_805030EF = 8;
    func_80404B84(r, 7);
    q[3] = (s32)(r + 0x84);
    r[0x87] = func_80404B5C(r[0x88]);
    r[1] = (s32)b;
    func_80404C44((u8 *)r, (u32)a);
    q[4] = (s32)func_804046C8;
    func_804046C8((u8 *)r);
    return q;
}

/* func_8004B500. D_800814A0 is D_8008DAB4 here, by a 24-vote symmap entry,
 * and the local `g` is the live body's -- see main.h's note on the two
 * spellings of that object. */
void func_80404DBC(u8 *p)
{
    u32 g;

    if (p != 0)
    {
        g = D_8008DAB4;
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_8008DAB4 = g | 0x8000;
    }
}

/* Live func_800242D8, MOVED OVER UNCHANGED except for the symbol
 * (D_80027FD0 -> D_80408CF0). It was the headline blocked function -- 28
 * differing, and the triage above records five spellings of the indexed load
 * swept against it, because under 4.1's compiler the load could not be made
 * to fold: GO_IF_LEGITIMATE_ADDRESS rejects a constant address plus a
 * register whenever mips_split_addresses is on, and override_options turns it
 * on unconditionally. tools/psx272gcc has no such pass, so
 * `D_80408CF0[i++]` emits the one insn the assembler expands into
 * `lui $at` / `addu $at,$at,$a3` / `lbu %lo(...)($at)`, and the live body
 * matches with nothing added. Every lever in the live source's own long
 * comment -- the empty third for-clause, `i++` before `p += 0x18` -- still
 * does its work; see csrc/main/cardbk.c. */
u8 *func_80404DEC(u8 *p, s32 w, s32 h)
{
    s32 i;

    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 0;
    p += 0xC;
    for (i = 0; i < 4;)
    {
        p[1] = 0x38;
        *(s32 *)(p + 4) = 0x9CBCBC;
        *(s32 *)(p + 0xC) = 0x9CBCBC;
        *(s32 *)(p + 0x10) = 0x9CBCBC;
        *(s32 *)(p + 0x14) = 0x8C9C9C;
        p[0xA] = w / 2;
        p[0xB] = h / 2;
        p[0] = D_80408CF0[i++];
        p += 0x18;
    }
    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 1;
    p[0] = 0x80;
    return p + 0xC;
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S `.data`, 0x80408CC4..0x80408CF4.

   The twin's data is two runs: the TABLES, 0x804089E0..0x8040922C, which the
   nine units own, and the STATE run behind it, which one object has to hold
   because one object contributes one `.data` run. That is the same shape and
   the same forced split as the live 0x80020000 overlay -- docs/overlays.md,
   "The data IS carved, and a unit can only own the FIRST data run".

   Ownership is by DEFINITION, not by use: link order makes a unit's objects
   contiguous and in unit order, so the referencing units read down the
   address list come out non-decreasing, and where they do not, the object is
   merely USED by the later unit. Applying this partition moves no bytes, so
   byte-identity cannot check it -- it is a claim about the original source.
   ------------------------------------------------------------------------- */

/* 0x80408CC4 */
/* Nine words the unit reads one at a time, D_80071660..D_80071680 in the
   engine build this block was linked against. Each one is its own object
   because each is read as a scalar, which at -G8 is also why each needs the
   section attribute. */
u32 D_80408CC4 __attribute__((section(".data"))) = 0xB8B8AC00;

/* 0x80408CC8 */
u32 D_80408CC8 __attribute__((section(".data"))) = 0x00000000;

/* 0x80408CCC */
u32 D_80408CCC __attribute__((section(".data"))) = 0x0070D0D8;

/* 0x80408CD0 */
u32 D_80408CD0 __attribute__((section(".data"))) = 0xD8D80000;

/* 0x80408CD4 */
u32 D_80408CD4 __attribute__((section(".data"))) = 0x00000000;

/* 0x80408CD8 */
u32 D_80408CD8 __attribute__((section(".data"))) = 0x0080B0B8;

/* 0x80408CDC */
u32 D_80408CDC __attribute__((section(".data"))) = 0xD8D80000;

/* 0x80408CE0 */
u32 D_80408CE0 __attribute__((section(".data"))) = 0x00000000;

/* 0x80408CE4 */
u32 D_80408CE4 __attribute__((section(".data"))) = 0x0080B0B8;

/* 0x80408CE8 */
/* D_800717BC's twin. */
u8 D_80408CE8[0x8] __attribute__((section(".data"))) = {
    0x05, 0x06, 0x01, 0x02, 0x03, 0x04, 0x07, 0x30};

/* 0x80408CF0 */
/* D_80027FD0's twin. */
u8 D_80408CF0[0x4] __attribute__((section(".data"))) = {
    0x00, 0x01, 0x02, 0x03};

