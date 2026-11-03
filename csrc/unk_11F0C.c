/* The remainder of unk_11940 after the gprel_11CFC carve-out. Named for its
 * start offset. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);
void func_8003F624(void);
void func_8003F5EC(void);
u8 func_8003F240(void);

/* File-scope definitions of the small-data objects this unit reaches
   $gp-relatively -- the definitions the original translation unit had. cc1psx
   emits a `.comm` for each, maspsx records it in sbss_entries, and every load
   and store of the symbol in this unit becomes %gp_rel while an address
   materialisation stays absolute. Nothing is allocated: these objects are
   already defined in asm/data/gp_data.data.s and placed by the linker script,
   and a tentative definition is a COMMON symbol, which merges with that
   definition rather than claiming storage of its own. A definition is
   file-scope and changes every access to its symbol in the whole unit, so
   grep this file before adding one. */
u8 D_800814D0[4];
u32 D_800814CC;
u8 D_800814D4;
u8 D_80081450[4];
u8 D_80080AD0;
u16 D_80080AD2;
u16 D_80080AD4;
u32 D_80080AD8;

void func_8003E70C(void)
{
    D_80083958.unk_08 = D_80083958.unk_08 + 1;
    D_80083958.unk_06 = D_80083958.unk_06 + 1;
    if (D_80083958.unk_06 > 0x400)
    {
        D_80083958.unk_06 = 0;
        CdInit();
    }
}

