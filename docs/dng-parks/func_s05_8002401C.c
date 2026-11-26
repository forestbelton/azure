/* PARK: func_s05_8002401C, csrc/ovl_dng_s05.c, agent-h244, 2026-11-26.

   TARGET 842 words. BEST BUILT 825 words, 363 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports 687 masked, which is the
   position-by-position inflation this body's 17-word deficit produces -- use
   the aligndiff figure. 1 member, 842 words (tools/decomp/dngfam.py), so no
   replication value. relcand ok (10 data symbols, 28 calls). declcheck clean.

   THE FIGURE BREAKS DOWN AS: 255 register-only rows, 20 opcode rows,
   51 target-only words, 37 candidate-only words. 14 of the 51 target-only
   words are `lw $tN, 0xA4($sp)' and one is `sw $a1, 0xA4($sp)'.

   VERDICT: ALLOC, and it is ONE ALLOCNO. The whole residue is that the
   target SPILLS BOTH PARAMETERS p AND b to their incoming argument homes
   (0xA0 and 0xA4 off $sp) and reloads them through $t0/$t1 at every use,
   while this body spills only p and keeps b in $fp. Frames are IDENTICAL --
   `.frame $sp,160,$31  vars= 88, regs= 10/0, args= 32' in both, and both
   save s0..s7, fp, ra in the same order. The 17-word deficit is b's 14
   reload words plus their load-delay `nop's; the 255 register-only rows are
   the same instruction with $s8 as the base where the target has $t0/$t1,
   plus the one-position shift of every callee-saved name that follows from
   it.

   THE COLOUR COUNT, measured off -da `.greg' rather than guessed. At the
   switch dispatch, eight values are live: p, b, c, s, o, w, tx, ty. Case 0's
   search loop adds i, ax, ay and the two coordinates; case 1's spawn loop
   adds i, the hoisted D_800DEC28, n, q, m and e. This body's allocation
   order ends `... 87 82 84 81 80 531 362 346 85 98 99' -- e, c, o, b, p,
   then w, tx, ty -- and the dispositions are
   `81 in 30  82 in 21  83 in 22  84 in 23  86 in 17  87 in 20  88 in 18
   89 in 19 ... 92 in 16', i.e. b=$fp, c=$s5, s=$s6, o=$s7, q=$s1, e=$s4,
   n=$s2, i=$s3, m=$s0 -- NINE registers for nine allocnos, with p, w, tx and
   ty spilled. The target needs a TENTH, so b goes too.

   THE TENTH ALLOCNO IS THE SECOND COORDINATE'S PRE-MASK QUANTITY, and that
   is where this body stops. In the target's search loop the x chain is tied
   into one register
     sll $v1,$v1,6 / addiu $s1,$v1,0x20 / andi $s1,$s1,0xFFFF
   and the y chain is NOT
     sll $v0,$v0,6 / addiu $s2,$v0,0x20 / andi $s0,$s2,0xFFFF
   so y costs two callee-saved registers where x costs one. This body ties
   both ($s1 and $s0). The tie is local-alloc's `combine_regs'
   (local-alloc.c around 1759): it refuses to tie an output to a dying input
   whose `reg_qty' is -1, which is the case when the input is not local to
   the block or dies more than once. Both chains here are straight-line
   inside one basic block with one death each, so both tie.

   HYPOTHESIS, NOT MEASURED: the asymmetry is an artefact of the target's own
   higher pressure -- p and b being spilled gives reload more $t temporaries
   to place, and the y chain loses its tie as a consequence -- in which case
   the two facts are mutually causal and no source change reaches it. The
   alternative, that the biased coordinate crosses a basic-block boundary in
   the original (which would explain the missing tie AND the missing
   `flow.c:2143' LOG_LINK at once), was looked for and no plausible source
   shape produces it: the two calls that consume the coordinates sit in the
   same basic block in the target's own listing.

   THE INSTRUCTION STREAM OF THE SEARCH LOOP NOW MATCHES EXACTLY, which is
   how the above was isolated. Getting there needed one non-obvious fact:

   `addiu $s1, $v1, 0x20' AFTER AN `sll ... 6' NEEDS TWO USES OF THE BIASED
   VALUE. combine.c:3792 rewrites `(plus X C)' as `(ior X C)' whenever
   `nonzero_bits' says the operands share no bits, and a left shift by 6
   always clears the low six, so every one-use spelling emits
   `ori $reg,$reg,0x20'. SIX spellings measured on a standalone probe, all
   `ori': one expression with a u16 destination; an s32 step local with two
   sets; a u16 step local with two sets; an s16 step local; a sum local then
   scale-and-bias; and scale into a local then bias at the use. What lands
   `addiu' + `andi 0xFFFF' is coordinates that are PLAIN s32 LOCALS with the
   `u16' conversion coming from BOTH callees' parameter types -- two uses of
   the biased value, so flow.c builds no LOG_LINK from the plus to either and
   combine never sees the shift. `x = (x + 0x20) & 0xFFFF' also keeps the
   `addiu' but narrows the mask to `andi 0xFFE0' (`simplify_and_const_int'
   ands the constant with the operand's nonzero bits), which is one wrong
   word, and it measured 780 words / 516 rows against 780 / 512 for the
   `ori' form at that stage.

   SO func_800A4688 AND func_800BCB04 BOTH TAKE u16 COORDINATES HERE. That
   is a real claim about two LANDED resident bodies and it needs saying out
   loud: csrc/ovl_dungeon_8009F644.c defines
   `s32 func_800A4688(u16 px, u16 py, u16 face, u16 ang, s32 flag)' and its
   own comment says "Nothing calls this function anywhere in the project, so
   the width is unwitnessed" -- this body is the first witness, and it wants
   `face' and `ang' SIGNED: the listing passes `(s16)func_800BCB04(...)'
   with `sll 16'/`sra 16' and `*(s16 *)(s + 0x2A)' with a plain `lh', where
   u16 parameters would have forced an `andi 0xFFFF' and an `lhu'. The
   declaration block below therefore carries a unit-local alias
   `func_800A4688s' and leaves the landed definition alone. func_800BCB04 is
   declared `s32 func_800BCB04(u16, u16, s16)' in this unit; the two call
   sites here want exactly that, and the case-0 tail site confirms it
   independently with `lhu $a0, 0x22($sp)' / `lhu $a1, 0x26($sp)'.

   IT OWNS A JUMP TABLE AND THE TABLE IS SETTLED AND MEASURED.
   jtbl_s05_80024008, 5 words = 5 cases + 0 pad, dispatch bound 5, 5 distinct
   targets (tools/decomp/jtcheck.py; MYTABLES.txt agrees). MINVAL IS 0 --
   the listing has a bare `sltiu $v0, $a1, 0x5' with no subtraction -- and
   the range check's `beqz' goes to the function's EPILOGUE, which is not one
   of the five entries, so there is no `default:' among them and none is
   written. Entries in listing order:
     0 -> .Ls05_800240C8   1 -> .Ls05_8002467C   2 -> .Ls05_80024AB0
     3 -> .Ls05_80024C78   4 -> .Ls05_80024CB8
   which is ascending by entry address, so case order IS entry-address order.
   NOT INFERRED FROM A WORD COUNT: a standalone `tools/cdkgcc -S' of this
   body emits `.rdata / .align 3 / $L62: .word $L3, $L29, $L48, $L57, $L59',
   five entries, ascending, matching the listing one for one.

   WHAT THE BODY DOES. A five-state cutscene director on *(s16 *)(p + 0xA),
   relinked from the same family as csrc/ovl_dng_s12.c func_s12_80024020
   (dngcallee.py, 10 of 22 symbols with a clean gap at 9/22; dngtwin found
   NOTHING at 0.70 for this stub). s = *(u8 **)p, o = *(u8 **)(s - 0x14),
   w = s - 0x20, and tx/ty are the step pair D_8006CCD8/D_8006CCE8 indexed by
   `(*(u16 *)(s + 0x2A) >> 9) & 7'. State 0 projects the party record into b,
   re-registers the renderable, dresses c from D_800DEBD8, asks func_800A05A4
   for a target and -- if there is none -- walks the facing direction with
   func_800BCB04/func_800A4688 until it is blocked, then solves a
   constant-acceleration path into p + 0x44/0x48/0x4C and seeds b's velocity
   half. State 1 spawns 21 smoke nodes running func_s05_80024D44, each one
   jittered by a func_800DBA90 rotation of a -(rand() & 0x1FF) vector, then
   advances b and retires once both axes line up with s + 0x72/0x73. State 2
   spawns 81 nodes running func_s05_80024E98. State 3 fades c + 0xC by
   0xFFEFEFF0 until its low byte reaches 0. State 4 is the teardown.

   EVERY FACT MEASURED ON THIS BODY, in the order it was priced. The counts
   are aligndiff rows at the state the body was in when measured, so only the
   last column is priced against the body below.

     SPELLING                                            WORDS / ROWS
     first compile, before any of the below               784 / 527
     case 0 dress block with +0x10 and +0x8 in the
       donor's order instead of the listing's             784 / 527
     rec + 0x8 and rec + 0xC read back in place
       instead of through a local (two loads, `lh'
       for the test and `lhu' for the sum, where the
       target has one `lh')                               784 / 527
     the same two reads through locals, and
       *(s16 *)(s + 0x2A) through a local too             780 / 512
     tx * (*(s16 *)(p + 0x50) * 0x66666)                  780 / 512
     tx * 0x66666 * *(s16 *)(p + 0x50)                    798 / 510
     case 0's else arm keyed on its own local `e'         798 / 510
     the same arm keyed on `n', the local cases 1
       and 2 already use for the func_8003FD64 node       829 / 370
     *(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0)
       + 0x4) in case 1                                   829 / 370
     the same read written off D_800DEC28 directly        828 / 364
     the same change in case 2                            827 / 364
     coordinates as u16 locals, `ori 0x20'                827 / 364
     coordinates as s32 locals, `addiu 0x20'              827 / 368
     the two rec angle gates with the arms the
       target's way round                                 827 / 367
     e = (u8 *)n + 0x20 after the m clamp                 827 / 367
     the same assignment before the clamp                 827 / 366
     the c + 0x8 reads in case 1's teardown written
       off D_800DEC00 and D_800DEB70 directly             825 / 363

   MEASURED AND FREE, so not the reason anything above moved:
     declaring `i' first in the local list                 -- / no change
     n = (UnkNode *)(p - 0x20) as its own statement        -- / no change
     a local holding D_800DEC28 across case 1's loop
       (cse propagates the SYMBOL_REF into the store and
       the local disappears, which is the levers file's
       "cse eats every plain-C way of buying a reference
       to an ADDRESS")                                     -- / no change
     one s32 step local shared between the two
       coordinates, two sets                               -- / no change
     removing the then-unused u16 coordinate locals        -- / no change

   THREE THINGS THE LISTING SETTLES THAT ARE EASY TO GET WRONG, all read off
   the encodings rather than the mnemonics:
     the biased coordinate is `20007124' = addiu, NOT ori;
     `0x300 - j * 32 - j * 8' is the source order that emits
       `sll 3 / subu / sll 5 / subu', because fold's split_tree
       (fold-const.c:991, caller at 4201) rewrites `C - A - B' as
       `(C - B) - A';
     `b->unk_14 -= 0x4000 + (rand() << 2)' is what emits
       `addiu -0x4000' on the loaded word then `subu'; written
       `b->unk_14 - 0x4000 - (rand() << 2)' the same split_tree path gives
       `a - (r + 0x4000)' and costs 3 rows -- that one is measured on
       func_s05_80024E98 in this unit, which lands at 0 with it.

   WHAT TO TRY NEXT, in the order I would try it:
     1. find the source shape that gives the second coordinate's pre-mask
        value its own quantity. Everything else in this body matches.
     2. failing that, price a `volatile s32' on the two coordinates: it is
        the one spelling measured to produce `addiu 0x20' + `andi 0xFFFF'
        AND a separate quantity (probe variant w6), but it costs a frame
        slot and a store/load pair per coordinate, so it is a DEVICE and it
        was not tried on the real body.
     3. the permuter. It was not run -- this batch's agents do not run it.

DECLARATIONS
s32 rand(void);
void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A3820(s16);
s32 func_800A4688s(u16, u16, s16, s16, s32) __asm__("func_800A4688");
s32 func_800A56E0(s32);
s32 func_800BCB04(u16, u16, s16);
void func_800DBA90(u8 *);
void func_s05_80024D44(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s05_80024E98(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_80045340[];
extern s32 D_8008346C;
extern u8 D_80083498[];
extern u8 D_800DEB70[];
extern u8 D_800DEBD8[];
extern u8 D_800DEC00[];
extern u8 D_800DEC28[];
END DECLARATIONS */
void func_s05_8002401C(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q;
    u8 *e;
    UnkNode *n;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 r;
    s32 t;
    s32 u;
    s32 x;
    s32 y;
    s32 tx;
    s32 ty;
    s32 ax;
    s32 ay;
    UnkVec v;
    s16 out[4];
    u8 rec[0x20];
    SVECTOR vec;

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    i = (*(u16 *)(s + 0x2A) >> 9) & 7;
    tx = D_8006CCD8[i];
    ty = D_8006CCE8[i];
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (func_8003DE58(*(u8 **)(*(u8 **)(w + 0xC) + 0x8), (UnkSlot *)*(u8 **)(w + 0xC), out,
                          0) == 0)
        {
            out[0] = out[1] = 0;
            out[2] = ((*(u16 *)(o + 0x14) & 0x8000) != 0) ? -0x30 : 0;
        }
        b->unk_00 = *(s32 *)(*(u8 **)(w + 0x8) + 0x0) + (out[0] << 16);
        b->unk_04 = *(s32 *)(*(u8 **)(w + 0x8) + 0x4) + (out[1] << 16);
        b->unk_08 = *(s32 *)(*(u8 **)(w + 0x8) + 0x8) + (out[2] << 16);
        n = (UnkNode *)(p - 0x20);
        func_8004491C(n, (u32)D_80045340);
        *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x1000;
        *(u8 **)(c + 0x0) = D_800DEBD8;
        *(s32 *)(c + 0xC) = 0x808080;
        *(u16 *)(c + 0x14) |= 0xC;
        *(u8 *)(c + 0x4) = 0;
        *(u8 *)(c + 0x5) = 0;
        *(s16 *)(c + 0x10) = 0x20;
        *(s32 *)(c + 0x8) = *(s32 *)(*(u8 **)(c + 0x0) + 0x4);
        *(s32 *)(s + 0x60) = func_800A05A4(s, *(u8 *)(o + 0x24), *(u8 *)(o + 0x25),
                                           *(s16 *)(s + 0x2A), (s16)func_800A3820(5));
        if (*(s32 *)(s + 0x60) == 0)
        {
            ax = ay = i = 0;
            while (i < func_800A3820(5))
            {
                x = (*(u8 *)(o + 0x24) + ax) * 64 + 0x20;
                y = (*(u8 *)(o + 0x25) + ay) * 64 + 0x20;
                if ((s16)func_800A4688s(x, y, (s16)func_800BCB04(x, y, -0x400),
                                        *(s16 *)(s + 0x2A), *(s32 *)(s + 0x60)) != 0)
                {
                    break;
                }
                ay += ty;
                i++;
                ax += tx;
            }
            *(u8 *)(s + 0x72) = *(u8 *)(o + 0x24) + tx * i;
            *(u8 *)(s + 0x73) = *(u8 *)(o + 0x25) + ty * i;
        }
        else
        {
            n = (UnkNode *)(*(u8 **)(s + 0x60) - 0x20);
            q = (u8 *)n->unk_0C;
            if ((*(u16 *)(q + 0x14) & 0x8000) != 0 && (*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                func_8009CE1C(*(u8 **)(s + 0x60), 8, *(u8 *)(p + 0x9), 2, *(s16 *)(s + 0x2A),
                              (u32)s, 2);
                *(s16 *)(p + 0xA) = 4;
                break;
            }
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        *(s16 *)((u8 *)&v + 0x2) = *(s8 *)(s + 0x72) * 0x40 + 0x20;
        *(s16 *)((u8 *)&v + 0x6) = *(s8 *)(s + 0x73) * 0x40 + 0x20;
        if (*(s32 *)(s + 0x60) == 0)
        {
            *(s16 *)((u8 *)&v + 0xA) =
                func_800BCB04(*(u16 *)((u8 *)&v + 0x2), *(u16 *)((u8 *)&v + 0x6),
                              *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30);
            if (*(s16 *)((u8 *)&v + 0xA) >= 0x200)
            {
                *(s16 *)((u8 *)&v + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA);
            }
            *(s16 *)((u8 *)&v + 0xA) = *(u16 *)((u8 *)&v + 0xA) - 0x30;
        }
        else
        {
            *(s16 *)((u8 *)&v + 0xA) = *(u16 *)((u8 *)n->unk_08 + 0xA) - 0x28;
        }
        j = *(s16 *)((u8 *)&v + 0x2) - *(s16 *)((u8 *)b + 0x2);
        k = *(s16 *)((u8 *)&v + 0x6) - *(s16 *)((u8 *)b + 0x6);
        m = __builtin_abs(j);
        r = __builtin_abs(k);
        if (m < r)
        {
            m = r;
        }
        *(s16 *)(p + 0x50) = 0;
        for (t = 0; t < (m << 16); t += 0xD6666)
        {
            *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) + 1;
        }
        r = 0;
        for (i = 1; i <= *(s16 *)(p + 0x50); i++)
        {
            r += i;
        }
        *(s32 *)(p + 0x44) = ((j << 16) - tx * 0x66666 * *(s16 *)(p + 0x50)) / r;
        *(s32 *)(p + 0x48) = ((k << 16) - ty * 0x66666 * *(s16 *)(p + 0x50)) / r;
        *(s32 *)(p + 0x4C) = (v.unk_08 - b->unk_08) / r;
        b->unk_0C = tx * 0x66666 + *(s16 *)(p + 0x50) * *(s32 *)(p + 0x44);
        b->unk_10 = ty * 0x66666 + *(s16 *)(p + 0x50) * *(s32 *)(p + 0x48);
        b->unk_14 = *(s16 *)(p + 0x50) * *(s32 *)(p + 0x4C);
        *(s16 *)(p + 0x52) = *(u16 *)(p + 0x50);
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        for (i = 0x14; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s05_80024D44;
                vec.vx = -(rand() & 0x1FF);
                vec.vy = vec.vz = 0;
                *(u8 **)(rec + 0x0) = (u8 *)&vec;
                *(u8 **)(rec + 0x4) = (u8 *)&vec;
                j = *(s16 *)(p + 0x52) - *(s16 *)(p + 0x50);
                m = 0x300 - j * 32 - j * 8;
                e = (u8 *)n + 0x20;
                if (m < 0x40)
                {
                    m = 0x40;
                }
                *(s16 *)(rec + 0x8) = (rand() & 0xFF) - 0x80;
                *(s16 *)(rec + 0xC) = (rand() & 0xFF) - 0x80;
                k = *(s16 *)(rec + 0x8);
                if (k <= 0)
                {
                    *(s16 *)(rec + 0x8) = k - m;
                }
                else
                {
                    *(s16 *)(rec + 0x8) = k + m;
                }
                r = *(s16 *)(rec + 0xC);
                u = *(s16 *)(s + 0x2A);
                if (r <= 0)
                {
                    *(s16 *)(rec + 0xC) = r + (u - m);
                }
                else
                {
                    *(s16 *)(rec + 0xC) = r + (m + u);
                }
                *(s16 *)(rec + 0x10) = 0;
                *(s16 *)(rec + 0x12) = 0;
                *(s16 *)(rec + 0x14) = 0;
                *(s16 *)(rec + 0x18) = 1;
                *(s16 *)(rec + 0x1A) = 0;
                func_800DBA90(rec);
                *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00 + (((rand() & 0x1FF) - 0xFF) << 9);
                *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04 + (((rand() & 0x1FF) - 0xFF) << 9);
                *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + (((rand() & 0x1FF) - 0xFF) << 9);
                *(s32 *)((u8 *)n->unk_08 + 0x0) += (vec.vx << 12) + (tx << 21);
                *(s32 *)((u8 *)n->unk_08 + 0x4) += (vec.vy << 12) + (ty << 21);
                *(s32 *)((u8 *)n->unk_08 + 0x8) += vec.vz << 12;
                *(s32 *)((u8 *)n->unk_08 + 0xC) = b->unk_0C >> 3;
                *(s32 *)((u8 *)n->unk_08 + 0x10) = b->unk_10 >> 3;
                *(s32 *)((u8 *)n->unk_08 + 0x14) = b->unk_14 >> 3;
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x800;
                *(s16 *)(q + 0x10) = 0x60;
                *(u8 **)(q + 0x0) = D_800DEC28;
                *(u16 *)(q + 0x14) |= 0xC;
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(s32 *)(q + 0xC) = 0x808080;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DEC28 + 0x4);
                *(u8 **)(e + 0x0) = p;
                *(s16 *)(e + 0x48) = rand() & 3;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
        b->unk_0C -= *(s32 *)(p + 0x44);
        b->unk_10 -= *(s32 *)(p + 0x48);
        b->unk_14 -= *(s32 *)(p + 0x4C);
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u16 *)(c + 0x14) &= 0x9FFF;
            *(u8 **)(c + 0x0) = D_800DEC00;
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
            *(s32 *)(c + 0x8) = *(s32 *)(D_800DEC00 + 0x4);
        }
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if ((*(s16 *)((u8 *)b + 0x2) / 0x40 == *(s8 *)(s + 0x72) &&
             *(s16 *)((u8 *)b + 0x6) / 0x40 == *(s8 *)(s + 0x73) && *(s16 *)(p + 0x50) < 0) ||
            *(s16 *)(p + 0x50) < -0x20)
        {
            if (*(s32 *)(s + 0x60) == 0)
            {
                *(s16 *)(p + 0xA) = 3;
                break;
            }
            *(u8 **)(c + 0x0) = D_800DEB70;
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
            *(s32 *)(c + 0xC) = 0xC0C0C0;
            *(s16 *)(c + 0x10) = 0x20;
            *(s32 *)(c + 0x8) = *(s32 *)(D_800DEB70 + 0x4);
            *(s16 *)((u8 *)b + 0x2) = *(s8 *)(s + 0x72) * 0x40 + 0x20;
            b->unk_0C = b->unk_10 = b->unk_14 = 0;
            *(s16 *)((u8 *)b + 0x6) = *(s8 *)(s + 0x73) * 0x40 + 0x20;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            break;
        }
        if (*(s32 *)(s + 0x60) != 0)
        {
            func_8009CE1C(*(u8 **)(s + 0x60), 0xA, *(u8 *)(p + 0x9), 2, *(s16 *)(s + 0x2A), (u32)s,
                          2);
            for (i = 0x50; i >= 0; i--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    q = (u8 *)n->unk_0C;
                    n->unk_10 = (u32)func_s05_80024E98;
                    *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00 + (((rand() & 0x3FF) - 0x1FF) << 13);
                    *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04 + (((rand() & 0x3FF) - 0x1FF) << 13);
                    *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + (((rand() & 0x3FF) - 0x1FF) << 12);
                    *(s32 *)((u8 *)n->unk_08 + 0xC) =
                        (b->unk_00 - *(s32 *)((u8 *)n->unk_08 + 0x0)) >> 3;
                    *(s32 *)((u8 *)n->unk_08 + 0x10) =
                        (b->unk_04 - *(s32 *)((u8 *)n->unk_08 + 0x4)) >> 3;
                    *(s32 *)((u8 *)n->unk_08 + 0x14) = -((rand() & 0x3FF) << 9);
                    *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x800;
                    *(s16 *)(q + 0x10) = 0x20;
                    *(u8 **)(q + 0x0) = D_800DEC28;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(s32 *)(q + 0xC) = 0x808080;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DEC28 + 0x4);
                    e = (u8 *)n + 0x20;
                    *(u8 **)((u8 *)n + 0x20) = p;
                    *(s16 *)(e + 0x48) = rand() & 3;
                }
            }
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 3:
        *(s32 *)(c + 0xC) += 0xFFEFEFF0;
        if (*(u8 *)(c + 0xC) == 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if ((*(s32 *)(p + 0xC) & 0x8000) != 0)
        {
            *(s32 *)(p + 0xC) &= 0xFFFF7FFF;
        }
        else
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
