#!/usr/bin/env python3
"""advise.py zz.c UNIT FUNC -- say WHAT KIND of difference is left, and which lever it is.

`verify.py` compares the two word streams POSITIONALLY, which is right for
deciding MATCH but close to useless for diagnosis: one missing instruction near
the top makes every later word "differ", and a 101-word function reads as
"101 DIFF" when the real defect is a single register copy. This tool instead

  1. ALIGNS the two streams on instruction shape -- mnemonic plus immediate,
     registers deliberately left out -- so a pure register difference still
     lines up and an inserted word is reported as an insertion;
  2. decodes both sides from the raw 32-bit words rather than from disassembly
     text, because the two sides are printed by different tools (objdump says
     `li v0,-1` and `move a2,a0` where splat says `addiu $v0, $zero, -0x1` and
     `addu $a2, $a0, $zero`, and a branch operand is a label on one side and an
     address on the other);
  3. classifies each surviving difference into a KIND, and prints the source
     shapes that move that kind, cheapest first, with the measurement behind
     each.

The headline is the VERDICT, because it answers the only question that changes
what you do next:

  SHAPE  -- opcodes, immediates or displacements differ. The C is wrong. Keep
            rewriting it; the per-finding hints below say how.
  ALLOC  -- every opcode, immediate and displacement already matches and only
            register numbers differ. No amount of rewriting the logic helps;
            this is matching-levers section 3, and `--greg` prints the
            allocator's own numbers for it.
  LENGTH -- the word counts differ. The shift is localised to a branch span
            below, which is far more informative than the diff.

After the findings it prints a FRAME census whenever the two builds disagree
about any `$sp` slot. On a spilled function that is the sharpest diagnostic
there is: every local that loses its register takes a slot, the slots are
handed out in pseudo order, and the displacements are in the listing -- so a
frame that agrees offset for offset means the set of locals and their
declaration order are both already right, and a row on one side only names
the local to look at. It says nothing when the frames agree, which is itself
worth reading: func_8005BBFC's 220 differing words print no FRAME rows at
all, so none of them is a spill-layout problem.

It decides nothing and asserts nothing it has not measured -- same contract as
annotate.py, and for the same reason: a tool that ranked confidently would
reproduce this project's two worst detours faster. annotate.py reads the TARGET
alone and says what levers COULD apply; this reads your candidate against the
target and says which ones DO.

    python3 tools/decomp/advise.py scratch/zz_foo.c unk_1C540 func_8004F780
    python3 tools/decomp/advise.py scratch/zz_foo.c unk_1C540 func_8004F780 --greg
"""
import difflib
import math
import os
import re
import subprocess
import sys

import asmlib
from asmlib import REGS, BRANCHES, CompileError

# ---------------------------------------------------------------- knowledge
#
# Every entry cites the function it was measured on. A hint with no measured
# cost behind it does not belong here.

SIGNED_PAIRS = {
    frozenset(('slt', 'sltu')): 'a comparison',
    frozenset(('slti', 'sltiu')): 'a comparison against a constant',
    frozenset(('mult', 'multu')): 'a multiply',
    frozenset(('div', 'divu')): 'a divide',
    frozenset(('sra', 'srl')): 'a right shift',
    frozenset(('lh', 'lhu')): 'a halfword load',
    frozenset(('lb', 'lbu')): 'a byte load',
}

POLARITY_PAIRS = [frozenset(('beq', 'bne')), frozenset(('blez', 'bgtz')),
                  frozenset(('bltz', 'bgez'))]

COMMUTATIVE = set('addu add and or xor nor mult multu'.split())

SIGNED_HINT = """an operand's declared type differs in signedness. This is the
    cheapest thing on this list and the most expensive to leave wrong: it cost
    11 words on func_8004CECC, where `u32` locals turned all three divides
    unsigned. `multu`/`divu`/`sltu`/`srl`/`lhu` mean UNSIGNED operands, so an
    intermediate declared `u32` (or a `u16`/`u8` global read into one) makes
    the whole expression unsigned. Check every local and field in the
    expression at this address, not just the obvious one."""

POLARITY_HINT = """the if/else arms are the wrong way round. `jump` decides
    which arm goes out of line and the two are NOT interchangeable
    (matching-levers section 8). Write the arm that FALLS THROUGH in the target
    first. Measured on func_80037C7C (the `>= 0x37` arm had to come first) and
    func_8005AB50 (`return id` had to become the taken branch)."""

