#include "common.h"
#include "main.h"
#include "strings.h"

/* c_server.c -- the FIRST of the nine translation units the 0x80020000
   overlay was built from, carved out of csrc/ovl_main_80020000.c on
   2026-11-10. Built at cc1psx -G0 like the rest of the overlay; the Makefile
   pattern rule covers csrc/main/ too.

   WHAT IS CARVED AND WHAT IS NOT. This file owns its TEXT -- the 62 functions
   from set_card_slot to erase_tmp_file, live-block indices 0..61 -- and its
   RODATA, 0x80020000..0x800200A0: ASSERT_FMT, FILENAME_C_SERVER, the two
   event-handler tables, MEM_CARD1/MEM_CARD0 and the six save-file name
   strings. docs/overlays.md records those five rodata addresses as
   c_server.c's with none disputed.

   It owns NO DATA. Every data object stays in csrc/ovl_main_80020000.c,
   including the ones this file's functions use (MEM_CARD_NAMES,
   MEM_CARD_PREFIX, MEM_CARD_FILENAMES, card_st, card_dir, the eight event
   descriptors). That is deliberate: the overlay's .data region is NOT
   partitionable per unit from what is known yet. Its middle run,
   0x80027F48..0x800280B4, is a perfect per-unit concatenation in link order,
   but the three objects before it and everything from gCardMessages on are
   not, under usage-based attribution -- see docs/node-payload.md's sibling
   section in docs/overlays.md. An object has one .data section, so a wrong
   guess there would be unbuildable rather than merely wrong, and an empty one
   constrains nothing.

   THE END BOUNDARY IS AN INFERENCE, inside an evidenced bracket.
   docs/overlays.md puts c_server.c's last assert anchor at index [56, 58] and
   card_mes.c's first function in [59, 65]. The cut here is at 61/62, between
   erase_tmp_file and the message window's text measurer func_8002225C, which
   is the semantic break in that window: everything at or below 61 is the card
   server and its file layer, everything at or above 62 is UI. No function
   below the cut calls anything above it. */

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
void set_card_slot(s32 use_second_slot)
{
    s32 *p;

    reset_sw_events();
    _card_wait(0);
    _card_wait(1);
    p = &card_slot;
    if (use_second_slot)
    {
        *p = 0x10;
    }
    else
    {
        *p = 0;
    }
    check_card_slot(card_slot);
}

void recheck_card_slot(void)
{
    reset_sw_events();
    _card_wait(0);
    _card_wait(1);
    check_card_slot(card_slot);
}

// Same as recheck_card_slot, but swaps the active card slot
void switch_card_slot(void)
{
    long next_card_slot;

    reset_sw_events();
    _card_wait(0);
    _card_wait(1);
    next_card_slot = 0x10 - card_slot;
    card_slot = next_card_slot;
    check_card_slot(next_card_slot);
}

