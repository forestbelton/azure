/* func_80056E10 (unk_2879C) -- 713 of 718 words, 687 DIFF. COMPILES.
 *
 * FIRST ATTEMPT, 2026-10-27. The largest function left in the executable and
 * the third member of the func_8005BBFC / func_8005B4D0 voice family; the
 * other two are parked at 220 and 367 DIFF, so expect this to be hard for the
 * same reasons. READ BOTH OF THOSE HEADERS FIRST -- everything from
 * func_80056DB4 onward is their block, and every shape they list is paid for
 * here too. What this file adds is the READING of the part that is new: the
 * seven fallback scans and the portamento block between them.
 *
 * The full note-on. It counts how many of the bank's first `prog` entries
 * have a non-zero +0x20, walks the layers of the +0x20 record, and for each
 * layer whose [+0x06, +0x07] range contains the key it looks for a voice:
 *
 *   1. by channel: the first D_80085458 entry whose +0x06 is this channel.
 *      If that entry's +0x1A is clear it is keyed off with func_80056DB4 and
 *      func_8005E97C; if it is set, `flag` is raised instead and the later
 *      func_8005EC0C call is skipped.
 *   2. if 1 found nothing: the first voice func_8005EB78 reports as 0.
 *   3. if 2 found nothing: the first it reports as 2 OR 0.
 *      Scans 1-3 only run when the channel record's +0x98 is set.
 *   <the portamento block: +0x50/+0x52/+0x54/+0x56/+0x58 off the channel>
 *   4. if no voice yet: func_8005EB78 == 0 again.
 *   5. then func_8005EB78 == 2 or 0 again.
 *   6. then by (+0x06 == channel && +0x0A == key).
 *   7. then by +0x1A == 0. If that fails too the layer is skipped.
 *
 * ALL SEVEN ARE WRITTEN AS EXPLICIT `goto` LOOPS, which is func_8005B4D0's
 * standing lever for this family: the listing has one test at the TOP with
 * the back edge jumping to it, and `while`/`for (;;)` both let
 * `duplicate_loop_exit_test` rotate a second copy to the bottom. The index is
 * an `s16` recomputed with `sll`/`sra` at each use, which is why it is
 * declared `s16 i` and not `s32`.
 *
 * FIVE WORDS SHORT at the first compile, which for 718 words means the shape
 * is close; 687 differing words means almost nothing is in the right PLACE
 * yet, so the next pass should work the opcode diff region by region rather
 * than reading the differing-word count.
 *
 * WHAT IS ALREADY KNOWN TO BE WRONG, from that diff:
 *
 *  - THE PORTAMENTO BLOCK IS DUPLICATED IN THE LISTING, once at .L80057100
 *    (reached after scans 1-3) and once at .L80057144 (reached when +0x98 is
 *    clear), sharing only their arms. Writing it once after the `if` is five
 *    words short; writing a copy in each arm of the `+0x98` test, as done
 *    here in one measured attempt, overshoots to 729 -- cross-jumping merges
 *    more than the listing does. The right spelling is somewhere between.
 *  - The opening count loop (mine[74:88]) is thirteen words out, the biggest
 *    single region: the listing spills the bank pointer to 0x50($sp) AND
 *    0x54($sp) and re-reads D_80086D50 for the second group, where this
 *    source keeps one `b`.
 *  - `D_80084918` is written through casts here. It is a global SpuVoiceAttr
 *    (voice +0, mask +4, volume +8/+0xA, volmode +0xC/+0xE, addr +0x1C,
 *    a_mode +0x24, note +0x16, adsr1 +0x3A, adsr2 +0x3C), and the sibling
 *    parks use the typed form on their own local `attr`; the typed spelling
 *    is probably what this wants too.
 *  - `Unk80084960.unk_10` did not exist (pad); the +0x10 store is through a
 *    cast and wants a hand-split like the +0x54..+0x5C group added for this
 *    function.
 *
 * The declarations it needs: `extern u8 D_80084930[];` and the five callee
 * prototypes at the top. Everything else is already in structs.h.
 */
#include "common.h"

extern u8 D_80084930[];

void func_80056DB4(s16, s32, s16);
void func_8005E4A0(s32, u32);
s32 func_8005E78C(void);
void func_8005E97C(s32, u32);
s32 func_8005EB78(u32);
void func_8005EC0C(u8 *, s32, s32);
s16 func_800565D8(u8 *, u32);
void func_800561D8(u8 *, Unk80084960 *);
void func_80055E74(s16, s32, s16, s16, s32);

