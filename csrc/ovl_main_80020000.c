#include "common.h"
#include "main.h"
#include "strings.h"

/* main.bin's overlay at 0x80020000: the memory card save/load subsystem.
   Built at cc1psx -G0, not the -G8 the resident executable uses; the Makefile
   has the per-unit override and says why.

   This file is still one splat unit, but the original source was NINE:
   c_server.c, card_mes.c, card_opt.c, card_win.c, cardbk.c, cardinfo.c,
   cardslot.c, cardsme.c and memory.c, in that link order. The names and the
   boundaries come from the debug twin at 0x80400000, which is the same source
   built at -G8 with more of its asserts compiled in; `tools/assertmap.py`
   recovers the map and docs/overlays.md records it. Carve this file along
   those boundaries before matching anything whose shape depends on which unit
   it is in. */

/* c_server.c, somewhere in lines 1-180: the first function in the overlay and
   the first one landed. Its twin is func_804008A0.

   The pointer local is load-bearing and is the lever at
   docs/matching-levers.md:3173, spent at the two stores and NOT at the read.
   Storing straight to the global emits each `%hi` beside its own store, so the
   else branch gets a second `lui` and the function runs one word long (25).
   Through a local assigned after the three calls, the `%hi` is emitted at the
   assignment -- before the branch, so both stores share $v1, and reorg pulls
   it into the beqz delay slot. The trailing read is left as the bare symbol so
   it keeps its own re-materialised `lui $v0`, which is what the listing has.
   The local costs no instruction: combine folds the `%lo` back into the
   stores. */
void func_80020924(s32 arg0)
{
    s32 *p;

    func_800214A4();
    _card_wait(0);
    _card_wait(1);
    p = &gCardChannel;
    if (arg0)
    {
        *p = 0x10;
    }
    else
    {
        *p = 0;
    }
    func_800213C4(gCardChannel);
}

void func_80020984(void)
{
    func_800214A4();
    _card_wait(0);
    _card_wait(1);
    func_800213C4(gCardChannel);
}

void func_800209C4(void)
{
    s32 v;

    func_800214A4();
    _card_wait(0);
    _card_wait(1);
    v = 0x10 - gCardChannel;
    gCardChannel = v;
    func_800213C4(v);
}

void func_80020A0C(void)
{
    s32 *p = &D_800287C8;

    *p = 0;
    func_800214A4();
}

long HandleCardEOF(void)
{
    gCardState = CARD_STATE_EOF;
    return 0;
}

long HandleCardError(void)
{
    gCardState = CARD_STATE_ERROR;
    return 0;
}

long HandleCardTimeout(void)
{
    gCardState = CARD_STATE_TIMEOUT;
    return 0;
}

long HandleCardNew(void)
{
    gCardState = CARD_STATE_NEW;
    return 0;
}

/* c_server.c's event setup, and the overlay's second largest function: eight
   OpenEvent (OpenEvent) calls inside the EnterCriticalSection / ExitCriticalSection
   critical section, each asserting the handle is not -1, then eight
   EnableEvent (EnableEvent) calls each asserting a non-zero result. Source
   lines 185..200 and 212..226.

   It falls out whole once exit is NORETURN (see func_80020F18): each
   of the sixteen %hi values then stays live in its own callee-saved register
   across the calls, which is why the function saves $s0..$s7 and $fp, and
   $s0 holds the -1 the first eight comparisons share. */
void func_80020A80(void)
{
    EnterCriticalSection();
    gSwCardEOFEvent = OpenEvent(SwCARD, EvSpIOE, EvMdNOINTR, NULL);
    if (gSwCardEOFEvent == -1)
    {
        printf(D_80020000, D_80020028, 185);
        exit(1);
    }
    gSwCardErrorEvent = OpenEvent(SwCARD, EvSpERROR, EvMdNOINTR, NULL);
    if (gSwCardErrorEvent == -1)
    {
        printf(D_80020000, D_80020028, 187);
        exit(1);
    }
    D_80028558 = OpenEvent(SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    if (D_80028558 == -1)
    {
        printf(D_80020000, D_80020028, 189);
        exit(1);
    }
    D_8002855C = OpenEvent(SwCARD, EvSpNEW, EvMdNOINTR, NULL);
    if (D_8002855C == -1)
    {
        printf(D_80020000, D_80020028, 191);
        exit(1);
    }
    D_80028560 = OpenEvent(HwCARD, EvSpIOE, EvMdINTR, HandleCardEOF);
    if (D_80028560 == -1)
    {
        printf(D_80020000, D_80020028, 194);
        exit(1);
    }
    D_80028564 = OpenEvent(HwCARD, EvSpERROR, EvMdINTR, HandleCardError);
    if (D_80028564 == -1)
    {
        printf(D_80020000, D_80020028, 196);
        exit(1);
    }
    D_80028568 = OpenEvent(HwCARD, EvSpTIMOUT, EvMdINTR, HandleCardTimeout);
    if (D_80028568 == -1)
    {
        printf(D_80020000, D_80020028, 198);
        exit(1);
    }
    D_8002856C = OpenEvent(HwCARD, EvSpNEW, EvMdINTR, HandleCardNew);
    if (D_8002856C == -1)
    {
        printf(D_80020000, D_80020028, 200);
        exit(1);
    }
    ExitCriticalSection();
    if (EnableEvent(gSwCardEOFEvent) == 0)
    {
        printf(D_80020000, D_80020028, 212);
        exit(1);
    }
    if (EnableEvent(gSwCardErrorEvent) == 0)
    {
        printf(D_80020000, D_80020028, 214);
        exit(1);
    }
    if (EnableEvent(D_80028558) == 0)
    {
        printf(D_80020000, D_80020028, 216);
        exit(1);
    }
    if (EnableEvent(D_8002855C) == 0)
    {
        printf(D_80020000, D_80020028, 218);
        exit(1);
    }
    if (EnableEvent(D_80028560) == 0)
    {
        printf(D_80020000, D_80020028, 220);
        exit(1);
    }
    if (EnableEvent(D_80028564) == 0)
    {
        printf(D_80020000, D_80020028, 222);
        exit(1);
    }
    if (EnableEvent(D_80028568) == 0)
    {
        printf(D_80020000, D_80020028, 224);
        exit(1);
    }
    if (EnableEvent(D_8002856C) == 0)
    {
        printf(D_80020000, D_80020028, 226);
        exit(1);
    }
}

void func_80020EA0(void)
{
    if (D_80084130[0] == 0)
    {
        D_80084130[0] = 1;
        D_80084118 = 0;
        (&D_80084118)[1] = 0;
        bzero((unsigned char *)D_80083E98, 0x280);
    }
    func_80020A80();
    D_800287D0 = 0;
    (&D_800287D0)[1] = 0;
    D_800287D8 = 0;
    (&D_800287D8)[1] = 0;
    D_800287C8 = 0;
}

/* c_server.c's teardown: eight asserts bracketed by EnterCriticalSection /
   ExitCriticalSection (EnterCriticalSection / ExitCriticalSection), one per event
   handle, at source lines 263..277 in steps of two.

   exit has to be declared NORETURN. Without it gcc assumes each
   assert body falls through into the next test, so after the two calls it
   re-materialises the next handle's %hi -- one extra word per assert, 121
   against 114. With it the `lui` that reorg puts in each `bnez`'s delay slot
   is the one the branch target's `lw` uses. This is the lever that makes the
   whole assert family reachable; see func_80020A80, which has 24 of them. */
void func_80020F18(void)
{
    EnterCriticalSection();
    if (CloseEvent(gSwCardEOFEvent) == 0)
    {
        printf(D_80020000, D_80020028, 263);
        exit(1);
    }
    if (CloseEvent(gSwCardErrorEvent) == 0)
    {
        printf(D_80020000, D_80020028, 265);
        exit(1);
    }
    if (CloseEvent(D_80028558) == 0)
    {
        printf(D_80020000, D_80020028, 267);
        exit(1);
    }
    if (CloseEvent(D_8002855C) == 0)
    {
        printf(D_80020000, D_80020028, 269);
        exit(1);
    }
    if (CloseEvent(D_80028560) == 0)
    {
        printf(D_80020000, D_80020028, 271);
        exit(1);
    }
    if (CloseEvent(D_80028564) == 0)
    {
        printf(D_80020000, D_80020028, 273);
        exit(1);
    }
    if (CloseEvent(D_80028568) == 0)
    {
        printf(D_80020000, D_80020028, 275);
        exit(1);
    }
    if (CloseEvent(D_8002856C) == 0)
    {
        printf(D_80020000, D_80020028, 277);
        exit(1);
    }
    ExitCriticalSection();
}

s32 func_800210E0(void)
{
    return D_800287C8;
}

s32 func_800210F0(void)
{
    s32 st = D_800287C8;
    s32 r = 0;
    s32 *t;
    s32 *u;

    if (st == 0)
    {
        goto out;
    }
    if (st == 2)
    {
        goto out;
    }
    if ((u32)st >= 3)
    {
        goto high;
    }
    if (st == 1)
    {
        goto one;
    }
    r = 5;
    goto out;
high:
    if (st == 3)
    {
        goto three;
    }
    r = 5;
    goto out;
one:
    t = &D_80084118;
    if (gCardChannel != 0)
    {
        t++;
    }
    if (t[0] != 0)
    {
        r = 1;
        D_800287C8 = 0;
    }
    else if ((_card_status(0) & 1) != 0 || (_card_status(1) & 1) != 0)
    {
        func_800214A4();
        while (_card_load(gCardChannel) == 0)
        {
        }
        D_800287E0 = 0;
        D_800287C8 = 3;
    }
    goto out;
three:
    u = &D_80084118;
    if (gCardChannel != 0)
    {
        u++;
    }
    r = 2;
    u[0] = 1;
    D_800287C8 = 0;
    /* DEVICE: an empty volatile asm, worth 1 word. It emits nothing; it exists
       only to put a non-SET insn at the end of this basic block. gcc 2.7.2 runs
       jump_optimize with cross_jump AFTER sched2 and BEFORE dbr_schedule
       (toplev.c:3117/3142/3167), and for an unconditional `j' find_cross_jump is
       called with minimum == 1 (jump.c:1978), so ONE matching insn before the
       jump is enough to merge. Without this asm the insn before `out:' is
       `sw $zero,%lo(D_800287C8)($a1)', identical to the one that ends the st==1
       block above, so the st==1 block's `j out' is retargeted onto this one; dbr
       then fills its delay slot with the preceding `addiu $s1,1' instead of the
       store and the function comes out 74 words (47 DIFF). The asm's pattern is
       ASM_INPUT, not SET, so find_cross_jump breaks on the GET_CODE(p1) !=
       GET_CODE(p2) test with minimum still 1 and no merge happens. It cannot be
       moved: between the two stores it is 74 words again, and at the end of the
       st==1 block it is 76 (ASM_INPUT also stops dbr's backward scan, so that
       block's delay slot goes to a nop).

       Ruled out, all measured with tools/decomp/ovlverify.py: reordering the
       st==1 tail to `D_800287C8 = 0; r = 1;' keeps 75 words but 2 DIFF (sched2
       does not swap the two leaf stores back -- they tie on priority and the tie
       breaks to source order); swapping the st==3 tail (any of its six orders)
       74w/47 or 75w/4; `*(volatile s32 *)&D_800287C8 = 0' on either store
       74w/47 (rtx_renumbered_equal_p ignores MEM_VOLATILE_P); `*(u32 *)&',
       `*(s32 *)&' and `*(&D_800287C8)' 74w/47; a trailing `goto out;' 74w/47
       (the early jump_optimize deletes a jump to the next label long before the
       cross-jump pass); a trailing `return r;' 76w/15; a trailing `while (0) {}'
       75w/24; a trailing `;' or an unused label 74w/47.

       It does NOT warn under -Wall. */
    __asm__ volatile("");
out:
    return r;
}

s32 func_8002121C(void)
{
    s32 r = 0;

    if (D_800287C8 == 2)
    {
        func_800214A4();
        if (_card_load(gCardChannel) == 0)
        {
            D_800287C8 = 0;
            r = 5;
        }
        else
        {
            D_800287E0 = 0;
            D_800287C8 = D_800287C8 + 1;
        }
    }
    return r;
}

/* The three `goto`s out of the dispatch, the DUPLICATED fail body and the
   trailing `def:` block are all load-bearing; measurements in
   docs/matching-levers.md under "A DUPLICATED FAIL BODY IS WHAT PUTS IT IN
   cse1's PATH".

   - The fail body has to appear TWICE, once per test. cse1 can only share the
     one `lui %hi(D_800287C8)` with a block it reaches on a path from the
     function's first insn, and cse_end_of_basic_block (cse.c:8103) will only
     extend a path across a conditional jump whose target label has
     LABEL_NUSES == 1. A single shared fail block (`&&`, or either nesting)
     gives that label two uses, so the fail body gets a SECOND `lui`: 58 words
     / 23 DIFF. jump2's cross-jumping merges the two copies back into one.
   - `def:` must sit between the st==3 arm and `out:`. Without it the st==3
     arm's trailing `D_800287C8 = 0` is the insn before `out:` and is the same
     word as the fail body's, so find_cross_jump (jump.c:1978, minimum 1)
     steals the fail copies' store instead and each copy loses a word to the
     merge: 58 words / 25 DIFF. With `def:` in between, the insn before `out:`
     is `r = 5`, the compare fails, and the two fail copies merge with each
     other instead -- which is the 3-word block the target has.

   Ruled out, all measured: plain if/else-if chain 56w/44; `st != 1` first
   56w/44; fully nested `!=` 56w/44; `switch` 58w/23; a single-fail `goto`
   chain 58w/23; duplicated fail bodies inline (if/else-if/else) 58w/36;
   duplicated fail bodies out of line without `def:` 58w/25; duplicated with
   `D_800287C8 = 0;` before `r = 5;` 57w/2; duplicated with the st==3 arm's
   two stores swapped 57w/2. */
s32 func_80021298(void)
{
    s32 st = D_800287C8;
    s32 r = 0;
    s32 *t;

    if (st == 0)
    {
        goto out;
    }
    if (st == 1)
    {
        goto one;
    }
    if (st == 3)
    {
        goto three;
    }
    goto def;
one:
    func_80021538();
    if (_card_clear(gCardChannel) == 0)
    {
        goto fail1;
    }
    if ((_card_status(gCardChannel != 0) & 4) == 0)
    {
        goto fail2;
    }
    D_800287E0 = st;
    D_800287C8 = D_800287C8 + 1;
    goto out;
fail1:
    r = 5;
    D_800287C8 = 0;
    goto out;
fail2:
    r = 5;
    D_800287C8 = 0;
    goto out;
three:
    t = &D_80084118;
    if (gCardChannel != 0)
    {
        t++;
    }
    r = 3;
    t[0] = 0;
    D_800287C8 = 0;
    goto out;
def:
    r = 5;
out:
    return r;
}

s32 func_8002137C(void)
{
    s32 *p = &D_800287C8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 4;
        *p = 0;
    }
    return r;
}

s32 func_800213A0(void)
{
    s32 *p = &D_800287C8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 5;
        *p = 0;
    }
    return r;
}

