#include "common.h"
#include "main_twin.h"

/* Data objects this unit's own initialisers point at but another
   unit defines; main_twin.h does not declare them. */
extern u8 D_804091A0[0xC];
extern u8 D_804091AC[0xC];
extern u8 D_804091B8[0xC];
extern u8 D_804091C4[0xC];
extern u8 D_804091D0[0xC];
extern u8 D_804091DC[0xC];
extern u8 D_804091E8[0xC];
extern u8 D_804091F4[0xC];
extern u8 D_80409200[0xC];
extern u8 D_8040920C[0xC];
extern u8 D_80409218[0x14];

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern void func_80400908(void);
extern void func_80400EF8(void);
extern void func_804023C4(void);
extern void func_804031E4(s32 a, s32 p);
extern void func_80403324(s32 a, s32 p);
extern void func_8040339C(s32 a, s32 p);
extern void func_80404688(s32 *p);

/* THE 0x80400000 DEBUG TWIN -- cardinfo.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x80400738..0x804007A4   .text from 0x80404EA0

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main/debug/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */


/* --- cardinfo.c's assert pair. --- */
/* 0x80400738 */
const char D_80400738[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400760 */
const char D_80400760[12] __attribute__((section(".rodata"))) = "cardinfo.c";

/* 0x8040076C */
const UnkTwinDispatch4 D_8040076C __attribute__((section(".rodata"))) = {{func_80405D54, func_80406150, func_80406274, func_80405D54}};

/* 0x8040077C  the card-info header template func_80406844 memcpy's,
   0x27 bytes of it; u16 because the copy is 2-aligned */
const u16 D_8040077C[20] __attribute__((section(".rodata"))) = {
    0x4183, 0x5583, 0x5B81, 0x8983, 0x4383, 0x7483, 0x4581, 0x4183, 0x5583, 0x5B81,
    0x6883, 0x8A83, 0x5B81, 0x8083, 0x5883, 0x4081, 0x6D82, 0x6E82, 0x4481, 0x0C00};


/* Live func_80024818's sibling, cut down.  LANDED 2026-11-13 at 320 of 320
 * words with a three-part DEVICE on each of the three constant divisions;
 * without it this function is 322 words and 154 differing.  The two findings
 * that stood for three sessions were both real -- an unsigned division's
 * magic with bit 31 set is a CONST_DOUBLE and never splits, and the copy out
 * of `hi` is a reload insn that can never be the consumer's register -- and
 * both are BYPASSED rather than fixed, by never letting the compiler expand
 * the division at all.  docs/matching-levers.md carries the three levers;
 * the device comments below carry the mechanism and the measurements.
 */
void func_80404EA0(s32 *p)
{
    u8 buf[0x40] __attribute__((aligned(4)));
    u8 num[0x10];
    u8 tim[8];
    UnkCardPair *e;
    s32 i;
    s32 m;
    u8 *s;

    e = ((UnkTwinGrpAEC *)p)->ptrs[4];
    func_8003830C(p[31] + 1, buf);
    func_8007BF50((char *)buf, D_80400660);
    func_8007BF50((char *)buf, (char *)&D_8009DDD8[p[31]][3]);
    func_80051B50((u8 *)p + 0x80, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x80);
    e->unk_04->unk_08 = 0xA9;
    e->unk_04->unk_0A = 0x110;
    e = ((UnkTwinGrpAEC *)p)->ptrs[5];
    memcpy(buf, D_80400668, 0x2A);
    func_80051B50((u8 *)p + 0x188, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x188);
    e->unk_04->unk_08 = 0xBC;
    e->unk_04->unk_0A = 0x110;
    if (D_8009DDD8[p[31]][2] != 0)
    {
        e = ((UnkTwinGrpAEC *)p)->ptrs[6];
        func_8007BF80((char *)buf, (char *)D_80408CF4);
        func_8003830C(D_8009DDD8[p[31]][13], num);
        func_8007BF50((char *)buf, (char *)num);
        func_8007BF50((char *)buf, D_804006AC);
        func_80051B50((u8 *)p + 0x290, buf, 1);
        e->unk_00 = (s32)((u8 *)p + 0x290);
        e->unk_04->unk_08 = 0xBC;
        e->unk_04->unk_0A = 0x140;
    }
    e = ((UnkTwinGrpAEC *)p)->ptrs[7];
    func_80051804(D_8009DDD8[p[31]][9], 0xA, buf);
    func_80051900(buf);
    func_80051520((u8 *)p + 0x398, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x398);
    e->unk_04->unk_08 = 0x140;
    e->unk_04->unk_0A = 0x110;
    e = ((UnkTwinGrpAEC *)p)->ptrs[8];
    func_80051898(D_8009DDD8[p[31]][7], buf);
    {
        /* DEVICE: the three constructs below are one device, applied to each
         * of this function's three constant divisions.  They buy the last
         * 2 words and all 22 register names: 322/154 without them, 320/0
         * with.  See the park header's 2026-11-13 FOURTH PASS section for the
         * mechanism and for the spellings ruled out.
         *
         *   1. `(u32)<u64 product>` asks for the LOW 32 bits in C and comes
         *      out as ONE `mfhi` -- the HIGH 32 bits.  The lowpart subreg
         *      reaches output_move_single (mips.c:937) with SUBREG_WORD 0;
         *      that function builds the mnemonic from the NAME of
         *      REGNO+subreg_word (mips.c:953, `ret = "mf%1\t%0"`), and word 0
         *      of a DImode in MD_REGS is regno 64, whose REGISTER_NAMES entry
         *      (mips.h:3151) is "hi".  This target is WORDS_BIG_ENDIAN
         *      (mips.h:981), where the lowpart of a two-word value is word 1,
         *      so the C and the emitted instruction disagree: THIS STATEMENT
         *      COMPUTES THE WRONG VALUE AND EMITS THE RIGHT ONE.  That is how
         *      a C-level magic constant reaches a real `multu` and splits
         *      into lui/ori, which the compiler's own division cannot do (its
         *      magic is a CONST_DOUBLE -- FINDING 1).  Writing the C-correct
         *      half instead, `(u32)(q >> 32)`, goes through
         *      output_move_double and costs TWO DEAD WORDS per multiply --
         *      `mflo` and a zeroing `move`, each one half of a single insn's
         *      two-word template (the DImode copy at mips.c:1352 and
         *      lshrdi3_internal2 at mips.md:4208), so no pass can delete
         *      either: 326 words, 155 differing.
         *   2. the `"memory"` barrier stops combine folding the truncation
         *      back into umulsidi3_internal, which simplifies to mulsi3 and
         *      gives `mult`/`mflo` (320 words, 13 differing).  A plain
         *      asm volatile("") does not block it.
         *   3. `__asm__("" ::"r"(h))` stops combine folding the same move
         *      FORWARD into the `srl`; folded, the subreg reaches reload and
         *      comes back as `mfhi $t3 / srl $a0,$t3,11` out of the
         *      function's one spill register (320 words, 22 differing).
         *
         * Both asms are zero-length (#APP/#NO_APP), so they cost no words.
         * Neither warns under -Wall; nothing here reads an uninitialised
         * local.  Without the device this function is 322 words.
         */
        u32 k0 = 0x026D60DDu;
        unsigned long long q0 = (unsigned long long)(D_8009DDD8_u[p[31]][7] >> 6) * (unsigned long long)k0;
        u32 h0;

        __asm__ __volatile__("" ::: "memory");
        h0 = (u32)q0;
        __asm__("" ::"r"(h0));
        h0 >>= 5;
        func_80051804(h0, 3, buf);
    }
    func_8007BF50((char *)buf, D_804006BC);
    {
        u32 k = 0x91A2B3C5u;
        unsigned long long q = (unsigned long long)D_8009DDD8_u[p[31]][7] * (unsigned long long)k;
        unsigned long long r;
        /* DEVICE: pinning `a` to $a0.  local-alloc's combine_regs
         * (local-alloc.c:1808) records $a0 in qty_phys_sugg for BOTH dying
         * inputs of `subu $a0,$a0,$v0` and returns 0, so the operand loop
         * does not break and `a` and the `b * 3600` temp chain both want it;
         * qty_sugg_compare_1 then orders that group by qty_compare_1, where
         * the chain (one tied quantity, 10 refs / 6 insns = 25000) beats `a`
         * (6 refs / 17 insns = 3529) and takes $a0, pushing `a` to $a3.
         * Taking `a` out of the quantity pool leaves the chain $v0 and `b`
         * $v1, which is the target.  Measured equivalent without the pin:
         * 24 extra zero-word references to `a`, as three
         * __asm__("" ::"r"(a), ... x8) insns, raise it to 4*30/40 = 30000 and
         * also reach 320/0; 18 refs gives 320/9 and 16 gives 320/9.
         */
        register u32 a __asm__("$4");
        u32 b;

        __asm__ __volatile__("" ::: "memory");
        a = (u32)q;
        __asm__("" ::"r"(a));
        a >>= 11;
        r = (unsigned long long)a * (unsigned long long)k;
        __asm__ __volatile__("" ::: "memory");
        b = (u32)r;
        __asm__("" ::"r"(b));
        b >>= 11;
        func_8005184C(a - b * 3600, 2, tim);
    }
    func_8007BF50((char *)buf, (char *)tim);
    func_8007BF50((char *)buf, D_804006BC);
    {
        u32 k = 0x88888889u;
        unsigned long long q = (unsigned long long)D_8009DDD8_u[p[31]][7] * (unsigned long long)k;
        unsigned long long r;
        /* DEVICE: pinning `a` to $a0.  local-alloc's combine_regs
         * (local-alloc.c:1808) records $a0 in qty_phys_sugg for BOTH dying
         * inputs of `subu $a0,$a0,$v0` and returns 0, so the operand loop
         * does not break and `a` and the `b * 3600` temp chain both want it;
         * qty_sugg_compare_1 then orders that group by qty_compare_1, where
         * the chain (one tied quantity, 10 refs / 6 insns = 25000) beats `a`
         * (6 refs / 17 insns = 3529) and takes $a0, pushing `a` to $a3.
         * Taking `a` out of the quantity pool leaves the chain $v0 and `b`
         * $v1, which is the target.  Measured equivalent without the pin:
         * 24 extra zero-word references to `a`, as three
         * __asm__("" ::"r"(a), ... x8) insns, raise it to 4*30/40 = 30000 and
         * also reach 320/0; 18 refs gives 320/9 and 16 gives 320/9.
         */
        register u32 a __asm__("$4");
        u32 b;

        __asm__ __volatile__("" ::: "memory");
        a = (u32)q;
        __asm__("" ::"r"(a));
        a >>= 5;
        r = (unsigned long long)a * (unsigned long long)k;
        __asm__ __volatile__("" ::: "memory");
        b = (u32)r;
        __asm__("" ::"r"(b));
        b >>= 5;
        func_8005184C(a - b * 60, 2, tim);
    }
    func_8007BF50((char *)buf, (char *)tim);
    func_80051900(buf);
    func_80051520((u8 *)p + 0x4A0, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x4A0);
    e->unk_04->unk_08 = 0x147;
    e->unk_04->unk_0A = 0x120;
    e = ((UnkTwinGrpAEC *)p)->ptrs[9];
    func_80051804(D_8009DDD8[p[31]][11], 4, buf);
    func_80051900(buf);
    func_80051520((u8 *)p + 0x5A8, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x5A8);
    e->unk_04->unk_08 = 0xF2;
    e->unk_04->unk_0A = 0x120;
    e = ((UnkTwinGrpAEC *)p)->ptrs[10];
    memcpy(buf, D_804006C0, 3);
    func_80051900(buf);
    func_80051520((u8 *)p + 0x6B0, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x6B0);
    e->unk_04->unk_08 = 0x180;
    e->unk_04->unk_0A = 0x130;
    for (i = 0; i < 10; i++)
    {
        s32 f = D_8009DDD8[p[31]][12];

        e = ((UnkTwinGrpAEC *)p)->ptrs[11 + i];
        m = 1 << i;
        if (m & f)
        {
            s = D_80408CFC[i];
        }
        else
        {
            s = D_80408CFC[10];
        }
        e->unk_00 = (s32)s;
        e->unk_04->unk_08 = 0x38 + 0x10 * i;
        e->unk_04->unk_0A = 0xB0;
    }
}


/* Live func_80024818, cut down: no `k`, no count_used_save_slots arm, and
 * `strcpy(buf, num); strcat(buf, D_800283FC)` replaced by a five-byte seed
 * block-moved into buf followed by two strcats.  LANDED 2026-11-13 at 82 of
 * 82 words, the last function in the project, with a three-part DEVICE on
 * the `n / 3`; without it this is 82 words and 3 differing
 * (`sra $a0 / mfhi $a3 / subu $a0,$a3,$a0` against the target's
 * `sra $v0 / mfhi $a0 / subu $a0,$a0,$v0`), where it stood for three passes.
 * The device comment below carries the mechanism and the measurements.  The
 * park's `u8 *b; b = buf;` after the first call is gone: with the division
 * device in place `buf` used directly is 82/0 as well.
 *
 * The seed is ONE `movstrsi`: `memcpy` of a constant 5 bytes from a
 * 4-aligned symbol (main_twin.h declares D_80400714 as `const s32 []` for
 * exactly this) expands through mips.c's block_move into a single insn, so
 * it schedules as one atom and its byte step is `lb`.  Two explicit stores
 * are 9 differing; a 1-aligned source is 84 words.
 */
void func_804053A0(s32 *p)
{
    u8 buf[0x40];
    u8 num[0x10];
    s32 n;
    s32 i;

    n = func_80402268();
    if (n < 3)
    {
        func_80051B50((u8 *)p + 0x80, D_804006C4, 1);
        func_80051B50((u8 *)p + 0x188, D_804006EC, 1);
    }
    else
    {
        /* DEVICE: `k`, the zero-length asm that reads it, and `hi` pinned to
         * $a0 are ONE device.  Together they compute exactly `n / 3` --
         * `hi` is the magic multiply's high word, `hi - (n >> 31)` is gcc's
         * own expansion of the quotient, and the asm template is empty -- and
         * they buy the function's last three words: 82/3 without, 82/0 with.
         * The target has the multiply's highpart ALLOCATED to $a0 and the
         * 0x55555556 multiplier in $v0; plain `n / 3` leaves the highpart in
         * HI (regclass ties HI_REG against GR_REGS at 6 each and
         * reg_class_subunion resolves the tie to HI_REG), so its `mfhi` is a
         * reload into the lowest unused register, $a3.
         *
         *   1. `register s32 hi __asm__("$4")` with `hi = n / 3 + (n >> 31)`.
         *      combine cancels the `+` against the division's own sign
         *      correction, leaving smulsi3_highpart with the hard register
         *      as its destination; recog checks predicates, not the "=h"
         *      constraint, so reload spills HI (`Spilling reg 64.` in .greg)
         *      and the `mfhi $a0` is that insn's output reload.  Alone this
         *      is 82/10: local-alloc's combine_regs hands the multiplier
         *      pseudo `qty_phys_sugg = {$a0}` off the same insn, and since
         *      the multiplier dies where $a0 is born, find_free_reg's
         *      half-open `[born_index, dead_index)` scan never sees $a0 live
         *      and the constant follows the highpart into $a0.
         *   2. `s32 k = 0x55555556` NAMES THE DIVISION'S OWN MULTIPLIER.
         *      expand_divmod loads the magic into a fresh pseudo; cse1 finds
         *      `k` already holding the same constant, rewrites the
         *      smulsi3_highpart to read `k` (canon_reg substitutes the
         *      equivalence class's first register, and a block-local
         *      newcomer does not displace `k` there) and deletes the second
         *      load.  .cse shows one `(set (reg/v k) (const_int 1431655766))`
         *      and `(sign_extend:DI (reg/v k))` in the multiply.  It must be
         *      a plain local set BEFORE the division: `register s32 k
         *      __asm__("$2")` is not adopted (make_regs_eqv puts a pseudo
         *      ahead of a non-fixed hard register as the class's first
         *      member) and materialises the constant twice, 84 words and 63
         *      differing;
         *      assigned after the division, 84/64.  On its own it changes
         *      nothing -- with part 1 and no part 3 the bytes are those of
         *      part 1 alone, 82/10.
         *   3. `__asm__("" ::"r"(k))` straight after the division is a use of
         *      the multiplier AFTER the multiply, which is the one thing the
         *      suggestion in part 1 cannot survive: `k` now dies at the asm,
         *      one insn past the `mult`, so $a0 is live inside its range,
         *      find_free_reg strikes $a0 from the suggested set, and `k`
         *      falls to the lowest free register, $v0.  The asm is not
         *      volatile and has no output, so it is not a scheduling barrier
         *      (`__volatile__`: 82/12), and it emits only #APP/#NO_APP.
         *
         * ORDER IS PART OF IT.  The memcpy must follow the asm and precede
         * the call: before the division, or with the asm after it, `k` is
         * live across the block move and takes $v0 from the move's scratch
         * registers (82/6 both ways); after the call, 82/12.
         *
         * WARNS UNDER -Wall, spuriously: "`hi' might be used uninitialized".
         * flow's regno_uninitialized only asks whether the register is live
         * at the top of block 0, and $4 is -- it carries the incoming `p`.
         * `hi` is assigned by the statement before its only read.  The
         * default CFLAGS has no -Wall; do not "fix" it with an initialiser.
         *
         * Ruled out, all measured against main/debug/cardinfo: parts 2+3
         * without the pin, 82/3; the pin on `hi = n / 3` with no
         * cancellation, 82/3 (the hard register is then the subtract's
         * destination and the highpart is a pseudo again); the
         * `do { } while (0)` loop_cost route to a GPR highpart with parts
         * 2+3, 82/13.  func_80404EA0's device above -- mulsidi3, a "memory"
         * barrier and `(s32)<product>` -- also reaches 82/0 here with `hi`
         * pinned, but it reads the C LOW half and relies on the compiler
         * naming HI for it; this one needs no wrong value, so it was
         * preferred.  `const s32 k` and a named `s32 sgn = n >> 31` are both
         * 82/0 as well.  Scores are verify.py's, taken while this was a park.
         */
        s32 k = 0x55555556;
        register s32 hi __asm__("$4");

        hi = n / 3 + (n >> 31);
        __asm__("" ::"r"(k));
        memcpy(buf, D_80400714, 5);
        func_8003830C(hi - (n >> 31), num);
        func_8007BF50(buf, num);
        func_8007BF50(buf, D_8040071C);
        func_80051B50((u8 *)p + 0x80, buf, 1);
    }
    i = 0;
    /* DEVICE: the bound is the TEST, written as two comparisons.  The target
     * recomputes `slti $v0,$s3,3` inside the loop, so there is no count
     * local (the live source's `cnt`, set in both arms, is 73 words); and
     * `for (i = 0; i < (n < 3 ? 2 : 1); i++)` carries `i` and `i + 1` in two
     * pseudos: 84 words and 37 differing. */
    while (n < 3 ? i < 2 : i < 1)
    {
        /* Through the group struct, as the live func_80024818 writes it: a
         * flat `p[0x2C6 + i]` is a three-term sum and commutes the `addu`,
         * 82/1. */
        UnkCardPair *e = ((UnkTwinGrpAEC *)p)->ptrs[4 + i];

        e->unk_00 = (s32)((u8 *)p + 0x80 + 0x108 * i);
        e->unk_04->unk_08 = 0xA9;
        e->unk_04->unk_0A = 0x110 + 0x10 * i;
        i++;
    }
}

/* The live func_80024A50 has a second test on p[0x20] and a third callee in
 * the else; this build has neither. */
void func_804054E8(s32 *p)
{
    s32 n = p[0x1F];

    if (D_8009DDD8[n][0] != 0)
    {
        func_80404EA0(p);
    }
    else
    {
        func_804053A0(p);
    }
}

void func_8040553C(s32 *p)
{
    UnkCardPair *q;

    func_80404DEC((u8 *)(p + 1), 0xF8, 0x48);
    q = (UnkCardPair *)p[0x2C5];
    q->unk_04->unk_08 = 0xA0;
    q->unk_04->unk_0A = 0xB0;
    q->unk_00 = (s32)(p + 1);
    q = (UnkCardPair *)p[0x2C2];
    q->unk_04->unk_08 = 0x60;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_800841DC;
    q = (UnkCardPair *)p[0x2C3];
    q->unk_04->unk_08 = 0xD0;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_8008423C;
    q = (UnkCardPair *)p[0x2C4];
    q->unk_04->unk_08 = 0x20;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_8008429C;
}

void func_80405624(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

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

/* RETURNS: func_804057F0 stores $v0 straight after the call. */
s32 func_804056CC(s32 a)
{
    return func_80047358(a, D_80408D28, 0x15);
}

/* Live func_80024C70, the UnkTwinGrpAEC instance of the five-way template builder. The
 * group is 0xAEC bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_804056F4(UnkTwinGrpAEC *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_804057A8(s32 *p, s32 v)
{
    p[0] = v;
}

void func_804057B0(s32 *p, s32 v)
{
    func_804057A8(p, v);
    func_80405624((UnkCardPair *)(p + 0x2BE), (u32 *)(p + 0x2B4), (u32 *)(p + 0x2B8));
    func_8040553C(p);
}

/* Live func_80024D58. TWO arguments here, not three -- there is no r[0x20]
 * store -- and the allocation failure is an ASSERT this build carries and the
 * live one does not (D_80400738/D_80400760 are this translation unit's own
 * copies of the format and the filename, not c_server.c's D_80400004 pair).
 * The group is spelled in raw word indices, as func_804057B0 and
 * func_80405900 beside it are: root at r[0x2BE], its unk_0C at r[0x2C1],
 * ptrs at r[0x2C2]. */
s32 *func_804057F0(s32 *p, s32 a)
{
    s32 *q = (s32 *)D_8040B4F8;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040B4F8, 0x387);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_80400738, D_80400760, 0x1A0);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_804059A8(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0xDFC);
    }
    r[0x1F] = a;
    func_804056F4((UnkTwinGrpAEC *)r, 0x15);
    q[3] = (s32)(r + 0x2BE);
    r[0x2C1] = func_804056CC((s32)(r + 0x2C2));
    func_804057B0(r, (s32)p);
    q[4] = 0;
    return q;
}

