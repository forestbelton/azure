/* func_80042BDC (unk_154BC) -- 543 of 543 words, verify.py MATCH. NOT YET
 * LANDED, and not landable on its own: it dispatches through jtbl_8002D67C, so
 * it needs unk_154BC's .rodata run placed at 0x8002D67C. verify.py masks the
 * two relocated halfwords that reference the table, so this MATCH is necessary
 * and NOT sufficient -- only `make check` can see the table land.
 *
 * THE PLACEMENT IS NOW A ROUTINE CHANGE and the recipe is in the jump-table
 * section of docs/open-matching-problems.md, already applied once for
 * unk_14934. unk_154BC contributes exactly one 32-entry table, 128 bytes at
 * 0x8002D67C, which is the last of the three contiguous runs. The spec change
 * is to split the second code segment again at 0xefc:
 *
 *     - type: code
 *       start: 0xe7c
 *       vram: 0x8002D67C
 *       align: 4
 *       subsegments:
 *         - [0xe7c, .rodata, unk_154BC]
 *     - type: code
 *       start: 0xefc
 *       vram: 0x8002D6FC
 *       align: 4
 *       subsegments:
 *         - [0xefc, bin]
 *         ...the rest...
 *
 * Note the ordering constraint: under ld_legacy_generation a segment emits all
 * of its `.data` before its `.rodata`, so the run must be the ONLY subsegment
 * of its segment -- a `[0xe7c, bin]` ahead of it would be placed first. That is
 * why the bin moves into the following segment rather than staying put.
 *
 * This source came from a subagent that was stopped by a usage limit
 * immediately after reporting the match, so its write-up of the load-bearing
 * shapes was lost. The source is verified -- `python3 tools/decomp/verify.py
 * docs/near-misses/func_80042BDC.c unk_154BC func_80042BDC` reproduces the
 * MATCH -- but **the reasons for its shapes are not recorded**, and the
 * previous near-miss header (14 DIFF, all in case 10, one `addu $a0,$s1,$zero`
 * the target emits twice around a join) is in git history at the commit before
 * this one if the diff against it is wanted.
 *
 * nmcheck: 543 of 543 words, 0 DIFF
 */

#define func_80042BDC func_80042BDC_decl_clash
#include "common.h"
#undef func_80042BDC

extern u8 D_800E1D55[];
extern u8 D_800E1D61[];
extern u8 D_800E1D80[];
extern u8 D_800E1D9F[];
extern u8 D_800E1DCF[];
extern u8 D_800E1DEE[];
extern u8 D_800E1E0D[];
extern u8 D_800E1E32[];
extern u8 D_800E1E4B[];
extern u8 D_800E1E6A[];
extern u8 D_800E1E8C[];
extern u8 D_800E1EC1[];
extern u8 D_800E1EF7[];
extern u8 D_800E1F1C[];
extern u8 D_800E1F41[];
extern u8 *D_800E3D7C;
extern u8 *D_800E3DF0[];

typedef u8 *(*Fn42BDC)(s32, s32, s32, s16);

void *func_800990FC(void);
void *func_80099734(u8 *, void *);
void *func_80099194(u8 *, void *);
void func_80099290(void *);
void func_800A5720(void *);
void func_800AA53C(u8 *);
void func_800D5460(u8 *, u32, s32);
u8 *func_800A1618(s32, s16);
void func_8009A3D0(s32, s32, s32);
void func_8009A21C(s32, s32, s32);
Fn42BDC func_800A0B94(s32, u8 *, s32);
void func_800A9A0C(u8 *);
void func_800AA888(u8 *, u8 *, u8 *, u8 *);
void func_800AC82C(u8 *, u8 *, u8 *, u8 *);
void func_800A32A4(u8 *);
void func_8009A028(u8 *);
void func_800AA36C(u8 *, u8 *, u8 *, u8 *);
void func_80041E70(void *);
void func_8004397C(u8 *);
void func_80042984(u8 *);