/* The CD command-queue pump: drains the 32-slot ring at D_80083968 one entry
 * per call, dispatching on the entry's command byte through two switches --
 * one per value of the drive-state byte D_800814D3[0].
 *
 * THE $gp SIDE. Eight objects are reached $gp-relatively (22 of them
 * D_800814D0), and the bare file-scope size record below is what buys that:
 * it puts each symbol in maspsx's sdata_entries while defining nothing, so
 * .sdata stays 0 bytes and the linker script's own assignment resolves the
 * symbol. See func_8003F624 in csrc/unk_12DEC.c for the write-up. The widths
 * are each object's real size: D_800814D0 1 (lbu/sb), D_800814CC 4 (lw/sw),
 * D_800814D4 1 (sb), D_80081450 4 (the struct's size; the accesses here are
 * lbu), D_80080AD0 1 (lbu), D_80080AD2 2 (lhu/sh), D_80080AD4 2 (sh),
 * D_80080AD8 4 (sw).
 *
 * NOT A LOOP. The two `goto again` arms must not be a `while (1)`/`continue`:
 * loop.c only sees a loop where the front end left a loop note, and with one
 * it hoists &D_80083958, &D_80083968 and the constants 0xFF, 1 and 5 into the
 * preheader -- six more callee-saved registers, a 0x40 frame instead of 0x30,
 * and 670 words against 698. The label-and-goto form leaves loop.c nothing to
 * do, and the inner retry loops (cases 0xA, 0x1B, 27) still get their own
 * notes, which is where the hoisted `addiu $s1,$zero,0x5` and
 * `addiu $s1,$zero,0xFF` in the listing come from.
 *
 * THE CASE SET. `case 0xF` and `case 0x16` emit nothing at all -- each is a
 * single value in a gap, fully bounded by its neighbours, so emit_case_nodes
 * collapses its test into the jump to the default it shares. They are still
 * case_nodes, and balance_case_nodes needs them: without them the first
 * switch's tree roots at `case 0xA` instead of the `case 0xB`/`case 0xC`
 * range, and the right subtree roots at 0x1B instead of 0x15. Nothing in the
 * listing shows them; they were recovered by compiling candidate case sets and
 * comparing the emitted test sequence.
 *
 * TWO BYTES REACHED AS AGGREGATE MEMBERS. The D_800814D3[0] read in the dispatch
 * test and the four D_800814D2[0] stores in the arms that also load
 * `D_80083968[i].unk_04` have to be MEM_IN_STRUCT_P references, which is what
 * the CdByte casts below produce. gcc's dependence test lets a fixed-address
 * SCALAR and a varying-address STRUCT reference pass each other freely, so as
 * plain scalars those accesses float: the D_800814D3[0] load lifts above the
 * `D_80083958.unk_0A |= 1` store group and each D_800814D2[0] store sinks past
 * the array load it must precede (12 differing words across five sites). The
 * clean equivalent is `extern u8 D_800814D2[1]` / `extern u8 D_800814D3[1]` in
 * include/structs.h -- measured to leave func_8003E34C, the only other user,
 * byte-identical.
 *
 * D_800814D0 THROUGH A POINTER. Four sites reach the read index as `p[-3]` or
 * `p[-2]` off a materialised `&D_800814D3[0]` / `&D_800814D2[0]`, because the target
 * reaches it there with a register base and a negative displacement rather
 * than $gp-relatively. maspsx's rewrite only fires on a bare-symbol operand,
 * so a register-based access is left alone -- and the local pointer is what
 * makes cc1psx emit `la` plus a displacement instead of folding the address
 * into the operand. `D_800814D0[3]` would do it too, but maspsx keys on the
 * symbol and ignores the addend, so that spelling sends the +3 access
 * $gp-relative as well; see the comment on func_8003E34C.
 *
 * THE RELOAD IN case 6. The `addr != 0` arm re-reads `q[0]` where the entry
 * block has already read it, and three separate things have to hold for that
 * to come out as the listing's `lw`/`nop`/`srl`: the read must survive cse
 * (3 words), the `or` of PHYS_TO_K0 must stay ahead of it, which is what
 * leaves the load-delay nop (1 word), and `q`'s allocno priority must clear
 * nsec's so `q` takes $s1 and nsec $s2 rather than the other way round
 * (9 words). Dropping all three costs 13 words -- 695 of 698, and the whole
 * image shifts three words.
 *
 * Until 2026-11-02 that was one `__asm__ volatile ("" :: "r" (q) : "memory")`.
 * It is now plain C: a `volatile` read does the first two jobs and a
 * degenerate loop around it does the third. Measured on the landed body
 * (698 words throughout unless noted):
 *   do { nsec = *(volatile u32 *) q >> 23; } while (0);        MATCH
 *   the same wrapped `while (1) { ...; break; }`               MATCH
 *   the old asm, memory clobber and all                        MATCH
 *   `__asm__ volatile ("" ::: "memory")` alone, no wrapper      9 DIFF
 *     (a pure $s1/$s2 swap -- the clobber does both reload jobs
 *      and the `"r" (q)` operand did nothing but the priority)
 *   the same clobber WITH the wrapper                          MATCH
 *   `nsec = *(volatile u32 *) q >> 23;` with no wrapper   697w, 564 DIFF
 *   ditto via `volatile u32 *vq = q`, or `((volatile u32 *) q)[0]`
 *                                                        697w, 564 DIFF
 *   the same read wrapped `for (;;) { ...; break; }`      697w, 564 DIFF
 *     (`for` does not emit the LOOP_BEG note the other two do)
 *   the volatile read TWICE, for q's extra reference            5 DIFF
 *   both arms computing nsec themselves, volatile in the else
 *                                                        696w, 560 DIFF
 *   wrapping the entry block's `nsec =` or `addr =` instead    14, 15 DIFF
 * So the wrapper is a reference weight on `q`, exactly as in the
 * func_8008FAC0 case documented at matching-levers:717, and the read being
 * volatile is what a DMA'd sector header wants anyway.
 *
 * SMALLER SHAPES, each measured:
 *   - case 0xFF reaches unk_08 as `*(s32 *) ((u8 *) D_80083968 + i * 24 + 8)`
 *     because the target computes that address base-first
 *     (`addu $v0, $s2, $a1`) while every other access to the entry is the
 *     array form and comes out index-first (`addu $v0, $v0, $s2`). Pointer
 *     arithmetic always yields base-first here and an array reference always
 *     index-first, whichever way the source writes the sum.
 *   - `w = v - 1; if (v == 0) w = 0x1F;` rather than an if/else: reorg then
 *     moves the decrement into the branch's delay slot. The if/else form puts
 *     0x1F there instead and reloads D_800814D0 in the other arm.
 *   - case 0x1B's first retry counter is set between the CdIntToPos call and
 *     the CdControl check so that reorg has it to fill the call's delay slot,
 *     leaving `&loc` to be scheduled early. Either side of that and the two
 *     swap places.
 *   - D_8008395C and D_8008395E are D_80083958's unk_04 and unk_06 under their
 *     own labels, used only where cse has not materialised the struct base.
 *     `D_80083958.unk_04` links to the same bytes -- %lo(D_80083958)+4 ==
 *     %lo(D_8008395C) -- but verify.py then flags six differing relocation
 *     addends. Same hazard class as D_80083164.
 *
 * The 28-entry jtbl_8002D5C0 is this unit's only jump table; verify.py masks
 * the two halfwords that reference it, so only `make check` can see it land. */