void func_80405900(s32 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_80051FBC(*(s32 *)p[0x2C6 + i]);
        *(s32 *)p[0x2C6 + i] = 0;
    }
    for (i = 0; i < 4; i++)
    {
        *(s32 *)p[0x2C9 + i] = 0;
    }
    for (i = 0; i < 10; i++)
    {
        *(s32 *)p[0x2CD + i] = 0;
    }
}

void func_804059A8(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80405900((UnkCardGrpB20 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

void func_80405A00(s32 *p, s32 a)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[0x1F] = a;
        func_80405900((UnkCardGrpB20 *)q);
        func_804054E8(q);
    }
}

void func_80405A3C(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_80405A64(s32 *p)
{
    func_80405A3C(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_80406368;
    }
}

void func_80405AB4(s32 *p)
{
    func_80405AE8(p);
    p[-4] = (s32)func_80406368;
}

/* main.bin's 0x80400000 twin, live counterpart func_80025030.
 *
 * THE FUNCTION THE SECOND COMPILER WAS WORTH. Under the 4.1 cc1 it sat at
 * exactly one differing word for a week, parked, with its own note saying it
 * would land unchanged the day a cc1psx without reload_cse_regs existed. It
 * did, and it did -- not one character of the body changed.
 *
 * Two levers took it from 20 differing to 1, and both are already landed on
 * its sibling func_80405B98:
 *
 *   1. func_80405A00 TAKES TWO ARGUMENTS in this build where the live
 *      func_80024F3C takes three. Its listing reads $a0 and $a1 and never
 *      $a2.
 *   2. THE INCREMENT ORDER IS THE DELAY SLOTS. `i++` before `off++`, with a
 *      plain `while (i < 5)`, puts the counter increment in the two `j`
 *      delay slots and pushes the pointer increment into the `bnez`'s. The
 *      live spelling -- `off++;` then `while (++i < 5)` -- is 19 differing.
 *
 * What is left is word 10:
 *
 *     candidate  move  $s3, $v0          target  addiu $s3, $zero, 0x1
 *
 * `flag = 1` in the arm whose test already put 1 in $v0. 4.1's
 * reload_cse_simplify_set rewrites the constant to a copy of that register;
 * FSF 2.7.2 has no such pass, so it materialises, which is what the target
 * does. It could never have been spelled around -- the TARGET also carries
 * `addiu $v0, $zero, 0x1` for the comparison, so the register genuinely
 * holds 1 there and 4.1's substitution is unconditional.
 *
 * MEASURE IT WITH `ovlverify.py --G 8`, never the default -G0.
 */
void func_80405AE8(s32 *p)
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
        func_80403D7C((s32 *)p[6], p[0x11]);
        flag = 0;
    }
    off = 0;
    i = 0;
    do
    {
        if (i == p[10])
        {
            func_804045C8((s32 *)p[off + 1], flag);
        }
        else
        {
            func_80404668((s32 *)p[off + 1]);
        }
        i++;
        off++;
    } while (i < 5);
    func_80405A00((s32 *)p[7], p[10]);
}


