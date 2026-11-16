/* ===== LANDED, NOT A PARK =====
   func_80096C24, 266 of 266 words, 0 differing, measured against
   csrc/ovl_dungeon.c as of 2026-11-16 by agent j47.  This file is here
   ONLY so the body survives a /tmp cleanup; the body below is the one to
   splice over the INCLUDE_ASM line, comment block included.  It needs no
   new declarations and no prototype change.  Do not queue it for the
   permuter. */

/* The entry probe of the dispatcher func_8008B9FC and func_8008C13C serve,
   and both are line-for-line templates for it: the same func_8009ABA0 probe
   on +0x2A/+0x88 with 0x20 on the stack, the same D_8006CCD8/D_8006CCE8
   eight-way step, the same f/h/q materialisations of D_80083460 and the same
   `r > 0` / `r <= 0` fork, with a D_800DD274 or D_800DD294 re-skin in each
   arm instead of func_8008B9FC's D_800E3544 tail. 266 words, 0 differing.

   THE FIRST FLAG RMW IS THE ARRAY-REF SPELLING, `*((u16 *)f + 1) |= 0x8`,
   AND IT IS WORTH THREE WORDS. Written `*(u16 *)(f + 0x2) |= 0x8` the
   address expression's top node is a cast, so expand_expr leaves
   MEM_IN_STRUCT_P clear; `f` holds a SYMBOL address so cse_rtx_addr_varies_p
   is 0, and `in_struct` is then the only clause left in invalidate_memory by
   which `*(s32 *)(p + 0x8C) = 0` could kill the cached load. It does not, so
   the D_80013714 arm reuses the pre-or value as one `ori $v0, $v1, 0x88` and
   the function comes out 263 words of 266 with 32 aligned differences. The
   array-ref form is in-struct, the store kills it, the reload appears, and
   the arm is the target's four words. docs/matching-levers.md, "WITH A
   CONSTANT-ADDRESS BASE, MEM_IN_STRUCT_P IS THE ONLY WAY A STORE CAN KILL A
   LOAD IN cse". The two `|= 0x80` arms are left as casts; only the cached
   side of the pair decides this.

   `h` IS ITS OWN LOCAL AND MUST STAY BELOW THE `(s16)r != 4` BLOCK. Assigned
   beside `f`, cse turns it into a copy of `f`, the second
   `lui`/`addiu` of D_80083460 at .L80096E08 disappears and it is 264 words
   with 38 aligned differences.

   Each D_800DD274 arm has its own pointer local, the rule func_800974DC
   records, and each facing index is ONE expression inline in the subscript so
   fold collapses the `* 4` into `sra 7` / `andi 0x1C`.

   The two `*(s16 *)(...+ 0x4) = 8` stores are spelled against D_80083460
   where splat's listing names D_80083464, exactly as func_8008B9FC,
   func_8008E264 and func_800971A8 already do, so D_80083464 needs no
   declaration. */