/* Returns 1, and that return value is what the extra word is for. The unit's
   own matched twin of this tail -- func_800210F0's st==1 block, same
   `D_800287E0 = 0; D_800287C8 = <const>;' after the same call-and-spin -- comes
   out `lui' in the delay slot, `lui', `sw $zero', `addiu $v0, $zero, 3',
   `sw $v0'. Here the target instead materialises the 1 FIRST (into the delay
   slot) and copies it, `addu $v1, $v0, $zero', before the store. That is
   because $v0 is pinned: the return value's hard-register set is
   `(set (reg:SI 2 v0) (const_int 1))', cse2 unifies it with the store's
   constant, so the one pseudo holding 1 lives in $v0 up to the `jr' and the
   store's source has to be a second register. Three regs are live at the peak
   ($v0 = 1, $v1 = %hi(D_800287E0), $a0 = %hi(D_800287C8)), which is also why
   the second `lui' lands in $a0 rather than reusing $v0.

   Declaring it `void' is what parked it: with no return value the constant is
   an ordinary statement-level quantity, the allocator puts it straight in the
   store's register and there is no copy -- 18 words, 10 DIFF, whatever the tail
   is spelled like (`t = 1; ... D_800287C8 = t;' 18w/11; `while' for `do/while'
   and the two stores swapped 18w/10-11). `s32 t = 1; ... return t;' is worse
   still at 21w/19: `t' then spans the calls and takes $s1.

   The three callers in this unit discard the result, so the prototype change
   from `void' is invisible to them -- all four containers stay byte-identical. */
s32 func_800213C4(s32 a)
{
    func_800214A4();
    do
    {
    } while (_card_info(a) == 0);
    D_800287E0 = 0;
    D_800287C8 = 1;
    return 1;
}

s32 func_80021410(void)
{
    s32 r = 0;

    if (TestEvent(gSwCardEOFEvent) != 0)
    {
        r = 1;
    }
    else if (TestEvent(gSwCardErrorEvent) != 0)
    {
        r = 2;
    }
    else if (TestEvent(D_80028558) != 0)
    {
        r = 3;
    }
    else if (TestEvent(D_8002855C) != 0)
    {
        r = 4;
    }
    return r;
}

void func_800214A4(void)
{
    TestEvent(gSwCardEOFEvent);
    TestEvent(gSwCardErrorEvent);
    TestEvent(D_80028558);
    TestEvent(D_8002855C);
}

s32 func_800214FC(void)
{
    CARD_STATE v = gCardState;
    CARD_STATE r = 0;

    if (v != CARD_STATE_OK)
    {
        r = v;
        func_80021538();
    }
    return r;
}

void func_80021538(void)
{
    gCardState = CARD_STATE_OK;
    TestEvent(D_80028560);
    TestEvent(D_80028564);
    TestEvent(D_80028568);
    TestEvent(D_8002856C);
}

s32 func_80021594(void)
{
    UnkFnTab5 a = D_80020034;
    UnkFnTab5 b = D_80020048;
    s32 r = 0;
    s32 st = D_800287E0;

    if (st == 0)
    {
        r = func_80021410();
        if (r != 0)
        {
            r = a.f[r]();
        }
    }
    else if (st == 1)
    {
        r = func_800214FC();
        if (r != 0)
        {
            r = b.f[r]();
        }
    }
    return r;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020000);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020028);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020034);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020048);

/* The st==3 arm carries its own COPY of the D_80028530[i] >= 0xB check rather
   than a goto into the st==4/5 arm's copy: jump.c's post-reload cross-jump
   then merges the two copies from the `addu $a1, $v1, $v0` backwards
   (jump.c:1978, minimum==1 for an unconditional j), leaving the st==3 block
   with just the `lui`/`addiu` of D_80028530 and its own `sll $v1, $a0, 2`,
   which reorg.c:2907's backward scan moves into the jump's delay slot. The
   `goto` spelling instead collapses the arm to a bare jump and the table
   entry for st==3 is retargeted at the shared check: 79 words. Note st==3
   does NOT clear D_80028530[i] -- that store only exists in the >= 0xB body.
   The st==0 exit must be this enclosing `if (st != 0)` with one `return st`
   at the end; an early `return st` is 82 words but 46 DIFF, because the
   target's branch goes to the shared `addu $v0, $s0, $zero`. */
s32 func_8002168C(void)
{
    s32 st = func_80021594();
    s32 i = func_8002190C();

    if (st != 0)
    {
        if ((&D_80028538)[i] == st)
        {
            (&D_80028530)[i]++;
        }
        else
        {
            (&D_80028530)[i] = 0;
        }
        (&D_80028538)[i] = st;
        switch (st)
        {
        case 1:
        case 2:
            (&D_800287D0)[i] = st;
            break;
        case 3:
            if ((&D_80028530)[i] >= 0xB)
            {
                (&D_800287D0)[i] = st;
                (&D_80028530)[i] = 0;
                (&D_80028538)[i] = 0;
            }
            break;
        case 4:
        case 5:
            (&D_80084118)[i] = 0;
            if ((&D_80028530)[i] >= 0xB)
            {
                (&D_800287D0)[i] = st;
                (&D_80028530)[i] = 0;
                (&D_80028538)[i] = 0;
            }
            break;
        }
        func_80020924(i);
    }
    return st;
}

/* A button-repeat counter over the two-entry D_80028540/D_80028548 pair. It
   RETURNS &D_800287D8, which is why the target computes that address on both
   exits and then never reads it -- as a void function those two words
   disappear and nothing else changes. Both arms repeat the pair of clearing
   stores so jump.c cross-jumps them into the shared tail at $a0/$a1. */
s32 *func_800217D4(void)
{
    s32 v = func_80021410();
    s32 i;

    if (v != 0)
    {
        i = func_8002190C();
        if ((&D_80028548)[i] == v)
        {
            (&D_80028540)[i]++;
        }
        else
        {
            (&D_80028540)[i] = 0;
        }
        (&D_80028548)[i] = v;
        if (v == 1 || v == 4)
        {
            if ((&D_80028540)[i] >= 4)
            {
                (&D_800287D8)[i] = 1;
                (&D_80028540)[i] = 0;
                (&D_80028548)[i] = 0;
            }
        }
        else
        {
            (&D_80084118)[i] = 0;
            if ((&D_80028540)[i] >= 4)
            {
                (&D_800287D8)[i] = 0;
                (&D_80028540)[i] = 0;
                (&D_80028548)[i] = 0;
            }
        }
        func_800209C4();
    }
    return &D_800287D8;
}

void func_80021904(void)
{
}

s32 func_8002190C(void)
{
    return gCardChannel != 0;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020074);

s32 func_8002191C(void)
{
    u8 name[0x20];
    s32 n = 0;
    struct DIRENTRY *e = D_80028570;
    char **mem_card_name = &MEM_CARD_NAMES[0];

    if (gCardChannel != 0)
    {
        mem_card_name++;
    }
    strcpy(name, *mem_card_name);
    strcat(name, (u8 *)D_80020084);
    if (firstfile2(name, e) != NULL)
    {
        do
        {
            n++;
            e = (struct DIRENTRY *)((u8 *)e + 0x28);
        } while (nextfile(e) == e);
    }
    return n;
}

/* splat MIGRATED this block into func_8002191C's nonmatching listing, so
   landing that function in C left the symbol with no definition and the link
   failed with "undefined reference to D_80020084". It cannot be replaced by a
   `"*"` literal in the function either: the label covers 28 bytes holding
   SEVEN strings, lumped under one name because only the first is referenced
   symbolically, and "TMP" and "04".."00" belong to functions still in asm.
   So it is defined here, immediately after the function whose listing used to
   carry it, which is its place in the assembly stream. As those other
   functions land, their literals come from gcc and this shrinks.

   (Turning splat's `migrate_rodata_to_functions` off is NOT the fix, though
   it looks like one: with migration off splat routes the whole `.rodata`
   subsegment to a standalone asm/data file instead of into the sibling C
   object, and then gcc's own constants -- func_800258C8's local
   function-pointer array -- have nowhere correct to land.) */
const u8 D_80020084[28] = "*\0\0\0TMP\0"
                          "04\0\0"
                          "03\0\0"
                          "02\0\0"
                          "01\0\0"
                          "00\0\0";

s32 func_800219B8(s32 n)
{
    s32(*p)[10] = (s32(*)[10])D_80028570;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < n; i++)
    {
        sum += p[i][6];
    }
    sum = sum / 0x2000;
    return 0xF - sum;
}

s32 func_80021A04(s32 n, u8 *s)
{
    s32 i = 0;
    s32 ret = 0;
    u8 *tbl = (u8 *)D_80028570;
    u8 *row;

    if (n > 0)
    {
        row = tbl;
        do
        {
            s32 bad = 0;
            s32 j = 0;

            do
            {
                u8 c = s[j];

                if (c == 0)
                {
                    break;
                }
                if (c != row[j])
                {
                    bad++;
                    break;
                }
                j++;
            } while (j < 0x15);
            if (bad == 0)
            {
                ret = 1;
                break;
            }
            row += 0x28;
        } while (++i < n);
    }
    return ret;
}

s32 func_80021A84(s32 flag)
{
    char **t;
    s32 r;

    _card_wait(0);
    _card_wait(1);
    func_80021538();
    t = D_80027F18;
    if (flag != 0)
    {
        t++;
    }
    format(*t);
    while ((r = func_800214FC()) == 0)
    {
        VSync(0);
    }
    if (r != 1)
    {
        if (r == 3)
        {
            r = 4;
        }
        else
        {
            r = 5;
        }
    }
    return r;
}

void func_80021B18(u8 *s, s32 idx)
{
    char **mem_card_name = &MEM_CARD_NAMES[0];

    if (gCardChannel != 0)
    {
        mem_card_name++;
    }
    strcpy(s, *mem_card_name);
    strcat(s, D_80027F20);
    strcat(s, D_80027F30[idx]);
}

s32 func_80021B98(u8 *name, u8 *buf, s32 n, s32 off)
{
    s32 r = 0;
    s32 fd;

    fd = open(name, 1);
    if (fd != -1)
    {
        n = n << 7;
        if (off == 0 || lseek(fd, off << 7, 0) != -1)
        {
            r = read(fd, buf, n) == n;
        }
        close(fd);
    }
    /* DEVICE: the two nested degenerate loops weight the `return r`
       reference by 3 (flow.c:440 counts the LOOP_BEG/END notes,
       propagate_block does `reg_n_refs[regno] += loop_depth`), which takes
       r's quantity from 3 references to 5. Worth 11 words: without them r
       and n swap $s1 and $s2 and every use of either differs. They emit no
       instruction and the function is otherwise unchanged; it is still
       `return r;`. See "A `return` WEIGHTED BY TWO NESTED DEGENERATE LOOPS"
       in docs/matching-levers.md for the arithmetic -- global.c's
       allocno_compare wants r's priority strictly between n's 10/19 and
       fd's 10/17, and 10/18 is the only value that fits.
       Spellings ruled out, all 45 words: the plain `return r;` 11 DIFF; ONE
       degenerate loop (4 refs) 11; nested `if (1)` instead of the loops
       (inert, 3 refs) 11; THREE nested loops (6 refs, r takes $s0) 15;
       `do { r = 0; } while (0)` as well (prologue reschedules) 6;
       the two natural extra mentions of r -- `off == r` for `off == 0` and
       `r` for the lseek whence argument -- are constant-propagated by cse1
       and leave the dumps bit-identical, 11. `for (;;)` and `while (1)`
       nested twice both MATCH and are interchangeable with this.
       Warns under -Wall? No -- nothing is read uninitialised and the
       `while (0)` conditions are constant by construction, so the build is
       silent with or without the flag. */
    do
    {
        do
        {
            return r;
        } while (0);
    } while (0);
}

s32 func_80021C4C(u8 *name, u8 *buf, s32 n, s32 off, s32 skip)
{
    s32 ok = 0;
    s32 fd;

    if (skip == 0)
    {
        fd = open(name, 0x30200);
        if (fd == -1)
        {
            return ok;
        }
        close(fd);
    }
    fd = open(name, 2);
    if (fd == -1)
    {
        return ok;
    }
    if (off == 0 || lseek(fd, off << 7, 0) != -1)
    {
        if (write(fd, buf, n << 7) == (n << 7))
        {
            ok = 1;
        }
    }
    close(fd);
    return ok;
}

s32 func_80021D38(s32 i)
{
    return D_80083E98[i][0];
}

s32 func_80021D54(s32 idx)
{
    u8 name[0x20];
    s32 r = 1;
    s32 **t;
    s32 *e;
    s32(*tbl)[32];
    s32 v;

    func_80021B18(name, idx);
    t = (s32 **)D_80084128;
    if (gCardChannel != 0)
    {
        t++;
    }
    v = func_80021A04((s32)*t, name + 5);
    tbl = D_80083E98;
    e = tbl[idx];
    e[0] = v;
    if (v != 0)
    {
        r = func_80021B98(name, (u8 *)e, 1, 4);
        e[0] = 1;
    }
    return r;
}

void func_80021DF4(s32 idx, u8 *src)
{
    s32 *row;

    /* The second row address must be computed in its own statement: as a
       MEM address (`D_80083E98[idx][0] = 1;`) gcc's memory_address forces
       the symbol into a register BEFORE the index shift, giving
       lui/addiu/sll; assigning the row pointer first expands the shift in
       its own statement and the symbol materialises after it, which is the
       target's sll/lui/addiu order. Ruled out at 7 DIFF, all identical
       code: the plain 2-D index, a byte offset on either or both accesses,
       `*D_80083E98[idx]`, `&D_80083E98[idx][0]`, the D_80083E98_s struct
       alias, and `((s32 *)D_80083E98)[idx * 32]`. */
    *(UnkRow *)D_80083E98[idx] = *(UnkRow *)(src + 0x200);
    row = D_80083E98[idx];
    row[0] = 1;
}

void func_80021EB8(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 n = 0x177D;

    do
    {
        sum ^= *q++;
        n--;
    } while (n >= 0);
    gsw[0x81] = sum;
}

/* The `while (1)` plus `break` is load-bearing, not style: with a `for` or
   `while` whose test is the loop's last insn, loop.c's check_dbra_loop
   reverses this into the downcount form func_80021EB8 has (`li 0x177D` /
   `addiu -1` / `bgez`), 13 words. Here the conditional branch is followed by
   the backward jump, so get_condition_for_loop finds no final conditional
   branch and check_dbra_loop bails before it can reverse -- jump2 then merges
   the two branches back into the one `bnez`. The walking `*q++` is equally
   load-bearing: `q[i]` makes the cursor a loop.c giv whose initialisation is
   emitted at the loop start, i.e. AFTER the source's own `i = 0`, and
   local_alloc then gives the counter $a1 and the cursor $v1 -- 14 words, 6
   DIFF, all register names.
   Ruled out, measured: `q[i]` in twelve spellings (declaration orders, `i = 0`
   as a declaration initialiser, `while`, `do`/`while`, `gsw[0x82 + i]`,
   `(gsw + 0x82)[i]`, `(... ^ sum) == 0`, u32) -- all 14 words / 6 DIFF;
   `for (i = 0; i < 0x177E; i++) sum ^= *q++;` -- 13 words / 11 DIFF
   (reversed); `do { } while (++i < 0x177E)` with `*q++` -- 13 / 11;
   `for (;;)` in place of `while (1)` -- 14 / 3 (the first iteration is
   peeled); `if (++i == 0x177E) break;` -- 14 / 6 (`bne` against a
   register-held 0x177E instead of `slti`/`bnez`). `if (++i >= 0x177E) break;`
   and a `goto` loop also match. */