void reset_card_server(void)
{
    s32 *p = &card_st;

    *p = 0;
    reset_sw_events();
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

   It falls out whole once exit is NORETURN (see close_card_events): each
   of the sixteen %hi values then stays live in its own callee-saved register
   across the calls, which is why the function saves $s0..$s7 and $fp, and
   $s0 holds the -1 the first eight comparisons share. */
void open_card_events(void)
{
    EnterCriticalSection();
    gSwCardEOFEvent = OpenEvent(SwCARD, EvSpIOE, EvMdNOINTR, NULL);
    if (gSwCardEOFEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 185);
        exit(1);
    }
    gSwCardErrorEvent = OpenEvent(SwCARD, EvSpERROR, EvMdNOINTR, NULL);
    if (gSwCardErrorEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 187);
        exit(1);
    }
    gSwCardTimeoutEvent = OpenEvent(SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    if (gSwCardTimeoutEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 189);
        exit(1);
    }
    gSwCardNewEvent = OpenEvent(SwCARD, EvSpNEW, EvMdNOINTR, NULL);
    if (gSwCardNewEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 191);
        exit(1);
    }
    gHwCardEOFEvent = OpenEvent(HwCARD, EvSpIOE, EvMdINTR, HandleCardEOF);
    if (gHwCardEOFEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 194);
        exit(1);
    }
    gHwCardErrorEvent = OpenEvent(HwCARD, EvSpERROR, EvMdINTR, HandleCardError);
    if (gHwCardErrorEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 196);
        exit(1);
    }
    gHwCardTimeoutEvent = OpenEvent(HwCARD, EvSpTIMOUT, EvMdINTR, HandleCardTimeout);
    if (gHwCardTimeoutEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 198);
        exit(1);
    }
    gHwCardNewEvent = OpenEvent(HwCARD, EvSpNEW, EvMdINTR, HandleCardNew);
    if (gHwCardNewEvent == -1)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 200);
        exit(1);
    }
    ExitCriticalSection();
    if (EnableEvent(gSwCardEOFEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 212);
        exit(1);
    }
    if (EnableEvent(gSwCardErrorEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 214);
        exit(1);
    }
    if (EnableEvent(gSwCardTimeoutEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 216);
        exit(1);
    }
    if (EnableEvent(gSwCardNewEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 218);
        exit(1);
    }
    if (EnableEvent(gHwCardEOFEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 220);
        exit(1);
    }
    if (EnableEvent(gHwCardErrorEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 222);
        exit(1);
    }
    if (EnableEvent(gHwCardTimeoutEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 224);
        exit(1);
    }
    if (EnableEvent(gHwCardNewEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 226);
        exit(1);
    }
}

void start_card_server(void)
{
    if (card_first_flg[0] == 0)
    {
        card_first_flg[0] = 1;
        D_80084118 = 0;
        (&D_80084118)[1] = 0;
        bzero((unsigned char *)D_80083E98, 0x280);
    }
    open_card_events();
    D_800287D0[0] = 0;
    D_800287D0[1] = 0;
    D_800287D8[0] = 0;
    D_800287D8[1] = 0;
    card_st = 0;
}

/* c_server.c's teardown: eight asserts bracketed by EnterCriticalSection /
   ExitCriticalSection (EnterCriticalSection / ExitCriticalSection), one per event
   handle, at source lines 263..277 in steps of two.

   exit has to be declared NORETURN. Without it gcc assumes each
   assert body falls through into the next test, so after the two calls it
   re-materialises the next handle's %hi -- one extra word per assert, 121
   against 114. With it the `lui` that reorg puts in each `bnez`'s delay slot
   is the one the branch target's `lw` uses. This is the lever that makes the
   whole assert family reachable; see open_card_events, which has 24 of them. */
void close_card_events(void)
{
    EnterCriticalSection();
    if (CloseEvent(gSwCardEOFEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 263);
        exit(1);
    }
    if (CloseEvent(gSwCardErrorEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 265);
        exit(1);
    }
    if (CloseEvent(gSwCardTimeoutEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 267);
        exit(1);
    }
    if (CloseEvent(gSwCardNewEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 269);
        exit(1);
    }
    if (CloseEvent(gHwCardEOFEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 271);
        exit(1);
    }
    if (CloseEvent(gHwCardErrorEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 273);
        exit(1);
    }
    if (CloseEvent(gHwCardTimeoutEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 275);
        exit(1);
    }
    if (CloseEvent(gHwCardNewEvent) == 0)
    {
        printf(ASSERT_FMT, FILENAME_C_SERVER, 277);
        exit(1);
    }
    ExitCriticalSection();
}

s32 get_card_st(void)
{
    return card_st;
}

