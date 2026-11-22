/* func_800D92C0 (ovl_dungeon_800D50FC) -- LANDED 2026-11-22.
   344 of 344 words, 0 differing (masked), 0 aligned differences.

   THIS PARK IS CLOSED.  The body below is byte-exact and is the one to merge
   into csrc/ovl_dungeon_800D50FC.c in place of its INCLUDE_ASM stub.  Keep
   the declaration line below -- the one landed caller func_800D8C64 rides an
   implicit declaration without it, and re-measures at 250/250, 0 differing
   with it:

       void func_800D92C0(u8 *, void *, u8 *, u8 *);

   ================================================== THE LAST FIVE ROWS, AND
   THE ONE LINE THAT CLOSED THEM

   The whole residue was a QTY_CMP_PRI inequality in the loop block: the
   quantity holding D_8006CCD8's address took $v0 where the target wants it in
   $v1, and the c[0x24] load took $v1 where the target wants $v0.  Closing it
   needed the `%hi'/`%lo' pair moved to the TOP of the block, which needs
   `birthing_insn_p' (sched.c:2512) to FAIL on the `%lo' insn, which needs
   `reg_n_sets' of the `%lo' insn's destination to be anything but 1.

   THE FIX IS A SECOND REAL SET, NOT A DEVICE.  Split the table address into
   its own local and let the index add WRITE BACK INTO THAT SAME LOCAL, so the
   local is set twice -- once by the `%lo', once by the `addu' -- at a cost of
   no instructions at all:

       kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE
       t1 = (s32)D_8006CCD8
       t1 += kk
       c[0x24] += *(u8 *)t1
       c[0x25] += *(u8 *)(kk + (s32)D_8006CCE8)

   (t1 being a new s32 local, declared with kk.)  344 words, 0 rows.  SIX
   spellings of it are byte-identical and all land: the three statements in
   that order or with the kk line hoisted above the t1 line, with the add
   written `t1 += kk' or `t1 = kk + t1', and with the c[0x24] store written
   `+=' or `c[0x24] = c[0x24] + *(u8 *)t1'.  `fold' canonicalises PLUS, so the
   addu's operand order is not a source-level choice here.

   WHY THE SECOND LOOKUP STAYS INLINE.  Giving D_8006CCE8 the same treatment
   is 343 words / 26 rows -- cse turns the second `%hi'/`%lo' into
   `addiu t1, t1, 0x10' because D_8006CCE8 IS D_8006CCD8 + 0x10, and a word
   disappears.  The target materialises the second symbol in full, and its
   second `addu' ties into kk's quantity ($a0) because kk dies there, which is
   why that lookup already matched with no local at all.

   ============================== WHAT THE PREVIOUS HEADER GOT RIGHT AND WRONG

   RIGHT, and it is what made this findable: the proof that the target's
   `c[0x24]' line is VALUE-FIRST.  `addsi3_internal' (config/mips/mips.md:507)
   prints `addu %0,%z1,%2', so the target's own listing gives the RTL operand
   order, and `block_alloc's tying loop (local-alloc.c:1349) walks from
   operand 1 and breaks at the first operand it can tie.  At 0x800D9674 the
   target has `addu $v0, $v0, $v1' with $v0 the c[0x24] load, so the sum ties
   to the LOADED VALUE and the source reads `c[0x24] += table'.  The park's
   old table-first spelling scored 5 rows against value-first's 8 purely by
   coincidence and could never reach 0.  The landed body is value-first.

   RIGHT: `span(Q2)' cannot grow while the `%lo' insn is BIRTHING.
   `adjust_priority' (sched.c:2549) has a dead n_deaths test so every ready
   insn falls to `case 0:', and a birthing insn is bumped to LAUNCH_PRIORITY,
   so the `%hi'/`%lo' is chosen the instant its consumer is.  That is why
   seventy spellings of the three loop statements all landed on 5 or 8.

   WRONG, AND THIS IS THE ENTIRE COST OF THE PARK: "every C device that adds a
   reference also adds a set or a DEATH, and both disqualify the pseudo ...
   the two handles are mutually exclusive".  Two separate errors:

     * local_alloc's eligibility gate (local-alloc.c:472) is
       `reg_basic_block[i] >= 0 && reg_n_deaths[i] == 1'.  There is NO
       `reg_n_sets' condition in it.  A pseudo set five times is eligible.
     * A second set does NOT bring a second death when it is a
       READ-MODIFY-WRITE.  flow.c:2578 makes a REG_DEAD note only under
       `some_not_needed && ! dead_or_set_p (insn, x)', so an insn that both
       reads and writes the pseudo gets no note, and the pseudo still dies
       exactly once -- at its last use.  Confirmed in the `-dL' flow dump of
       the t1 body: register 99 reads "used 12 times across 11 insns in block
       37" with NO "dies in N places" line, which flow.c:2961 prints only when
       `reg_n_deaths != 1'.

   WHERE THE OLD HEADER'S SECOND DEATH ACTUALLY CAME FROM, because this is the
   trap and it is positional, not structural.  The old 5-row body bought its
   second set with a CANCELLED PAIR.  combine folds the pair, and for
   `t1 = t1 + 1' it takes the `i2dest_in_i2src' path (combine.c:2310) which
   does `reg_n_deaths[i2dest]++' and then hands the extra REG_DEAD note to
   `distribute_notes'.  That walks BACKWARD from i3 looking for an insn that
   mentions the pseudo (combine.c:11155) and, finding none, hits a CODE_LABEL
   and INSERTS A `use' INSN after the label to carry the note
   (combine.c:11231).  The note therefore gets placed, the increment stands,
   and `reg_n_deaths' is 2.  Had the walk found nothing at all, or found an
   insn where the pseudo is already dead or set, `place' would be 0 and
   combine.c:11375 would decrement it back to 1.

   The CODE_LABEL it hit is the join label of the
   `(*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000' ternary in the
   func_8009A3D0 call, which sits immediately above these statements -- so the
   cancelled pair was in the worst possible position in this function.  In the
   lreg dump of that body, register 99 carries REG_DEAD twice (on the inserted
   `use' and on the addu), and it is first in `.greg's "26 regs to allocate"
   list -- local_alloc had dropped it, so the address chain never became one
   quantity and global alloc gave it $v0.

   A real RMW set has none of this: combine folds nothing, so no note is ever
   re-placed, and the local keeps its single death and its place in
   local-alloc.  Note also that the local must stay LOOP-VARIANT -- it is, the
   second set reads kk -- so `scan_loop' (loop.c:804) cannot hoist it into a
   callee-saved register, which is what cost the single-set `t1' family two
   words and 14 rows.

   =========================================== THE FOUR DEVICES STILL IN THE
   BODY, EACH RE-PRICED BY DELETION AT 0 ROWS

     the `cc2' local one block up plus the do-while around the `|=' store
       -- delete the do-while 4 rows, delete cc2 as well 4 rows
     the duplicated arm `if (n)' around the func_8009A66C call   delete -> 3
     the `do { return; } while (0);' around ONE of the eight returns,
       the one under the i == 0 / D_80082E80 test                delete -> 4
     the duplicated arm `if (d)' around func_800BCB04            delete -> 91

   All four stay.  Every one of their conditions is definitely assigned on
   every path that reaches it -- `cc2' and `n' are assigned in the same block
   just above, `d' is a parameter -- so none of them rests on an uninitialised
   read.  The body carries no asm and no undefined behaviour.

   The permuter was imported into the value-first basin as commissioned and
   stopped once the lever was derived from the dumps instead, so it recorded
   no find.  The 2026-11-21 150-minute / 314k-candidate `found=none' run was
   spent in the table-first basin and was never going to close this.
 */
