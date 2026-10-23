/* The remainder of unk_11940 after the gprel_11CFC carve-out. Named for its
 * start offset. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);
void func_8003F624(void);
void func_8003F5EC(void);
u8  func_8003F240(void);

__asm__(".sdata\n\t.size D_800814D0,1\n\t.size D_800814CC,4\n"
        "\t.size D_800814D4,1\n\t.size D_80081450,4\n"
        "\t.size D_80080AD0,1\n\t.size D_80080AD2,2\n"
        "\t.size D_80080AD4,2\n\t.size D_80080AD8,4\n\t.text");

void func_8003E70C(void) {
    D_80083958.unk_08 = D_80083958.unk_08 + 1;
    D_80083958.unk_06 = D_80083958.unk_06 + 1;
    if (D_80083958.unk_06 > 0x400) {
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
 * THE BARRIER IN case 6. `__asm__ volatile ("" :: "r" (q) : "memory")` emits
 * nothing and does three measured jobs at once, all in the `addr != 0` arm:
 * the memory clobber stops cse reusing the entry block's `q[0]` across the
 * branch, so the arm reloads it and shifts again (3 words); it keeps the `or`
 * ahead of that reload, which is what leaves the load-delay nop (1 word); and
 * the `"r" (q)` operand is a reference to `q` that costs no instruction,
 * lifting its allocno priority from 4/34 above nsec's 5/41 so `q` takes $s1
 * and nsec $s2 rather than the other way round (9 words). Dropping it costs
 * 13 words; a `volatile u32 *` read in its place gets the reload but not the
 * order or the priority.
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
void func_8003E758(void) {
    u8 loc[8];
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
    if (D_800814D0[0] != D_800814D1) {
        D_80083958.unk_0A |= 1;
        if (D_800814D3[0] == 0xFF) {
            switch (D_80083968[D_800814D0[0]].unk_00) {
            case 0:
                D_800814D3[0] = 0xFF;
                D_80083958.unk_04 = 0;
                D_800814D2[0] = 0;
                D_800814D0[0] = (D_800814D0[0] + 1) & 0x1F;
                goto again;
            case 1:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 2;
                    D_80083958.unk_04 = 2;
                    D_800814D0[0] = D_800814D0[0] + 1;
                }
                break;
            case 2:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    func_8006124C(D_80083968[D_800814D0[0]].unk_04, loc);
                    if (CdControl(2, loc, (u8 *) &D_80081450) != 0) {
                        D_800814D0[0] = D_800814D0[0] + 1;
                    }
                }
                break;
            case 6:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    q = (u32 *) D_80083968[D_800814D0[0]].unk_04;
                    nsec = q[0] >> 23;
                    addr = q[0] & 0x7FFFFF;
                    if (addr == 0) {
                        addr = D_80081480;
                    } else {
                        addr = addr | 0x80000000;
                        __asm__ volatile ("" :: "r" (q) : "memory");
                        nsec = q[0] >> 23;
                    }
                    if (nsec == 0) {
                        u8 *p = &D_800814D3[0];
                        D_800814D3[0] = 0xFF;
                        p[-3] = p[-3] + 1;
                        break;
                    }
                    D_800814CC = addr + (nsec << 11) - 4;
                    D_800814D4 = 0x80;
                    if (CdControl(0xE, &D_800814D4, NULL) == 0) {
                        return;
                    }
                    r = CdSync(1, res);
                    if (r != 0) {
                        if (r == 5) {
                            func_8003E70C();
                            return;
                        }
                    }
                    func_8006124C(q[1], loc);
                    if (CdControl(2, loc, NULL) == 0) {
                        return;
                    }
                    r = CdSync(1, res);
                    if (r != 0) {
                        if (r == 5) {
                            func_8003E70C();
                            return;
                        }
                    }
                    if (CdRead(nsec, (u32 *) addr, 0x80) == 0) {
                        return;
                    }
                    D_800814D3[0] = 1;
                }
                break;
            case 9:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    if (CdControl(9, NULL, (u8 *) &D_80081450) != 0) {
                        D_800814D3[0] = 1;
                    }
                }
                break;
            case 0xD:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    if (CdControl(0xD, (u8 *) &D_80083968[D_800814D0[0]].unk_08,
                                 (u8 *) &D_80081450) != 0) {
                        D_800814D3[0] = 0xFF;
                        D_800814D0[0] = D_800814D0[0] + 1;
                    }
                }
                break;
            case 0xE:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    if (CdControl(0xE, (u8 *) &D_80083968[D_800814D0[0]].unk_08,
                                 (u8 *) &D_80081450) != 0) {
                        D_800814D3[0] = 0xFF;
                        D_800814D0[0] = D_800814D0[0] + 1;
                    }
                }
                break;
            case 0x15:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    func_8006124C(D_80083968[D_800814D0[0]].unk_04, loc);
                    if (CdControlF(0x15, loc) != 0) {
                        D_800814D3[0] = 1;
                    }
                }
                break;
            case 0x1B:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                    }
                    D_800814D2[0] = 0;
                    D_80083958.unk_04 = 0;
                    func_8006124C(D_80083968[D_800814D0[0]].unk_04, loc);
                    i = 0x10;
                    if (CdControl(2, loc, NULL) == 0) {
                        return;
                    }
                    while (i != 0) {
                        r = CdSync(1, res);
                        if (r != 0) {
                            if (r != 5) {
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
                    while (CdRead2(0xC8) == 0) {
                        i--;
                        if (i == 0) {
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
                while (CdReset(0) == 0) {
                    i--;
                    if (i == 0) {
                        func_8003E70C();
                        return;
                    }
                }
                D_800814D3[0] = 0xFF;
                D_800814D0[0] = D_800814D0[0] + 1;
                break;
            case 0xFF:
                (*(void (*)(s32)) D_80083968[D_800814D0[0]].unk_04)(
                    *(s32 *) ((u8 *) D_80083968 + D_800814D0[0] * 24 + 8));
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
        } else if (D_800814D3[0] == 1) {
            switch (D_80083968[D_800814D0[0]].unk_00) {
            case 0: {
                    u8 *p = &D_800814D3[0];
                    D_800814D3[0] = 0xFF;
                    D_8008395C[0] = 0;
                    D_800814D2[0] = 0;
                    p[-3] = (p[-3] + 1) & 0x1F;
                    goto again;
                }
            case 6:
                r = CdSync(1, (u8 *) &D_80081450);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        D_8008395C[0] = 0;
                        D_800814D2[0] = 0;
                        break;
                    }
                    n = CdReadSync(1, D_800814D8);
                    D_80080AD8 = n;
                    if (n > 0) {
                        break;
                    }
                    if (n == 0) {
                        if (D_80080AD0 != 0) {
                            if ((*(u32 *) D_800814CC & 0xFFFF0000) == 0x10120000) {
                                break;
                            }
                            func_8003E70C();
                            D_800814D2[0] = 0;
                            D_80080AD2 = D_80080AD2 + 1;
                            if ((D_80080AD2 & 3) == 3) {
                                CdReset(0);
                                func_8003F5EC();
                            }
                            D_800814D3[0] = 0xFF;
                            break;
                        } else {
                            u8 *p = &D_800814D3[0];
                            D_800814D3[0] = 0xFF;
                            p[-3] = p[-3] + 1;
                            D_80083958.unk_04 = 2;
                            D_800814D2[0] |= 1;
                            D_80083958.unk_05 = D_80083958.unk_05 + 1;
                            break;
                        }
                    }
                    if (n >= 0) {
                        break;
                    }
                    func_8003E70C();
                    D_800814D3[0] = 0xFF;
                    D_8008395C[0] = 0;
                    D_800814D2[0] = 0;
                }
                break;
            case 9:
                r = CdSync(1, (u8 *) &D_80081450);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        D_8008395C[0] = 0;
                        D_800814D2[0] = 0;
                    } else if (CdControl(1, NULL, (u8 *) &D_80081450) != 0) {
                        if ((*(u8 *) &D_80081450 & 0xFD) == 0) {
                            D_8008395C[0] = 2;
                            D_800814D3[0] = 0xFF;
                            D_800814D0[0] = D_800814D0[0] + 1;
                        }
                    }
                }
                break;
            case 21:
                r = CdSync(1, res);
                if (r != 0) {
                    if (r == 5) {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        D_8008395C[0] = 0;
                        D_800814D2[0] = 0;
                    } else if (CdControl(1, NULL, (u8 *) &D_80081450) != 0) {
                        if ((*(u8 *) &D_80081450 & 0xFD) == 0) {
                            D_8008395C[0] = 2;
                            D_800814D3[0] = 0xFF;
                            D_800814D0[0] = D_800814D0[0] + 1;
                        }
                    }
                }
                break;
            case 27:
                if (CdSync(1, (u8 *) &D_80081450) == 5) {
                    D_800814D3[0] = 0xFF;
                    break;
                }
                i = 0x10;
                while (CdControl(1, NULL, (u8 *) &D_80081450) == 0) {
                    i--;
                    if (i == 0) {
                        func_8003E70C();
                        D_800814D3[0] = 0xFF;
                        return;
                    }
                }
                st = *(u8 *) &D_80081450;
                if ((st & 0x40) != 0) {
                    break;
                }
                if ((st & 0x20) != 0) {
                    u8 *p = &D_800814D2[0];
                    D_800814D2[0] = 0;
                    D_800814D3[0] = 0xFF;
                    if (D_80083968[p[-2]].unk_17 != 0xFF) {
                        D_8008395C[0] = 4;
                        D_80080AD4 = 1;
                    }
                    p[-2] = p[-2] + 1;
                    break;
                }
                if ((st & 0x80) != 0) {
                    D_800814D3[0] = 0xFF;
                }
                break;
            }
        }
    } else {
        if (CdSync(1, NULL) == 5) {
            if ((func_8003F240() & 0xFF) == 0x1B) {
                v = D_800814D0[0];
                D_800814D3[0] = 0xFF;
                w = v - 1;
                if (v == 0) {
                    w = 0x1F;
                }
                D_800814D0[0] = w;
                func_8003F624();
            } else {
                CdControl(1, NULL, NULL);
            }
        }
    }
    if ((D_80083164[0] & 0x7FFF) == 0) {
        D_8008395E[0] = 0;
    }
    D_800814D0[0] &= 0x1F;
}
