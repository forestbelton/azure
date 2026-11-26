#include "common.h"

s32 rand(void);

void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_80053EF0(s32);
s32 func_800644B8(s32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A56E0(s32);
void func_800DBA90(u8 *);
void func_s68_800244BC(u8 *p, UnkVec *b, u8 *c);
void func_s68_80024810(u8 *p);
s32 func_s68_80024B20(u8 *p);
void func_s68_80024E4C(u8 *p);
extern u8 D_80045340[];
extern u8 D_80082E80[];
extern u16 D_80082E94;
extern Unk80083462 D_80083460;
extern u8 D_80083498[];
extern u8 D_80083780[];
extern u32 D_800814A8;
extern u8 D_800DEE38[];

/* The bank's six-state cutscene director, a JUMP TABLE on *(s16 *)(p + 0xA)
   over cases 0..5. THIS FUNCTION OWNS jtbl_s68_80024008 (6 words = 6 cases +
   0 pad, dispatch bound 6, minval 0, 6 distinct targets, arms written
   ascending so the entries come out in the listing's address order). The
   range check's `beqz' lands on the function's EPILOGUE, which is NOT one of
   the six entries, so there is no default among them and no `default:' is
   written.

   State 0 raises D_800814A8's +0x102 flag, clears its +0xF4, publishes the
   party record's position into b, asks func_800A05A4 for the escort actor at
   the party's tile and parks it in s + 0x60, copies that actor's (or, when
   there is none, the caller's own record's) +0x24/+0x25 facing pair into
   s + 0x72/+0x73, and FALLS THROUGH into state 1. State 1 waits for 0x80 in
   the record at p + 0x4, decrements D_800814A8's +0xA6 counter, and spawns
   the 0x312 smoke node that runs func_s68_800244BC with D_80045340 as its
   renderable, dressed from D_800DEE38 in 0x606060 and offset by
   func_8003DE58's projection. State 2 fires func_800A56E0 with 0x4300 or
   0x300 off func_80053EF0(4). State 3 spawns, at +0x50 == 7, the 0x302 node
   that gets func_s68_80024810 as its step and func_s68_80024B20 as its
   renderer, then at +0x50 == 4 calls func_8009CE1C on the escort, and
   advances once D_80082E94 carries 0x8000 or the timer goes negative.
   State 4 waits out the timer. State 5 tears the actor down after clearing
   D_80083460's +0xC and decrementing its +0xA. 295 words, 0 differing.

   NO TWIN AT ANY RATIO -- dngtwin reports nothing at 0.70 for this stub.
   Written from the listing plus csrc/ovl_dng_s12.c func_s12_80024020 (the
   same `s = *(u8 **)(p + 0x0); o = *(u8 **)(s - 0x14); w = s - 0x20;'
   skeleton, the same 0x312 dress block and the same case-5 teardown) and
   csrc/ovl_dng_s63.c func_s63_8002401C (the D_800814A8 +0x102/+0xF4 pair,
   the `(**(u16 **)(p + 0x4) & 0x80)' gate, the +0xA6/+0xA8 group and the
   func_80053EF0/func_800A56E0 pair). func_80066460 and func_8006649C are
   GetTPage and GetClut out of splat/common_syms.txt and need no declaration.

   FIVE FACTS, all priced from this body at 295 words / 0 differing. Four of
   the five are about WHICH ARMS SHARE A LOCAL, and they do not all point the
   same way -- that is the result worth carrying:

   * THE D_800814A8 POINTER MUST BE A SEPARATE LOCAL PER ARM: `g' in case 0
     and `h' in case 1. One local for both is 3 aligned differences (the
     second `lw' lands in $v1 and costs a `move $a0, $v1' for the
     func_800A05A4 argument), and case 5's &D_80083460 reusing `g' is 5.
     A second set on the pseudo loses `adjust_priority's single-set boost.
   * BUT THE ACTOR RECORD IN CASE 0 AND THE NODE PAYLOAD `n->unk_0C' IN CASE 1
     MUST BE THE *SAME* LOCAL `q' -- a local of its own for case 0 is 18
     aligned differences. The target spends $a2 on it in both arms, which two
     independent locals are not obliged to agree on.
   * AND THE NODE PAYLOAD POINTER IS PER ARM AGAIN: `e' in case 1, `d' in
     case 3. Sharing one is 23 aligned differences, every row an $a0/$s0
     exchange. So "split" and "share" are both right in this one body and the
     only way to tell is the residue.
   * `*(u16 *)(q + 0x14) |= 0xC;' MUST COME BEFORE `*(u8 **)(q + 0x0) =
     D_800DEE38;'. The other order is 297 words -- TWO WORDS LONG -- and 10
     aligned differences: with the `|=' store sitting between the q + 0x0
     store and the q + 0x0 read, cse stops forwarding the stored symbol and
     the `lw $v1, 0x0($a2)' reload survives.
   * CASE 3's `*(s32 *)(d + 0x34) = *(s32 *)(s + 0x60);' MUST BE THE FIRST
     STORE AFTER `d = (u8 *)n + 0x20'. Written after the +0x3C/+0x3E/+0x30/
     +0x32 group it is 5 aligned differences: the `lw 0x60($s2)' then
     schedules four slots late and reorg pulls a different store into
     GetTPage's delay slot. */
void func_s68_80024020(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *e;
    u8 *d;
    u8 *q;
    u8 *g;
    u8 *h;
    u8 *z;
    u8 *t;
    UnkNode *n;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u8 *)(D_800814A8 + 0x102) = 1;
        g = (u8 *)D_800814A8;
        *(s32 *)(g + 0xF4) = 0;
        *(s32 *)((u8 *)b + 0x0) = *(s32 *)(*(u8 **)(w + 0x8) + 0x0);
        *(s32 *)((u8 *)b + 0x4) = *(s32 *)(*(u8 **)(w + 0x8) + 0x4);
        *(s32 *)((u8 *)b + 0x8) = *(s32 *)(*(u8 **)(w + 0x8) + 0x8);
        t = (u8 *)func_800A05A4(g, D_80082E80[0x24], D_80082E80[0x25], *(s16 *)(g + 0x2A), 7);
        *(u8 **)(s + 0x60) = t;
        if (t == NULL)
        {
            *(u8 *)(s + 0x72) = *(u8 *)(o + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(o + 0x25);
        }
        else
        {
            q = *(u8 **)(t - 0x14);
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        h = (u8 *)D_800814A8;
        *(s16 *)(p + 0x50) = 0xA;
        *(s16 *)(h + 0xA6) = *(u16 *)(h + 0xA6) - 1;
        *(u8 *)(h + 0xA8) = *(u8 *)(p + 0x8);
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            if (func_8003DE58(*(u8 **)(*(u8 **)(w + 0xC) + 0x8), (UnkSlot *)*(u8 **)(w + 0xC), out,
                              0) == 0)
            {
                out[0] = out[1] = out[2] = 0;
            }
            n->unk_10 = (u32)func_s68_800244BC;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n->unk_0C;
            *(s32 *)((u8 *)n->unk_08 + 0x0) = *(s32 *)(*(u8 **)(w + 0x8) + 0x0) + (out[0] << 16);
            *(s32 *)((u8 *)n->unk_08 + 0x4) = *(s32 *)(*(u8 **)(w + 0x8) + 0x4) + (out[1] << 16);
            *(s32 *)((u8 *)n->unk_08 + 0x8) = *(s32 *)(*(u8 **)(w + 0x8) + 0x8) + (out[2] << 16);
            e = (u8 *)n + 0x20;
            *(s16 *)(e + 0xC) = *(u16 *)((u8 *)n->unk_08 + 0x2);
            *(s16 *)(e + 0xE) = *(u16 *)((u8 *)n->unk_08 + 0x6);
            *(s16 *)(e + 0x10) = *(u16 *)((u8 *)n->unk_08 + 0xA);
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
            *(s16 *)(q + 0x10) = 0x20;
            *(u16 *)(q + 0x14) |= 0xC;
            *(u8 **)(q + 0x0) = D_800DEE38;
            *(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0) + 0x4);
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(s32 *)(q + 0xC) = 0x606060;
            *(u8 **)((u8 *)n + 0x20) = p;
            *(s16 *)(e + 0x4C) = 0;
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        if (func_80053EF0(4) != 2)
        {
            func_800A56E0(0x300);
        }
        else
        {
            func_800A56E0(0x4300);
        }
        *(s16 *)(p + 0x50) = 0xA;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 3:
        if (*(s16 *)(p + 0x50) == 7)
        {
            if (*(s32 *)(s + 0x60) != 0)
            {
                n = func_8003FD64(0x302, D_80083498);
                if (n != NULL)
                {
                    n->unk_10 = (u32)func_s68_80024810;
                    func_8004491C(n, (u32)func_s68_80024B20);
                    d = (u8 *)n + 0x20;
                    *(s32 *)(d + 0x34) = *(s32 *)(s + 0x60);
                    *(s16 *)(d + 0x3C) = 0;
                    *(s16 *)(d + 0x3E) = 0;
                    *(s16 *)(d + 0x30) = *(s16 *)(d + 0x32) = 0x1F;
                    *(s16 *)(d + 0x40) = GetTPage(0, 1, 0x2C0, 0x100);
                    *(s16 *)(d + 0x42) = GetClut(0x60, 0x1F8);
                    *(s32 *)(d + 0x38) = 0x404040;
                    *(u8 **)((u8 *)n + 0x20) = p;
                }
            }
        }
        if (*(s16 *)(p + 0x50) == 4)
        {
            func_8009CE1C(*(u8 **)(s + 0x60), 0xA, *(u8 *)(p + 0x9), 0xC,
                          *(s16 *)(D_800814A8 + 0x2A), (u32)s, 2);
        }
        if ((D_80082E94 & 0x8000) != 0 || *(s16 *)(p + 0x50) < 0)
        {
            *(s16 *)(p + 0x50) = 0x14;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else
        {
            z = (u8 *)&D_80083460;
            *(s32 *)(z + 0xC) = 0;
            *(s16 *)(z + 0xA) = *(u16 *)(z + 0xA) - 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The 0x312 smoke node's per-frame step, installed as the +0x10 step by
   func_s68_80024020's state 1, so the three-argument signature is fixed by
   that call site. Counts the +0x48 timer down, raises bit 0x8000 in the
   +0x52 halfword of the record at the actor's +0x0, then runs a three-case
   compare chain on +0x4C. State 0 seeds the timer at 0xA and FALLS THROUGH
   into state 1, which re-projects the party slot through func_8003DE58,
   swings the caller's vector by the projection delta << 14, pulls its z down
   by func_800644B8((+0x48 << 11) / 10) << 9, spawns the 0x302 trail node
   that runs func_s68_80024E4C (its first trail SVECTOR memcpy'd from
   p + 0xC, its last seeded from the caller's vector) and re-arms the timer
   at 8 when it expires. State 2 does the same swing at << 15 and retires the
   actor when the timer runs out. 213 words, 0 differing.

   NO CARVED SIBLING AT dngtwin's 0.70 FLOOR. Written from the listing plus csrc/ovl_dng_s63.c
   func_s63_80024648 for the `+= (out[k] + *(s16 *)(t + 0x2) -
   *(s16 *)(b + 0x2)) << 14' idiom and csrc/ovl_dng_b04.c func_b04_801743EC
   for the `u8' memcpy that gives the lwl/lwr + swl/swr pair with no
   alignment test. func_80066460 and func_80069EF8 are GetTPage and rand out
   of splat/common_syms.txt.

   ONE FACT, priced from this body at 213 words / 0 differing, and it is the
   same mechanism as func_s68_80024020's first fact:

   * THE TWO BANK-TABLE POINTERS NEED A SEPARATE LOCAL PER SWITCH ARM --
     `g'/`t' in case 1 and `g2'/`t2' in case 2. One `g' and one `t' shared by
     both arms is 18 aligned differences: with two sets on each pseudo every
     `la' splits into `lui $v0, %hi' plus `addiu $aN, $v0, %lo' instead of
     staying in one register, and the 0x66666667 division constant and `t'
     exchange $a1/$a2 across the whole arm. Merging `g' and `t' into ONE
     scratch pointer is worse still, 215 words / 24. Permuting the
     declaration order is measured and FREE (18 either way), so this is
     `reg_n_sets', not an allocno priority. */
void func_s68_800244BC(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    u8 *e;
    u8 *g;
    u8 *t;
    u8 *g2;
    u8 *t2;
    s32 i;
    s16 out[4];

    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x48) = 0xA;
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        /* fallthrough */
    case 1:
        g = D_80082E80;
        if (func_8003DE58(*(u8 **)(g + 0x8), (UnkSlot *)g, out, 0) != 0)
        {
            func_800478B8(c);
            t = D_80083780;
            b->unk_00 += (out[0] + *(s16 *)(t + 0x2) - *(s16 *)((u8 *)b + 0x2)) << 14;
            b->unk_04 += (out[1] + *(s16 *)(t + 0x6) - *(s16 *)((u8 *)b + 0x6)) << 14;
            b->unk_08 += (out[2] + *(s16 *)(t + 0xA) - *(s16 *)((u8 *)b + 0xA)) << 14;
            b->unk_08 -= func_800644B8((*(s16 *)(p + 0x48) << 11) / 10) << 9;
            for (i = 0; i >= 0; i--)
            {
                n = func_8003FD64(0x302, D_80083498);
                if (n != NULL)
                {
                    e = (u8 *)n + 0x20;
                    n->unk_10 = (u32)func_s68_80024E4C;
                    *(s16 *)(e + 0x3C) = 0;
                    *(s16 *)(e + 0x3E) = (rand() & 3) + 2;
                    *(s16 *)(e + 0x30) = *(s16 *)(e + 0x32) = 0x1F;
                    *(s16 *)(e + 0x40) = GetTPage(0, 1, 0x2C0, 0x100);
                    *(s32 *)(e + 0x38) = 0x404040;
                    memcpy((u8 *)n + 0x24, p + 0xC, 8);
                    *(s16 *)(e + 0x24) = *(u16 *)((u8 *)b + 0x2);
                    *(s16 *)(e + 0x26) = *(u16 *)((u8 *)b + 0x6);
                    *(s16 *)(e + 0x28) = *(u16 *)((u8 *)b + 0xA);
                    *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
                }
            }
        }
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(s16 *)(p + 0x48) = 8;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        g2 = D_80082E80;
        if (func_8003DE58(*(u8 **)(g2 + 0x8), (UnkSlot *)g2, out, 0) != 0)
        {
            func_800478B8(c);
            t2 = D_80083780;
            b->unk_00 += (out[0] + *(s16 *)(t2 + 0x2) - *(s16 *)((u8 *)b + 0x2)) << 15;
            b->unk_04 += (out[1] + *(s16 *)(t2 + 0x6) - *(s16 *)((u8 *)b + 0x6)) << 15;
            b->unk_08 += (out[2] + *(s16 *)(t2 + 0xA) - *(s16 *)((u8 *)b + 0xA)) << 15;
        }
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The 0x302 trail node's per-frame step that func_s68_80024020's state 3
   installs alongside func_s68_80024B20 as its renderer. Raises bit 0x8000 in
   the +0x52 halfword of the record at the actor's +0x0, re-seeds the trail's
   last SVECTOR at +0x24 from the escort record's projected position and its
   first at +0x4 from D_80083780 offset by func_8003DE58's projection, then
   interpolates the three intermediate 8-byte trail rows between them with a
   rand() & 0x3F jitter, republishes the +0x2C/+0x2E UV pair from the low
   bits of +0x3E, and runs a three-case compare chain on +0x3C: state 0 halves
   the span and FALLS THROUGH into state 1, which resets +0x3E to 2 once it
   reaches 6, and state 2 retires the actor. Every arm then bumps +0x3E.
   196 words, 0 differing.

   NO CARVED SIBLING AT dngtwin's 0.70 FLOOR. The trail loop and the +0x2C/+0x2E publish are
   func_s68_80024E4C's, landed first in this unit; the rest came from the
   listing with csrc/ovl_dng_s12.c func_s12_80024020 case 0 for the
   `*(u16 *)(*(u8 **)(w + 0x8) + 0x2)' group.

   TWO FACTS, both priced from this body at 196 words / 0 differing:

   * `w = *(u8 **)(p + 0x34) - 0x20;' MUST COME BEFORE THE +0x52 FLAG STORE.
     After it the body is 199 words / 8 aligned differences, and no cast
     rescues it -- the array-ref spelling `((u8 **)p)[0xD]' measures the same
     8. `true_dependence' (sched.c:835) only excuses a load against a store
     when the OTHER ref's address does not vary, and this store's address is
     a register, so the load can never be hoisted above it; the two `nop's
     the target fills are the whole cost.
   * BOTH BANK SYMBOLS NEED A POINTER LOCAL. With `*(u16 *)(D_80083780 +
     0x2)' inline the body is 195 words -- ONE WORD SHORT -- and 6 aligned
     differences, because cse anchors the shared base at D_80083780 + 2 and
     reaches +0x6/+0xA as +4/+8 off it, where the target materialises the
     symbol itself and uses 2/6/0xA.

   MEASURED AND FREE at 196 words / 0 differing, so neither is why this
   lands: a `(u16)' cast on the `out[k]' reads in the projection add (free
   because convert.c already narrows the whole sum to unsigned short, which
   is what makes both operands `lhu'), and a cursor local `e = p + i * 8'
   with `*(s16 *)(e + 0x4)' in place of the inline `p + i * 8 + 0x4'. */
void func_s68_80024810(u8 *p)
{
    u8 *w;
    u8 *t;
    u8 *g;
    s32 i;
    s16 out[4];

    w = *(u8 **)(p + 0x34) - 0x20;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    *(s16 *)(p + 0x24) = *(u16 *)(*(u8 **)(w + 0x8) + 0x2);
    *(s16 *)(p + 0x26) = *(u16 *)(*(u8 **)(w + 0x8) + 0x6);
    *(s16 *)(p + 0x28) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x20;
    t = D_80083780;
    *(s16 *)(p + 0x4) = *(u16 *)(t + 0x2);
    *(s16 *)(p + 0x6) = *(u16 *)(t + 0x6);
    *(s16 *)(p + 0x8) = *(u16 *)(t + 0xA);
    g = D_80082E80;
    if (func_8003DE58(*(u8 **)(g + 0x8), (UnkSlot *)g, out, 0) != 0)
    {
        *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + out[0];
        *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + out[1];
        *(s16 *)(p + 0x8) = *(u16 *)(p + 0x8) + out[2];
    }
    out[0] = (*(s16 *)(p + 0x24) - *(s16 *)(p + 0x4)) >> 2;
    out[1] = (*(s16 *)(p + 0x26) - *(s16 *)(p + 0x6)) >> 2;
    out[2] = (*(s16 *)(p + 0x28) - *(s16 *)(p + 0x8)) >> 2;
    for (i = 1; i < 4; i++)
    {
        *(s16 *)(p + i * 8 + 0x4) = *(u16 *)(p + 0x4) + out[0] * i + (rand() & 0x3F) - 0x20;
        *(s16 *)(p + i * 8 + 0x6) = *(u16 *)(p + 0x6) + out[1] * i + (rand() & 0x3F) - 0x20;
        *(s16 *)(p + i * 8 + 0x8) = *(u16 *)(p + 0x8) + out[2] * i - (rand() & 0x3F);
    }
    *(s16 *)(p + 0x2C) = ((*(u16 *)(p + 0x3E) & 3) << 5) + 0x80;
    *(s16 *)(p + 0x2E) = ((s16) * (u16 *)(p + 0x3E) >> 2) << 5;
    switch (*(s16 *)(p + 0x3C))
    {
    case 0:
        *(s16 *)(p + 0x24) = *(u16 *)(p + 0x4) + ((*(s16 *)(p + 0x24) - *(s16 *)(p + 0x4)) >> 1);
        *(s16 *)(p + 0x26) = *(u16 *)(p + 0x6) + ((*(s16 *)(p + 0x26) - *(s16 *)(p + 0x6)) >> 1);
        *(s16 *)(p + 0x28) = *(u16 *)(p + 0x8) + ((*(s16 *)(p + 0x28) - *(s16 *)(p + 0x8)) >> 1);
        *(s16 *)(p + 0x3C) = *(u16 *)(p + 0x3C) + 1;
        /* fallthrough */
    case 1:
        if (*(s16 *)(p + 0x3E) >= 6)
        {
            *(s16 *)(p + 0x3E) = 2;
            *(s16 *)(p + 0x3C) = *(u16 *)(p + 0x3C) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x3E) >= 8)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(s16 *)(p + 0x3E) = *(u16 *)(p + 0x3E) + 1;
}

/* The trail renderer func_s68_80024020's state 3 and func_s68_80024E4C's
   state 0 register through func_8004491C: walk the tag chain, project the
   five SVECTORs at the record's +0x4 (stride 8) through RotTransPers into a
   six-element DVECTOR array, project the record's own +0x4 vector pushed
   0x30 further back into the sixth slot, then hand func_800DBA90 a 0x20-byte
   matrix record that rotates the vector (0, |xy[0].vy - xy[5].vy|, 0) by
   D_80083160's +0xC8 camera angle to get the ribbon's half-extent, and hang
   four semi-transparent POLY_FT4 quads per node on the OT row given by the
   mean of each segment's two depths when that is under 0x1E0. The prim slot
   is 0x28 bytes of D_80083160's +0x8D0 cursor, which is exactly a POLY_FT4.
   203 words, 0 differing.

   TWIN: csrc/ovl_dng_s12.c func_s12_800249BC (dngtwin 0.742, 161 words), the
   same +0x8D0-cursor POLY_FT4 tag-chain walk. I took its SHAPE and its
   spellings and did not re-price any of them: its one locally priced fact
   (both loops share one `i', 21 rows with separate counters) and the three
   it inherited verbatim from csrc/ovl_dng_s70.c func_s70_80024440 (`DVECTOR
   xy[]' read as fields rather than `s32' with plain casts, `(long *)&xy[i]'
   rather than a cursor local, and `t->u1 = t->u3 = t->u0 + ...' rather than a
   local holding the first byte) all held here at the first compile -- so
   treat them as UNMEASURED on this body. My deltas from the twin are the
   per-vertex depth ARRAY instead of an accumulator, the sixth projection, the
   func_800DBA90 extent rotation in place of its
   `__builtin_abs(...) / (+0x46 + 2)' divide, and the `u8 rec[0x20]' record
   (csrc/ovl_dng_b04.c func_b04_801743EC's type for the same callee).

   ONE FACT, priced from this body at 203 words / 0 differing:

   * `*(s16 *)(rec + 0x1A) = 0;' MUST BE WRITTEN BEFORE
     `*(s16 *)(rec + 0x18) = 1;'. Ascending it is 2 aligned differences, the
     0x62($sp) store one slot late. Every other pair in the record block is a
     CHAINED assignment, which emits right-to-left and so descending for
     free; this pair has two different values, so it cannot be chained and
     the source order is the only thing that can produce it.

   Note the reads of `xy[i].vx' and `v.vx' come out `lhu' with no cast
   anywhere: the destination `t->x0' is a short and both operands unwiden to
   16 bits, so convert.c distributes the truncation and does the whole
   subtraction in unsigned short. The twin has `lh' at the same site only
   because its second operand is an `s32'. */
s32 func_s68_80024B20(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    UnkNode *n;
    POLY_FT4 *t;
    s32 i;
    s16 a;
    SVECTOR v;
    s32 dep[5];
    DVECTOR xy[6];
    u8 rec[0x20];
    s32 w;

    while (1)
    {
        q = p;
        for (i = 4; i >= 0; i--)
        {
            dep[i] =
                RotTransPers((SVECTOR *)(q + 0x4 + i * 8), (long *)&xy[i], (long *)&w, (long *)&w) -
                8;
        }
        v.vx = *(u16 *)(q + 0x4);
        v.vy = *(u16 *)(q + 0x6);
        v.vz = *(u16 *)(q + 0x8) + 0x30;
        RotTransPers(&v, (long *)&xy[5], (long *)&w, (long *)&w);
        v.vx = 0;
        v.vz = 0;
        *(u8 **)rec = *(u8 **)(rec + 0x4) = (u8 *)&v;
        *(s16 *)(rec + 0x8) = *(s16 *)(rec + 0xA) = 0;
        *(s16 *)(rec + 0x10) = *(s16 *)(rec + 0x12) = *(s16 *)(rec + 0x14) = 0;
        *(s16 *)(rec + 0x1A) = 0;
        *(s16 *)(rec + 0x18) = 1;
        a = g->unk_C8;
        v.vy = __builtin_abs(xy[0].vy - xy[5].vy);
        *(s16 *)(rec + 0xC) = a;
        func_800DBA90(rec);
        for (i = 0; i < 4; i++)
        {
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
            *(u32 *)&t->r0 = *(u32 *)(q + 0x38);
            SetPolyFT4(t);
            SetSemiTrans((u8 *)t, 1);
            t->tpage = *(u16 *)(q + 0x40);
            t->clut = *(u16 *)(q + 0x42);
            t->x0 = xy[i].vx - v.vx;
            t->x1 = xy[i + 1].vx - v.vx;
            t->x2 = xy[i].vx + v.vx;
            t->x3 = xy[i + 1].vx + v.vx;
            t->y0 = xy[i].vy - v.vy;
            t->y2 = xy[i].vy + v.vy;
            t->y1 = xy[i + 1].vy - v.vy;
            t->y3 = xy[i + 1].vy + v.vy;
            t->u0 = t->u2 = *(u8 *)(q + 0x2C);
            t->u1 = t->u3 = t->u0 + *(u8 *)(q + 0x30);
            t->v2 = t->v3 = *(u8 *)(q + 0x2E);
            t->v0 = t->v1 = t->v2 + *(u8 *)(q + 0x32);
            w = (dep[i] + dep[i + 1]) >> 1;
            if ((u32)w < 0x1E0)
            {
                addPrim((u32 *)(g->unk_00 + 0xB0) + w, t);
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

/* The 0x302 trail node's per-frame step that func_s68_800244BC's state 1
   installs. Raises bit 0x8000 in the +0x52 halfword of the record at the
   actor's +0x0, interpolates the three intermediate 8-byte trail rows at
   +0x4 (stride 8) between row 0 and row 4 with a rand() & 0x3F jitter,
   republishes the +0x2C/+0x2E UV pair from the low bits of +0x3E, and runs a
   three-case compare chain on +0x3C: state 0 registers func_s68_80024B20 as
   the renderable and FALLS THROUGH into state 1, which re-rolls +0x3E, fades
   the +0x38 colour word by 0x80808 a frame, re-rolls the CLUT into +0x42
   from a random 4-bit page and advances the state; state 2 retires the actor.
   130 words, 0 differing.

   NO CARVED SIBLING AT dngtwin's 0.70 FLOOR. The idioms came
   from csrc/ovl_dng_s12.c func_s12_80024F40 and func_s12_80024C40 (the +0x52
   flag, the retire pair, the `((*(u16 *)(p + 0x3E) & 3) << 5) + 0x80' /
   `((s16) * (u16 *)(p + 0x3E) >> 2) << 5' pair, the `switch' rather than an
   if-chain, and the `p + k * 8 + 0x4' trail loop), and
   splat/common_syms.txt resolved func_8006649C to GetClut and func_80069EF8
   to rand, so neither needs a declaration. This was the first body landed in
   this unit and the other four were built out of it.

   ONE FACT, priced from this body at 130 words / 0 differing:

   * THE THREE PER-AXIS STEPS MUST BE AN ARRAY, `s16 d[3]', NOT THREE `s16'
     LOCALS. Three scalars is 96 aligned differences and NINE callee-saved
     registers where the target saves three: scalars get registers, and the
     target reloads each step with `lh' off a fixed 0x10/0x12/0x14($sp) slot
     every iteration, which only an addressable object does. The frame proves
     it before any compile -- 0x28 bytes with four saves leaves exactly eight
     bytes of locals, which is one array of four halfwords and not three
     live registers.

   MEASURED AND FREE at 130 words / 0 differing: a cursor local
   `e = p + i * 8' with `*(s16 *)(e + 0x4)' in place of the inline
   `p + i * 8 + 0x4'. The emitted giv is the same either way. */
void func_s68_80024E4C(u8 *p)
{
    s32 i;
    s16 d[3];

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    d[0] = (*(s16 *)(p + 0x24) - *(s16 *)(p + 0x4)) >> 2;
    d[1] = (*(s16 *)(p + 0x26) - *(s16 *)(p + 0x6)) >> 2;
    d[2] = (*(s16 *)(p + 0x28) - *(s16 *)(p + 0x8)) >> 2;
    for (i = 1; i < 4; i++)
    {
        *(s16 *)(p + i * 8 + 0x4) = *(u16 *)(p + 0x4) + d[0] * i + (rand() & 0x3F) - 0x20;
        *(s16 *)(p + i * 8 + 0x6) = *(u16 *)(p + 0x6) + d[1] * i + (rand() & 0x3F) - 0x20;
        *(s16 *)(p + i * 8 + 0x8) = *(u16 *)(p + 0x8) + d[2] * i - (rand() & 0x3F);
    }
    *(s16 *)(p + 0x2C) = ((*(u16 *)(p + 0x3E) & 3) << 5) + 0x80;
    *(s16 *)(p + 0x2E) = ((s16) * (u16 *)(p + 0x3E) >> 2) << 5;
    switch (*(s16 *)(p + 0x3C))
    {
    case 0:
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_s68_80024B20);
        /* fallthrough */
    case 1:
        *(s16 *)(p + 0x3E) = (rand() & 3) + 2;
        *(s32 *)(p + 0x38) -= 0x80808;
        *(s16 *)(p + 0x42) = GetClut((rand() & 0xF) << 4, 0x1F8);
        *(s16 *)(p + 0x3C) = *(u16 *)(p + 0x3C) + 1;
        break;
    case 2:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}