s32 on_sw_card_eof(void)
{
    s32 st = card_st;
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
    if (card_slot != 0)
    {
        t++;
    }
    if (t[0] != 0)
    {
        r = 1;
        card_st = 0;
    }
    else if ((_card_status(0) & 1) != 0 || (_card_status(1) & 1) != 0)
    {
        reset_sw_events();
        while (_card_load(card_slot) == 0)
        {
        }
        card_evflg = 0;
        card_st = 3;
    }
    goto out;
three:
    u = &D_80084118;
    if (card_slot != 0)
    {
        u++;
    }
    r = 2;
    u[0] = 1;
    card_st = 0;
    /* DEVICE: an empty volatile asm, worth 1 word. It emits nothing; it exists
       only to put a non-SET insn at the end of this basic block. gcc 2.7.2 runs
       jump_optimize with cross_jump AFTER sched2 and BEFORE dbr_schedule
       (toplev.c:3117/3142/3167), and for an unconditional `j' find_cross_jump is
       called with minimum == 1 (jump.c:1978), so ONE matching insn before the
       jump is enough to merge. Without this asm the insn before `out:' is
       `sw $zero,%lo(card_st)($a1)', identical to the one that ends the st==1
       block above, so the st==1 block's `j out' is retargeted onto this one; dbr
       then fills its delay slot with the preceding `addiu $s1,1' instead of the
       store and the function comes out 74 words (47 DIFF). The asm's pattern is
       ASM_INPUT, not SET, so find_cross_jump breaks on the GET_CODE(p1) !=
       GET_CODE(p2) test with minimum still 1 and no merge happens. It cannot be
       moved: between the two stores it is 74 words again, and at the end of the
       st==1 block it is 76 (ASM_INPUT also stops dbr's backward scan, so that
       block's delay slot goes to a nop).

       Ruled out, all measured with tools/decomp/ovlverify.py: reordering the
       st==1 tail to `card_st = 0; r = 1;' keeps 75 words but 2 DIFF (sched2
       does not swap the two leaf stores back -- they tie on priority and the tie
       breaks to source order); swapping the st==3 tail (any of its six orders)
       74w/47 or 75w/4; `*(volatile s32 *)&card_st = 0' on either store
       74w/47 (rtx_renumbered_equal_p ignores MEM_VOLATILE_P); `*(u32 *)&',
       `*(s32 *)&' and `*(&card_st)' 74w/47; a trailing `goto out;' 74w/47
       (the early jump_optimize deletes a jump to the next label long before the
       cross-jump pass); a trailing `return r;' 76w/15; a trailing `while (0) {}'
       75w/24; a trailing `;' or an unused label 74w/47.

       It does NOT warn under -Wall. */
    __asm__ volatile("");
out:
    return r;
}

s32 on_hw_card_eof(void)
{
    s32 r = 0;

    if (card_st == 2)
    {
        reset_sw_events();
        if (_card_load(card_slot) == 0)
        {
            card_st = 0;
            r = 5;
        }
        else
        {
            card_evflg = 0;
            card_st = card_st + 1;
        }
    }
    return r;
}

/* The three `goto`s out of the dispatch, the DUPLICATED fail body and the
   trailing `def:` block are all load-bearing; measurements in
   docs/matching-levers.md under "A DUPLICATED FAIL BODY IS WHAT PUTS IT IN
   cse1's PATH".

   - The fail body has to appear TWICE, once per test. cse1 can only share the
     one `lui %hi(card_st)` with a block it reaches on a path from the
     function's first insn, and cse_end_of_basic_block (cse.c:8103) will only
     extend a path across a conditional jump whose target label has
     LABEL_NUSES == 1. A single shared fail block (`&&`, or either nesting)
     gives that label two uses, so the fail body gets a SECOND `lui`: 58 words
     / 23 DIFF. jump2's cross-jumping merges the two copies back into one.
   - `def:` must sit between the st==3 arm and `out:`. Without it the st==3
     arm's trailing `card_st = 0` is the insn before `out:` and is the same
     word as the fail body's, so find_cross_jump (jump.c:1978, minimum 1)
     steals the fail copies' store instead and each copy loses a word to the
     merge: 58 words / 25 DIFF. With `def:` in between, the insn before `out:`
     is `r = 5`, the compare fails, and the two fail copies merge with each
     other instead -- which is the 3-word block the target has.

   Ruled out, all measured: plain if/else-if chain 56w/44; `st != 1` first
   56w/44; fully nested `!=` 56w/44; `switch` 58w/23; a single-fail `goto`
   chain 58w/23; duplicated fail bodies inline (if/else-if/else) 58w/36;
   duplicated fail bodies out of line without `def:` 58w/25; duplicated with
   `card_st = 0;` before `r = 5;` 57w/2; duplicated with the st==3 arm's
   two stores swapped 57w/2. */