EVALORDER_HINT = """two adjacent instructions of the same kind have their
    operands transposed, and for LOADS that is evaluation order rather than
    reassociation: the operand the source names first is loaded first, and
    sched1's tie-break for equal-priority insns is RTL order. If this is the
    pair feeding a comparison, swap the comparison round -- `a > b` loads `a`
    first where `b < a` loads `b` first, and the two are the same test. That
    was worth the first of func_80059DAC's differences."""

COMMUTE_HINT = """the two source operands of a commutative operation are
    swapped, which is reassociation, not allocation: the operand the source
    groups first becomes `rs`. For `base + index`, writing the INDEX first in
    the sum gives `addu $v0, $v0, $base` where writing the base first gives
    `addu $v0, $base, $v0` (carved-sweep, 'Write the index FIRST in the sum').
    The reliable fix is to bind one side into its own pointer local --
    `u8 *t = SYM + i * STRIDE;` then `t[j]` -- which forces base+stride to
    combine before the index. That is what closed func_8004FC98 (2 DIFF -> 0)
    and func_8004F780.
    CAVEAT: a transposed pair is detected from register NUMBERS alone, so it
    can also be a plain RENAME of two independent values -- identical code
    under a different allocation. Check whether the two registers' defining
    instructions swap as well: if they do it is a rename and belongs under
    ALLOCATION. func_80039470 looked exactly like this and was really the
    table base being materialised AFTER the index instead of before it, fixed
    by the pointer-local idiom (`T *t = SYM;` then `&t[k]`)."""

NOP_HINT = """built has {n} extra nop(s), which are load-delay slots the
    target's scheduler filled and yours could not. sched1 decides memory
    dependence on offsets off the same base register, and a store to a GLOBAL
    before a load through a pointer IS a real dependence (matching-levers
    section 5), so nothing can be hoisted across it. Read the value into a
    local BEFORE the store that pins it:
        cur = GLOBAL[0]; lim = p->limit; cur += n; GLOBAL[0] = cur;
    rather than `GLOBAL[0] += n; if (GLOBAL[0] < p->limit)`. Worth exactly the
    two nops on func_8005AB50."""

SAVE_HINT = """the target saves {regs} and built does not, which is {cost}
    words ({n} sw/lw pair(s)): one more value has to stay live across a call.
    THE USUAL CAUSE IS A NARROW PARAMETER DECLARED WIDE. A parameter declared
    `s32` and then used as `(s16) e` gets a pseudo that is a plain copy of its
    incoming MEM, so local-alloc's update_equiv_regs substitutes it back (one
    set, one use): the load sinks to the use, combine folds the sign-extend
    into a single `lh`, and no register is needed at all. Declaring the
    parameter `s16` instead makes the entry insn a PROMOTION rather than a
    plain copy, the substitution cannot fire, the value lives across the calls
    in a callee-saved register, and the sign-extension reappears at the use.
    The target's signature for that is `lw $sN` (or a copy out of an argument
    register) at ENTRY plus `sll`/`sra` at the USE; a built `lh` straight off
    `$sp`, or a missing save, is the symptom. Worth exactly these words on
    func_8004A24C (declaring args 4-6 `s16` took it from 55/57 to MATCH) and
    on func_8003AB44 (52/54 to 54/54). It applies to a register parameter too,
    not just a stack one. Check EVERY `(s16)`/`(u16)` cast on a parameter in
    this function before looking anywhere else."""

EXTRA_SAVE_HINT = """built saves {regs} that the target does not, which is
    {cost} words of prologue and epilogue it does not spend. Something is
    being kept live across a call that the target recomputes or re-reads.
    Re-dereference instead of caching (matching-levers section 4): read the
    field again after the call rather than holding it."""

SHIFT_HINT = """the word counts differ by {d}. DO NOT read the positional
    diff -- every word after the defect is shifted and reads as wrong. The
    branch spans below bracket it: every branch whose span contains the
    missing word is off by exactly that count, so the FIRST branch whose delta
    changes is the one to look inside. That localised func_80042BDC's missing
    word to one arm of a 180-instruction switch in a single run, where the
    differing-word count said nothing at all."""

ALLOC_HINT = """every opcode, immediate, displacement and branch offset
    matches and only register numbers differ, in {n} place(s). No rewrite of
    the logic reaches this; it is matching-levers section 3. In rough order of
    how often it has paid here:
      - Assign back to a parameter or an existing local instead of introducing
        a fresh one, when the target reuses that register. gcc then ties the
        two and the WAR dependency also pins the instruction order
        (func_8004A170: `n = -h;` rather than a new local, 1 DIFF -> MATCH).
      - Split one assignment into two statements that compile to the same
        single insn. That makes the pseudo multi-set, drops it to sched
        priority 1, and moves its allocation priority at zero word cost.
      - Give an existing allocno a second role in another block, importing
        that block's hard-reg conflicts (bought 3 of func_80041E70's 4 words).
      - Shorten a temporary's live range to free an argument register
        (func_8004D12C), or lengthen it to push a value off one.
    Run with --greg for the allocator's own numbers before guessing."""