s32 func_80021EE0(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 i = 0;

    while (1)
    {
        sum ^= *q++;
        i++;
        if (i >= 0x177E)
        {
            break;
        }
    }
    return gsw[0x81] == sum;
}

void func_80021DF4(s32 idx, u8 *src);

/* rename is almost certainly libapi `rename`: it takes two of the
   0x20-byte name buffers func_80021B18 builds, and this function uses it to
   move the real save to the scratch name (slot 5), read it back, and move it
   home again. The two buffers must be declared nameA-then-nameB to land at
   sp+0x18 and sp+0x38 in that order; reversed gives 19 DIFF. */
s32 func_80021F18(s32 idx, u8 *buf)
{
    u8 nameA[0x20];
    u8 nameB[0x20];
    s32(*tbl)[32];
    s32 *row;
    s32 r;

    func_80021B18(nameA, idx);
    func_80021B18(nameB, 5);
    tbl = D_80083E98;
    row = tbl[idx];
    if (row[0] != 0)
    {
        rename(nameA, nameB);
    }
    *(s32 *)(buf + 0x204) = 0;
    func_80021EB8((s32 *)buf);
    r = func_80021C4C(nameB, buf, 0xC0, 0, row[0]);
    if (r != 0)
    {
        func_80021DF4(idx, buf);
        func_800220DC();
    }
    rename(nameB, nameA);
    return r;
}

s32 func_80021FF0(s32 idx, u8 *buf, s32 flag)
{
    u8 name[0x20];
    s32 saved;
    s32 *row;
    s32(*tbl)[32];

    func_80021B18(name, idx);
    if (func_80021B98(name, buf, 0xC0, 0) != 0 && func_80021EE0((s32 *)buf) != 0)
    {
        if (flag != 0)
        {
            saved = *(s16 *)(buf + 0x208);
            *(s16 *)(buf + 0x208) = 0;
            *(s32 *)(buf + 0x204) = 0;
            func_80021EB8((s32 *)buf);
            tbl = D_80083E98;
            row = tbl[idx];
            *(s16 *)((u8 *)row + 8) = 0;
            row[1] = *(s32 *)(buf + 0x204);
            func_80021C4C(name, (u8 *)row, 1, 4, row[0]);
            func_80021C4C(name, buf, 1, 0, row[0]);
            *(s16 *)(buf + 0x208) = saved;
        }
        else
        {
            *(s16 *)(buf + 0x208) = 0;
        }
    }
}

void func_800220DC(void)
{
    s32 i = gCardChannel != 0;
    s32 n;

    n = func_8002191C();
    D_80084128[i] = n;
    D_80084120[i] = func_800219B8(n);
}

s32 func_80022138(void)
{
    s32 *p = D_80084120;

    if (gCardChannel != 0)
    {
        p++;
    }
    return *p;
}

s32 func_80022160(void)
{
    s32 count = 0;
    s32 i = 0;

    do
    {
        if (D_80083E98[i][0] != 0)
        {
            count++;
        }
    } while (++i < 5);
    return count;
}

s32 func_8002219C(s32 idx)
{
    u8 name[0x20];
    s32(*tbl)[32];
    s32 v;

    func_80021B18(name, idx);
    _card_wait(0);
    _card_wait(1);
    func_80021538();
    erase(name);
    while ((v = func_800214FC()) == 0)
    {
        VSync(0);
    }
    if (idx < 5)
    {
        tbl = D_80083E98;
        tbl[idx][0] = 0;
        func_800220DC();
    }
    return v == 1;
}

void func_8002223C(void)
{
    func_8002219C(5);
}

/* The space-skipping width measurer: for each of the a+1 segments it skips a
   run of spaces (one unit each), then walks the glyphs two bytes at a time
   (two units each), reporting the run start in *x, the glyph count in *y and
   the accumulated glyph total in *z.

   The space skip has to be a plain `while`, not a guarded `do`/`while`. Both
   compile to the same guard-plus-rotated-test shape, but written as
   `if (...) do ... while (...)` gcc shares ONE register for the 0x20 it
   compares against, where the target has three: one hoisted out of the outer
   loop, one hoisted into the skip loop by loop rotation, and one for the
   glyph walk. Writing the skip as a loop in its own right is what makes
   loop.c hoist that second copy. */
void func_8002225C(u8 *s, s32 a, s32 *x, s32 *y, s32 *z)
{
    s32 total = 0;
    s32 n = 0;
    s32 i = 0;

    *z = 0;
    if (a < 0)
    {
        return;
    }
    do
    {
        *z += n;
        while (*s == ' ')
        {
            s++;
            total++;
        }
        *x = total;
        n = 0;
        while (*s != ' ' && *s != 0)
        {
            s += 2;
            n++;
        }
        total += n * 2;
        *y = n;
    } while (++i <= a);
}

/* The statement order is load-bearing, and only one thing about it is: the
   `p[0xA]` store has to come SECOND, before the 0x13 and the word store.
   sched1 schedules this block backward and fills the `lbu p[0]` load delay
   with whatever insn pair is still unplaced; written here, the `(n << 3) + 4`
   pair is consumed early and the 0x13 constant becomes the filler, which puts
   its one-insn live range INSIDE the p[0] quantity's. local-alloc then prices
   the 0x13 pair at 2 refs / 1 insn = 1.0 against the p[0] quantity's 4 refs /
   5 insns = 0.8, so 0x13 takes $v0 and p[0] is pushed to $v1; the 0xA04010
   constant, now spanning insns 1..11, is blocked from $v0 by both byte
   constants and from $v1 by p[0], and lands in $a0, free because the first
   parameter is dead. With p[0] in $v1 the return value no longer collides with
   it, so sched2 hoists `addiu $v0, $a1, 0xC` to word 9 and reorg takes the
   `sb $v1, 0($a1)` for the `jr` delay slot instead of the return value. All
   three differences in the old park were that one local-alloc flip.

   The four stores other than `p[0] |= 0x80` must be in the order p[1], p[0xA],
   p[0xB], p+4; `p[0] |= 0x80` matches at any of the five positions. Measured
   (all 15 words): all 120 permutations of the five statements scored, five
   match, the next best is 3 differing. The parked order (p[1], p[0xB], p+4,
   p[0xA], p[0]) was 12, as were an explicit `u8`/`s32` temp for p[0] first, a
   local for the return value, and `u32 *` for the word store. */
u8 *func_80022300(s32 *a, u8 *p, s32 n)
{
    p[1] = 0x28;
    p[0xA] = (n << 3) + 4;
    p[0xB] = 0x13;
    *(s32 *)(p + 4) = 0xA04010;
    p[0] |= 0x80;
    return p + 0xC;
}

void func_8002233C(u8 *p, s32 idx, s32 n)
{
    s32 i;

    func_8004E884(p, 1);
    p += idx * 12;
    for (i = 0; i < n; i++)
    {
        func_8004E264(p, 0);
        p += 12;
    }
}

u8 *func_80022300(s32 *a, u8 *p, s32 n);

/* p[569] is ptrs[6] of the 0x8B0 group (ptrs starts at 0x8CC) and p[563 + n]
   is ptrs[n]; the two s16 stores are the sprite's x and y, each a position
   minus half the corresponding extent. `x * 4` has to be a local declared
   BEFORE the other two: the target computes it at word 3, ahead of the $s4
   save and the `p + 0xE4` address, and inline it is computed after both. */
void func_800223B4(s32 *p, s32 x, s32 y, s32 n, s32 z)
{
    s32 sx = x * 4;
    UnkCardPair *e = (UnkCardPair *)p[569];
    u8 *q = (u8 *)&p[57];

    e->unk_04->unk_08 = sx - (p[6] / 2) + 0xA2;
    e->unk_04->unk_0A = (n * 17) - (p[7] / 2) + 0x7A;
    func_80022300(p, q, y);
    e->unk_00 = (s32)q;
    func_8002233C((u8 *)((UnkCardPair *)p[563 + n])->unk_00, z, y);
}

void func_80022488(s32 *p)
{
    UnkStrSet *t = D_800280B4;
    UnkStrSet *r = &t[p[2]];
    s32 n = r->unk_17 - 1;
    s32 x;
    s32 y;
    s32 z;

    if (r->unk_14 != 0)
    {
        func_8002225C(r->strs[n], p[0], &x, &y, &z);
        func_800223B4(p, x, y, n, z);
    }
}

/* The same input-handler shape as func_800265B8, and the same two levers:
   the `p[4] >= 3` arm comes first with the increment as the `else`, and the
   0x8000/0x2000 pair is written out in BOTH arms so jump.c cross-jumps the
   two `dir` assignments into one tail. func_800231E4(1) is likewise written
   in both arms of the p[8] test -- the target calls it either way but only
   stores the result when p[8] is non-zero, and the store needs p[8] reloaded
   after the call, so the test has to precede it. */
void func_80022524(s32 *p)
{
    s32 dir = 0;
    s32 n;

    if (D_80083160.unk_08 == 0)
    {
        return;
    }
    if (D_80083160.unk_10 & 0x20)
    {
        func_80053DA8(0x515);
        func_800231E4(1);
        if (p[8] != 0)
        {
            *(s32 *)p[8] = 1;
        }
    }
    else if (D_80083160.unk_10 & 0x40)
    {
        func_80053DA8(0x514);
        if (p[8] != 0)
        {
            *(s32 *)p[8] = func_800231E4(1);
        }
        else
        {
            func_800231E4(1);
        }
    }
    else if (D_80083160.unk_08 & 0xA000)
    {
        if (D_80083160.unk_10 & 0xA000)
        {
            p[4] = 0;
            if (D_80083160.unk_10 & 0x8000)
            {
                dir = -1;
            }
            else if (D_80083160.unk_10 & 0x2000)
            {
                dir = 1;
            }
        }
        else if (p[4] >= 3)
        {
            if (D_80083160.unk_08 & 0x8000)
            {
                dir = -1;
            }
            else if (D_80083160.unk_08 & 0x2000)
            {
                dir = 1;
            }
        }
        else
        {
            p[4]++;
        }
    }
    if (dir != 0)
    {
        n = func_80049E1C(p[0], dir, D_800280B4[p[2]].unk_14);
        if (n != p[0])
        {
            p[0] = n;
            func_80053DA8(0x502);
        }
        func_80022488(p);
    }
}

void func_800226A8(s32 *p)
{
    if (*(s32 *)p[8] == 0)
    {
        func_800231E4(0);
    }
}

