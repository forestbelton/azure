#include "common.h"

/* 279 of 279 words, and EVERY differing word is a register assignment. Two
   swapped pairs and nothing else: the target puts the second parameter in $s6
   and the D_80083160 byte base in $s5, where this gives $s5 and $s6; and the
   target puts the allocated node in $s1 and its +0x20 view in $s0, where this
   gives $s0 and $s1. Everything downstream -- which slot each register saves
   to, and the 114 rows the masked verify counts -- follows from those two.
   This is an ALLOC park: the shape is done, the ordering is not, and
   docs/overlays.md says that is the permuter's job and not a sweep's.

   Landing it lands BOTH members of the pair: func_800BA468 is the same 279
   words with 0xA where this has 0x1A and 0x9 where it has 0x19, both stored
   to the third object's +0x08.

   WHAT THE FUNCTION IS. An entity spawner with four construction blocks, all
   the same recipe: func_8003FD64(type, base) to allocate an UnkNode,
   func_8004491C(node, tag) to initialise it, a handler into node->unk_10,
   then node+0x20 is an actor view that gets the parent and the parent's
   +0x98, then node->unk_08 is an UnkVec whose +0x00/+0x04 come from the
   second parameter. Block 1 is type 0x136 four times, block 2 type 0x36 four
   times, blocks 3 and 4 are single, types 0x36 and 0x136. Each bails to the
   next block on a null allocation.

   SIX THINGS THAT ARE SETTLED, and worth not re-deriving:

   * The first parameter is an INTERIOR pointer, used at -0x10 and -0x20. Every
     allocated object is handled the same way, as obj and obj+0x20, so the
     caller passes the +0x20 view and the container base is 0x20 below it.
   * The "templates" are engine FUNCTIONS, not data. D_80045C34 and D_80046398
     are func_80045C34 and func_80046398 (csrc/unk_19434.c has the first), but
     town.bin's splat only emits them as D_ symbols, so the D_ names are what
     the link resolves and (u32) casts are what func_8004491C's tag wants.
   * `bgez` + `addiu 0xFFF` + `sra 12` is ONE signed divide by 0x1000, and the
     same shape with 0x1FF/`sra 9` is 0x200 and 0x3/`sra 2` is 4. The `mult`
     by 0x38E38E39 with `sra 1` is a divide by 9. Each is followed by
     `sll`/`subu`, which makes it a MODULO.
   * The three byte reads at +0xA8/+0xA9/+0xAA are an RGB triple out of
     D_80083160.unk_A8, which csrc/unk_108F0.c sets to 0x2C808080. They must go
     through a LOCAL u8 pointer: written inline as ((u8 *)&D_80083160)[0xA8],
     `fold` reassociates 0xA8 onto the symbol and the hoisted base becomes
     D_80083160+0xA8, after which the other two come out as `lbu 1` and
     `lbu 2` instead of `lbu 0xA9`/`lbu 0xAA`.
   * A LOAD AFTER A STORE THROUGH ANOTHER POINTER NEEDS A TEMP, in five
     places. Block 1's third colour byte is one: written
     `p->unk_0E = col[0xAA]` after the `p->unk_08` store the load cannot hoist
     over it, where the target reads it first. The permuter found that one and
     it was worth five words. And `v->unk_98 = s->unk_98` needs one in all
     four blocks. The load cannot
     move above the stores through `v` -- same pointer type, so gcc must assume
     they alias -- and each block pays a load-delay nop. Reading it into a
     local before the stores was worth 4 words. Block 3's `p->unk_1A` has the
     same problem and the same fix; block 4's does NOT, and the target carries
     the nop there.
   * `dir` is an s32 compared as `(s8)dir`, not an s8 variable: the target
     truncates at the USE (`sll`/`sra` around the `lb`, which also fills the
     load delay), not at the assignment. Both spellings compile identically,
     so the choice is cosmetic.

   WHAT THE PERMUTER DID AND DID NOT DO, recorded as the protocol asks:
   142,531 candidates in 30 minutes at 79.2/s took its score from 2595 to
   1675 and did NOT match. In this file's terms that is 114 differing words
   down to 59, and of that, five words came from the `blue` temp above --
   legal, and adopted -- while the other fifty came from wrapping five
   statements of block 3 in a `do { ... } while (0)`. That is a DEVICE, not a
   source shape: it buys a basic-block boundary that moves the allocation. It
   is NOT adopted, because a device that still leaves 59 differing words buys
   nothing. It does say where the hinge is -- the allocator here is sensitive
   to a block boundary in block 3 -- and 142K candidates is evidence that this
   residue is not one statement away.

   THE ONE THING THAT IS NOT SETTLED, and it may be why the search floored.
   Loop 1 is written with a `goto`. As a `for` loop, loop.c hoists the
   2-instruction `(s8)dir` truncation into the preheader, costing 2 there to
   save 1 at the use, and the function comes out 280 words. A goto loop
   carries no LOOP_BEG, loop.c never sees it and nothing is hoisted -- the
   lever func_8005B4D0 established -- and that is what makes the length
   exact. But it also makes the three address initialisations before the loop
   source statements rather than loop.c hoists, and a `for` loop with inline
   uses would explain those equally well. The permuter mutates statements; it
   will not turn a goto loop back into a `for`, so if the `for` reading is the
   true one then this entire search ran from a wrong base. Settle that from a
   loop.c `-da` dump BEFORE spending more CPU here. */