IMM_HINT = """the constant differs. For a load or store this is a STRUCT FIELD
    OFFSET and the diff will read as a register problem if you let it (see the
    context audit in where-we-are.md, item 4). For an `addiu`/`andi`/`ori` it
    is a literal or a mask. Note also that a constant binds to the term the
    source groups it with, and parentheses do not survive `fold`: `(v & 0xF) +
    0x7C80` then `+ ((v & 0x10) << 3)` must be split into a local, or the
    constant folds into the second term instead."""

OPCHANGE_HINTS = [
    (('lh', 'lw'), """built folded a sign-extending load where the target
    loads a word and sign-extends separately (`lw` then `sll`/`sra`). combine
    folds `(sign_extend (mem))` into one `lh` only WITHIN a basic block, so the
    target's value was loaded in a different block from its cast -- copy the
    parameter or field into a local at the top of the function and cast it at
    the use. func_8003AB44."""),
    (('lhu', 'lw'), """as lh/lw: built folded the zero-extending load into one
    insn. The target loaded the word in an earlier basic block."""),
    (('addiu', 'subu'), """built materialised a constant and subtracted
    (`li`/`subu`) where the target negated and added (`negu`/`addiu`). Write
    the negation into its own local and add the constant to it -- `m = -n;`
    then `m + 0x40` -- rather than `0x40 - n`. Measured on func_80037C7C,
    where one of the two `C - n` sites needed its own `-n` local and the other
    did not."""),
    (('subu', 'addiu'), """the reverse of the above: the target materialises
    the constant and subtracts, so write `C - n` directly rather than through a
    `-n` local."""),
]


def wrap(text, indent='    '):
    """Re-wrap to 78 columns, keeping bullet lines (`- ...`) as their own."""
    chunks, cur = [], []
    for raw in text.split('\n'):
        s = raw.strip()
        if s.startswith('-') or not s:
            if cur:
                chunks.append(' '.join(cur))
                cur = []
            if s:
                chunks.append(s)
        else:
            cur.append(s)
    if cur:
        chunks.append(' '.join(cur))
    out = []
    for ch in chunks:
        pad = indent + ('  ' if ch.startswith('-') else '')
        line = indent
        for w in ch.split():
            if len(line) + len(w) + 1 > 78 and line.strip():
                out.append(line)
                line = pad + w
            else:
                line = line + (' ' if line.strip() else '') + w
        if line.strip():
            out.append(line)
    return '\n'.join(out)


def saved_regs(insns):
    """{reg name: offset} for the prologue's register saves."""
    out = {}
    for ins in insns:
        if ins.op == 'sw' and REGS[ins.rs] == 'sp' and REGS[ins.rt] != 'sp':
            n = REGS[ins.rt]
            if n.startswith('s') and n != 'sp' or n in ('ra', 'fp'):
                out[n] = ins.simm
    return out


def frame_size(insns):
    for ins in insns[:4]:
        if ins.op == 'addiu' and REGS[ins.rd if False else ins.rt] == 'sp' \
                and REGS[ins.rs] == 'sp' and ins.simm < 0:
            return -ins.simm
    return 0


def branch_spans(insns):
    """[(index, op, displacement_in_words)] for every conditional branch."""
    out = []
    for i, ins in enumerate(insns):
        if ins.op in BRANCHES:
            out.append((i, ins.op, ins.simm))
    return out


def slot_census(insns):
    """{offset: {op: count}} for every $sp-relative load and store."""
    out = {}
    for ins in insns:
        if ins.op in ('lw', 'sw', 'lh', 'lhu', 'sh', 'lb', 'lbu', 'sb') \
                and REGS[ins.rs] == 'sp':
            out.setdefault(ins.simm, {}).setdefault(ins.op, 0)
            out[ins.simm][ins.op] += 1
    return out