void func_800226E0(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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
    p->unk_04->unk_0A = -72;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* func_80022A30's bigger sibling: the same unaligned D_800200A0 copy, the
   same four iterations, the same `p[-0x18] |= 0x80` and return, but THREE
   primitives per iteration (0xC, 0xC, 0x18 bytes), the last a D_80027F48
   template copied through a 24-byte alignment-1 struct and then patched with
   four 0x90D0E0 words. Width and height come from D_800280B4[q[2]].unk_16 * 4
   and .unk_17 * 17, stored to q[6]/q[7] with +8 and halved for the sprite
   bytes; `(w + 0xA) >> 1` is the one signed halving.

   Shape notes (each measured, none a device): `i = 0` before the loop rather
   than in the `for` (4); `bh = h + 0xA` AFTER the `(w + 0xA) >> 1` store, the
   natural order -- written before it the `h + 0xA` movable is hoisted ahead
   of the `w + 0xA`/`sra` pair and those three preheader words rotate (3);
   `h = (hh << 4) + hh` rather than `hh * 17` (11, see the pin below). */
u8 *func_80022774(u8 *p, s32 *q)
{
    u8 buf[4];
    u32 w;
    u32 h;
    u32 bh;
    /* DEVICE: a hard-register pin on the unk_17 byte, worth 11 words
       (11 -> 0). Without it `hh` is a three-reference block-0 quantity that
       local-alloc prices (`qty_compare_1`, local-alloc.c:1598,
       `floor_log2(refs)*refs/(death-birth)` over the SCHED1 order) below the
       seven-reference unk_16/w chain, so w takes $v1 and hh $a0: 11 differing
       words, all register names in the preheader. Three references cannot win
       at any live length this block admits; four can, but the only free fourth
       reference is a `__asm__("" ::"r"(hh))` marker, and that marker is then
       the load's last consumer in sched1's backward schedule: it is what
       launches the `lbu` (sched.c `adjust_priority`, LAUNCH_PRIORITY), and to
       launch it late enough to sit above the `sll`/`addiu` of w the marker
       must take sched1's one priority-1 hole at T-13 -- the hole the giv
       initialiser `p + 0xB` needs in order to end up above the `lo_sum` in
       sched2's LUID tie-break (`rank_for_schedule`, sched.c:2425). The two
       cannot share it: every marker form measured is 3 at best (two markers),
       and 0 only with a THIRD marker holding the `lo_sum` back
       (`"r"((u8 *)&D_800280B4[q[2]] - q[2] * 24)`, which cse folds to the
       lo_sum pseudo), declined as a third device. The pin removes the pricing
       question entirely, leaves the load's launch to the `sll` so the giv gets
       the hole, and sched2 then orders the two `lbu` by priority on its own.
       `(hh << 4) + hh` is required with it: `hh * 17` goes through
       expand_mult, which copies $3 into a pseudo that combine substitutes back
       into both uses, and the pin is lost (11, the same words).
       Legal spellings ruled out: no pin at all 11; `hh * 17` with the pin 11;
       `register u8 hh` 11; `__asm__("" ::"r"(hh))` alone 3 (giv permutation);
       `"r"(hh), "r"(w + 8)` 3 (the rotation); `"r"(hh), "r"(w)` 11;
       `__asm__("" : "+r"(p) : "r"(hh), "r"(hh), "r"(hh))` 8; two markers
       `"r"(hh)` + `"r"(w + 8)` 7; the volatile and `"memory"` forms 28..62.
       Warns under -Wall? No. */
    register u32 hh __asm__("$3");
    s32 g;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    i = 0;
    w = D_800280B4[q[2]].unk_16 * 4;
    hh = D_800280B4[q[2]].unk_17;
    h = (hh << 4) + hh;
    q[6] = w + 8;
    q[7] = h + 8;
    for (; i < 4; i++)
    {
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = (s32)(w + 0xA) >> 1;
        bh = h + 0xA;
        p[0xB] = bh >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        *(UnkBlock24 *)p = *(UnkBlock24 *)D_80027F48;
        g = 0x90D0E0;
        /* DEVICE: one marker doing three jobs, worth 85 words (85 -> 0).
           (a) `"+r"(g)` is a SECOND set of g in the loop whose source is not
           invariant, so `consec_sets_invariant_p` (loop.c) fails and
           `g = 0x90D0E0` never becomes a movable: the lui/ori stays in the
           loop where the target has it. The threshold cannot do this job --
           cse1 merges the four stores onto one pseudo of lifetime 4 and
           `threshold*1*4 >= insn_count` always holds. `"r"(i)` is what makes
           the asm variant; without it (`"+r"(g)` alone) loop.c moves BOTH
           insns out as a two-insn consec movable (84), and `volatile`
           instead is 68.
           (b) `"r"(bh)` makes the asm the last use of `h + 0xA`, so
           force_movables finds no movable at `regno_last_uid[bh]` and does
           not link the `srl` to the `addiu`; the addiu is hoisted on its own
           merit and the shift is left in the loop on the threshold, as the
           target has it. Written `p[0xB] = (h + 0xA) >> 1;` the shift rides
           out free on the addiu's link (36 on the park body).
           (c) `"r"(bh)` is also bh's THIRD reference, which keeps its
           global-alloc priority above the two hoisted constants so it takes
           $t4; a dead second assignment `g = bh ^ i;` does (a) and (b) for
           free but is deleted by flow before the references are counted, bh
           drops to $t8 and t5..t8 shift down (15).
           Re-measured on the landed body: dropping `"r"(bh)` with the shift
           inline 27, dropping the marker entirely 85.
           Legal spellings ruled out for (a): four separate `s32` locals, one
           per store (85 -- cse1 collapses them anyway); `g` assigned 0 before
           the loop (85 -- the gate also needs maybe_never, which a rotated
           for-loop never gives); the dead assignment above (15).
           Warns under -Wall? No -- g is initialised. */
        __asm__("" : "+r"(g) : "r"(i), "r"(bh));
        *(s32 *)(p + 4) = g;
        *(s32 *)(p + 0xC) = g;
        *(s32 *)(p + 0x10) = g;
        *(s32 *)(p + 0x14) = g;
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[0] = buf[i];
        p += 0x18;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_8002290C(s32 *p)
{
    s32 **q = (s32 **)p[0x238];

    ((u16 *)q[1])[4] = 0xA0;
    ((u16 *)q[1])[5] = 0x78;
    q[0] = p + 9;
}

/* Five sprites in a row, 0x180 bytes of buffer apart, each label rendered by
   func_8004DA74 into `(u8 *)p + 0x114 + i * 0x180`. D_800280B4 must be
   indexed directly: hoisting it into a `UnkStrSet *t` local costs 5 DIFF
   even though gcc hoists the base address either way, and giving the row its
   own local inside the loop costs 21. */
void func_80022934(s32 *p)
{
    s32 i;

    for (i = 0; i < 5; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[563 + i];

        e->unk_04->unk_08 = 0x124 - (p[6] / 2);
        e->unk_04->unk_0A = (i * 17) - (p[7] / 2) + 0xFC;
        e->unk_00 = (s32)func_8004DA74((u8 *)p + 0x114 + i * 0x180,
                                       D_800280B4[p[2]].strs[i], 1);
    }
}

/* The second parameter is unused by the body but func_80022AE8 sets $a1
   before the call and sets nothing else. */
u8 *func_80022A30(u8 *p, s32 *q)
{
    u8 buf[4];
    u8 w1;
    u8 h1;
    u8 c;
    s32 d;
    u8 w2;
    u8 h2;
    s32 i;

    /* Every one of these seven assignments is its own pre-loop statement, and
       that order is the whole function. The target's preheader is
       `i = 0`, 5, a COPY of the 5, 0x48, 0x104058, 6, a COPY of the 6, and
       then the giv `addiu $a0, $a3, 4`. Two things it is NOT:

       - the pairs are not hoisted out of the loop. Written inside the body
         they ARE hoisted in body order, which looks like the listing, but
         cse1 collapses the two pseudos holding 5 before loop.c ever sees
         them (confirmed in the .cse dump: one `(set (reg:QI N) (const_int 5))`
         serves both stores) and the function comes out 44 words / 36 DIFF.
         Widening the source so the copy is mode-invisible to cse does not
         help either -- `u32 w1; u8 h1 = w1;` is still 44/36 -- and even if a
         pair survived cse1, loop.c's combine_movables would match the two
         movables (equal mode, and `matched_regs[w1]` makes the copy's REG
         source count as the same value) and replace one register with the
         other. A hoisted pair can never keep its copy.
       - 0x48 and 0x104058 are not literals at their use sites. As literals
         they are hoisted by loop.c, which puts them AFTER the for-init
         instead of between the two pairs: 46 words, 8 DIFF.

       So all seven are statements, `i = 0` is lifted out of the `for` to put
       it first, and the pairs survive because the copy and its only use sit
       in different cse basic blocks -- the copy is before the loop, the use
       inside it, so canon_reg never rewrites the use. Measured alternatives,
       all on the same body: pairs assigned before the loop with 0x48/0x104058
       left as literals 46w/8; pairs inside the loop body 44w/36; pairs inside
       with w1 `u32`/`u16` and h1 `u8` 44w/36; h1 `u32` and w1 `u8` 44w/36;
       all four `u32` 44w/36; `h1 = 5` instead of `h1 = w1` 44w/36; a dead
       pre-loop `h1 = 0` to make the movable global 44w/36. */
    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    i = 0;
    w1 = 5;
    h1 = w1;
    c = 0x48;
    d = 0x104058;
    w2 = 6;
    h2 = w2;
    for (; i < 4; i++)
    {
        p[0xA] = w1;
        p[0xB] = h1;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = w2;
        p[0xB] = h2;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_80022AE8(s32 *p)
{
    func_80022A30((u8 *)(p + 9), p);
    func_8002290C(p);
}

s32 func_80022B20(s32 a)
{
    return func_800491F4(a, D_80027F60, 7);
}

/* The arrayed primitive initialiser, six copies of it at six base offsets.
   The lever, found by decomp-permuter after two long searches on the wrong
   idea: D_8002E5D8[0] must be read into a LOCAL before the `ptrs[i]` store,
   not fetched where it is used. That one reordering is the whole difference
   between 9 differing words and zero, in all six functions.

   What it buys: with the load first, gcc keeps the base in $t3 (copied from
   $a0 at word 0) and leaves $a0 free as the scratch for the prims address,
   which is the register assignment the targets have. Fetched in place, the
   base stays in $a0 and the prims address takes $v1 -- and no amount of
   restructuring around a second base pointer reaches it, which is what the
   earlier 9-DIFF park was trying. Both UnkCardPair fields must also go
   through `c->ents[i]` rather than a live `e` local, so they stay on the
   stride-16 induction variable at displacements 0x8EC/0x8F0. */
void func_80022B48(UnkCardGrp8B0 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

/* The pointer bump belongs in the TEST, not the body. `duplicate_loop_exit_test`
   (jump.c:2131) copies the inner loop's exit test above the loop in either
   spelling, so the three extra words of `while (*s != 0) { s++; ... }` are not
   a guard that failed to appear -- they are a guard that failed to be merged
   back. `jump2`'s conditional cross-jump (jump.c:1923-1950) collapses the guard
   into the plain `j` at 0x80022BFC only if the guard copy and the bottom test
   end in an identical tail of at least `minimum` = 2 insns (find_cross_jump,
   jump.c:2371, which scans BACKWARD from each conditional jump and stops at the
   first mismatch). With `s++` in the test both tails are `lbu` + `addiu`, so two
   insns match and the merge fires. With `s++` in the body, sched1 fills the load
   delay after the `lbu` with `last = i` at the bottom and with `len = 0` in the
   guard, so the very first backward pair already differs and nothing merges.
   Measured: `while (*s != 0) { s++; ... }` 27 words / 23 DIFF;
   `for (len = 0; *s != 0; s++)` 28 words / 23 DIFF; this spelling 24 / 0. */
void func_80022BE8(UnkStrSet *q)
{
    s32 maxlen = 0;
    s32 last = 0;
    s32 i;

    for (i = 0; i < 5; i++)
    {
        u8 *s = q->strs[i];
        s32 len = 0;

        while (*s++ != 0)
        {
            len++;
            if (maxlen < len)
            {
                maxlen = len;
            }
            last = i;
        }
    }
    q->unk_16 = maxlen;
    q->unk_17 = last + 1;
}

void func_80022C48(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    p[5] = a;
    p[2] = b;
    p[3] = c;
    p[8] = d;
    func_80022BE8(&D_800280B4[b]);
}

void func_80022C90(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    func_80022C48(p, a, b, c, d);
    func_800226E0((UnkCardPair *)(p + 0x22F), (u32 *)(p + 0x225), (u32 *)(p + 0x229));
    func_80022AE8(p);
}

void func_80022CD8(s32 *p)
{
    func_80022774((u8 *)(p + 9), p);
    func_80022934(p);
    p[0] = D_800280B4[p[2]].unk_15;
    func_80022488(p);
    if (p[3] == 2)
    {
        p[-4] = (s32)func_800226A8;
    }
    else
    {
        p[-4] = (s32)func_80022524;
    }
}

void func_80022B48(UnkCardGrp8B0 *c, s32 n);

/* The same constructor idiom as func_80023FF0 and func_800238D4, at the
   singleton D_800287E8. It RETURNS q even though every caller ignores the
   value: the target copies $s0 into $v0 and stores the func_80022CD8 pointer
   through $v0, which as a void function is three words instead of four.
   func_80022B20 likewise had to become `s32` with a `return`, since its
   value lands in r[0x232]; its own ten words are unchanged. */
s32 func_80022D60(s32 a, s32 id, s32 kind, s32 p)
{
    s32 *q = (s32 *)D_800287E8;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_800287E8, 0x27A);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_800231E4(r[3] != 2);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x9C8);
    }
    func_80022B48((UnkCardGrp8B0 *)r, 7);
    q[3] = (s32)(r + 0x22F);
    r[0x232] = func_80022B20((s32)(r + 0x233));
    func_80022C90(r, a, id, kind, p);
    q[4] = (s32)func_80022CD8;
    return (s32)q;
}

void func_80022E74(s32 a, s32 p)
{
    func_80022D60(a, 0, 1, p);
}

void func_80022E9C(s32 a)
{
    func_80022D60(a, 1, 0, 0);
}

void func_80022EC4(s32 a, s32 p)
{
    func_80022D60(a, 2, 2, p);
}

void func_80022EEC(s32 a)
{
    func_80022D60(a, 8, 0, 0);
}

void func_80022F14(s32 a, s32 p)
{
    func_80022D60(a, 4, 2, p);
}

void func_80022F3C(s32 a, s32 p)
{
    func_80022D60(a, 5, 1, p);
}

void func_80022F64(s32 a, s32 p)
{
    func_80022D60(a, 6, 2, p);
}

void func_80022F8C(s32 a, s32 p)
{
    func_80022D60(a, 14, 2, p);
}

void func_80022FB4(s32 a)
{
    func_80022D60(a, 9, 0, 0);
}

void func_80022FDC(s32 a)
{
    func_80022D60(a, 10, 0, 0);
}

void func_80023004(s32 a)
{
    func_80022D60(a, 11, 0, 0);
}

void func_8002302C(s32 a)
{
    func_80022D60(a, 12, 0, 0);
}

void func_80023054(s32 a)
{
    func_80022D60(a, 13, 0, 0);
}

void func_8002307C(s32 a)
{
    func_80022D60(a, 3, 0, 0);
}

void func_800230A4(s32 a)
{
    func_80022D60(a, 7, 0, 0);
}

void func_800230CC(s32 a, s32 p)
{
    func_80022D60(a, 15, 2, p);
}

void func_800230F4(s32 a)
{
    func_80022D60(a, 16, 0, 0);
}

void func_8002311C(s32 a)
{
    func_80022D60(a, 17, 0, 0);
}

void func_80023144(s32 a, s32 p)
{
    func_80022D60(a, 18, 1, p);
}

void func_8002316C(s32 a)
{
    func_80022D60(a, 19, 0, 0);
}

/* The local copy of the parameter is load-bearing, and so is declaring the
   counter first: the target saves $s1 (the counter) before $s0 (the cursor)
   and fills the load-delay slot with the cursor's own increment. Walking the
   parameter directly puts $s0 first and leaves a nop there -- 6 differing. */
void func_80023194(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_8004E994(*p[0x233]);
        p++;
    } while (++i < 5);
}

s32 func_800231E4(s32 flag)
{
    s32 *p = &D_80028808;

    if (flag != 0)
    {
        ((u16 *)p[5])[15] |= 0x2000;
    }
    func_80023194((s32 **)p);
    ((u16 *)p)[-1] |= 0x8000;
    D_800814A0_a[0] |= 0x8000;
    return D_80028808;
}

void func_80023258(s32 *p)
{
    s32 w = p[35];
    s32 d = ((p[34] - w) * (p[33] << 4)) / p[32] + 0xA;
    s32 i;

    ((UnkCardPair *)p[105])->unk_04->unk_0A = (w << 4) + d;
    for (i = 0; i < 3; i++)
    {
        s32 v = ((u8 *)p)[0x7C + i];
        s32 u;
        s32 n;
        u8 *q = (u8 *)p + i + 0x7C;

        if (i == p[34])
        {
            n = v + (8 - v) / (p[32] - p[33] + 1);
        }
        else
        {
            n = v;
            if (n != 0)
            {
                n -= 1;
            }
        }
        *q = n;
        u = ((u8 *)p)[0x7C + i];
        ((UnkCardPair *)p[114 + i])->unk_04->unk_08 = u + 0x24;
        ((UnkCardPair *)p[118 + i])->unk_04->unk_08 = u + 0x8F;
        ((UnkCardPair *)p[106 + i])->unk_04->unk_08 = u + 9;
        ((UnkCardPair *)p[110 + i])->unk_04->unk_08 = u + 0x42;
        ((u8 *)((UnkCardPair *)p[114 + i])->unk_04)[0] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkCardPair *)p[114 + i])->unk_04)[1] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkCardPair *)p[114 + i])->unk_04)[2] = ((u8 *)p)[0x7C + i] * 4 + 0x60;
    }
    if (p[33] < p[32])
    {
        p[33]++;
    }
}

void func_800233D0(s32 *p)
{
    s32 i;

    *(s32 *)p[0x68] = func_80049E6C(3);
    *(s32 *)p[0x69] = (s32)D_80077E84;
    for (i = 0; i < 3; i++)
    {
        *(s32 *)p[0x6A + i] = (s32)D_80077EF0;
        *(s32 *)p[0x6E + i] = (s32)D_80077EFC;
        *(s32 *)p[0x72 + i] = (s32)(p + 1);
        *(s32 *)p[0x76 + i] = (s32)((u8 *)p + 0x90 + i * 0x48);
    }
}

void func_80023470(s32 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        ((UnkCardPair *)p[0x72 + i])->unk_04->unk_08 = 0x24;
        ((u8 *)((UnkCardPair *)p[0x72 + i])->unk_04)[0] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x72 + i])->unk_04)[1] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x72 + i])->unk_04)[2] = 0x60;
        ((UnkCardPair *)p[0x6A + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[0x6E + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[0x76 + i])->unk_04->unk_08 = 0x8F;
    }
}

void func_8002352C(u8 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_8004DA74(p + 0x90 + i * 0x48, D_800282BC[i], 1);
    }
}