typedef struct UnkTownSpawn
{
    u8 pad_00[0x66];
    s16 unk_66;
    u8 pad_68[0x2F];
    s8 unk_97;
    u32 unk_98;
    struct UnkTownSpawn *unk_9C;
} UnkTownSpawn;

typedef struct UnkTownRend
{
    u8 pad_00[0x8];
    u32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 pad_0F[0x7];
    s16 unk_16;
    s16 unk_18;
    s16 unk_1A;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20;
} UnkTownRend;

extern u8 D_80045C34[];
extern u8 D_80046398[];
extern void *D_800D1BCC[];
extern u8 D_800F8A1C[];
extern u8 D_800F8A28[];

s32 func_80069EF8(void);
UnkNode *func_8003FD64(s32, UnkNode *);
s32 func_8004491C(UnkNode *n, u32 tag);
s32 func_800644B8(s32);
void func_8008F104(UnkTownSpawn *v, UnkVec *x, void *p);
void func_800B9CB8();
void func_800B9D64();
void func_800B9EDC();
void func_800B9F74();

void func_800BA00C(UnkTownSpawn *s, UnkVec *m)
{
    s32 i;
    s8 dir;
    UnkTownSpawn *v;
    UnkNode *o;
    UnkTownRend *p;
    UnkVec *x;
    u8 *col;
    u8 *col2;
    u8 blue;
    u32 n;
    s16 ang;
    void (*h)();
    void **tab;

    *(u32 *)((u8 *)s - 0x10) = (u32)func_800B9CB8;
    s->unk_66 = func_80069EF8() % 0x1000;
    s->unk_97 = func_80069EF8() % 9 + 0xC;
    dir = s->unk_66 / 0x200 % 4;
    i = 0;
    h = func_800B9D64;
    col = (u8 *)&D_80083160;
    tab = D_800D1BCC;
loop1:
    {
        o = func_8003FD64(0x136, (UnkNode *)((u8 *)s - 0x20));
        if (o != 0)
        {
            o->unk_10 = (u32)h;
            func_8004491C(o, (u32)D_80046398);
            n = s->unk_98;
            v = (UnkTownSpawn *)((u8 *)o + 0x20);
            v->unk_9C = s;
            v->unk_66 = 0;
            v->unk_97 = i;
            v->unk_98 = n;
            p = (UnkTownRend *)o->unk_0C;
            p->unk_1C = 0x1000;
            p->unk_1E = 0x1000;
            p->unk_20 = 0xAAA;
            p->unk_0C = col[0xA8];
            p->unk_0D = col[0xA9];
            blue = col[0xAA];
            p->unk_08 = i + 0x1A;
            p->unk_0E = blue;
            x = o->unk_08;
            x->unk_00 = m->unk_00;
            x->unk_04 = m->unk_04;
            if (v->unk_97 == dir)
            {
                x->unk_08 = -((func_800644B8((s16)(v->unk_9C->unk_66 % 0x200) * 2) >> 4) * 0x1E00);
                v->unk_66 = 0x1E;
            }
            else
            {
                x->unk_08 = 0;
            }
            func_8008F104(v, x, *tab);
        }
        tab++;
        i++;
        if (i < 4)
        {
            goto loop1;
        }
    }
    for (i = 0; i < 4; i++)
    {
        o = func_8003FD64(0x36, (UnkNode *)((u8 *)s - 0x20));
        if (o != 0)
        {
            o->unk_10 = (u32)func_800B9EDC;
            func_8004491C(o, (u32)D_80045C34);
            n = s->unk_98;
            v = (UnkTownSpawn *)((u8 *)o + 0x20);
            v->unk_9C = s;
            v->unk_97 = i;
            v->unk_98 = n;
            p = (UnkTownRend *)o->unk_0C;
            p->unk_08 = (u32)D_800F8A1C;
            p->unk_0E = 0x80;
            p->unk_0D = 0x80;
            p->unk_0C = 0x80;
            p->unk_16 = 0x370;
            p->unk_18 = 0;
            p->unk_1E = 0x1000;
            p->unk_1C = 0x1000;
            p->unk_1A = s->unk_66 + i * 0x400;
            x = o->unk_08;
            x->unk_00 = m->unk_00;
            x->unk_08 = 0xFF2C0000;
            x->unk_04 = m->unk_04 + 0x200000;
        }
    }
    o = func_8003FD64(0x36, (UnkNode *)((u8 *)s - 0x20));
    if (o != 0)
    {
        o->unk_10 = (u32)func_800B9EDC;
        func_8004491C(o, (u32)D_80045C34);
        n = s->unk_98;
        v = (UnkTownSpawn *)((u8 *)o + 0x20);
        v->unk_9C = s;
        v->unk_98 = n;
        ang = s->unk_66;
        p = (UnkTownRend *)o->unk_0C;
        p->unk_08 = (u32)D_800F8A28;
        p->unk_0E = 0x80;
        p->unk_0D = 0x80;
        p->unk_0C = 0x80;
        p->unk_16 = 0x370;
        p->unk_18 = 0;
        p->unk_1E = 0x1000;
        p->unk_1C = 0x1000;
        p->unk_1A = ang;
        x = o->unk_08;
        x->unk_00 = m->unk_00;
        x->unk_08 = 0xFF2C0000;
        x->unk_04 = m->unk_04 + 0x200000;
    }
    o = func_8003FD64(0x136, (UnkNode *)((u8 *)s - 0x20));
    if (o != 0)
    {
        o->unk_10 = (u32)func_800B9F74;
        func_8004491C(o, (u32)D_80046398);
        n = s->unk_98;
        v = (UnkTownSpawn *)((u8 *)o + 0x20);
        v->unk_9C = s;
        v->unk_98 = n;
        col2 = (u8 *)&D_80083160;
        p = (UnkTownRend *)o->unk_0C;
        p->unk_1C = 0x1000;
        p->unk_1E = 0x1000;
        p->unk_20 = 0x1000;
        p->unk_1A = s->unk_66;
        p->unk_0C = col2[0xA8];
        p->unk_0D = col2[0xA9];
        p->unk_0E = col2[0xAA];
        p->unk_08 = 0x19;
        x = o->unk_08;
        x->unk_00 = m->unk_00;
        x->unk_08 = 0;
        x->unk_04 = m->unk_04;
    }
}