void func_8003E758(void)
{
    CdlLOC loc;
    u8 res[8];
    s32 r;
    s32 i;
    s32 n;
    u32 *q;
    u32 addr;
    u32 nsec;
    u8 st;
    s32 w;
    s32 v;

again:
    D_80083958.unk_0A &= ~1;
    if (D_800814D0[0] != D_800814D1)
    {
        D_80083958.unk_0A |= 1;
        if (D_800814D3[0] == 0xFF)
        {
            switch (D_80083968[D_800814D0[0]].unk_00)
            {
            case 0:
                D_800814D3[0] = 0xFF;
                D_80083958.unk_04 = 0;
                D_800814D2[0] = 0;
                D_800814D0[0] = (D_800814D0[0] + 1) & 0x1F;
                goto again;
            case 1:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 2;
                    D_80083958.unk_04 = 2;
                    D_800814D0[0] = D_800814D0[0] + 1;
                }
                break;
            case 2:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    CdIntToPos(D_80083968[D_800814D0[0]].unk_04, &loc);
                    if (CdControl(CdlSetloc, (u_char *)&loc, (u8 *)&D_80081450) != 0)
                    {
                        D_800814D0[0] = D_800814D0[0] + 1;
                    }
                }
                break;
            case 6:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    q = (u32 *)D_80083968[D_800814D0[0]].unk_04;
                    nsec = q[0] >> 23;
                    addr = q[0] & 0x7FFFFF;
                    if (addr == 0)
                    {
                        addr = D_80081480;
                    }
                    else
                    {
                        addr = PHYS_TO_K0(addr);
                        /* DEVICE: the degenerate loop is a reference weight on `q`,
                           not control flow -- flow.c:440 counts the
                           LOOP_BEG/END notes and propagate_block does
                           `reg_n_refs[regno] += loop_depth`, which lifts q's
                           allocno priority past nsec's so q takes $s1. The
                           `volatile` is not a device: this is a re-read of a
                           sector header the drive has written, and it is what
                           stops cse folding the read onto the entry block's.
                           Together worth 13 words. Without either, 695 of 698
                           words and the image is three words short.
                           See the unit header for every spelling measured.
                           This replaced the project's first and longest-lived
                           asm device on 2026-11-02; `-Wall` is silent. */
                        do
                        {
                            nsec = *(volatile u32 *)q >> 23;
                        } while (0);
                    }
                    if (nsec == 0)
                    {
                        u8 *p = &D_800814D3[0];
                        D_800814D3[0] = 0xFF;
                        p[-3] = p[-3] + 1;
                        break;
                    }
                    D_800814CC = addr + (nsec << 11) - 4;
                    D_800814D4 = 0x80;
                    if (CdControl(CdlSetmode, &D_800814D4, NULL) == 0)
                    {
                        return;
                    }
                    r = CdSync(1, res);
                    if (r != 0)
                    {
                        if (r == 5)
                        {
                            func_8003E70C();
                            return;
                        }
                    }
                    CdIntToPos(q[1], &loc);
                    if (CdControl(CdlSetloc, (u_char *)&loc, NULL) == 0)
                    {
                        return;
                    }
                    r = CdSync(1, res);
                    if (r != 0)
                    {
                        if (r == 5)
                        {
                            func_8003E70C();
                            return;
                        }
                    }
                    if (CdRead(nsec, (u_long *)addr, 0x80) == 0)
                    {
                        return;
                    }
                    D_800814D3[0] = 1;
                }
                break;
            case 9:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    if (CdControl(CdlPause, NULL, (u8 *)&D_80081450) != 0)
                    {
                        D_800814D3[0] = 1;
                    }
                }
                break;
            case 0xD:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    if (CdControl(CdlSetfilter, (u8 *)&D_80083968[D_800814D0[0]].unk_08,
                                  (u8 *)&D_80081450) != 0)
                    {
                        D_800814D3[0] = 0xFF;
                        D_800814D0[0] = D_800814D0[0] + 1;
                    }
                }
                break;
            case 0xE:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    if (CdControl(CdlSetmode, (u8 *)&D_80083968[D_800814D0[0]].unk_08,
                                  (u8 *)&D_80081450) != 0)
                    {
                        D_800814D3[0] = 0xFF;
                        D_800814D0[0] = D_800814D0[0] + 1;
                    }
                }
                break;
            case 0x15:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    CdIntToPos(D_80083968[D_800814D0[0]].unk_04, &loc);
                    if (CdControlF(CdlSeekL, (u_char *)&loc) != 0)
                    {
                        D_800814D3[0] = 1;
                    }
                }
                break;
            case 0x1B:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    CdIntToPos(D_80083968[D_800814D0[0]].unk_04, &loc);
                    i = 0x10;
                    if (CdControl(CdlSetloc, (u_char *)&loc, NULL) == 0)
                    {
                        return;
                    }
                    while (i != 0)
                    {
                        r = CdSync(1, res);
                        if (r != 0)
                        {
                            if (r != 5)
                            {
                                goto readloop;
                            }
                            func_8003E70C();
                            return;
                        }
                        i--;
                    }
                    func_8003E70C();
                    return;
                readloop:
                    i = 0x10;
                    while (CdRead2(0xC8) == 0)
                    {
                        i--;
                        if (i == 0)
                        {
                            func_8003E70C();
                            return;
                        }
                    }
                    D_8008395C[0] = 0;
                    D_800814D3[0] = 1;
                }
                break;
            case 0xA:
                D_800814D2[0] = 0;
                D_80083958.unk_04 = 0;
                i = 0x10;
                while (CdReset(0) == 0)
                {
                    i--;
                    if (i == 0)
                    {
                        func_8003E70C();
                        return;
                    }
                }
                D_800814D3[0] = 0xFF;
                D_800814D0[0] = D_800814D0[0] + 1;
                break;
            case 0xFF:
                (*(void (*)(s32))D_80083968[D_800814D0[0]].unk_04)(
                    *(s32 *)((u8 *)D_80083968 + D_800814D0[0] * 24 + 8));
                D_800814D0[0] = D_800814D0[0] + 1;
                break;
            case 0xFC:
                StUnSetRing();
                D_800814D0[0] = D_800814D0[0] + 1;
                break;
            case 4:
            case 8:
            case 0xB:
            case 0xC:
            case 0xF:
            case 0x16:
                break;
            }
        }
        else if (D_800814D3[0] == 1)
        {
            switch (D_80083968[D_800814D0[0]].unk_00)
            {
            case 0:
            {
                u8 *p = &D_800814D3[0];
                D_800814D3[0] = 0xFF;
                D_8008395C[0] = 0;
                D_800814D2[0] = 0;
                p[-3] = (p[-3] + 1) & 0x1F;
                goto again;
            }
            case 6:
                r = CdSync(1, (u8 *)&D_80081450);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        D_8008395C[0] = 0;
                        D_800814D2[0] = 0;
                        break;
                    }
                    n = CdReadSync(1, D_800814D8);
                    D_80080AD8 = n;
                    if (n > 0)
                    {
                        break;
                    }
                    if (n == 0)
                    {
                        if (D_80080AD0 != 0)
                        {
                            if ((*(u32 *)D_800814CC & 0xFFFF0000) == 0x10120000)
                            {
                                break;
                            }
                            func_8003E70C();
                            D_800814D2[0] = 0;
                            D_80080AD2 = D_80080AD2 + 1;
                            if ((D_80080AD2 & 3) == 3)
                            {
                                CdReset(0);
                                func_8003F5EC();
                            }
                            D_800814D3[0] = 0xFF;
                            break;
                        }
                        else
                        {
                            u8 *p = &D_800814D3[0];
                            D_800814D3[0] = 0xFF;
                            p[-3] = p[-3] + 1;
                            D_80083958.unk_04 = 2;
                            D_800814D2[0] |= 1;
                            D_80083958.unk_05 = D_80083958.unk_05 + 1;
                            break;
                        }
                    }
                    if (n >= 0)
                    {
                        break;
                    }
                    func_8003E70C();
                    D_800814D3[0] = 0xFF;
                    D_8008395C[0] = 0;
                    D_800814D2[0] = 0;
                }
                break;
            case 9:
                r = CdSync(1, (u8 *)&D_80081450);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        D_8008395C[0] = 0;
                        D_800814D2[0] = 0;
                    }
                    else if (CdControl(CdlNop, NULL, (u8 *)&D_80081450) != 0)
                    {
                        if ((*(u8 *)&D_80081450 & 0xFD) == 0)
                        {
                            D_8008395C[0] = 2;
                            D_800814D3[0] = 0xFF;
                            D_800814D0[0] = D_800814D0[0] + 1;
                        }
                    }
                }
                break;
            case 21:
                r = CdSync(1, res);
                if (r != 0)
                {
                    if (r == 5)
                    {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        D_8008395C[0] = 0;
                        D_800814D2[0] = 0;
                    }
                    else if (CdControl(CdlNop, NULL, (u8 *)&D_80081450) != 0)
                    {
                        if ((*(u8 *)&D_80081450 & 0xFD) == 0)
                        {
                            D_8008395C[0] = 2;
                            D_800814D3[0] = 0xFF;
                            D_800814D0[0] = D_800814D0[0] + 1;
                        }
                    }
                }
                break;
            case 27:
                if (CdSync(1, (u8 *)&D_80081450) == 5)
                {
                    D_800814D3[0] = 0xFF;
                    break;
                }
                i = 0x10;
                while (CdControl(CdlNop, NULL, (u8 *)&D_80081450) == 0)
                {
                    i--;
                    if (i == 0)
                    {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        return;
                    }
                }
                st = *(u8 *)&D_80081450;
                if ((st & 0x40) != 0)
                {
                    break;
                }
                if ((st & 0x20) != 0)
                {
                    u8 *p = &D_800814D2[0];
                    D_800814D2[0] = 0;
                    D_800814D3[0] = 0xFF;
                    if (D_80083968[p[-2]].unk_17 != 0xFF)
                    {
                        D_8008395C[0] = 4;
                        D_80080AD4 = 1;
                    }
                    p[-2] = p[-2] + 1;
                    break;
                }
                if ((st & 0x80) != 0)
                {
                    D_800814D3[0] = 0xFF;
                }
                break;
            }
        }
    }
    else
    {
        if (CdSync(1, NULL) == 5)
        {
            if ((func_8003F240() & 0xFF) == 0x1B)
            {
                v = D_800814D0[0];
                D_800814D3[0] = 0xFF;
                w = v - 1;
                if (v == 0)
                {
                    w = 0x1F;
                }
                D_800814D0[0] = w;
                func_8003F624();
            }
            else
            {
                CdControl(CdlNop, NULL, NULL);
            }
        }
    }
    if ((D_80083164[0] & 0x7FFF) == 0)
    {
        D_8008395E[0] = 0;
    }
    D_800814D0[0] &= 0x1F;
}