s32 func_800235A0(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

/* Four sprite slots per row, three rows, each x and y written through a
   freshly reloaded `p[k + i]` -- gcc cannot CSE the second store's base
   because the first store may alias it, so spelling each store as its own
   full expression is what produces the doubled `lw` pairs. A `do`/`while`
   here is 78 words against 76. */
void func_80023600(s32 *p)
{
    s32 i;

    p[32] = 1;
    p[33] = 1;
    func_8002352C((u8 *)p);
    func_800235A0((s32)(p + 1), 0x36, 0x10);
    func_800233D0(p);
    ((UnkCardPair *)p[104])->unk_04->unk_0A = 0x1A;
    for (i = 0; i < 3; i++)
    {
        ((UnkCardPair *)p[118 + i])->unk_04->unk_08 = 0x8E;
        ((UnkCardPair *)p[118 + i])->unk_04->unk_0A = (i * 16) + 0x82;
        ((UnkCardPair *)p[106 + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[106 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[110 + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[110 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[114 + i])->unk_04->unk_08 = 0x24;
        ((UnkCardPair *)p[114 + i])->unk_04->unk_0A = (i * 16) + 0xA;
    }
    func_80023470(p);
}

void func_80023730(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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
    p->unk_04->unk_08 = -152;
    p->unk_04->unk_0A = -98;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

s32 func_800237C4(s32 a)
{
    return func_800491F4(a, D_80027F68, 0x12);
}

void func_800237EC(UnkCardGrp184 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_8002388C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80023894(s32 *p, s32 v)
{
    func_8002388C(p, v);
    func_80023730((UnkCardPair *)(p + 0x64), (u32 *)(p + 0x5A), (u32 *)(p + 0x5E));
    func_80023600(p);
}

s32 *func_800238D4(s32 *p)
{
    s32 *q = (s32 *)D_800291D0;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_800291D0, 0x112);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80023A00(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x428);
    }
    func_800237EC((UnkCardGrp184 *)r, 0x12);
    q[3] = (s32)(r + 0x64);
    r[0x67] = func_800237C4((s32)(r + 0x68));
    func_80023894(r, (s32)p);
    q[4] = (s32)func_80023258;
    func_80023258(r);
    return q;
}

void func_800239B0(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_8004E994(*p[0x76]);
        p++;
    } while (++i < 3);
}

void func_80023A00(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800239B0((s32 **)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_80023A50(s32 *p, s32 v)
{
    s32 *q;
    s32 t;

    if (p != 0)
    {
        q = p + 8;
        t = q[34];
        q[33] = 0;
        q[32] = 3;
        q[34] = v;
        q[35] = t;
    }
}

void func_80023A78(UnkCardPair *p, u32 *a, u32 *b, s32 n)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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
    p->unk_04->unk_08 = -0xA0;
    p->unk_04->unk_0A = n * 24 - 0x78;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

void func_80023B18(s32 *p)
{
    UnkCardPair *q = (UnkCardPair *)p[0x98];
    s32 k;

    k = func_80022138() >= 3;
    q->unk_00 = (s32)func_8004DA74((u8 *)p + 8, D_800282A4[k], 1);
    q->unk_04->unk_08 = 0xF0;
    q->unk_04->unk_0A = 0x91;
    q = (UnkCardPair *)p[0x9C];
    q->unk_04->unk_08 = 0x70;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_800791A4;
    q = (UnkCardPair *)p[0x9E];
    q->unk_00 = (s32)((u8 *)p + 0x134);
    q->unk_04->unk_08 = 0xAE;
    q->unk_04->unk_0A = 0x18;
}

s32 func_80023BE4(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

/* Four UnkCardPair slots set up in a row, each `e->unk_00` plus the x and y
   in its UnkCardPrim. Two things are load-bearing:

   `e = (UnkCardPair *)p[155]` must be assigned BEFORE the func_8004E5A0 /
   func_8004E69C pair, not after: the target loads $s3 at word 39, ahead of
   both calls, and with the load after them the function is 71 words instead
   of 72 (gcc fills the p[0] load-delay slot with the `li $a1, 4` that the
   target has already issued back at word 27).

   The second table access has to be spelled as a BYTE offset. The target
   advances the D_80083EA4 base register in place at word 37
   (`addu $s0, $s0, $v0`), which gives that pseudo a fourth reference and so
   wins it $s0 over `p`; `D_80083EA4[p[0]][8]` computes the sum into a
   scratch instead and leaves $s0/$s1 swapped through the whole function (18
   DIFF). A named `s32 (*tbl)[32]` local with `tbl += p[0]` gets the in-place
   update but hoists the `lui`/`addiu` into the prologue, which is worse
   (23). */
void func_80023C44(s32 *p)
{
    u8 buf[8];
    UnkCardPair *e;

    e = (UnkCardPair *)p[156];
    e->unk_04->unk_08 = 0x70;
    e->unk_04->unk_0A = 0x10;
    e->unk_00 = (s32)D_80078FD0;

    e = (UnkCardPair *)p[152];
    e->unk_00 = (s32)func_8004DA74((u8 *)(p + 2), (u8 *)D_80083EA4[p[0]], 1);
    e->unk_04->unk_08 = 0xF0;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[155];
    func_8004E5A0(*(s32 *)((u8 *)D_80083EA4[p[0]] + 0x20), 4, buf);
    func_8004E69C(buf);
    e->unk_00 = (s32)func_8004E298((u8 *)(p + 0x41), buf, 1);
    e->unk_04->unk_08 = 0x147;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[158];
    e->unk_00 = (s32)(p + 77);
    e->unk_04->unk_08 = 0xAE;
    e->unk_04->unk_0A = 0x18;
}

void func_80023D64(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x9D];
    q->unk_04->unk_08 = 0xFA;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_80027F7C;
    q = (UnkCardPair *)p[0x9A];
    q->unk_00 = (s32)func_8004DA74((u8 *)p + 0xA4, D_800282B0, 1);
    q->unk_04->unk_08 = 0x178;
    q->unk_04->unk_0A = 0x90;
    q = (UnkCardPair *)p[0x9F];
    q->unk_00 = (s32)((u8 *)p + 0x1AC);
    q->unk_04->unk_08 = 0x10C;
    q->unk_04->unk_0A = 0x18;
}

void func_80023E14(s32 *p)
{
    s32(*tbl)[32];

    func_80023BE4((s32)(p + 0x4D), 0x80, 0x18);
    func_80023BE4((s32)(p + 0x6B), 0x2A, 0x14);
    tbl = D_80083E98;
    if (tbl[p[0]][0] != 0)
    {
        func_80023C44(p);
        if (((s16 *)tbl[p[0]])[4] != 0)
        {
            func_80023D64(p);
        }
    }
    else
    {
        func_80023B18(p);
    }
}

void func_80023EC4(s32 *p)
{
    p[-4] = 0;
}

s32 func_80023ECC(s32 a)
{
    return func_800491F4(a, D_80027FC4, 0xA);
}

void func_80023EF4(UnkCardGrp240 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_80023F94(s32 *p, s32 a, s32 b)
{
    p[0] = b;
    p[1] = a;
}

void func_80023FA0(s32 *p, s32 a, s32 b)
{
    func_80023F94(p, a, b);
    func_80023A78((UnkCardPair *)(p + 0x93), (u32 *)(p + 0x89), (u32 *)(p + 0x8D), b);
    func_80023E14(p);
}

void func_80023EF4(UnkCardGrp240 *c, s32 n);

/* The func_800238D4 constructor idiom at an INDEXED row: D_80029618 is an
   array of 0x3E4-byte rows, and gcc synthesises the 996 multiply as
   `(((b << 5) - b) << 3) + b) << 2`. func_80023ECC had to become `s32` with
   a `return` -- its value lands in r[150] here -- which leaves its own ten
   words unchanged. `q[3] = ...` must come BEFORE the func_80023ECC call:
   reversed, the function grows to 64 words. */
s32 func_80023FF0(s32 a, s32 b)
{
    s32 *q = (s32 *)D_80029618[b];
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, (u8 *)q, 0xF9);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80024184(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x3C4);
    }
    func_80023EF4((UnkCardGrp240 *)r, 0xA);
    q[3] = (s32)(r + 147);
    r[150] = func_80023ECC((s32)(r + 151));
    func_80023FA0(r, a, b);
    q[4] = (s32)func_80023EC4;
    return (s32)q;
}

void func_800240EC(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        func_8004E994(*(s32 *)p[off + 0x98]);
        *(s32 *)p[off + 0x98] = 0;
        off++;
    } while (++i < 3);
    *(s32 *)p[0x9B] = 0;
}

void func_80024160(s32 **p)
{
    s32 i = 0;

    do
    {
        *p[0x97] = 0;
        p++;
    } while (++i < 10);
}

void func_80024184(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800240EC(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_800241D4(s32 *p, s32 flag)
{
    s32 *q;
    s32 *r;

    if (p != 0)
    {
        q = p + 8;
        r = (s32 *)q[0xA0];
        q[0x89] = 0x808080;
        if (flag != 0)
        {
            r[0] = (s32)D_800786E8;
        }
        else
        {
            r[0] = 0;
        }
        ((u16 *)r[1])[4] = 0x60;
        ((u16 *)r[1])[5] = 0x18;
    }
}

void func_80024224(s32 *p, s32 flag)
{
    s32 *q;
    s32 *r;

    if (p != 0)
    {
        q = p + 8;
        r = (s32 *)q[0xA0];
        q[0x89] = 0x808080;
        if (flag != 0)
        {
            r[0] = (s32)D_800786DC;
        }
        else
        {
            r[0] = 0;
        }
        ((u16 *)r[1])[4] = 0x66;
        ((u16 *)r[1])[5] = 0x18;
    }
}

void func_80024274(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[0x89] = 0x606868;
        *(s32 *)q[0xA0] = 0;
    }
}

void func_80024298(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800240EC(q);
        func_80024160((s32 **)q);
        func_80023E14(q);
    }
}

/* A 0xC-byte header, four 0x18-byte sprite records and a 0xC-byte trailer.
   Two spellings in here are load-bearing and both are measured:

   - the halvings `w / 2` and `h / 2` are written INLINE at the two stores,
     not in locals. They are loop-invariant either way, so loop.c hoists
     them, but written inline they are hoisted AFTER the three constant
     movables (0x38, 0x9CBCBC, 0x8C9C9C), which is where the target has
     them. In locals assigned inside the loop is 22 DIFF; in locals before
     the loop, 24.

   - `i` is incremented INSIDE the body, as `D_80027FD0[i++]`, and the `for`
     has an empty third clause. This is not about the top of the body: it is
     about being before `p += 0x18`. loop.c emits the reduced giv's own
     update (`$v1 += 0x18`, for the `p + 0xB` the seven record stores are
     addressed off) immediately before the biv update `p += 0x18`, so the
     giv update's LUID is fixed relative to that statement. sched1 here is a
     backward list scheduler, and the slot after the `sb $v0, 0($a0)` is a
     load-delay filler with exactly two candidates of priority 1, the `i`
     increment and the giv update; `rank_for_schedule` breaks that tie on
     INSN_LUID and prefers the HIGHER one (sched.c:2425 -- it returns
     `INSN_LUID (y) - INSN_LUID (x)`, and ready[0] is taken). With `i++` in
     the `for` clause it lands after `p += 0x18` and outranks the giv update,
     which rotates three insns and costs 10 DIFF. Moved before `p += 0x18`,
     the giv update takes the slot and the loop is exact.

   Measured, all 46 words: `D_80027FD0[i++]` 0; `p[0] = D_80027FD0[i]; i++;`
   before `p += 0x18` 0; the same with `i = 0` lifted out of the `for` 0; an
   address local `q = &D_80027FD0[i]; i++; p[0] = *q;` 0; `i++` AFTER
   `p += 0x18` 10 (the parked shape); `i++` first with `D_80027FD0[i - 1]` 2;
   `for (i = 0; i < 4; i++)` with an address local at the top of the body 23,
   and with the local declared in the body, spelled `D_80027FD0 + i`, or with
   `i = 0` lifted out, 23 each; `while (i < 4)` / `do ... while (i < 4)` with
   `i++` at the top of the body 14, as is `&D_80027FD0[i++]` in a `while`.
   Reading the byte (not the address) into a local at the top of the body is
   17, as `u8` or as `s32`. The permuter was not used. */
u8 *func_800242D8(u8 *p, s32 w, s32 h)
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
        p[0] = D_80027FD0[i++];
        p += 0x18;
    }
    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 1;
    p[0] = 0x80;
    return p + 0xC;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200A0);

/* splat migrated this one-character string into func_80024390's listing, so
   landing that function loses the symbol. Defined here, immediately after
   D_800200A0, which is where it belongs in the rodata stream -- the same
   trap and the same fix as D_80020084. */
const u8 D_800200A4[] = " ";

/* The save-slot detail panel, and the overlay's largest function: eight text
   sprites built into one 0x100-byte stack buffer and handed to the font
   renderer, then eight icon slots driven by a bitmask.

   Four things are load-bearing:

   - D_80083E98 has to be the UnkSaveRow STRUCT alias. The first row access
     in the function is the name at +0xC, so gcc relocates against
     D_80083E98+0xC -- splat calls that D_80083EA4 -- holds it in $s1 across
     every call and derives the row base as `$s1 - 0xC` in $fp for the other
     seven fields. Through the two separate symbols each access materialises
     its own %hi and the function is five words longer.
   - Each block's `e = (UnkCardPair *)p[k]` has to come BEFORE that block's
     calls, not after: $s3 is callee-saved and the target loads it first.
   - The two clamps are `x >= 1000 ? 999 : x`, not `x < 1000 ? x : 999` --
     the target branches on the false case with the 999 in the delay slot.
   - In the icon loop the mask needs its own local (`m = 1 << i`) or combine
     rewrites `flags & (1 << i)` into `(flags >> i) & 1`, and the flags word
     needs one too, which is what settles the order of the two hoists in the
     loop preheader. The two arms assign a pointer and share one store.

   The playtime is `row.unk_1C / 60` unsigned for seconds, then `/ 3600` and
   `/ 60` signed for hours and minutes -- three different magic multiplies. */
void func_80024390(s32 *p)
{
    u8 buf[0x100];
    u8 num[0x10];
    UnkCardPair *e;
    s32 sec;
    s32 hr;
    s32 mn;
    s32 i;
    s32 m;
    u8 *s;

    e = (UnkCardPair *)p[723];
    func_8003AD08(p[31] + 1, buf);
    strcat(buf, D_800282D8);
    strcat(buf, D_80083E98_s[p[31]].name);
    func_8004DA74((u8 *)p + 0x84, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x84);
    e->unk_04->unk_08 = 0xA8;
    e->unk_04->unk_0A = 0x110;
    e = (UnkCardPair *)p[724];
    strcpy(buf, D_800282E8);
    func_8004DA74((u8 *)p + 0x204, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x204);
    e->unk_04->unk_08 = 0xA0;
    e->unk_04->unk_0A = 0x110;
    if (D_80083E98_s[p[31]].unk_08 != 0)
    {
        e = (UnkCardPair *)p[725];
        strcpy(buf, D_800283A8[2]);
        func_8003AD08(D_80083E98_s[p[31]].unk_34, num);
        strcat(buf, num);
        strcat(buf, D_800283A8[3]);
        func_8004DA74((u8 *)p + 0x384, buf, 1);
        e->unk_00 = (s32)((u8 *)p + 0x384);
        e->unk_04->unk_08 = 0xB8;
        e->unk_04->unk_0A = 0x140;
    }
    e = (UnkCardPair *)p[726];
    func_8004E5A0(D_80083E98_s[p[31]].unk_24, 0xA, buf);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x504, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x504);
    e->unk_04->unk_08 = 0x148;
    e->unk_04->unk_0A = 0x110;
    sec = D_80083E98_s[p[31]].unk_1C / 60;
    hr = sec / 3600;
    e = (UnkCardPair *)p[727];
    func_8004E5A0(hr >= 1000 ? 999 : hr, 3, buf);
    strcat(buf, (u8 *)D_800200A4);
    mn = sec / 60;
    func_8004E5E8(mn - hr * 60, 2, num);
    strcat(buf, num);
    strcat(buf, (u8 *)D_800200A4);
    func_8004E5E8(sec - mn * 60, 2, num);
    strcat(buf, num);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x684, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x684);
    e->unk_04->unk_08 = 0x150;
    e->unk_04->unk_0A = 0x120;
    e = (UnkCardPair *)p[728];
    func_8004E5A0(D_80083E98_s[p[31]].unk_2C >= 1000 ? 999 : D_80083E98_s[p[31]].unk_2C,
                  3, buf);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x804, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x804);
    e->unk_04->unk_08 = 0x100;
    e->unk_04->unk_0A = 0x120;
    e = (UnkCardPair *)p[729];
    func_8004E5A0(D_80083E98_s[p[31]].unk_28, 2, buf);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x984, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x984);
    e->unk_04->unk_08 = 0x188;
    e->unk_04->unk_0A = 0x130;
    for (i = 0; i < 8; i++)
    {
        s32 f = D_80083E98_s[p[31]].unk_30;

        e = (UnkCardPair *)p[730 + i];
        m = 1 << i;
        if (f & m)
        {
            s = D_80027FD4[i];
        }
        else
        {
            s = D_80027FD4[8];
        }
        e->unk_00 = (s32)s;
        e->unk_04->unk_08 = 0x34 + 0x10 * i;
        e->unk_04->unk_0A = 0xB0;
    }
}