def slot_report(B, T):
    """Compare the two frames slot by slot.

    On a function with heavy spilling the frame IS the diagnostic. Every
    local that loses its register takes a slot, the slots are handed out in
    pseudo order, and the displacement of each one is in the listing -- so a
    frame that agrees offset for offset means the set of locals and the order
    they were declared in are both right, and the first row that does not
    agree names the local to look at. func_8005B4D0's gap came down to one
    row: a seventh slot this source had and the listing did not, which was a
    spilled copy of a parameter the listing kept in its own home slot.

    Rows inside the saved-register block and the incoming-argument area are
    printed too, because a count that differs there says something different
    -- an argument read one more time, or a register saved that need not be.
    """
    cb, ct = slot_census(B), slot_census(T)
    fb, ft = frame_size(B), frame_size(T)
    sb, st = saved_regs(B), saved_regs(T)
    lo = min([o for o in list(sb.values()) + list(st.values())] or [fb])
    rows = []
    for off in sorted(set(list(cb) + list(ct))):
        b, t = cb.get(off, {}), ct.get(off, {})
        if b == t:
            continue
        rows.append((off, b, t))
    if not rows and fb == ft:
        return
    print()
    print('  FRAME  built 0x%X, target 0x%X; saved registers from 0x%X'
          % (fb, ft, lo))
    if not rows:
        print('    every $sp slot agrees')
        return
    def fmt(d):
        return ' '.join('%s*%d' % (k, v) for k, v in sorted(d.items())) or '-'
    for off, b, t in rows:
        where = ('saved reg' if off >= lo and off < (fb if fb else 0)
                 else 'incoming arg' if fb and off >= fb else 'local')
        print('    0x%02X  %-12s built %-18s target %s'
              % (off, where, fmt(b), fmt(t)))
    print(wrap('a LOCAL row that exists on one side only is a local that one '
               'build spilled and the other kept; the slots are handed out in '
               'pseudo order, so declaring the spilled ones in the listing\'s '
               'slot order is the first thing to try. An INCOMING ARG row '
               'means the listing reads a parameter from its own home slot '
               'where this source keeps a copy, or the other way round.',
               '    '))


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    flags = {a for a in sys.argv[1:] if a.startswith('--')}
    if len(args) < 3:
        print(__doc__)
        return 2
    src, unit, func = args[0:3]
    gasG = asmlib.gas_g_for_unit(unit)
    for a in flags:
        if a == '--g8':
            gasG = 8
        elif a.startswith('--gas-G='):
            gasG = int(a.split('=', 1)[1])
        elif a == '--g0':
            gasG = 0

    try:
        words, relocs = asmlib.build(src, gasG)
    except CompileError as e:
        print(e)
        return 2
    tgt = asmlib.target_words(unit, func)
    vram = asmlib.target_vram(unit, func)

    B = asmlib.decode_built(words)
    for ins, (addr, _, _) in zip(B, words):
        ins.rel = addr in relocs
    T = asmlib.decode_target(tgt)
    for ins in T:
        ins.rel = '%hi(' in ins.text or '%lo(' in ins.text

    def shape(ins):
        op, imm = ins.shape
        return (op, 'REL' if ins.rel else imm)

    print('built %d words, target %d words' % (len(B), len(T)))

    sm = difflib.SequenceMatcher(a=[shape(i) for i in B], b=[shape(i) for i in T],
                                 autojunk=False)
    ops = sm.get_opcodes()

    findings = []       # (priority, tag, addr, detail, hint)
    reg_only = []
    shape_diff = False

    for tag, i1, i2, j1, j2 in ops:
        if tag == 'equal':
            k = 0
            while k < i2 - i1:
                b, t = B[i1 + k], T[j1 + k]
                if b.regs == t.regs:
                    k += 1
                    continue
                addr = vram + 4 * (j1 + k)
                # (a) one commutative insn with its two sources swapped
                if (b.op in COMMUTATIVE and b.op == t.op and len(b.regs) == 3
                        and b.regs[0] == t.regs[0]
                        and b.regs[1] == t.regs[2] and b.regs[2] == t.regs[1]
                        and b.regs[1] != b.regs[2]):
                    shape_diff = True
                    findings.append((1, 'COMMUTED', addr,
                                     'built %-28s target %s' % (b, t), COMMUTE_HINT))
                    k += 1
                    continue
                # (b) two ADJACENT same-op insns whose addends are transposed:
                # `addu v0,v0,IDX` then `addu v0,v0,BASE` against the reverse.
                # That is the base/index reassociation of carved-sweep, not an
                # allocation difference, and it is what func_8004FC98 was.
                if k + 1 < i2 - i1:
                    b2, t2 = B[i1 + k + 1], T[j1 + k + 1]
                    if (b.op == t.op == b2.op == t2.op
                            and b.regs != t.regs and b2.regs != t2.regs
                            and b.regs == t2.regs and b2.regs == t.regs):
                        shape_diff = True
                        # A transposed pair of LOADS is which operand the
                        # source evaluates first; a transposed pair of adds is
                        # base/index reassociation. Different fix either way.
                        load = b.op in asmlib.LOADS or b.op in asmlib.STORES
                        findings.append((1, 'EVAL-ORDER' if load else 'REASSOCIATED',
                                         addr,
                                         'built %s then %s; target %s then %s'
                                         % (b, b2, t, t2),
                                         EVALORDER_HINT if load else COMMUTE_HINT))
                        k += 2
                        continue
                reg_only.append((addr, b, t))
                k += 1
            continue

        if tag == 'replace':
            n = min(i2 - i1, j2 - j1)
            for k in range(n):
                b, t = B[i1 + k], T[j1 + k]
                addr = vram + 4 * (j1 + k)
                pair = frozenset((b.op, t.op))
                if b.op != t.op and pair in SIGNED_PAIRS:
                    shape_diff = True
                    findings.append((0, 'SIGNEDNESS', addr,
                                     '%s is unsigned/signed the wrong way (built %s, target %s)'
                                     % (SIGNED_PAIRS[pair], b.op, t.op), SIGNED_HINT))
                    continue
                if b.op != t.op and pair in POLARITY_PAIRS:
                    shape_diff = True
                    findings.append((0, 'BRANCH-ARMS', addr,
                                     'built %s, target %s' % (b.op, t.op), POLARITY_HINT))
                    continue
                if b.op == t.op and b.imm_key != t.imm_key:
                    shape_diff = True
                    findings.append((2, 'IMMEDIATE', addr,
                                     'built %s imm %s, target imm %s'
                                     % (b.op, b.imm_key, t.imm_key), IMM_HINT))
                    continue
                if b.op != t.op:
                    shape_diff = True
                    hint = None
                    for (bo, to), h in OPCHANGE_HINTS:
                        if b.op == bo and t.op == to:
                            hint = h
                            break
                    findings.append((3, 'OPCODE', addr,
                                     'built %-28s target %s' % (b, t),
                                     hint or 'no measured lever for this pair yet; '
                                             'read the two instructions in context.'))
            # leftover on either side inside a replace block
            for k in range(n, i2 - i1):
                shape_diff = True
                findings.append((4, 'EXTRA', vram + 4 * j2,
                                 'built has %s with nothing aligned to it' % B[i1 + k], ''))
            for k in range(n, j2 - j1):
                shape_diff = True
                findings.append((4, 'MISSING', vram + 4 * (j1 + k),
                                 'target has %s with nothing aligned to it' % T[j1 + k], ''))
            continue

        # An insert or a delete is a real shape difference even when the word
        # COUNTS happen to agree: an instruction the target has and built does
        # not, balanced by one built has and the target does not, aligns as a
        # delete plus an insert and must not be reported as MATCH. That bug
        # called func_80035AF8 a match when verify.py said 3 DIFF.
        if tag == 'insert':      # words only in the target
            shape_diff = True
            for k in range(j1, j2):
                findings.append((4, 'MISSING', vram + 4 * k,
                                 'target has %s and built has nothing there' % T[k], ''))
        elif tag == 'delete':    # words only in built
            shape_diff = True
            for k in range(i1, i2):
                findings.append((4, 'EXTRA', vram + 4 * j1,
                                 'built has %s and the target has nothing there' % B[k], ''))

    # ---- whole-function findings -----------------------------------------
    nb = sum(1 for i in B if i.op == 'nop')
    nt = sum(1 for i in T if i.op == 'nop')
    if nb > nt:
        findings.insert(0, (0, 'DELAY-NOPS', vram,
                            'built %d nop(s), target %d' % (nb, nt),
                            NOP_HINT.format(n=nb - nt)))

    sb, st = saved_regs(B), saved_regs(T)
    only_t = sorted(set(st) - set(sb))
    only_b = sorted(set(sb) - set(st))
    if only_t:
        findings.insert(0, (0, 'CALLEE-SAVED', vram,
                            'target saves %s, built does not; frame %#x vs %#x'
                            % (','.join('$' + r for r in only_t),
                               frame_size(B), frame_size(T)),
                            SAVE_HINT.format(regs=','.join('$' + r for r in only_t),
                                             cost=2 * len(only_t), n=len(only_t))))
    if only_b:
        findings.insert(0, (0, 'CALLEE-SAVED', vram,
                            'built saves %s, target does not; frame %#x vs %#x'
                            % (','.join('$' + r for r in only_b),
                               frame_size(B), frame_size(T)),
                            EXTRA_SAVE_HINT.format(regs=','.join('$' + r for r in only_b),
                                                   cost=2 * len(only_b))))

    # ---- verdict ----------------------------------------------------------
    if len(B) != len(T):
        verdict = 'LENGTH'
    elif shape_diff:
        verdict = 'SHAPE'
    elif reg_only:
        verdict = 'ALLOC'
    else:
        verdict = 'MATCH on every opcode, immediate, displacement, branch '\
                  'offset and register advise.py aligned -- confirm with '\
                  'verify.py, then make check'
    print('VERDICT: %s' % verdict)

    if len(B) != len(T):
        print()
        print('  LENGTH  built %d, target %d (%+d)' % (len(B), len(T), len(B) - len(T)))
        print(wrap(SHIFT_HINT.format(d=abs(len(B) - len(T))), '          '))
        bs, ts = branch_spans(B), branch_spans(T)
        print()
        print('          branch spans (first row whose delta differs brackets the defect):')
        if bs and ts and all(bd == td for (_, _, bd), (_, _, td) in zip(bs, ts)):
            print('          none differ: the extra/missing words are AFTER the last')
            print('          branch, or in the prologue/epilogue -- see the findings below.')
        shown = 0
        for (bi, bop, bd), (ti, top, td) in zip(bs, ts):
            flag = '  <-- differs' if bd != td else ''
            if bd != td or shown < 3:
                lo = vram + 4 * (ti + 1)
                hi = vram + 4 * (ti + 1 + td)
                print('          %-6s at %08X  built %+4d  target %+4d   span %08X..%08X%s'
                      % (top, vram + 4 * ti, bd, td, min(lo, hi), max(lo, hi), flag))
                shown += 1
            if bd != td:
                break

    if reg_only and verdict == 'ALLOC':
        findings.append((5, 'ALLOCATION', vram,
                         '%d register-only difference(s)' % len(reg_only),
                         ALLOC_HINT.format(n=len(reg_only))))
    elif reg_only:
        findings.append((6, 'ALLOCATION', vram,
                         '%d register-only difference(s) as well -- fix the shape '
                         'findings first, these usually follow' % len(reg_only), ''))

    seen = set()
    order = 0
    for pri, tag, addr, detail, hint in sorted(findings, key=lambda f: f[0]):
        key = (tag, hint[:40])
        if key in seen:
            continue
        seen.add(key)
        order += 1
        print()
        print('  %-13s %08X  %s' % (tag, addr, detail))
        if hint:
            print(wrap(hint, '                '))
        # list the other sites of the same kind compactly
        more = [a for p, g, a, d, h in findings
                if g == tag and h[:40] == hint[:40] and a != addr]
        if more:
            print('                also at: %s'
                  % ' '.join('%08X' % a for a in sorted(set(more))[:12]))

    if reg_only and '--regs' in flags:
        print()
        print('  register-only differences, in target order:')
        for addr, b, t in reg_only:
            print('    %08X  built %-30s target %s' % (addr, b, t))

    slot_report(B, T)

    if '--greg' in flags:
        print()
        greg_report(src)
    return 0



