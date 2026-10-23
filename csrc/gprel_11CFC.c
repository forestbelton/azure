/* Carved out of unk_11940; see docs/addressing-and-translation-units.md.
 * Replace this header with the evidence for the split. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

/* maspsx will not emit the load-delay nop before a $gp-relative store of a
 * symbol it knows only from an `.extern` record. Its `_uses_gp` reads
 * `.sdata`/`.comm`/`.lcomm` and explicitly skips `.extern`, so with
 * `--dont-force-G0 -G8` -- where gas turns a small extern into a one-insn
 * %gp_rel access anyway -- the pair `lw $v0,0x4($a1)` + the store in case 6
 * silently loses its nop and the unit comes out one word short, with every
 * later branch offset off by one. This directive tells maspsx the symbol is
 * small data and DEFINES NOTHING: `readelf -sS` shows .sdata at 0 bytes and
 * D_80080ADC still UND with size 4, so there is no linker placement to arrange
 * and nothing to collide with the linker script's own `D_80080ADC = ...`.
 * Unlike the `.sdata` recipe in annotate.py's gp-relative lever there is no
 * `.globl`, no `.type`, no label and no `.word`.
 *
 * The principled fix is in vendor/maspsx: record `.extern SYM,size` and let
 * `_uses_gp` consider those symbols when gas is left at a nonzero -G, gated on
 * `--dont-force-G0` so the -G0 bulk of the game is unaffected. If that lands,
 * this line comes straight back out. */
__asm__(".sdata\n\t.size D_80080ADC,4\n\t.text");

/* The command dispatcher: turns a command byte into one or more func_8003E39C
 * queue entries and returns that call's entry, or NULL for a command it does
 * not handle.
 *
 * Three shapes are load-bearing:
 *  - the two case-0xD calls pass the LITERAL 0xD, not `kind`. With `kind`, cse
 *    rewrites the argument as the switch-index pseudo rather than folding it to
 *    13, which hands that pseudo a hard-reg COPY preference for $a0 -- and
 *    find_reg honours a copy preference ahead of excluding
 *    `regs_someone_prefers`, so the index takes $a0 and the parameter is pushed
 *    to $v1 behind an extra `move`. 93 differing words at the same word count.
 *    This is the exception to matching-levers section 3's ordering rule, and
 *    the diagnostic is a `(set (reg:SI 4 a0) (reg:SI N))` in the `.lreg` dump;
 *    the `;; N preferences: 4` line in `.greg` does not distinguish a copy
 *    preference from a plain one.
 *  - cases 0xF..0x13 are listed in the do-nothing group. They emit nothing at
 *    all, but they are one RANGE node in the case list, and a range node counts
 *    double in `balance_case_nodes`' bisect -- which moves the high subtree's
 *    root from 0x16 to 0x15, the tree the target has. 13 differing words at the
 *    same word count. Any range wholly inside 0xF..0x13 gives the same tree; a
 *    range ending at 0x14 does not.
 *  - the `func_8003E39C(0xD, ...)` call is written out in BOTH arms of the NULL
 *    test. jump2 cross-jumps the null arm's tail with case 0xE's identical
 *    `jal`/`$a1 = 0`/`j` (the non-null arm's copy is not a tail, so it
 *    survives), leaving `li $a0,0xD` alone in a block reorg then steals into
 *    the beqz's delay slot -- which is why that `addiu $a0,0xD` appears twice.
 *
 * `u8 kind` is required: it is what gives `andi $v1,$a0,0xFF` for the switch
 * index and `andi $a0,$a0,0xFF` at the three two-label call sites. The third
 * argument is forwarded to func_8003E39C only where the target leaves $a2
 * untouched at the call (0xFF, 6/0xFE, 0xD's first call, 0xE), so the
 * parameter keeps its incoming $a2 and the argument copy disappears; every
 * other call passes 0 explicitly. */
Unk80083968 *func_8003E4FC(u8 kind, u8 *arg, s32 extra) {
    switch (kind) {
    case 0:
        return func_8003E39C(kind, 0, NULL);
    case 2:
    case 0xA:
        return func_8003E39C(kind, (s32) arg, NULL);
    case 0xFF:
        return func_8003E39C(kind, (s32) arg, (void *) extra);
    case 6:
    case 0xFE:
        D_80080ADC = ((u32 *) arg)[1];
        return func_8003E39C(kind, (s32) arg, (void *) extra);
    case 9:
    case 0xFC:
        return func_8003E39C(kind, 0, NULL);
    case 0xD: {
        Unk80083968 *r;

        D_80080AD4 = 0;
        if (arg == NULL) {
            return func_8003E39C(0xD, 0, (void *) extra);
        }
        func_8003E39C(0xD, 0, (void *) extra);
        func_8003E39C(2, (s32) arg, NULL);
        r = func_8003E39C(0x1B, (s32) arg, NULL);
        D_80080ADC = (u32) arg;
        return r;
    }
    case 0xE:
        return func_8003E39C(0xE, 0, (void *) extra);
    case 0x15:
        func_8003E39C(2, (s32) arg, NULL);
        return func_8003E39C(kind, (s32) arg, NULL);
    case 0x1B:
        return func_8003E39C(kind, (s32) arg, NULL);
    case 1:
    case 4:
    case 8:
    case 0xB:
    case 0xC:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x16:
        break;
    }
    return NULL;
}