void func_80042BDC(s8 *arg0, s16 arg1) {
    u8 *p = (u8 *) arg0;
    void *msg;
    u8 *e;
    s32 k;

    switch (arg1) {
    case 1:
        *(u32 *) (p + 0x1C) &= ~0x200;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D55, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 2:
        *(u32 *) (p + 0x1C) &= ~0x400;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D61, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 3:
        *(u32 *) (p + 0x1C) &= ~0x10;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D80, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        if (p[0x13] != 0) {
            func_800AA53C(p);
        }
        break;
    case 4:
        *(u32 *) (p + 0x1C) &= ~0x20;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D9F, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 5:
        *(u32 *) (p + 0x1C) &= ~0x40;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1DCF, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 6:
        *(u32 *) (p + 0x1C) &= ~0x80;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1DEE, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 7:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E0D, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 8:
        *(u32 *) (p + 0x1C) &= ~0x800;
        break;
    case 9:
        *(u32 *) (p + 0x1C) &= ~0x1000;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E32, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 10: {
        if (p[0x13] == 0x2E || p[0x13] == 0) {
            func_800D5460(p - 0x20, 0x20A0A0, 0x613);
            if (p[0x13] == 0) {
                goto done10;
            }
        }
        if (p[0x13] == 0x2E) {
            u8 *q;
            u8 *nw;
            Fn42BDC fn;
            s32 s3;
            u8 bit;
            u8 *g;

            e = *(u8 **) (p - 0x14);
            k = 1;
            p[0x13] = p[0xA8];
            if (*(u32 *) (p + 0x14) & 0x4000) {
                k = 3;
            }
            q = func_800A1618(p[0x13], (s16) k);
            *(u32 *) (p + 0x1C) |= 0x10000;
            s3 = (*(u32 *) (p + 0x14) >> 14) & 1;
            func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_8004397C(p);
            fn = func_800A0B94(p[0x13], q, 1);
            func_8003F320();
            nw = fn(s3, e[0x24], e[0x25], *(s16 *) (p + 0x88));
            *(u32 *) (nw + 0x14) = 0;
            *(u32 *) (nw + 0x1C) = 0;
            bit = (*(u32 *) (p + 0x1C) >> 19) & 1;
            func_80042710((Unk42710 *) nw, (Unk42710 *) p);
            func_80042984(nw);
            g = (u8 *) &D_80083460;
            if (*(u16 *) (g + 2) & 0x1000) {
                if (p[0x71] > *(s16 *) (p + 0x8A)) {
                    *(u16 *) (g + 8) -= p[0x71] - *(u16 *) (p + 0x8A);
                }
            }
            func_800A9A0C(nw);
            *(u32 *) (nw + 0x1C) &= ~0x10000;
            if (*(u32 *) (p + 0x14) & 0x4000) {
                k = func_800A1BD0(p);
                D_800E3DF0[(*(u8 **) (((s16) k) * 4 + D_800E3D7C + 0xD0))[3] & 0x1F] = nw;
                *(u8 **) (((s16) k) * 4 + D_800E3D7C + 0xAC) = nw;
            }
            if ((bit & 1) != 0) {
                u8 *w = D_800E3D7C;

                *(u8 **) (w + 0x124) = nw;
                *(u8 **) (nw + 0x60) = w;
                *(u32 *) (nw + 0x1C) |= 0x80000;
                func_800AA888(nw, *(u8 **) (nw - 0x18), *(u8 **) (nw - 0x14), nw);
                func_800AC82C(nw, *(u8 **) (nw - 0x18), *(u8 **) (nw - 0x14), nw);
            } else if (nw[0x25] == 0) {
                u8 *g2 = (u8 *) &D_80083460;

                *(u16 *) (g2 + 0xA) += 1;
                *(u32 *) (nw + 0x1C) &= ~8;
            }
            *(u32 *) (p + 0x14) |= 0x20000000;
            func_800A32A4(p);
            func_8009A028(p);
            *(u16 *) (p - 2) |= 0x8000;
            D_800814A0 |= 0x8000;
            p = nw;
        }
    done10:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E4B, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    }
    case 27:
        e = *(u8 **) (p - 0x14);
        func_8009A21C(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        func_800AA36C(p, *(u8 **) (p - 0x18), e, p);
        break;
    case 11:
        e = *(u8 **) (p - 0x14);
        func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *) (p + 0x1C) &= ~0x2000;
        func_8009A21C(e[0x24], e[0x25], 0x3000);
        break;
    case 12:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E6A, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        e = *(u8 **) (p - 0x14);
        func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *) (p + 0x1C) |= 0x2000;
        func_8009A21C(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 26:
        e = *(u8 **) (p - 0x14);
        *(u16 *) (p + 0x98) &= 0x7FFF;
        func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        if (*(u16 *) (p + 0x98) & 0x4000) {
            *(u32 *) (p + 0x1C) |= 0x2000;
        }
        func_8009A21C(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u16 *) (e + 0x12) = *(u16 *) (p + 0xA6);
        break;
    case 32:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E8C, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 21:
    case 22:
    case 23:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1EC1, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 18:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1EF7, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 19:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1F1C, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 20:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1F41, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    }
    func_80041E70(p);
}