# ---------------------------------------------------------- margins
#
# The point of a margin rather than a state. A register-permutation diff tells
# you WHICH register each quantity got; it does not tell you how close the
# contest was, so every candidate edit is a guess and the loop is a random
# restart. Both allocators decide by a closed-form formula, so the distance to
# flipping a pair is computable: how many more references, or how much less
# live range, quantity A needs to overtake quantity B. That turns "try
# something" into "this needs +1 ref", which is a directed edit.
#
# Both formulas were read out of gcc 2.7.2 (scratch/gcc-src), not guessed:
#   global.c   allocno_compare: floor_log2(refs) * refs / live_length
#   local-alloc.c qty_compare_1: floor_log2(refs) * refs * size / live_length
# The caveats live with the printout, because a margin is only meaningful when
# the two quantities are decided by the formula at all.


def refs_to_beat(refs, live, target_pri, size):
    """Smallest reference count at this live length that beats target_pri.

    Returns None if no count up to 4x the current one gets there -- that is
    the honest answer for a quantity whose live range is the problem rather
    than its reference count.
    """
    for r in range(refs + 1, max(refs * 4, refs + 32)):
        if math.floor(math.log2(r)) * r * size / max(live, 1) > target_pri:
            return r
    return None


def live_to_beat(refs, live, target_pri, size):
    """Largest live length at this reference count that beats target_pri."""
    if refs < 2:
        return None
    num = math.floor(math.log2(refs)) * refs * size
    if target_pri <= 0:
        return None
    want = num / target_pri
    lim = int(math.floor(want))
    if lim >= live or lim < 1:
        return None
    return lim