s32 on_card_new(void)
{
    s32 st = card_st;
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
    reset_hw_events();
    if (_card_clear(card_slot) == 0)
    {
        goto fail1;
    }
    if ((_card_status(card_slot != 0) & 4) == 0)
    {
        goto fail2;
    }
    card_evflg = st;
    card_st = card_st + 1;
    goto out;
fail1:
    r = 5;
    card_st = 0;
    goto out;
fail2:
    r = 5;
    card_st = 0;
    goto out;
three:
    t = &D_80084118;
    if (card_slot != 0)
    {
        t++;
    }
    r = 3;
    t[0] = 0;
    card_st = 0;
    goto out;
def:
    r = 5;
out:
    return r;
}

s32 on_card_timeout(void)
{
    s32 *p = &card_st;
    s32 r = 0;

    if (*p != 0)
    {
        r = 4;
        *p = 0;
    }
    return r;
}

s32 on_card_error(void)
{
    s32 *p = &card_st;
    s32 r = 0;

    if (*p != 0)
    {
        r = 5;
        *p = 0;
    }
    return r;
}

/* Returns 1, and that return value is what the extra word is for. The unit's
   own matched twin of this tail -- on_sw_card_eof's st==1 block, same
   `card_evflg = 0; card_st = <const>;' after the same call-and-spin -- comes
   out `lui' in the delay slot, `lui', `sw $zero', `addiu $v0, $zero, 3',
   `sw $v0'. Here the target instead materialises the 1 FIRST (into the delay
   slot) and copies it, `addu $v1, $v0, $zero', before the store. That is
   because $v0 is pinned: the return value's hard-register set is
   `(set (reg:SI 2 v0) (const_int 1))', cse2 unifies it with the store's
   constant, so the one pseudo holding 1 lives in $v0 up to the `jr' and the
   store's source has to be a second register. Three regs are live at the peak
   ($v0 = 1, $v1 = %hi(card_evflg), $a0 = %hi(card_st)), which is also why
   the second `lui' lands in $a0 rather than reusing $v0.

   Declaring it `void' is what parked it: with no return value the constant is
   an ordinary statement-level quantity, the allocator puts it straight in the
   store's register and there is no copy -- 18 words, 10 DIFF, whatever the tail
   is spelled like (`t = 1; ... card_st = t;' 18w/11; `while' for `do/while'
   and the two stores swapped 18w/10-11). `s32 t = 1; ... return t;' is worse
   still at 21w/19: `t' then spans the calls and takes $s1.

   The three callers in this unit discard the result, so the prototype change
   from `void' is invisible to them -- all four containers stay byte-identical. */
s32 check_card_slot(s32 card_slot)
{
    reset_sw_events();
    do
    {
    } while (_card_info(card_slot) == 0);
    card_evflg = 0;
    card_st = 1;
    return 1;
}

s32 test_sw_card_events(void)
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
    else if (TestEvent(gSwCardTimeoutEvent) != 0)
    {
        r = 3;
    }
    else if (TestEvent(gSwCardNewEvent) != 0)
    {
        r = 4;
    }
    return r;
}

void reset_sw_events(void)
{
    TestEvent(gSwCardEOFEvent);
    TestEvent(gSwCardErrorEvent);
    TestEvent(gSwCardTimeoutEvent);
    TestEvent(gSwCardNewEvent);
}

CARD_STATE take_card_state(void)
{
    CARD_STATE state = gCardState;
    CARD_STATE old_state = CARD_STATE_OK;

    if (state != CARD_STATE_OK)
    {
        old_state = state;
        reset_hw_events();
    }
    return old_state;
}