/* Three label layouts for the save-slot list, then a loop writing the text
   sprites. `k` is `p[32] == 1` -- gcc's `xori` plus `sltu 1` -- and it is used
   BOTH as the first condition and as an index into D_800283B8 and
   D_800283F4, even though it is provably 0 on the two arms that index with
   it. n/3 is the signed magic multiply by 0x55555556. */
void func_80024818(s32 *p)
{
    u8 buf[0x40];
    u8 num[0x10];
    s32 k = p[32] == 1;
    s32 n;
    s32 cnt;
    s32 i;

    n = func_80022138();
    if (k != 0 && func_80022160() == 0)
    {
        func_8004DA74((u8 *)p + 0x84, D_800283EC[0], 1);
        func_8004DA74((u8 *)p + 0x204, D_800283EC[1], 1);
        cnt = 2;
    }
    else if (n < 3)
    {
        func_8004DA74((u8 *)p + 0x84, D_800283C4, 1);
        func_8004DA74((u8 *)p + 0x204, D_800283B8[k], 1);
        func_8004DA74((u8 *)p + 0x384, D_800283B8[2], 1);
        cnt = 3;
    }
    else
    {
        func_8004DA74((u8 *)p + 0x84, D_800283F4[k], 1);
        func_8003AD08(n / 3, num);
        strcpy(buf, num);
        strcat(buf, D_800283FC);
        func_8004DA74((u8 *)p + 0x204, buf, 1);
        cnt = 2;
    }
    for (i = 0; i < cnt; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[723 + i];

        e->unk_00 = (s32)((u8 *)p + 0x84 + 0x180 * i);
        e->unk_04->unk_08 = 0xA9;
        e->unk_04->unk_0A = 0x110 + 0x10 * i;
    }
}

void func_800249B4(s32 *p)
{
    s32 i;

    func_8004DA74((u8 *)p + 0x84, D_800283EC[0], 1);
    func_8004DA74((u8 *)p + 0x204, D_800283EC[1], 1);
    for (i = 0; i < 2; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[0x2D3 + i];

        e->unk_00 = (s32)((u8 *)p + 0x84 + i * 0x180);
        e->unk_04->unk_08 = 0xA9;
        e->unk_04->unk_0A = 0x110 + i * 0x10;
    }
}

void func_80024A50(s32 *p)
{
    s32 n = p[0x1F];

    if (D_80083E98[n][0] != 0)
    {
        func_80024390(p);
    }
    else
    {
        s32 m = p[0x20];

        if (m != 0 && m != 2)
        {
            func_80024818(p);
        }
        else
        {
            func_800249B4(p);
        }
    }
}

void func_80024ACC(s32 *p)
{
    UnkCardPair *q;

    func_800242D8((u8 *)(p + 1), 0xF8, 0x48);
    q = (UnkCardPair *)p[0x2D2];
    q->unk_04->unk_08 = 0xA0;
    q->unk_04->unk_0A = 0xB0;
    q->unk_00 = (s32)(p + 1);
    q = (UnkCardPair *)p[0x2CF];
    q->unk_04->unk_08 = 0x60;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_80079054;
    q = (UnkCardPair *)p[0x2D0];
    q->unk_04->unk_08 = 0xD0;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_800790B4;
    q = (UnkCardPair *)p[0x2D1];
    q->unk_04->unk_08 = 0x20;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_80079114;
}

void func_80024BB4(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

void func_80024C48(s32 a)
{
    func_800491F4(a, D_80027FF8, 0x15);
}

void func_80024C70(UnkCardGrpB20 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_80024D10(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80024D18(s32 *p, s32 v)
{
    func_80024D10(p, v);
    func_80024BB4((UnkCardPair *)(p + 0x2CB), (u32 *)(p + 0x2C1), (u32 *)(p + 0x2C5));
    func_80024ACC(p);
}

s32 *func_80024D58(s32 *p, s32 a, s32 b)
{
    s32 *q = (s32 *)D_8002A990;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002A990, 0x394);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80024EEC(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0xE30);
    }
    r[0x1F] = a;
    r[0x20] = b;
    func_80024C70((UnkCardGrpB20 *)r, 0x15);
    q[3] = (s32)(r + 0x2CB);
    r[0x2CE] = (s32)func_80024C48w(r + 0x2CF);
    func_80024D18(r, (s32)p);
    q[4] = 0;
    return q;
}

void func_80024E44(s32 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_8004E994(*(s32 *)p[0x2D3 + i]);
        *(s32 *)p[0x2D3 + i] = 0;
    }
    for (i = 0; i < 4; i++)
    {
        *(s32 *)p[0x2D6 + i] = 0;
    }
    for (i = 0; i < 10; i++)
    {
        *(s32 *)p[0x2DA + i] = 0;
    }
}

void func_80024EEC(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80024E44(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_80024F3C(s32 *p, s32 a, s32 b)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[0x1F] = a;
        q[0x20] = b;
        func_80024E44(q);
        func_80024A50(q);
    }
}