def margin_report(rows, size, what):
    """rows: [(pseudo, refs, live, priority, got)] in allocation order.

    Prints, for each adjacent pair the formula separates, what it would take
    to swap them. Adjacent pairs are the only ones worth printing: a
    non-adjacent swap needs every pair in between to move too.
    """
    sep = [r for r in rows if r[3] > 0]
    if len(sep) < 2:
        return
    print()
    print('  MARGINS (%s): what it would take to swap each adjacent pair' % what)
    any_close = False
    for (na, ra, la, pa, ga), (nb, rb, lb, pb, gb) in zip(sep, sep[1:]):
        if pa <= pb:
            continue
        need_r = refs_to_beat(rb, lb, pa, size)
        need_l = live_to_beat(rb, lb, pa, size)
        gap = (pa - pb) / pa * 100
        bits = []
        if need_r is not None:
            bits.append('%d refs (+%d)' % (need_r, need_r - rb))
        if need_l is not None:
            bits.append('live <= %d (-%d)' % (need_l, lb - need_l))
        if not bits:
            bits.append('nothing within 4x its reference count')
        if gap < 25:
            any_close = True
        print('    %-5d (%.3f) over %-5d (%.3f)  gap %5.1f%%   to flip %d: %s'
              % (na, pa, nb, pb, gap, nb, ', '.join(bits)))
    if any_close:
        print(wrap("""A gap under 25% is worth attacking; above that the
        reference count usually has to cross a power of two, because
        floor_log2 is a step. The cheapest reference that costs no instruction
        is splitting one assignment into two statements that compile to the
        same single insn (matching-levers section 3); the cheapest live-range
        change is moving a value's first or last use.""", '    '))
    print(wrap("""A MARGIN IS ONLY MEANINGFUL WHERE THE FORMULA DECIDES. It
    does not apply to a pair where one side has a hard-register SUGGESTION and
    the other does not -- that pair is settled in local-alloc's first phase,
    which ignores priority entirely (func_8004DE88) -- nor to a quantity whose
    register was forced by a conflict rather than by order. Check the
    conflicts column and the xcall column before trusting a row here.""",
    '    '))