void reset_hw_events(void)
{
    gCardState = CARD_STATE_OK;
    TestEvent(gHwCardEOFEvent);
    TestEvent(gHwCardErrorEvent);
    TestEvent(gHwCardTimeoutEvent);
    TestEvent(gHwCardNewEvent);
}

s32 poll_card_events(void)
{
    UnkFnTab5 a = gSwCardEventHandlers;
    UnkFnTab5 b = gHwCardEventHandlers;
    s32 r = 0;
    s32 st = card_evflg;

    if (st == 0)
    {
        r = test_sw_card_events();
        if (r != 0)
        {
            r = a.f[r]();
        }
    }
    else if (st == 1)
    {
        r = take_card_state();
        if (r != 0)
        {
            r = b.f[r]();
        }
    }
    return r;
}

const char ASSERT_FMT[] = "Assertion failed: file \"%s\", line %d\n";
const char FILENAME_C_SERVER[] = "c_server.c";

const UnkFnTab5 gSwCardEventHandlers = {{
    NULL,
    on_sw_card_eof,
    on_card_error,
    on_card_timeout,
    on_card_new,
}};

const UnkFnTab5 gHwCardEventHandlers = {{
    NULL,
    on_hw_card_eof,
    on_card_error,
    on_card_timeout,
    on_card_new,
}};

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
    s32 st = poll_card_events();
    s32 i = is_second_card_slot();

    if (st != 0)
    {
        if (D_80028538[i] == st)
        {
            D_80028530[i]++;
        }
        else
        {
            D_80028530[i] = 0;
        }
        D_80028538[i] = st;
        switch (st)
        {
        case 1:
        case 2:
            D_800287D0[i] = st;
            break;
        case 3:
            if (D_80028530[i] >= 0xB)
            {
                D_800287D0[i] = st;
                D_80028530[i] = 0;
                D_80028538[i] = 0;
            }
            break;
        case 4:
        case 5:
            (&D_80084118)[i] = 0;
            if (D_80028530[i] >= 0xB)
            {
                D_800287D0[i] = st;
                D_80028530[i] = 0;
                D_80028538[i] = 0;
            }
            break;
        }
        set_card_slot(i);
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
    s32 v = test_sw_card_events();
    s32 i;

    if (v != 0)
    {
        i = is_second_card_slot();
        if (D_80028548[i] == v)
        {
            D_80028540[i]++;
        }
        else
        {
            D_80028540[i] = 0;
        }
        D_80028548[i] = v;
        if (v == 1 || v == 4)
        {
            if (D_80028540[i] >= 4)
            {
                D_800287D8[i] = 1;
                D_80028540[i] = 0;
                D_80028548[i] = 0;
            }
        }
        else
        {
            (&D_80084118)[i] = 0;
            if (D_80028540[i] >= 4)
            {
                D_800287D8[i] = 0;
                D_80028540[i] = 0;
                D_80028548[i] = 0;
            }
        }
        switch_card_slot();
    }
    return D_800287D8;
}

void print_card_status(void)
{
}

int is_second_card_slot(void)
{
    return card_slot != 0;
}

const char MEM_CARD1[8] = "bu10:";
const char MEM_CARD0[8] = "bu00:";