/* Live func_800250E8, with two differences and neither of them an index: the
 * p[10]/p[11]/p[7] reads are at the same offsets as the live ones.
 *
 * 1. func_80405A00 TAKES TWO ARGUMENTS HERE where the live func_80024F3C
 *    takes three -- its listing reads $a0 and $a1 and never $a2, which is
 *    also the 2-word length difference between the two bodies.
 * 2. THE INCREMENT ORDER IS THE DELAY SLOTS. Written the live way --
 *    `off++;` then `while (++i < 5)` -- the pointer increment fills the two
 *    `j` delay slots and the `bnez` gets a nop: 18 differing. Incrementing
 *    the counter FIRST puts `i++` in those slots and pushes `off++` into the
 *    `bnez`'s, which is what this build does. 0 differing. */
void func_80405B98(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_80404618((s32 *)p[off + 1], 1);
        }
        else if (i == p[10])
        {
            func_804045C8((s32 *)p[off + 1], 1);
        }
        else
        {
            func_80404668((s32 *)p[off + 1]);
        }
        i++;
        off++;
    } while (i < 5);
    func_80405A00((s32 *)p[7], p[10]);
}

/* splat seeded this as an empty `(void)` function, which is a correct
 * spelling of its two words (`jr $ra`, `nop`) but not the source's: the
 * live counterpart func_8002519C takes a u8 *, and an empty body emits the
 * same two words either way. */