void func_80096C24(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *f;
    u8 *h;
    u8 *q;
    void **t;
    void **t2;
    void **t3;
    u8 *t4;
    void **t5;
    void **cur;
    s32 r;
    s32 k;

    /* DEVICE: worth 25 aligned differences and no words. Without it `p` and
       the D_80083460 base `h` come out with each other's callee-saved
       registers -- p in $s4 and h in $s3 where the target has p in $s3 and h
       in $s4 -- and every one of the 25 differences is that one swap, at 266
       of 266 words. `h` is 4 references over a ~40-insn range (pri 2*4/40)
       against p's 15 over the whole function (3*15/250); one more reference
       to p crosses the floor_log2 step at 16 and puts it in front in
       allocno_compare. This is the same device func_8008C13C carries on its
       own fourth parameter for the same $s3/$s4 rotation.
       Measured and rejected as legal spellings, all 266 words / 25 aligned
       differences: `*(u16 *)0x80013714` for the declared symbol; `cur`
       dropped so the +0x2C load is tested inline; a `pa = p` copy feeding
       `p[0x9B] = 8`, and another feeding `*(u16 *)(p + 0x98) |= 0xC`; an
       `h2 = h` copy so `h` carries 3 references instead of 4; `UnkVec *b`
       for `void *b`; and `*(s16 *)((u8 *)&D_80083460 + 0x4) = 8` for the
       `h + 0x4` store, which cse folds straight back onto `h`. Inlining the
       `k` facing index at both D_8006CCD8/E8 reads is 269 words / 45.
       It does not warn under -Wall. */
    g = (u8 *)&D_80083160;
    __asm__("" ::"r"(p));
    func_800A67F4();
    r = func_8009ABA0(*(s16 *)(d + 0x2A), b, c, *(s16 *)(d + 0x88), 0x20);
    if ((s16)r > 0)
    {
        p[0x9B] = 0;
        func_8009A3D0(c[0x24], c[0x25], 0x300);
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        c[0x24] += *(u8 *)&D_8006CCD8[k];
        c[0x25] += *(u8 *)&D_8006CCE8[k];
        func_8009A21C(c[0x24], c[0x25], 0x300);
        f = (u8 *)&D_80083460;
        *((u16 *)f + 1) |= 0x8;
        *(s32 *)(p + 0x8C) = 0;
        if ((s16)r != 4)
        {
            if ((D_80013714 & 2) != 0)
            {
                *(u16 *)(f + 0x2) |= 0x80;
                cur = *(void ***)(c + 0x2C);
                t = D_800DD274;
                if (cur != t)
                {
                    *(void ***)(c + 0x2C) = t;
                    func_8003DB94(c, t[((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
                }
            }
            else if ((*(s32 *)(g + 0x8) & 0x20) != 0 && func_800A5C70() != 0)
            {
                t2 = D_800DD274;
                *(u16 *)(f + 0x2) |= 0x80;
                *(void ***)(c + 0x2C) = t2;
                func_8003DB94(c, t2[((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            else
            {
                r = 5;
                p[0x9B] = 0xA;
            }
        }
        h = (u8 *)&D_80083460;
        if ((*(u16 *)(h + 0x2) & 0x80) != 0 || (s16)r == 1)
        {
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            if ((*(u16 *)(h + 0x2) & 0x80) == 0)
            {
                t3 = D_800DD274;
                *(void ***)(c + 0x2C) = t3;
                func_8003DB94(c, t3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0x35;
            *(s16 *)(h + 0x4) = 8;
        }
        else
        {
            *(u16 *)(p + 0x98) |= 0xC;
            t4 = D_800DD294;
            *(u8 **)(c + 0x2C) = t4;
            func_8003DB94(c, *(void **)(t4 + (((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) * 4), 0);
            if ((s16)r < 4)
            {
                *(u16 *)(p + 0xA2) |= 1;
            }
            else if ((s16)r == 2 || (s16)r == 4)
            {
                p[0x9B] = 8;
            }
            func_80094ED4(p, b, c, d);
            *(s16 *)((u8 *)&D_80083460 + 0x4) = 8;
            p[0x9A] = 0x36;
            *(s16 *)(p + 0x96) = 2;
            *(s32 *)(d + 0x1C) |= 0x40000000;
            func_8009F644(d, 8, 0, 0);
            return;
        }
        func_8009F644(d, 8, 0, 0);
    }
    else
    {
        t5 = D_800DD274;
        *(void ***)(c + 0x2C) = t5;
        func_8003DB94(c, t5[((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(p + 0x8C) = (s32)func_80096384;
        return;
    }
    func_80094ED4(p, b, c, d);
    *(s32 *)(d + 0x1C) |= 0x40000000;
    q = (u8 *)&D_80083460;
    *(u16 *)(q + 0x2) |= 0x812;
}