# ------------------------------------------------------------------ --greg
#
# This is the automation of what used to be a hand reading of the dumps. The
# two numbers that decide a register are reg_n_refs and reg_live_length, and
# global.c's priority is floor_log2(refs) * refs / live_length; local-alloc
# runs FIRST and orders its own (single-block) quantities by refs/live_length,
# so it takes the low registers a global allocno wanted. The one derived
# finding below -- a global conflicting with every one of $v0..$a3 -- is that
# situation, and it cost two hand investigations to recognise before this.

ARGRET = [2, 3, 4, 5, 6, 7]     # $v0 $v1 $a0 $a1 $a2 $a3


def local_priority(refs, live):
    """local-alloc.c's qty_compare_1, with the quantity size taken as 4."""
    if refs < 2 or not live:
        return 0.0
    return math.floor(math.log2(refs)) * refs * 4 / live
SPECIAL = {64: 'hi', 65: 'lo', 66: 'md'}


def regname(n):
    """A disposition can be a hard reg, MIPS's hi/lo, or a spill number."""
    if n < 32:
        return '$' + REGS[n]
    return SPECIAL.get(n, '#%d' % n)


def greg_report(src):
    base = os.path.splitext(os.path.basename(src))[0]
    d = os.path.join(asmlib.SP, 'dumps', base)
    r = subprocess.run(['bash', os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                             'dump.sh'), src],
                       capture_output=True, text=True, cwd=asmlib.PROJ)
    if not os.path.isdir(d):
        print('  --greg: no dumps collected (%s)' % (r.stderr.strip()[:200] or d))
        return

    lreg = {}
    local_of = {}
    crosses = {}
    try:
        for ln in open(os.path.join(d, 'lreg')):
            m = re.match(r'Register (\d+) used (\d+) times across (\d+) insns'
                         r'(?:[^;]*?in block (\d+))?', ln)
            if m:
                n, refs, live = int(m.group(1)), int(m.group(2)), int(m.group(3))
                lreg[n] = (refs, live)
                if m.group(4) is not None:
                    local_of[n] = int(m.group(4))
                # `crosses N calls` is the fact that forces a callee-saved
                # register, and it is in this dump but was not being printed.
                c = re.search(r'crosses (\d+) calls?', ln)
                crosses[n] = int(c.group(1)) if c else 0
    except OSError:
        pass

    conflicts, order, disp = {}, [], {}
    try:
        g = open(os.path.join(d, 'greg')).read()
    except OSError:
        print('  --greg: no greg dump')
        return
    m = re.search(r';; \d+ regs to allocate:([^\n]*)', g)
    if m:
        order = [int(x) for x in m.group(1).split()]
    for m in re.finditer(r';; (\d+) conflicts:([^\n]*)', g):
        conflicts[int(m.group(1))] = [int(x) for x in m.group(2).split()]
    m = re.search(r';; Register dispositions:(.*?)\n\n', g, re.S)
    if m:
        for a, b in re.findall(r'(\d+) in (\d+)', m.group(1)):
            disp[int(a)] = int(b)

    grows = []
    print('  ALLOCATION (from the compiler\'s own .lreg/.greg dumps)')
    print('  %-6s %-5s %-5s %-8s %-6s %-5s %-5s %s'
          % ('pseudo', 'refs', 'live', 'priority', 'block', 'xcall', 'got',
             'hard-reg conflicts'))
    for n in order:
        refs, live = lreg.get(n, (0, 0))
        pri = (math.floor(math.log2(refs)) * refs / live) if refs > 1 and live else 0.0
        hard = sorted(r for r in conflicts.get(n, []) if r < 32)
        blk = local_of.get(n)
        print('  %-6d %-5d %-5d %-8.3f %-6s %-5s %-5s %s'
              % (n, refs, live, pri,
                 ('b%d' % blk) if blk is not None else 'global',
                 crosses.get(n, 0) or '-',
                 regname(disp[n]) if n in disp else '-',
                 ' '.join(regname(r) for r in hard) or '-'))
        grows.append((n, refs, live, pri, regname(disp[n]) if n in disp else '-'))

    margin_report(grows, 1, "global-alloc, allocno_compare")

    # Everything global-alloc never saw. In a single-basic-block function that
    # is the ENTIRE contest -- func_8004DE88's table above comes out empty
    # because local-alloc decides all of it, and the report used to say
    # nothing at all about the quantities that actually competed.
    locals_only = sorted((n for n in lreg if n not in order),
                         key=lambda n: (-local_priority(*lreg[n]), n))
    if locals_only:
        print()
        print('  LOCAL-ALLOC quantities (global-alloc never saw these; it runs')
        print('  SECOND, so these took their registers first)')
        print('  %-6s %-5s %-5s %-9s %-6s %s'
              % ('pseudo', 'refs', 'live', 'priority', 'block', 'xcall'))
        lrows = []
        for n in locals_only[:14]:
            refs, live = lreg[n]
            print('  %-6d %-5d %-5d %-9.3f %-6s %s'
                  % (n, refs, live, local_priority(refs, live),
                     ('b%d' % local_of[n]) if n in local_of else '?',
                     crosses.get(n, 0) or '-'))
            lrows.append((n, refs, live, local_priority(refs, live), '-'))
        print(wrap("""priority is local-alloc.c's own qty_compare_1,
        floor_log2(refs) * refs * size / live_length, with size taken as 4 --
        read out of gcc 2.7.2 rather than guessed, and the final tie-break is
        the QUANTITY NUMBER, so among equals birth order decides. floor_log2
        means a quantity has to cross a power of two in its reference count to
        move far: 4 and 5 refs both give 2, 7 and 8 give 2 and 3.

        TWO THINGS THIS TABLE CANNOT SHOW, both from local-alloc.c:
        - `block_alloc` runs a SUGGESTION PHASE first, and it ignores priority.
          Any quantity that is one side of a move whose other side is a hard
          register -- a call's return value, or a value copied into an argument
          register -- gets a suggested register from `combine_regs` and is
          allocated BEFORE every quantity that has no such copy, however much
          higher the latter's priority. If the contest you are looking at is
          between a value born from a call return and one born from
          arithmetic, priority is the wrong story entirely (func_8004DE88).
        - that phase calls `find_free_reg` with accept_call_clobbered = 0, and
          it starts `used` from call_used_reg_set whenever the quantity crosses
          a call. So for a quantity with a $v0 suggestion, ONE intervening call
          is the difference between taking $v0 in phase 1 and falling through
          to phase 2 with its copy left behind -- which is what the xcall
          column is here for.""", '  '))

    if locals_only:
        margin_report(lrows, 4, "local-alloc, qty_compare_1")

    squeezed = [n for n in order
                if n not in local_of
                and all(r in conflicts.get(n, []) for r in ARGRET)]
    if squeezed:
        print()
        print(wrap("""DERIVED: pseudo(s) %s are global allocnos conflicting with
        EVERY one of $v0..$a3, so find_reg cannot give them an argument
        register whatever their priority -- local-alloc already handed those
        out to single-block quantities, which it allocates first and orders by
        refs/live_length. If the target keeps a long-lived value in an argument
        register here, its version of this block had FEWER long-lived
        block-local quantities than yours: look for a local you introduced that
        the original wrote inline, or one whose live range you lengthened.
        Diagnosed by hand twice (func_8004CECC, func_8005500C) before this line
        existed.""" % ', '.join(str(n) for n in squeezed), '  '))


if __name__ == '__main__':
    sys.exit(main())