void func_80405C44(u8 *p)
{
}

/* Live func_800251A4. Three differences: D_8008CA34 and func_80400EF8 stand in
 * for its func_80027BF4, the D_80082E60 flag update and two of its calls are
 * absent, and the two absolute addresses are this link's -- 0x80011F48 for the
 * halfword store and 0x80010208 for the word read, where the live pair is
 * 0x80013714 and 0x80010208 and the read is a HALFWORD. */
void func_80405C4C(s32 *p)
{
    s32 v;

    D_8008CA34 = 2;
    func_80400EF8();
    v = 1;
    if (p[0x11] == 0)
    {
        v = 3;
    }
    D_80011F48 = v;
    func_8003D548(D_80010208.unk_00 != 0 ? 5 : 4);
}

void func_80405CB8(s32 *p)
{
    if (func_80402154(p[10], (u8 *)0x80010000) == 0)
    {
        p[13] = (s32)func_80406368;
        func_8040325C((s32)(p - 8));
        p[-4] = (s32)func_80405A3C;
    }
    else
    {
        func_80405C44((u8 *)0x80010000);
        p[-4] = (s32)func_80405C4C;
    }
    func_80400908();
    p[16] = 0;
}

void func_80405D2C(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80405CB8;
    }
}

void func_80405D54(s32 *p)
{
    s32 n = p[10];

    if (D_8009DDD8[n][0] != 0)
    {
        p[15] = 1;
        func_804031E4((s32)(p - 8), (s32)(p + 16));
        p[16] = 1;
        p[-4] = (s32)func_80405D2C;
    }
    else
    {
        p[13] = (s32)func_80406368;
        func_804032FC((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_80025398. Four differences: func_80402154 takes two arguments
 * here, the live func_80025358 call is a plain `D_80010208.unk_00 = 0;`, the loop
 * calls func_80404688 rather than func_80404C84, and the fail arm's two
 * handlers are func_80406368 and func_80405A3C where the live one has
 * func_80024FFC twice over. */
void func_80405DE0(s32 *p)
{
    u8 *b = (u8 *)0x80010000;
    s32 i;
    s32 off;

    if (func_80402154(p[11], b) == 0)
    {
        goto fail;
    }
    D_80010208.unk_00 = 0;
    if (func_80402084(p[10], b) == 0)
    {
        goto fail;
    }
    func_80405C44(b);
    func_80402214();
    off = 0;
    for (i = 0; i < 5; i++)
    {
        func_80404688((s32 *)p[off + 1]);
        off++;
    }
    func_80405AB4(p);
    goto done;
fail:
    p[13] = (s32)func_80406368;
    func_8040334C((s32)(p - 8));
    p[-4] = (s32)func_80405A3C;
done:
    func_80400908();
    p[16] = 0;
}

void func_80405EAC(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80405DE0;
    }
}

void func_80405ED4(s32 *p)
{
    p[15] = 1;
    func_80403324((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80405EAC;
}

/* Live func_800254E4. REVISION, and the MIRROR of the one func_804065CC
 * carries: here the twin has TWO handlers where the live source has one.
 * func_80024FFC is assigned at both `p[13]` sites live; this build assigns
 * func_80406368 in the not-enough-blocks arm and func_80405AB4 in the
 * cannot-overwrite arm. That is why twinmove refuses the symbol -- the map's
 * vote is a genuine tie, one live symbol against two twin ones, and the
 * listing is the only thing that can break it. */
void func_80405F24(s32 *p)
{
    s32(*tbl)[32] = D_8009DDD8;

    if (tbl[p[10]][0] == 0)
    {
        if (func_80402268() < 3)
        {
            p[13] = (s32)func_80406368;
            func_80403234((s32)(p - 8));
            p[-4] = (s32)func_80405A64;
        }
        else
        {
            func_80405ED4(p);
        }
    }
    else
    {
        p[13] = (s32)func_80405AB4;
        func_80403374((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_80025584. D_801379A8/D_801379B0 are this link's two input words,
 * read as plain globals where the live source has D_80083160.unk_08/unk_10;
 * the 0x40 and 0x20 arms are the other way round; and the tail calls
 * func_804083FC with a literal 0 where the live one passes p[9]. */
void func_80405FC4(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            p[-4] = (s32)func_80405AB4;
        }
        if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            func_80405F24(p);
        }
        else if (D_801379A8.unk_00 & 0x5000)
        {
            if (D_801379B0.unk_00 & 0x5000)
            {
                p[12] = 0;
                if (D_801379B0.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379B0.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 2;
                if (D_801379A8.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379A8.unk_00 & 0x4000)
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
            func_80063FF8(0x502);
            p[10] = func_80058FF0(p[10], dir, 5);
            func_80405B98(p);
        }
    }
    r = func_804016D0();
    func_804018FC();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
    func_80406720(p - 8);
    func_804083FC(p[8], 0);
}

/* Live func_8002570C. REVISION: the live true-arm calls func_80024F3C's
 * sibling between the two stores; this build has no call there at all, just
 * `p[11] = n` and the handler. */
void func_80406150(s32 *p)
{
    s32 n = p[10];

    if (D_8009DDD8[n][0] != 0)
    {
        p[11] = n;
        p[-4] = (s32)func_80405FC4;
    }
    else
    {
        p[13] = (s32)func_80406368;
        func_804032FC((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_80025788, with func_80404688 in the loop where it has
 * func_80404C84, func_80405A64 as the else arm's handler where it has
 * func_80405A3C, and no `func_80400908(); p[16] = 0;` tail. */
void func_804061C4(s32 *p)
{
    s32 i;
    s32 off;

    if (p[14] != 0)
    {
        p[-4] = (s32)func_80406368;
    }
    else
    {
        if (func_804022C8(p[10]) != 0)
        {
            p[-4] = (s32)func_80406368;
            off = 0;
            for (i = 0; i < 5; i++)
            {
                func_80404688((s32 *)p[off + 1]);
                off++;
            }
        }
        else
        {
            p[13] = (s32)func_80406368;
            func_804033C4((s32)(p - 8));
            p[-4] = (s32)func_80405A64;
        }
    }
}

/* Live func_80025848, MOVED OVER UNCHANGED except for the four symbols. The
 * other half of the folded-indexed-load pair with func_80404DEC: under 4.1's
 * compiler two levers took it 32 -> 5 and the five were entirely
 * `D_8009DDD8[n][0]`, which came out as a split address plus an `addu` plus a
 * `lw 0(...)` where the target has the assembler's three-word expansion of
 * one insn. tools/psx272gcc folds it, and the live body matches with no
 * device. */
void func_80406274(s32 *p)
{
    s32 n = p[10];

    if (D_8009DDD8[n][0] != 0)
    {
        p[13] = (s32)func_804061C4;
        func_8040339C((s32)(p - 8), (s32)(p + 14));
        p[-4] = (s32)func_80405A64;
    }
    else
    {
        p[13] = (s32)func_80406368;
        func_804032FC((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_800258C8. The source is `void (*fns[4])(s32 *) = {...}` -- written
 * that way gcc emits the template into this unit's .rodata a SECOND time and
 * the container comes out 0x10 long, because the unit has one dotted
 * `.rodata` subsegment and this template is 0x6DC further on, inside a
 * hand-owned blob. Copying the blob's own object instead produces the same
 * four lw/sw pairs and emits nothing; the declaration in main_twin.h carries
 * the rest of the note.
 *
 * It also remains true that the twin block is several translation units like
 * the live side's nine, and that our single `c` subsegment is an
 * approximation that works while the asm rodata blobs fill the gaps. This
 * function was the one piece of evidence for that and it no longer forces
 * the carve. */
void func_804062F4(s32 *p)
{
    UnkTwinDispatch4 tbl = D_8040076C;

    tbl.f[p[0x11]](p);
}

void func_80406358(s32 *p)
{
    p[18] = 1;
    p[19] = 1;
}

/* Live func_8002593C. The 0x40 and 0x20 arms are the other way round, the
 * counter loses 4 rather than 2, and the second func_80058FF0 call takes 4
 * where the live one takes 3. */
void func_80406368(s32 *p)
{
    s32 dir = 0;
    s32 upd = 0;
    s32 r;

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            func_80406720(p - 8);
            func_804083FC(p[8], 0);
            return;
        }
        if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            if (p[18] == 0)
            {
                func_80406358(p);
                upd = 1;
            }
            else
            {
                func_804062F4(p);
            }
        }
        else if (D_801379A8.unk_00 & 0x5000)
        {
            if (D_801379B0.unk_00 & 0x5000)
            {
                p[12] = 0;
                if (D_801379B0.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379B0.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_801379A8.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379A8.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        else if (D_801379A8.unk_00 & 0xA000)
        {
            if (D_801379A8.unk_00 & 0x2000)
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
            func_80063FF8(0x502);
            if (p[18] == 1)
            {
                p[10] = func_80058FF0(p[10], dir, 5);
            }
            else
            {
                p[17] = func_80058FF0(p[17], dir, 4);
            }
            func_80405AE8(p);
        }
    }
    r = func_804016D0();
    func_804018FC();
    if (r != 0)
    {
        if (r != 1)
        {
            func_80406720(p - 8);
            func_804083FC(p[8], 0);
        }
    }
    if (upd != 0)
    {
        func_80405AE8(p);
    }
}

void func_80406568(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80406570(s32 *p, s32 v)
{
    func_80406568(p, v);
}

void func_80406590(s32 *p)
{
    func_80405AE8(p);
    func_80400908();
    p[-4] = (s32)func_80406368;
}

/* Live func_80025BD4, with one revision difference and one delay slot.
 *
 * THE REVISION DIFFERENCE: this build assigns func_80406590 at BOTH sites
 * where the live source has two separate handlers, func_80025B9C for p[13]
 * and func_80025B60 for the trailing p[-4]. Its listing names the same
 * symbol twice, so this is not a symmap miscount.
 *
 * THE DELAY SLOT is the last p[-4] only, and it was what the second compiler
 * fixed. Under 4.1's, `(s32)func_80406590` let reorg steal that address's
 * `lui` into the preceding `bne`'s slot where this build keeps a nop (11
 * differing), and a sized data alias at that one site bought it back. Under
 * tools/psx272gcc the address is one insn and the plain spelling is 0. */
void func_804065CC(s32 *p)
{
    s32 n = p[9];

    if (n < 5)
    {
        if (func_80401E2C(n) != 0)
        {
            p[n + 1] = func_80404364((s32)(p - 8), n);
            p[9] = p[9] + 1;
        }
        else
        {
            p[13] = (s32)func_80406590;
            func_8040325C((s32)(p - 8));
            p[-4] = (s32)func_80405A3C;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80406590;
    }
}

s32 *func_80406678(s32 *p, s32 a)
{
    s32 *q = func_8003C714(0, D_8040C628, 0x1C);
    s32 *r = q + 8;

    func_80406570(r, (s32)p);
    func_804023C4();
    func_80402214();
    r[0x12] = 1;
    q[8] = func_80404C84(q, q + 0x1A);
    r[6] = (s32)func_80403BC4(q);
    r[7] = (s32)func_804057F0(q, r[10]);
    r[9] = 0;
    q[4] = (s32)func_804065CC;
    return q;
}

/* TWO devices, and func_804070C4 below is the same function with three
 * different callees and the same two.
 *
 * The degenerate `do { } while (0)` sets the register assignment. cse rewrites
 * all three of `q[0]`, `q[6]` and `q[7]` as p-relative, so `q` loses the
 * references that would outrank `i`; flow.c:440 weights a reference by
 * loop_depth, and one more level over the body is enough to put them back in
 * the target's order (p in $s2, q in $s0, i in $s1).
 *
 * The empty asm sets the call's delay slot. reorg fills a CALL_INSN's slot
 * from BEHIND as well as from in front, so with `q++` written before the call
 * it takes `q++` either way; `stop_search_p` ends the backward scan at an
 * asm, and the forward scan then takes `i++`, which is the target's
 * `addiu $s0,$s0,4` / `jal` / `addiu $s1,$s1,1`. The hoisted `v` is what lets
 * `q++` precede the call at all. */
void func_80406720(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_80404DBC((u8 *)q[0]);
        func_80403D24((s32 *)q[6]);
        func_804059A8((s32 *)q[7]);
        i = 0;
        do
        {
            do
            {
                s32 *v = (s32 *)q[1];

                q++;
                __asm__("");
                func_80404570(v);
            } while (0);
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
}

void func_804067BC(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_804045C8((s32 *)p[off + 3], 1);
        }
        else
        {
            func_80404668((s32 *)p[off + 3]);
        }
        off++;
    } while (++i < 5);
    func_80405A00((s32 *)p[1], p[11]);
}

/* The second block copy's seven-byte tail is `lwl/lwr` + `lh` + `lb`, which
 * is expand_block_move with align 2 and a size of 0x27. No C TYPE can ask for
 * that pair -- a 0x27-byte struct has align 1, and raising its alignment
 * rounds the size to 0x28 -- but the alignment memcpy is handed does not come
 * from a type: get_pointer_alignment (expr.c) reads it off the pointer
 * EXPRESSION, and a cast is undone on the way (the NOP_EXPR arm re-reads the
 * inner pointer's target type), so only a pointer VARIABLE of u16 type keeps
 * it. Hence q, which every use goes through: route one of them through p and
 * the two pseudos cost a `move`. */
void func_80406844(u8 *p, s32 a, s32 mode)
{
    u8 buf[0x10];
    u16 *q = (u16 *)p;

    *(UnkTwinBlk200 *)q = D_80408EC8;
    memcpy(q + 2, D_8040077C, 0x27);
    func_8003830C(a + 1, buf);
    func_8007BF50((u8 *)q + 4, buf);
    D_80010208.unk_00 = mode == 2;
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S `.data`, 0x80408CF4..0x80408D40.

   The twin's data is two runs: the TABLES, 0x804089E0..0x8040922C, which the
   nine units own, and the STATE run behind it, which one object has to hold
   because one object contributes one `.data` run. That is the same shape and
   the same forced split as the live 0x80020000 overlay -- docs/overlays.md,
   "The data IS carved, and a unit can only own the FIRST data run".

   Ownership is by DEFINITION, not by use: link order makes a unit's objects
   contiguous and in unit order, so the referencing units read down the
   address list come out non-decreasing, and where they do not, the object is
   merely USED by the later unit. Applying this partition moves no bytes, so
   byte-identity cannot check it -- it is a claim about the original source.
   ------------------------------------------------------------------------- */

/* 0x80408CF4 */
/* Live D_800283A8[2] and [3]: two separate pointer words into the
   rodata string pool. Only the first has a name the unit uses. */
u8 *D_80408CF4 __attribute__((section(".data"))) = (u8 *)0x804006A0;

/* 0x80408CF8 */
u8 *D_80408CF8 __attribute__((section(".data"))) = (u8 *)0x80400694;

/* 0x80408CFC */
/* Live D_80027FD4: eleven cel records, [10] being the "clear" one
   func_80404EA0 indexes as D_80408CFC[i]. */
u8 *D_80408CFC[11] = {
    D_804091A0,
    D_804091AC,
    D_804091B8,
    D_804091C4,
    D_804091D0,
    D_804091DC,
    D_804091E8,
    D_804091F4,
    D_80409200,
    D_8040920C,
    D_80409218,
};

/* 0x80408D28 */
/* D_80027FF8's twin. */
u8 D_80408D28[0x18] = {
    0x03, 0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x51, 0x9D};