void func_80024F7C(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_80024FAC(s32 *p)
{
    func_80024F7C(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_8002593C;
    }
}

void func_80024FFC(s32 *p)
{
    func_80025030(p);
    p[-4] = (s32)func_8002593C;
}

void func_80025030(s32 *p)
{
    s32 flag;
    s32 off;
    s32 i;

    if (p[0x12] == 1)
    {
        flag = 1;
    }
    else
    {
        func_80023A50((s32 *)p[6], p[0x11]);
        flag = 0;
    }
    off = 0;
    i = 0;
    do
    {
        if (i == p[10])
        {
            func_800241D4((s32 *)p[off + 1], flag);
        }
        else
        {
            func_80024274((s32 *)p[off + 1]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[7], p[10], p[0x11]);
}

void func_800250E8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_80024224((s32 *)p[off + 1], 1);
        }
        else if (i == p[10])
        {
            func_800241D4((s32 *)p[off + 1], 1);
        }
        else
        {
            func_80024274((s32 *)p[off + 1]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[7], p[10], p[0x11]);
}

/* Empty, but it takes an argument: func_80025230 sets $a0 to the bare
   0x80010000 base before calling it and sets nothing else. A callee that
   never reads its parameter compiles identically either way, so this is the
   caller's requirement, not the callee's. */
void func_8002519C(u8 *p)
{
}

void func_800251A4(s32 *p)
{
    u16 *b = (u16 *)0x80010000;
    s32 v;

    func_80027BF4();
    v = 1;
    if (p[0x11] == 0)
    {
        v = 3;
    }
    b[0x1B8A] = v;
    D_80082E60.unk_16 |= 2;
    func_80025D34(p - 8);
    func_800439F8();
    func_80040AA0(b[0x104] != 0 ? 5 : 6);
}

void func_80025230(s32 *p)
{
    if (func_80021FF0(p[10], (u8 *)0x80010000, 1) == 0)
    {
        p[13] = (s32)func_8002593C;
        func_80023004((s32)(p - 8));
        p[-4] = (s32)func_80024F7C;
    }
    else
    {
        func_8002519C((u8 *)0x80010000);
        p[-4] = (s32)func_800251A4;
    }
    func_80020984();
    p[16] = 0;
}

void func_800252A4(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80025230;
    }
}

void func_800252CC(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[15] = 1;
        func_80022F8C((s32)(p - 8), (s32)(p + 16));
        p[16] = 1;
        p[-4] = (s32)func_800252A4;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        func_800230A4((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_80025358(s32 *p, s32 v)
{
    func_80025F0C((u8 *)p, v);
    func_80025FFC(p, v);
}

/* func_80025358's first argument is the 0x80010000 base, not 'p - 8': the
   'lui $a0, 0x8001' in that jal's delay slot is the live argument. The
   'addiu $a0, $s2, -0x20' that reorg puts in BOTH beqz delay slots is the
   fail block's own func_800230F4 argument, hoisted out of the block because
   every predecessor carries it. Spelling 'p - 8' here instead cost exactly
   one word (53w/1 DIFF) across twelve measured variants of the constant and
   of the 'p - 8' sites; nothing about the local or the goto structure was
   ever wrong. */
void func_80025398(s32 *p)
{
    u8 *b = (u8 *)0x80010000;
    s32 i;
    s32 off;

    if (func_80021FF0(p[11], b, 0) == 0)
    {
        goto fail;
    }
    func_80025358((s32 *)b, p[10]);
    if (func_80021F18(p[10], b) == 0)
    {
        goto fail;
    }
    func_8002519C(b);
    func_800220DC();
    off = 0;
    for (i = 0; i < 5; i++)
    {
        func_80024298((s32 *)p[off + 1]);
        off++;
    }
    func_80024FFC(p);
    goto done;
fail:
    p[13] = (s32)func_80024FFC;
    func_800230F4((s32)(p - 8));
    p[-4] = (s32)func_80024F7C;
done:
    func_80020984();
    p[16] = 0;
}

void func_8002546C(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80025398;
    }
}

void func_80025494(s32 *p)
{
    p[15] = 1;
    func_800230CC((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_8002546C;
}

void func_800254E4(s32 *p)
{
    s32(*tbl)[32] = D_80083E98;

    if (tbl[p[10]][0] == 0)
    {
        if (func_80022138() < 3)
        {
            p[13] = (s32)func_80024FFC;
            func_80022FDC((s32)(p - 8));
            p[-4] = (s32)func_80024FAC;
        }
        else
        {
            func_80025494(p);
        }
    }
    else
    {
        p[13] = (s32)func_80024FFC;
        func_8002311C((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

/* The third of the input handlers, and the closest to func_800265B8 -- same
   0x40 / 0x5000 chain, same `p[12] >= 0xD` arm first, same pair of separate
   exit tests -- with three differences: the 0x20 case FALLS THROUGH into the
   0x40 test instead of leaving through a goto, the held-key step is -2 rather
   than -4, and func_80027AFC takes a literal 0 as its second argument. The
   unk_08 test has to be an enclosing `if`, not an early return: on the
   zero path the target still runs the func_8002168C/func_80021904 tail. */
void func_80025584(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            p[-4] = (s32)func_80024FFC;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_800254E4(p);
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 2;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        if (dir != 0)
        {
            func_80053DA8(0x502);
            p[10] = func_80049DE8(p[10], dir, 5);
            func_800250E8(p);
        }
    }
    r = func_8002168C();
    func_80021904();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
    func_80025D34(p - 8);
    func_80027AFC(p[8], 0);
}

void func_8002570C(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[11] = n;
        func_800250E8(p);
        p[-4] = (s32)func_80025584;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        func_800230A4((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_80025788(s32 *p)
{
    s32 i;
    s32 off;

    if (p[14] != 0)
    {
        p[-4] = (s32)func_8002593C;
    }
    else
    {
        if (func_8002219C(p[10]) != 0)
        {
            p[-4] = (s32)func_8002593C;
            off = 0;
            for (i = 0; i < 5; i++)
            {
                func_80024298((s32 *)p[off + 1]);
                off++;
            }
        }
        else
        {
            p[13] = (s32)func_8002593C;
            func_8002316C((s32)(p - 8));
            p[-4] = (s32)func_80024F7C;
        }
        func_80020984();
        p[16] = 0;
    }
}

void func_80025848(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[13] = (s32)func_80025788;
        func_80023144((s32)(p - 8), (s32)(p + 14));
        p[-4] = (s32)func_80024FAC;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        func_800230A4((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_800258C8(s32 *p)
{
    void (*fns[4])(s32 *) = {func_800252CC, func_8002570C, func_80025848, func_800252CC};

    fns[p[0x11]](p);
}

void func_8002592C(s32 *p)
{
    p[18] = 1;
    p[19] = 1;
}

/* The fifth input handler, and the widest: two flags rather than one. `dir`
   drives the func_80049DE8 step (through p[10] with a limit of 5 or p[17]
   with 3, depending on p[18]) and `upd` records that the layout changed, so
   func_80025030 can run once at the very end. Its 0x20 case RETURNS, which
   is why it skips that final call. Same two levers as its siblings -- the
   `p[12] >= 0xD` arm first, and the two exit tests nested rather than
   `r == 0 || r == 1`. */
void func_8002593C(s32 *p)
{
    s32 dir = 0;
    s32 upd = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            func_80025D34(p - 8);
            func_80027AFC(p[8], 0);
            return;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            if (p[18] == 0)
            {
                func_8002592C(p);
                upd = 1;
            }
            else
            {
                func_800258C8(p);
            }
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        else if (D_80083160.unk_08 & 0xA000)
        {
            if (D_80083160.unk_08 & 0x2000)
            {
                p[18] = 1;
            }
            else
            {
                p[18] = 0;
            }
            upd = 1;
        }
        if (dir != 0)
        {
            func_80053DA8(0x502);
            if (p[18] == 1)
            {
                p[10] = func_80049DE8(p[10], dir, 5);
            }
            else
            {
                p[17] = func_80049DE8(p[17], dir, 3);
            }
            func_80025030(p);
        }
    }
    r = func_8002168C();
    func_80021904();
    if (r != 0)
    {
        if (r != 1)
        {
            func_80025D34(p - 8);
            func_80027AFC(p[8], 0);
        }
    }
    if (upd != 0)
    {
        func_80025030(p);
    }
}

void func_80025B38(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80025B40(s32 *p, s32 v)
{
    func_80025B38(p, v);
}

void func_80025B60(s32 *p)
{
    func_80025030(p);
    func_80020984();
    p[-4] = (s32)func_8002593C;
}

void func_80025B9C(s32 *p)
{
    func_80025D34(p - 8);
    func_80027AFC(p[8], 0);
}

void func_80025BD4(s32 *p)
{
    s32 n = p[9];

    if (n < 5)
    {
        if (func_80021D54(n) != 0)
        {
            p[n + 1] = func_80023FF0((s32)(p - 8), n);
            p[9] = p[9] + 1;
        }
        else
        {
            p[13] = (s32)func_80025B9C;
            func_80023004((s32)(p - 8));
            p[-4] = (s32)func_80024F7C;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80025B60;
    }
}

s32 *func_80025C7C(s32 *p, s32 a)
{
    s32 *q = func_8003FE78(0, D_8002B7E0, 0x1C);
    s32 *r = q + 8;

    func_80025B40(r, (s32)p);
    func_8002223C();
    func_800220DC();
    r[0x12] = 1;
    q[8] = func_8004F418(q, q + 0x1A);
    r[6] = (s32)func_800238D4(q);
    r[7] = (s32)func_80024D58(q, r[10], 0);
    r[9] = 0;
    q[4] = (s32)func_80025BD4;
    return q;
}

/* The destructor family's other member, and identical in shape to
   func_80026920: only the LAST of the four calls is inside the loop. The park
   had all three of func_80023A00, func_80024EEC and func_80024184 in the
   loop body, which is 36 words against 37 and 31 differing; reading the
   branch target (word 22's `bnez` goes to word 17, not word 11) says
   otherwise. The two outside calls take q[6] and q[7], the one inside takes
   q[1] with the cursor advancing. */
void func_80025D34(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_8004F52C(q[0]);
        func_80023A00((s32 *)q[6]);
        func_80024EEC((s32 *)q[7]);
        i = 0;
        do
        {
            func_80024184((s32 *)q[1]);
            q++;
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_80025DC8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_800241D4((s32 *)p[off + 3], 1);
        }
        else
        {
            func_80024274((s32 *)p[off + 3]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[1], p[11], 3);
}

s32 func_80025E54(void)
{
    s32 mask = 0;
    s32 i = 0;

    do
    {
        if (func_80033B2C(D_80028010[i]) != 0)
        {
            mask += 1 << i;
        }
    } while (++i < 8);
    return mask;
}

/* The byte table is NOT the symbol splat prints. The listing reads
   `lui $v1, %hi(D_800133A4)` / `lbu $v0, %lo(D_800133A4)($v1)` with `$v1`
   incremented by 2 each iteration -- a %lo in the displacement with a
   REGISTER index, which per docs/matching-levers.md:1870 cannot be a symbol
   reference at all: cse folds a %lo into a MEM only when what is left is a
   constant offset off the `(high)` pseudo. It is the constant base
   0x80010000, whose low half is zero so it needs a bare `lui`, with 0x33A4
   written inside the index expression. splat prints the displacement
   symbolically because 0x800133A4 happens to be in its symbol list.

   Four symbol spellings cost the extra `addiu` the target does not have, all
   17 words against 16: `extern u8 D_800133A4[]` indexed, the same as a
   scalar through `(&D_800133A4)[k]`, a pointer walked by 2, and
   `*((u8 *)0x800133A4 + i * 2)` -- the last one being a constant too, but
   with a NONZERO low half, so it still has to be materialised. The zero low
   half of 0x80010000 is the whole point. */
s32 func_80025ECC(void)
{
    u8 *b = (u8 *)0x80010000;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < 0x22; i++)
    {
        sum += D_80028020[b[i * 2 + 0x33A4]];
    }
    return sum;
}

/* Two levers here, and the long-standing park had both backwards.

   0x80010208 and 0x8001022C are CONSTANT addresses, not symbols. Through a
   symbol the address is split into a separate `high` pseudo, so the word load
   comes out as `lui $v0, %hi` + `lw $a0, %lo($v0)` -- two registers. A constant
   address materialises 0x80010000 into the DESTINATION and puts 0x22C in the
   load's displacement, which is the listing's `lui $a0` + `lw $a0, 0x22C($a0)`.
   ovlverify's mask cannot see the difference (the immediate is the masked
   field), so this one only shows up in the link.

   `p + 4` is written inline at every call site, not kept in a local. cse1
   gives it one pseudo per extended basic block, so the uses before the `if`
   are one block-local quantity and the two after the join are another; both
   are local-alloc's, and because the first outranks `n` it takes $s0 and
   pushes `n` to $s1. A single local for p+4 spans the branch, becomes
   global-alloc's, and the two end up the other way round (61w/31).

   The ternary -- rather than an `if`/`else` over a local -- is what buys the
   last word. expand_call computes arg0 before expanding the COND_EXPR, so
   `p + 4` lands in a fresh pseudo BEFORE the branch; cse1 rewrites it to a
   copy of the first quantity, and that copy survives into sched1 because
   combine cannot propagate across the block boundary. The copy's only use is
   `$a0 = <copy>` at the shared call, so local-alloc hands the copy $a0 itself:
   the copy becomes `addu $a0, $s0, $zero` in the block ABOVE the branch, where
   the beqz's backward scan takes it for the delay slot, and the argument move
   in the join block collapses to a self-move and disappears. The join then
   begins with the call, so its backward scan stops at the label and its slot
   is filled forwards with the next `addiu $s0, $s3, 0x4`. Writing the test as
   a statement leaves both insns in the join and costs the word (60w/4). */
void func_80025F0C(u8 *p, s32 n)
{
    u8 buf[0x10];

    strcpy(p + 4, D_80028294[0]);
    func_8003AD08(n + 1, buf);
    strcat(p + 4, buf);
    strcat(p + 4, D_80028294[3]);
    strcat(p + 4, (u8 *)0x8001020C);
    strcat(p + 4, D_80028294[3]);
    strcat(p + 4, *(s16 *)0x80010208 != 0 ? D_80028294[1] : D_80028294[2]);
    strcat(p + 4, D_800282A0);
    func_8003AD08(*(s32 *)0x8001022C, buf);
    strcat(p + 4, buf);
}

/* A three-pass VRAM upload. 0x80010080 is a constant address, not a symbol --
   the `lui` + `ori` pair is the tell (see docs/addressing-and-translation-units.md).
   Both block copies have to go through the alignment-1 structs: the loads are
   lwl/lwr, so gcc is not allowed to assume either side is word-aligned.
   The `r.y` update must come AFTER the row copy (43 DIFF before it), and the
   source pointer must be `(u8 *)0x80010080 + i * 0x80` rather than a walking
   local advanced at the bottom of the loop (56 DIFF). */
void func_80025FFC(s32 *p, s32 n)
{
    RECT r;
    s32 i;

    r = *(RECT *)D_800200B8;
    *(UnkBlock20 *)((u8 *)p + 0x60) = *(UnkBlock20 *)D_80027E78[n];
    r.y += n * 16;
    for (i = 0; i < 3; i++)
    {
        StoreImage(&r, (u_long *)((u8 *)0x80010080 + i * 0x80));
        r.x += 4;
    }
    DrawSync(0);
}

void func_80026128(u8 *p, s32 a, s32 mode)
{
    u8 *b = (u8 *)0x80010000;
    s32 f;

    *(UnkFour *)p = *(UnkFour *)D_80028064;
    f = (mode ^ 2) == 0;
    *(s32 *)(b + 0x224) = *(s32 *)(b + 0x2D5C);
    *(s16 *)(b + 0x208) = f;
    *(s16 *)(b + 0x20A) = f;
    *(s32 *)(b + 0x230) = func_80025E54();
    *(s32 *)(b + 0x228) = func_80025ECC();
    func_80025F0C((u8 *)p, a);
    func_80025FFC((s32 *)p, a);
}

void func_800261C0(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_800261F0(s32 *p)
{
    func_800261C0(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_800265B8;
    }
}

void func_80026240(s32 *p)
{
    func_80026920(p - 8);
    func_80027BF4();
    func_80040AA0(3);
}

void func_80026270(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_80024298((s32 *)p[3]);
        p++;
    } while (++i < 5);
}

/* The `p[-4] = ...` store is written out in all three arms, as in the twin
   func_80025230, and gcc's cross-jumping merges them into the tail: that is
   where the target's `sw $v0, -0x10($s0)` in func_80020984's delay slot comes
   from. Spelling the merge by hand -- one `s32 h` assigned in each arm and
   stored once after the if -- lands 55 words with 3 DIFF, because then the
   `p[13]` store and the `p - 8` argument are the last two insns of the fail
   block and sched2, which is a REVERSE list scheduler whose only live
   tiebreak on r3000 is descending INSN_LUID (every latency is 1, so every
   priority is 1), puts the argument in the jal's delay slot instead of the
   store. Measured on that spelling: all six permutations of the fail block's
   three statements (parked order 55w/3, next best 56w/44, anything with the
   `h` assignment before the call 57-58w), the argument in a local at block
   and at function scope (both 55w/3), `(s32)p - 32` and `(s32)&p[-8]` as the
   argument (both 55w/3), and the store re-based on the argument local as
   `q[21]` with `q = p - 8`, which fixes the order and leaves only the base
   register wrong (55w/1). */
void func_800262BC(s32 *p)
{
    func_80026128((u8 *)0x80010000, p[11], p[9]);
    if (func_80021F18(p[11], (u8 *)0x80010000) == 0)
    {
        p[13] = (s32)func_800265B8;
        func_80022EEC((s32)(p - 8));
        p[-4] = (s32)func_800261C0;
    }
    else
    {
        func_80024F3C((s32 *)p[1], p[11], 3);
        func_80026270((s32 **)p);
        func_800241D4((s32 *)p[p[11] + 3], 1);
        if (((s16 *)0x80010000)[0x104] != 0)
        {
            func_800A6104();
            p[-4] = (s32)func_80026240;
        }
        else
        {
            func_80053DA8(0x503);
            p[-4] = (s32)func_800265B8;
        }
    }
    func_80020984();
    p[16] = 0;
}

void func_80026398(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_800262BC;
    }
}

void func_800263C0(s32 *p)
{
    p[15] = 1;
    func_80022F64((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80026398;
}

void func_80026410(s32 *p)
{
    if (p[14] != 0)
    {
        p[-4] = (s32)func_800265B8;
    }
    else
    {
        func_800263C0(p);
    }
}

void func_8002644C(s32 *p)
{
    p[13] = (s32)func_80026410;
    func_80022F3C((s32)(p - 8), (s32)(p + 14));
    p[-4] = (s32)func_800261F0;
}

void func_80026494(s32 *p)
{
    p[13] = (s32)func_800265B8;
    func_80022FB4((s32)(p - 8));
    p[-4] = (s32)func_800261F0;
}

void func_800264D8(s32 *p)
{
    p[13] = (s32)func_800265B8;
    func_80022FDC((s32)(p - 8));
    p[-4] = (s32)func_800261F0;
}

void func_8002651C(s32 *p)
{
    if (D_80083E98[p[11]][0] != 0)
    {
        p[-4] = (s32)func_8002644C;
    }
    else if (func_80022138() >= 3)
    {
        p[-4] = (s32)func_800263C0;
    }
    else if (func_80022160() != 0)
    {
        func_80026494(p);
    }
    else
    {
        func_800264D8(p);
    }
}

/* The card-list input handler. Three things are load-bearing:

   The `p[12] >= 0xD` arm has to come FIRST, with the plain increment as the
   `else`: the target's `slti`/`bnez` jumps forward to the increment, so the
   increment is the block placed after the chain, and writing it the natural
   way round costs a `j` over it.

   The two exit tests must be two separate `if`s. As `r == 0 || r == 1` gcc
   folds them into `sltiu $s0, $s0, 2`, where the target keeps `beqz` plus a
   `beq` against a materialised 1.

   The 0x20 case leaves through a `goto` into the tail -- it does NOT fall
   into the func_8002168C/func_80021904 pair the way every other path does.

   D_80083160 is the Unk80083160 struct from include/structs.h, read as
   .unk_08 and .unk_10; the same pair appears in func_800277BC. */
void func_800265B8(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            goto close;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_8002651C(p);
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        if (dir != 0)
        {
            func_80053DA8(0x502);
            p[11] = func_80049DE8(p[11], dir, 5);
            func_80025DC8(p);
        }
    }
    r = func_8002168C();
    func_80021904();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
close:
    func_80026920(p - 8);
    func_80027AFC(p[8], p[9]);
}

void func_80026720(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80026728(s32 *p, s32 v)
{
    func_80026720(p, v);
}

void func_80026748(s32 *p)
{
    func_80025DC8(p);
    func_80020984();
    p[-4] = (s32)func_800265B8;
}

void func_80026784(s32 *p)
{
    func_80026920(p - 8);
    func_80027AFC(p[8], p[9]);
}

void func_800267C0(s32 *p)
{
    s32 n = p[10];

    if (n < 5)
    {
        if (func_80021D54(n) != 0)
        {
            p[n + 3] = func_80023FF0((s32)(p - 8), n);
            p[10] = p[10] + 1;
        }
        else
        {
            p[13] = (s32)func_80026784;
            func_80023004((s32)(p - 8));
            p[-4] = (s32)func_800261C0;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80026748;
    }
}

s32 *func_80026868(s32 *p, s32 a)
{
    s32 *q = func_8003FC64(0);
    s32 *r;

    if (q != 0)
    {
        r = q + 8;
        r[9] = a;
        func_80026728(r, (s32)p);
        func_8002223C();
        func_800220DC();
        q[8] = func_8004F418(q, 0);
        r[1] = (s32)func_80024D58(q, r[11], 3);
        r[2] = (s32)func_80027364(q);
        r[10] = 0;
        q[4] = (s32)func_800267C0;
    }
    return q;
}

/* Same destructor family as func_80025D34, which is still a near miss; this
   one lands because only the last of its four calls is inside the loop, so
   there is no delay-slot nop to account for. gcc folds the 0xC into the
   memory displacement and strength-reduces the index to the q cursor, so
   `q[3]` with `q++` and `q[3 + i]` in a `for` both give the same 37 words. */
void func_80026920(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_8004F52C(q[0]);
        func_80024EEC((s32 *)q[1]);
        func_80027458((s32 *)q[2]);
        i = 0;
        do
        {
            func_80024184((s32 *)q[3]);
            q++;
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Two slots, each selected through the D_80028068 pick table. Four spellings
   were load-bearing here, and three of them are about the ORDER gcc emits a
   two-register `addu` in:

   - D_80028068 has to be declared `[][2]` and indexed `[p[9]][i]`. As a flat
     `u8 []` with `[p[9] * 2 + i]` gcc adds `i` before the array base, which
     is six words wrong across the three evaluations.
   - The func_8004CBFC pointer needs its offset parenthesised,
     `(u8 *)p + (k * 12 + 4)`, so the `+ 4` lands in the scaled value rather
     than after the base is added.
   - The tested pointer has to come off a BYTE cursor whose scale is already
     applied (`(u8 *)p + k * 4`, then `0x68` as the displacement). Written
     `p[26 + k]` the offset is still a shift when the address is formed, and
     gcc canonicalises `(plus (mult ...) (reg))` with the shift first, giving
     `addu $v0, $v0, $s2` where the target has `addu $v0, $s2, $v0`. Four
     base-first spellings of the same index all leave that one word wrong.

   func_8004CBFC takes THREE arguments (see csrc/unk_1C540.c:2791); the
   declaration here had two, and the third is this same pointer, which is why
   the target loads it into $a2 before the test. */
void func_800269B4(s32 *p)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        s32 k = D_80028068[p[9]][i];
        u8 *q = (u8 *)p + k * 4;
        u32 *e = *(u32 **)(q + 0x68);

        if (e[0] != 0)
        {
            func_8004CBFC((u32 *)((u8 *)p + (k * 12 + 4)), D_80028074[i], e);
        }
        ((UnkCardPair *)p[24 + i])->unk_04->unk_00 = D_8002806C[D_80028068[p[9]][i]];
        ((UnkCardPair *)p[26 + i])->unk_04->unk_00 = D_8002806C[D_80028068[p[9]][i]];
    }
}

void func_80026AE4(s32 *p)
{
    func_8004CB2C(p + 1);
    func_8004CB2C(p + 4);
}

/* The two 0xC-stride induction variables are kept apart by WHERE the constant
   offset is parenthesised, the same lever as func_800269B4 above. The call's
   pointer takes `(i * 0xC + 4)` as one scaled value, so loop.c reduces it to a
   giv with add_val 4 and adds `p` at the use (`addu $a0, $s7, $s3`); the
   store's address leaves `+ 8` outside, so its giv is `p + i * 0xC` with
   add_val `p` and 8 as the displacement. Two givs whose add_vals are a
   register and a constant cannot differ by a constant, so combine_givs_p
   declines to merge them -- write both offsets the same way and gcc folds them
   into a single giv, which is two words short and also hoists
   `%hi(D_80027E68)` out of the loop, taking $s7 away from `p`. */
void func_80026B18(u8 *p, s32 *b, s32 *c)
{
    s32 any = 0;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (c[i] != 0)
        {
            UnkCardPair *q = (UnkCardPair *)((s32 *)p)[0x1A + i];

            any = 1;
            if (b[i] != 0)
            {
                func_8004CBFC((u32 *)(p + (i * 0xC + 4)), D_80027E68, (u32 *)q);
            }
            else
            {
                q->unk_00 = 0;
                *(s32 *)(p + i * 0xC + 8) = 0;
            }
        }
    }
    if (any != 0)
    {
        func_800269B4((s32 *)p);
    }
}

void func_80026C00(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

void func_80026C94(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x18];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80028418;
    q = (UnkCardPair *)p[0x19];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80028430;
}

/* func_8004CBFC takes THREE arguments (csrc/unk_1C540.c:2791) and the third
   is the UnkCardPair this function just wrote through -- which is why the
   target loads it into $a2, uses $a2 for the two field stores, and then
   leaves it there for the call. With two arguments the pointer goes to a
   different register and $s0/$s1 come out swapped for the whole function.
   Same discovery as func_800269B4, from the other side. */
void func_80026CF0(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x1A];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    func_8004CBFC((u32 *)(p + 1), D_80027E68, (u32 *)q);
    q = (UnkCardPair *)p[0x1B];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    func_8004CBFC((u32 *)(p + 4), D_80027E68, (u32 *)q);
}

void func_80026D78(s32 *p)
{
    func_80026C94(p);
    func_80026CF0(p);
    func_80026B18((u8 *)p, (s32 *)D_8002807C, (s32 *)D_80028084);
    func_800269B4(p);
}

s32 func_80026DC8(s32 a)
{
    return func_800491F4(a, D_8002808C, 4);
}

void func_80026DF0(UnkCardGrp044 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_80026E90(s32 *p, s32 v)
{
    p[0] = v;
    func_80026C00((UnkCardPair *)(p + 0x14), (u32 *)(p + 0xA), (u32 *)(p + 0xE));
    func_80026D78(p);
}

s32 *func_80026ED0(s32 *p, s32 a)
{
    s32 *q = (s32 *)D_8002B850;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002B850, 0x45);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80026FB4((u16 *)q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0xF0);
    }
    func_80026DF0((UnkCardGrp044 *)r, 4);
    q[3] = (s32)(r + 0x14);
    r[0x17] = func_80026DC8((s32)(r + 0x18));
    r[9] = a;
    func_80026E90(r, (s32)p);
    q[4] = (s32)func_80026AE4;
    return q;
}

void func_80026FB4(u16 *p)
{
    p[15] |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80026FD4(s32 *p, s32 v)
{
    s32 *q = p + 8;

    q[9] = v;
    func_800269B4(q);
}

void func_80026FF8(s32 *p, s32 *b, s32 *c)
{
    func_80026B18((u8 *)(p + 8), b, c);
}

void func_80027018(s32 *p)
{
    *(s32 *)p[0x3C] = (s32)D_80077EF0;
    *(s32 *)p[0x3D] = (s32)D_80077EFC;
    *(s32 *)p[0x3E] = (s32)(p + 1);
    *(s32 *)p[0x3F] = (s32)(p + 0x1F);
}

void func_80027054(u8 *p)
{
    func_8004DA74(p + 0x7C, D_800282CC, 1);
}

s32 func_80027080(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

void func_800270E0(s32 *p)
{
    func_80027054((u8 *)p);
    func_80027080((s32)(p + 1), 0x36, 0x10);
    func_80027018(p);
    ((UnkCardPair *)p[0x3F])->unk_04->unk_08 = 0x8E;
    ((UnkCardPair *)p[0x3F])->unk_04->unk_0A = 0x82;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_08 = 9;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_08 = 0x42;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_08 = 0x24;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_0A = 0xA;
}

void func_800271C0(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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
    p->unk_04->unk_08 = -152;
    p->unk_04->unk_0A = -98;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

s32 func_80027254(s32 a)
{
    return func_800491F4(a, D_80028094, 4);
}

void func_8002727C(UnkCardGrp0D4 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_8002731C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80027324(s32 *p, s32 v)
{
    func_8002731C(p, v);
    func_800271C0((UnkCardPair *)(p + 0x38), (u32 *)(p + 0x2E), (u32 *)(p + 0x32));
    func_800270E0(p);
}

s32 *func_80027364(s32 *p)
{
    s32 *q = (s32 *)D_8002B968;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002B968, 0x68);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80023A00(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x180);
    }
    func_8002727C((UnkCardGrp0D4 *)r, 4);
    q[3] = (s32)(r + 0x38);
    r[0x3B] = func_80027254((s32)(r + 0x3C));
    func_80027324(r, (s32)p);
    return q;
}

void func_8002742C(s32 *p)
{
    func_8004E994(*(s32 *)p[0x3F]);
}

void func_80027458(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_8002742C(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_800274A8(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[11];
    }
}

void func_800274D8(s32 *p)
{
    func_800274A8(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_8002789C;
    }
}

/* The two calls take the SAME local, which is the whole function: gcc knows
   `v` is 0 in the true arm and materialises `addu $a1, $zero, $zero` there,
   and in the false arm `v` is already in $a1 from the test, so nothing is
   emitted. func_80025C7C needs a second parameter declared for this -- it
   ignores it, so its own 46 words are unchanged, and it had no other
   caller. */
void func_80027528(s32 *p)
{
    s32 v;

    func_80027C90(p - 8);
    v = p[0];
    if (v == 0)
    {
        func_80025C7C((s32 *)p[5], v);
    }
    else
    {
        func_80026868((s32 *)p[5], v);
    }
}

void func_80027584(s32 *p)
{
    s32 i = 0;
    s32 r;

    do
    {
        r = func_80021A84(p[3]);
        if (r != 5)
        {
            break;
        }
    } while (i++ < 6);
    p[14] = 0;
    func_80020984();
    if (r == 1)
    {
        p[11] = (s32)func_8002789C;
        func_80023054((s32)(p - 8));
        p[-4] = (s32)func_800274D8;
    }
    else if (r == 4)
    {
        p[11] = (s32)func_8002789C;
        func_80022E9C((s32)(p - 8));
        p[-4] = (s32)func_800274A8;
    }
    else
    {
        p[11] = (s32)func_8002789C;
        func_8002302C((s32)(p - 8));
        p[-4] = (s32)func_800274D8;
    }
}

void func_8002765C(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80027584;
    }
}

void func_80027684(s32 *p)
{
    s32 h;

    if (p[15] != 0)
    {
        h = (s32)func_8002789C;
    }
    else
    {
        p[16] = 1;
        func_80022F14((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
        h = (s32)func_8002765C;
    }
    p[-4] = h;
}

void func_800276EC(s32 *p)
{
    p[11] = (s32)func_80027684;
    func_80022E74((s32)(p - 8), (s32)(p + 15));
    p[-4] = (s32)func_800274D8;
}

void func_80027734(s32 *p)
{
    p[11] = (s32)func_8002789C;
    func_80022E9C((s32)(p - 8));
    p[-4] = (s32)func_800274A8;
}

void func_80027778(s32 *p)
{
    p[11] = (s32)func_8002789C;
    func_8002307C((s32)(p - 8));
    p[-4] = (s32)func_800274D8;
}

void func_800277BC(s32 *p)
{
    s32 n;
    s32 h;

    if (D_80083160.unk_08 != 0 && (D_80083160.unk_10 & 0x20) != 0)
    {
        func_800209C4();
        p[14] = 0;
        p[-4] = (s32)func_8002789C;
        return;
    }
    n = func_8002168C();
    func_80021904();
    if (n != 0)
    {
        p[14] = 0;
        p[12] = n;
        h = D_8002809C[n];
        p[13] = n;
        p[-4] = h;
        return;
    }
    if (func_800210E0() == 3 && p[14] == 0)
    {
        func_80022EC4((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
    }
}

/* The fourth input handler. Its tail is the interesting part: it calls
   func_800217D4 for the &D_800287D8 pair that function returns, records
   whether each word changed, then copies the pair into p[6]/p[7]. That copy
   has to be an 8-byte STRUCT assignment -- gcc's aligned movstrsi, lw/lw
   then sw/sw -- because as two field assignments it comes out interleaved
   and one word longer. The 0x40 case returns outright rather than falling
   into the shared tail. */
void func_8002789C(s32 *p)
{
    s32 flag = 0;
    s32 *t;
    s32 diff[2];

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            func_80027C90(p - 8);
            func_80027BF4();
            if (D_80082E60.unk_0B == 3)
            {
                func_80040AA0(3);
            }
            else
            {
                func_80044144(0, 0, 0, 0);
                ((u16 *)p[5])[15] |= 0x2000;
                if (p[0] == 2)
                {
                    func_800A68F4();
                }
            }
        }
        else if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_80020924(p[3]);
            p[13] = 0;
            p[-4] = (s32)func_800277BC;
            return;
        }
        else if (D_80083160.unk_08 & 0xA000)
        {
            if (D_80083160.unk_08 & 0x8000)
            {
                if (p[3] != 0)
                {
                    flag = 1;
                }
                p[3] = 0;
            }
            else
            {
                if (p[3] == 0)
                {
                    flag = 1;
                }
                p[3] = 1;
            }
        }
        if (flag != 0)
        {
            func_80053DA8(0x502);
            func_80026FD4((s32 *)p[9], p[3]);
        }
    }
    t = func_800217D4();
    diff[0] = t[0] != p[6];
    diff[1] = t[1] != p[7];
    *(UnkTwoWords *)&p[6] = *(UnkTwoWords *)t;
    func_80026FF8((s32 *)p[9], t, diff);
    func_80021904();
}

void func_80027A68(s32 *p, s32 a, s32 b)
{
    p[5] = a;
    p[0] = b;
    p[3] = func_8002190C();
}

void func_80027A9C(s32 *p)
{
    s32 *q = p - 8;

    p[8] = func_8004F418(q, p + 3);
    p[9] = (s32)func_80026ED0(q, p[3]);
    func_800209C4();
    p[-4] = (s32)func_8002789C;
}

s32 *func_80027AFC(s32 a, s32 b)
{
    s32 *q = (s32 *)D_8002BB08;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002BB08, 0x19);
    }
    else
    {
        func_80027C90(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x44);
    }
    D_80082E60.unk_0F |= 0x80;
    func_80027A68(r, a, b);
    q[4] = (s32)func_80027A9C;
    return q;
}

void func_80027BC0(void)
{
    EnterCriticalSection();
    StartRCnt(0xF2000002);
    ExitCriticalSection();
}

void func_80027BF4(void)
{
    D_80080A84 = 2;
    func_80020F18();
    func_800585A0();
    if (D_80082E6A == 1)
    {
        func_80053DA8(0x200);
    }
    else
    {
        D_80080A88 = 0;
    }
    D_80082E60.unk_0F &= 0x7F;
}

void func_80027C60(void)
{
    func_8005863C();
    D_80080A84 = 1;
    func_80020EA0();
}

void func_80027C90(s32 *p)
{
    s32 *q = p + 8;

    func_8004F52C(q[8]);
    func_80026FB4((u16 *)q[9]);
    if (p[8] == 2)
    {
        func_800A6104();
    }
    ((u16 *)p)[15] |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80027D0C(s32 *p)
{
    func_80027C60();
    func_80027AFC((s32)p, 1);
}

void func_80027D40(s32 *p)
{
    func_800A68C0();
    func_80027C60();
    func_80027AFC((s32)p, 2);
}

void func_80027D7C(s32 *p)
{
    func_80027C60();
    D_80083200 = 0x200;
    SetGeomScreen(0x200);
    SetGeomOffset(0xA0, 0x78);
    func_80027AFC((s32)p, 0);
}

s16 func_80027DD0(void)
{
    return func_8004CAA0();
}

void func_80027DF4(s32 *p, s32 v)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[3] = v;
        func_80026FD4((s32 *)q[9], v);
    }
}

/* D_800200A8 is func_800258C8's own local array initialiser, which gcc now
   emits into this unit's .rodata, so it must NOT also be pulled in by
   INCLUDE_RODATA -- doing both put 0x10 extra bytes of rodata ahead of the
   text and moved every function from func_800210F0 on by 0x10, which the
   function-pointer tables at D_80020034 and D_80020048 then disagreed with.
   Each per-function ovlverify check still passed; only `make` caught it. */

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200B8);

/* ------------------------------------------------------------------------- */
/* The overlay's own .data, which starts at 0x80027E20 immediately after the
   text end and is being taken over from the bottom up -- see
   docs/owning-data.md. Everything below 0x80027F20 is still
   asm/data/ovl_main_80020000_data_pre.data.s; this unit owns 0x80027F20 and
   up, and the splat yaml's `.data` subsegment boundary has to move with every
   object that moves across.

   DEFINITION ORDER IS ADDRESS ORDER. cc1psx lays .data objects out in the
   order they are written here, so a new object goes at its address position in
   this block and nowhere else. */

/* The two memory-card device names, as pointers into this unit's own rodata:
   D_80020074 is "bu10:" and the "bu00:" that follows it at +8 shares the
   listing, so the second entry is spelled as an offset into the first symbol.
   Index 0 is "bu00:" and index 1 is "bu10:", which is the opposite of the
   order the strings sit in. */
char *D_80027F18[2] = {
    D_80020074 + 8,
    D_80020074,
};

/* The save-file prefix, pasted after a "bu10:"/"bu00:" device name by
   func_80021F0C. Sixteen bytes, not thirteen: the four bytes after the NUL are
   part of the object rather than inter-object padding, and `[]` would drop
   them and pull every later .data object down by four. */
char D_80027F20[16] = "BASLUS-00614";