s32 count_card_files(void)
{
    u8 name[0x20];
    s32 n = 0;
    struct DIRENTRY *e = card_dir;
    const char **mem_card_name = &MEM_CARD_NAMES[0];

    if (card_slot != 0)
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

/* splat MIGRATED this block into count_card_files's nonmatching listing, so
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
const char D_80020084[4] = "*";
const char D_80020088[4] = "TMP";
const char D_8002008C[4] = "04";
const char D_80020090[4] = "03";
const char D_80020094[4] = "02";
const char D_80020098[4] = "01";
const char D_8002009C[4] = "00";

s32 count_free_blocks(s32 n)
{
    s32(*p)[10] = (s32(*)[10])card_dir;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < n; i++)
    {
        sum += p[i][6];
    }
    sum = sum / 0x2000;
    return 0xF - sum;
}

s32 card_file_exists(s32 n, u8 *s)
{
    s32 i = 0;
    s32 ret = 0;
    u8 *tbl = (u8 *)card_dir;
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

s32 format_card(s32 card_slot)
{
    const char **mem_card;
    s32 r;

    _card_wait(0);
    _card_wait(1);
    reset_hw_events();
    mem_card = MEM_CARD_NAMES;
    if (card_slot != 0)
    {
        mem_card++;
    }
    format(*mem_card);
    while ((r = take_card_state()) == 0)
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

/**
 * Build the complete filepath for a given file ID.
 * @param out Where the filepath will be stored
 * @param file_id ID of file to access
 */
void get_card_path(char *out, s32 file_id)
{
    const char **mem_card_name = &MEM_CARD_NAMES[0];

    if (card_slot != 0)
    {
        mem_card_name++;
    }
    strcpy(out, *mem_card_name);
    strcat(out, MEM_CARD_PREFIX);
    strcat(out, MEM_CARD_FILENAMES[file_id]);
}

s32 read_card_file(u8 *name, u8 *buf, s32 n, s32 off)
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

s32 write_card_file(u8 *name, u8 *buf, s32 n, s32 off, s32 skip)
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

s32 save_slot_used(s32 i)
{
    return D_80083E98[i][0];
}

s32 load_save_slot_info(s32 idx)
{
    u8 name[0x20];
    s32 r = 1;
    s32 **t;
    s32 *e;
    s32(*tbl)[32];
    s32 v;

    get_card_path(name, idx);
    t = (s32 **)card_file_count;
    if (card_slot != 0)
    {
        t++;
    }
    v = card_file_exists((s32)*t, name + 5);
    tbl = D_80083E98;
    e = tbl[idx];
    e[0] = v;
    if (v != 0)
    {
        r = read_card_file(name, (u8 *)e, 1, 4);
        e[0] = 1;
    }
    return r;
}

void store_save_slot_info(s32 idx, u8 *src)
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

/* `gsw` and GSW_CHECK_SUM are the twin's own identifiers, from
   `printf("%s = %x\n", "(int)gsw->check_sum", ...)` in func_80401F98, this
   function's twin. The accumulator keeps its invented name here: the twin
   prints it only in func_80401FEC (as `c`), and in this one it never reaches a
   printf, so there is no witness for it. */
void set_gsw_checksum(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 n = 0x177D;

    do
    {
        sum ^= *q++;
        n--;
    } while (n >= 0);
    gsw[GSW_CHECK_SUM] = sum;
}

/* The `while (1)` plus `break` is load-bearing, not style: with a `for` or
   `while` whose test is the loop's last insn, loop.c's check_dbra_loop
   reverses this into the downcount form set_gsw_checksum has (`li 0x177D` /
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
   `(gsw + 0x82)[i]`, `(... ^ c) == 0`, u32) -- all 14 words / 6 DIFF;
   `for (i = 0; i < 0x177E; i++) c ^= *q++;` -- 13 words / 11 DIFF
   (reversed); `do { } while (++i < 0x177E)` with `*q++` -- 13 / 11;
   `for (;;)` in place of `while (1)` -- 14 / 3 (the first iteration is
   peeled); `if (++i == 0x177E) break;` -- 14 / 6 (`bne` against a
   register-held 0x177E instead of `slti`/`bnez`). `if (++i >= 0x177E) break;`
   and a `goto` loop also match.

   `c` is the accumulator's real name, not a guess: the twin's func_80401FEC
   prints it as `printf("%s = %x\n", "c", ...)` right before it prints
   `(int)gsw->check_sum`, so the debug build names both halves of this
   comparison. */
s32 check_gsw_checksum(s32 *gsw)
{
    s32 c = 0;
    s32 *q = gsw + 0x82;
    s32 i = 0;

    while (1)
    {
        c ^= *q++;
        i++;
        if (i >= 0x177E)
        {
            break;
        }
    }
    return gsw[GSW_CHECK_SUM] == c;
}

s32 write_save_file(s32 idx, u8 *buf)
{
    u8 nameA[0x20];
    u8 nameB[0x20];
    s32(*tbl)[32];
    s32 *row;
    s32 r;

    get_card_path(nameA, idx);
    get_card_path(nameB, MEM_CARD_FILE_ID_TMP);
    tbl = D_80083E98;
    row = tbl[idx];
    if (row[0] != 0)
    {
        rename(nameA, nameB);
    }
    *(s32 *)(buf + 0x204) = 0;
    set_gsw_checksum((s32 *)buf);
    r = write_card_file(nameB, buf, 0xC0, 0, row[0]);
    if (r != 0)
    {
        store_save_slot_info(idx, buf);
        refresh_card_info();
    }
    rename(nameB, nameA);
    return r;
}

s32 read_save_file(s32 idx, u8 *buf, s32 flag)
{
    u8 name[0x20];
    s32 saved;
    s32 *row;
    s32(*tbl)[32];

    get_card_path(name, idx);
    if (read_card_file(name, buf, 0xC0, 0) != 0 && check_gsw_checksum((s32 *)buf) != 0)
    {
        if (flag != 0)
        {
            saved = *(s16 *)(buf + 0x208);
            *(s16 *)(buf + 0x208) = 0;
            *(s32 *)(buf + 0x204) = 0;
            set_gsw_checksum((s32 *)buf);
            tbl = D_80083E98;
            row = tbl[idx];
            *(s16 *)((u8 *)row + 8) = 0;
            row[1] = *(s32 *)(buf + 0x204);
            write_card_file(name, (u8 *)row, 1, 4, row[0]);
            write_card_file(name, buf, 1, 0, row[0]);
            *(s16 *)(buf + 0x208) = saved;
        }
        else
        {
            *(s16 *)(buf + 0x208) = 0;
        }
    }
}

void refresh_card_info(void)
{
    s32 i = card_slot != 0;
    s32 n;

    n = count_card_files();
    card_file_count[i] = n;
    card_free_blocks[i] = count_free_blocks(n);
}

s32 get_free_blocks(void)
{
    s32 *p = card_free_blocks;

    if (card_slot != 0)
    {
        p++;
    }
    return *p;
}

s32 count_used_save_slots(void)
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

s32 erase_save_file(s32 idx)
{
    u8 name[0x20];
    s32(*tbl)[32];
    s32 v;

    get_card_path(name, idx);
    _card_wait(0);
    _card_wait(1);
    reset_hw_events();
    erase(name);
    while ((v = take_card_state()) == 0)
    {
        VSync(0);
    }
    if (idx < 5)
    {
        tbl = D_80083E98;
        tbl[idx][0] = 0;
        refresh_card_info();
    }
    return v == 1;
}

void erase_tmp_file(void)
{
    erase_save_file(5);
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S .data, moved here from csrc/ovl_main_80020000.c on 2026-11-10.
   The overlay's first data run, 0x80027E20..0x800280B4, partitions over the
   nine units with zero order violations; docs/overlays.md, "Rodata is per
   unit; so is data", has the argument. Definition order is address order, and
   the block sits at the END of the file on purpose: every function above it
   reaches these through main.h's declaration, and a definition earlier in the
   file would complete the type at those sites and could move codegen.
   --------------------------------------------------------------------------- */

/* 0x80027E20 */
/* The 8-frame animation script reached through D_80028074[1]; the last
   step is the op-1 jump back to the top. Nothing names it directly. */
UnkAnimStep D_80027E20[9] = {
    {0x6, 2, D_80028448},
    {0x4, 2, D_80028454},
    {0x2, 2, D_80028460},
    {0x4, 2, D_8002846C},
    {0x6, 2, D_80028478},
    {0x4, 2, D_80028484},
    {0x2, 2, D_80028490},
    {0x4, 2, D_8002849C},
    {0x0, 1, D_80027E20},
};

/* 0x80027E68 */
/* The 1-frame script reached through D_80028074[0], and handed to
   func_8004CBFC directly by func_80026B18 and func_80026CF0. */
UnkAnimStep D_80027E68[2] = {
    {0x2, 2, D_80028448},
    {0x0, 0, D_80027E68},
};

/* 0x80027E78 */
/* Five 16-entry 5551 CLUTs, one 0x20-byte row each; set_save_icon copies
   the selected row into its primitive at +0x60 as an UnkBlock20. */
u8 SAVE_ICON_CLUTS[5][0x20] = {{0x00, 0x00, 0x5A, 0x6F, 0xDF, 0x53, 0x3A, 0x26, 0x04, 0x6A, 0x04, 0x55, 0xE4, 0x3C, 0x85, 0x20,
                                0xCC, 0x10, 0x3F, 0x1F, 0x9A, 0x11, 0x34, 0x15, 0x73, 0x5A, 0x93, 0x7F, 0x30, 0x15, 0x66, 0x0C},
                               {0x00, 0x00, 0xFF, 0x7F, 0xFB, 0x5B, 0xD6, 0x47, 0x2D, 0x43, 0x49, 0x2E, 0x46, 0x19, 0x82, 0x0C,
                                0x9F, 0x57, 0xBF, 0x46, 0x1D, 0x1D, 0xD7, 0x62, 0xA6, 0x1C, 0x4C, 0x31, 0x65, 0x14, 0x43, 0x0C},
                               {0x00, 0x00, 0x9C, 0x73, 0xDD, 0x5A, 0x7A, 0x49, 0x1E, 0x27, 0x97, 0x15, 0xF3, 0x51, 0x67, 0x14,
                                0xAF, 0x10, 0x73, 0x37, 0x89, 0x2E, 0x86, 0x25, 0xC4, 0x10, 0xDD, 0x4F, 0x94, 0x30, 0x68, 0x18},
                               {0x00, 0x00, 0xFD, 0x6B, 0xFA, 0x52, 0x56, 0x7F, 0x6E, 0x72, 0x4F, 0x6D, 0xEA, 0x58, 0x84, 0x1C,
                                0xDF, 0x76, 0x7C, 0x51, 0x90, 0x38, 0x69, 0x18, 0x8C, 0x24, 0x34, 0x3E, 0x6E, 0x29, 0xC7, 0x14},
                               {0x00, 0x00, 0x6D, 0x20, 0x93, 0x3C, 0xD9, 0x58, 0x9A, 0x5D, 0x5B, 0x66, 0x2D, 0x09, 0xB4, 0x05,
                                0x5B, 0x02, 0xDB, 0x0A, 0x7B, 0x17, 0x42, 0x08, 0x29, 0x25, 0x52, 0x4A, 0x9C, 0x6F, 0x1F, 0x00}};

/* 0x80027F18 */
const char *MEM_CARD_NAMES[2] = {
    MEM_CARD0,
    MEM_CARD1,
};

/* 0x80027F20 */
char MEM_CARD_PREFIX[16] = "BASLUS-00614";

/* 0x80027F30 */
const char *MEM_CARD_FILENAMES[6] = {
    D_8002009C, // 00
    D_80020098, // 01
    D_80020094, // 02
    D_80020090, // 03
    D_8002008C, // 04
    D_80020088, // TMP
};

/* 0x80027F48 */
/* The 24-byte primitive template func_80022774 and card_mes.c's
   func_80022B18 copy into place; see UnkBlock24 in main.h. */
UnkBlock24 D_80027F48 = {{0x00, 0x38, 0x00, 0x00, 0xbc, 0xbc, 0x9c, 0x00, 0x00, 0x00, 0xa0, 0x70, 0xbc, 0xbc, 0x9c, 0x00,
                          0xbc, 0xbc, 0x9c, 0x00, 0xbc, 0xbc, 0x9c, 0x00}};