void func_800D92C0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 n;
    u8 *t80;
    s32 j;
    s32 k;
    s32 r;
    s32 v;
    s32 kk;
    s32 t1;
    s32 cc2;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        func_800A9A0C(d);
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    w = p + 0x9C;
    func_800A19E4((s32)c, d, 3, 6, w);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
    }
    else
    {
        if (*(s8 *)(d + 0x71) >= 0)
        {
            return;
        }
        if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
        {
            return;
        }
        func_800A19E4((s32)c, d, 3, 6, w);
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x2000) != 0)
        {
            if ((v & 0x20000) != 0)
            {
                t80 = D_80082E80;
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = t80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = t80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (c[0x24] == (u16)x && c[0x25] == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                m = func_800A0818(c[0x24], c[0x25], x, y, (u16 *)(p + 0x98));
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24],
                                                       D_80082E80[0x25], (u16 *)(p + 0x98));
                }
                if (func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
        else
        {
            j = *(s8 *)(c + 0x26);
            if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
            else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        n = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = n - tbl[i];
        }
        else
        {
            s = n + tbl[i];
        }
        if (n)
        {
            r = func_8009A66C((s16)s, c, d, 0x20);
        }
        else
        {
            r = func_8009A66C((s16)s, c, d, 0x20);
        }
        if ((s16)r > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = c[0x24];
            *(d + (d[0x71] & 0x7F) + 0x7C) = c[0x25];
            d[0x71]++;
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            c[0x24] += *(u8 *)t1;
            c[0x25] += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && *(u16 *)(D_80082E80 + 0x24) != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                do
                {
                    return;
                } while (0);
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
    }
    else
    {
        p[0x9C] = c[0x26];
        d[0x6D]--;
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0x8) = *(u16 *)(g2 + 0x8) + 1;
        if (*(s8 *)(d + 0x6D) == 0)
        {
            d[0x71] &= 0x7F;
            return;
        }
        if (d)
        {
            i = func_800BCB04(c[0x24] * 64 | 0x20, c[0x25] * 64 | 0x20,
                              *(u16 *)(d + 0x88) - 0x20);
        }
        else
        {
            i = func_800BCB04(c[0x24] * 64 | 0x20, c[0x25] * 64 | 0x20,
                              *(u16 *)(d + 0x88) - 0x20);
        }
        if (i < 0x200)
        {
            *(s16 *)(d + 0x88) = i;
        }
    }
}