void func_80056E10(u8 ch, s32 key, u8 vel)
{
    Unk80084960 *s;
    u8 *b;
    u8 *q;
    u8 *e;
    u8 *v;
    u32 *g;
    u16 *w;
    s32 n;
    s32 sum;
    s32 c;
    s32 flag;
    s32 x;
    s32 a;
    s32 l;
    s32 r;
    s32 pan;
    s16 i;
    s16 j;
    s16 prog;
    u16 t;

    n = 0;
    s = &D_80084960[ch];
    b = D_80086A40[D_80086D50].unk_04;
    prog = s->unk_00;
    i = 0;
    if (prog != 0)
    {
        do
        {
            if (b[i * 0x10 + 0x20] != 0)
            {
                n = n + 1;
            }
            i = i + 1;
        } while (i < prog);
    }
    q = b + prog * 0x10 + 0x20;
    if (q[0] == 0)
    {
        return;
    }
    j = 0;
    do
    {
        e = b + ((n * 0x10 + j) * 0x20 + 0x820);
        if ((u8)key >= e[6] && e[7] >= (u8)key)
        {
            flag = 0;
            if (s->unk_98 != 0)
            {
                i = 0;
            scan1:
                if (*(u16 *)(D_80085458 + i * 0x78 + 6) == ch)
                {
                    goto found1;
                }
                i = i + 1;
                if (i < D_80073734[0])
                {
                    goto scan1;
                }
                i = -1;
            found1:
                if (i != -1)
                {
                    if (*(u16 *)(D_80085458 + i * 0x78 + 0x1A) == 0)
                    {
                        func_80056DB4(i, 0, 0);
                        func_8005E97C(0, D_80073740[i]);
                    }
                    else
                    {
                        flag = 1;
                    }
                }
                else
                {
                    i = 0;
                scan2:
                    if (func_8005EB78(D_80073740[i]) == 0)
                    {
                        goto found2;
                    }
                    i = i + 1;
                    if (i < D_80073734[0])
                    {
                        goto scan2;
                    }
                    i = -1;
                found2:
                    if (i == -1)
                    {
                        i = 0;
                    scan3:
                        c = func_8005EB78(D_80073740[i]);
                        if (c == 2)
                        {
                            goto found3;
                        }
                        if (c == 0)
                        {
                            goto found3;
                        }
                        i = i + 1;
                        if (i < D_80073734[0])
                        {
                            goto scan3;
                        }
                        i = -1;
                    found3:;
                    }
                }
            }
            else
            {
                i = -1;
            }
            if (s->unk_50 != 0)
            {
                s->unk_52 = 0;
                if (s->unk_5C < (u8)key)
                {
                    s->unk_56 = ((u8)key - s->unk_5C) << 7;
                    s->unk_54 = (s->unk_56 << 2) / s->unk_50;
                    s->unk_58 = 1;
                }
                else if (s->unk_5C == (u8)key)
                {
                    s->unk_54 = 0;
                }
                else
                {
                    s->unk_56 = (s->unk_5C - (u8)key) << 7;
                    s->unk_54 = (s->unk_56 << 2) / s->unk_50;
                    s->unk_58 = 0;
                }
            }
            else
            {
                s->unk_52 = 0;
            }
            if (i == -1)
            {
                i = 0;
            scan4:
                if (func_8005EB78(D_80073740[i]) == 0)
                {
                    goto found4;
                }
                i = i + 1;
                if (i < D_80073734[0])
                {
                    goto scan4;
                }
                i = -1;
            found4:
                if (i == -1)
                {
                    i = 0;
                scan5:
                    c = func_8005EB78(D_80073740[i]);
                    if (c == 2)
                    {
                        goto found5;
                    }
                    if (c == 0)
                    {
                        goto found5;
                    }
                    i = i + 1;
                    if (i < D_80073734[0])
                    {
                        goto scan5;
                    }
                    i = -1;
                found5:
                    if (i == -1)
                    {
                        i = 0;
                    scan6:
                        if (*(u16 *)(D_80085458 + i * 0x78 + 6) == ch &&
                            *(u16 *)(D_80085458 + i * 0x78 + 0xA) == (u8)key)
                        {
                            goto found6;
                        }
                        i = i + 1;
                        if (i < D_80073734[0])
                        {
                            goto scan6;
                        }
                        i = -1;
                    found6:
                        if (i == -1)
                        {
                            i = 0;
                        scan7:
                            if (*(u16 *)(D_80085458 + i * 0x78 + 0x1A) == 0)
                            {
                                goto found7;
                            }
                            i = i + 1;
                            if (i < D_80073734[0])
                            {
                                goto scan7;
                            }
                            i = -1;
                        found7:
                            if (i == -1)
                            {
                                goto next;
                            }
                        }
                    }
                }
            }
            v = D_80085458 + i * 0x78;
            if (s->unk_98 == 0)
            {
                func_80056DB4(i, 0, 0);
                func_8005E97C(0, D_80073740[i]);
                func_80055E74(i, (u8)key, ch, *(s16 *)(D_80084918 + 8),
                              *(s16 *)(D_80084918 + 0xA));
            }
            sum = 0;
            w = (u16 *)(D_80086A40[D_80086D50].unk_04 +
                        (*(u16 *)(b + 0x12) << 9) + 0x820);
            c = 0;
            if (*(s16 *)(e + 0x16) > 0)
            {
                do
                {
                    sum = sum + *w;
                    c = c + 1;
                    w++;
                } while (c < *(s16 *)(e + 0x16));
            }
            *(u32 *)(D_80084918 + 4) = 0x601EF;
            *(s16 *)(D_80084918 + 0xC) = 0;
            *(s16 *)(D_80084918 + 0xE) = 0;
            *(u32 *)D_80084918 = D_80073740[i];
            *(u32 *)(D_80084918 + 0x1C) =
                D_80086A40[D_80086D50].unk_10 + sum * 8;
            t = *(u16 *)(e + 0x10);
            *(s16 *)(D_80084918 + 0x3A) = t;
            *(u32 *)(v + 0x60) = t;
            t = *(u16 *)(e + 0x12);
            *(s16 *)(D_80084918 + 0x3C) = t;
            *(u32 *)(v + 0x64) = t;
            if ((*(u16 *)(e + 0x10) & 0x80) != 0)
            {
                *(u32 *)(D_80084918 + 0x24) = 5;
            }
            else
            {
                *(u32 *)(D_80084918 + 0x24) = 1;
            }
            *(u32 *)(v + 0x68) = *(u32 *)(D_80084918 + 0x24);
            s->unk_5C = key;
            *(u32 *)((u8 *)s + 0x10) = vel;
            v[0x22] = e[4];
            v[0x23] = e[5];
            v[0x21] = e[0xC];
            *(s16 *)(v + 0x00) = i;
            *(u16 *)(v + 0x04) = prog;
            *(s16 *)(v + 0x0A) = key & 0x7F;
            *(s16 *)(v + 0x0C) = key & 0x7F;
            *(s16 *)(v + 0x08) = j;
            *(s16 *)(v + 0x1A) = 1;
            v[0x20] = e[0xD];
            *(s16 *)(v + 0x06) = ch;
            s->unk_3A = 0;
            v[0x14] = q[1];
            v[0x16] = q[4];
            v[0x15] = e[2];
            v[0x1C] = vel;
            v[0x27] = 0;
            v[0x26] = 0;
            v[0x28] = 0;
            v[0x17] = e[3];
            v[0x34] = s->unk_6C;
            v[0x36] = s->unk_6E;
            *(u32 *)(v + 0x38) = s->unk_70;
            *(u32 *)(v + 0x30) = s->unk_68;
            v[0x35] = s->unk_6D;
            *(u32 *)(v + 0x2C) = 0;
            *(u32 *)(v + 0x3C) = 0;
            v[0x43] = 0;
            v[0x42] = 0;
            v[0x44] = 0;
            v[0x24] = s->unk_5D;
            v[0x50] = s->unk_88;
            v[0x52] = s->unk_8A;
            *(u32 *)(v + 0x54) = s->unk_8C;
            *(u32 *)(v + 0x4C) = s->unk_84;
            v[0x51] = s->unk_89;
            *(u32 *)(v + 0x48) = 0;
            *(u32 *)(v + 0x58) = 0;
            v[0x40] = s->unk_78;
            func_800561D8(D_80085458 + i * 0x78, &D_80084960[ch]);
            *(s16 *)(D_80084918 + 8) = *(u16 *)(v + 0x10);
            *(s16 *)(D_80084918 + 0xA) = *(u16 *)(v + 0x12);
            *(s16 *)(v + 0x0A) = key & 0x7F;
            v[0x1D] = s->unk_18;
            *(u32 *)(v + 0x70) = -1;
            *(u32 *)(v + 0x74) = s->unk_1C;
            if (s->unk_28 < 0x40)
            {
                pan = *(u16 *)(v + 0x3C) + (s->unk_3A + s->unk_52);
                x = func_800565D8(v, s->unk_1C);
                pan = pan + ((*(u16 *)(v + 0x0C) << 7) + x);
                *(s16 *)(D_80084918 + 0x16) =
                    (pan & 0x7F) + (((s16)pan >> 7) << 8);
            }
            else
            {
                s->unk_1C = 0x40;
                *(s16 *)(D_80084918 + 0x16) = (key & 0xFF) << 8;
            }
            if (e[5] != 0)
            {
                *(s16 *)D_80084930 = ((e[4] - 1) << 8) | (0x7F - e[5]);
            }
            else
            {
                *(s16 *)D_80084930 = e[5] | (e[4] << 8);
            }
            if (flag == 0)
            {
                func_8005EC0C(D_80084918, 0, 0);
            }
            if (s->unk_30 == 0)
            {
                if ((e[1] & 4) != 0)
                {
                    do
                    {
                        func_8005E4A0(1, D_80073740[i]);
                    } while ((func_8005E78C() & D_80073740[i]) == 0);
                }
                else
                {
                    do
                    {
                        func_8005E4A0(0, D_80073740[i]);
                    } while ((func_8005E78C() & D_80073740[i]) != 0);
                }
            }
            else if (s->unk_30 != 1)
            {
                do
                {
                    func_8005E4A0(1, D_80073740[i]);
                } while ((func_8005E78C() & D_80073740[i]) == 0);
            }
            else
            {
                do
                {
                    func_8005E4A0(0, D_80073740[i]);
                } while ((func_8005E78C() & D_80073740[i]) != 0);
            }
        }
    next:
        j = j + 1;
    } while (j < q[0]);
}
