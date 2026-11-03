#!/usr/bin/env python3
"""leverpick.py -- pick the levers that match ONE target's shape.

`docs/matching-levers.md` holds ~390 levers in ~16,700 lines. A brief that
inlines twelve of them inlines the same twelve for a leaf function with no
loop and for a 31-case jump table. This module reads a splat listing, computes
a FINGERPRINT off the instruction stream, and returns the lever HEADINGS whose
own English text speaks to that fingerprint -- headings and line numbers only,
so an agent pays a `sed` per lever it decides to read instead of 16,690 lines.

The headings are already one-line lever statements, so the heading text is the
only thing matched against. There is no database and nothing to regenerate:
`leverindex.build()` is reused to extract every heading with its line number,
fresh, on every call. A lever that matches nothing is simply not printed; a
target that matches nothing still gets the brief's always-on core.

    python3 tools/decomp/leverpick.py asm/nonmatchings/ovl_town/func_X.s
    python3 tools/decomp/leverpick.py --signals FILE.s
    python3 tools/decomp/leverpick.py --prevalence asm/nonmatchings/ovl_town

NEVER use %-formatting on a lever heading: they are full of literal `%hi`,
`%lo` and `%gp_rel`, and `%hi` parses as an `h` length modifier plus an `i`
conversion. That crashed brief.py on every invocation for eight agent runs.
"""
import io
import math
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
DOC = os.path.join(PROJ, "docs/matching-levers.md")

sys.path.insert(0, HERE)
import leverindex  # noqa: E402  -- reuse build(); do not reparse the doc

# ---------------------------------------------------------------- listing parse

# splat emits:  /* FILEOFF VRAM WORD */  mnemonic  operands
# a delay-slot insn carries one extra leading space before the mnemonic.
INSN = re.compile(
    r"^\s*/\* [0-9A-Fa-f]+ ([0-9A-Fa-f]{8}) [0-9A-Fa-f]{8} \*/(\s+)(\S+)\s*(.*?)\s*$")
LABEL = re.compile(r"^\s*(\.L[0-9A-Fa-f]{8}):")

BRANCH = set("""beq bne beqz bnez blez bgez bltz bgtz b bc1t bc1f
                beql bnel blezl bgezl""".split())
JUMP = set("j jal jr jalr".split())
STORE = set("sw sh sb swl swr swc1 sdc1 sc".split())
LOADS = set("lw lh lhu lb lbu lwl lwr lwc1 ldc1 ll".split())


class Insn(object):
    __slots__ = ("addr", "op", "args", "delay", "regs")

    def __init__(self, addr, op, args, delay):
        self.addr = addr
        self.op = op
        self.args = args
        self.delay = delay
        self.regs = re.findall(r"\$([a-z0-9]+)", args)


def parse(text):
    """Return (insns, labels) for the FUNCTION body only.

    A listing may open with a .rodata jump table whose `.word` lines carry the
    same comment shape; those have no register operands and a `.word` mnemonic,
    and anything before `glabel` is skipped outright.
    """
    insns, labels, started = [], {}, False
    for line in text.split("\n"):
        if line.startswith("glabel ") or line.startswith("dlabel func_"):
            started = True
            continue
        if not started:
            continue
        m = LABEL.match(line)
        if m:
            labels[m.group(1)] = int(m.group(1)[2:], 16)
            continue
        m = INSN.match(line)
        if not m:
            continue
        op = m.group(3)
        if op.startswith("."):
            continue
        insns.append(Insn(int(m.group(1), 16), op, m.group(4),
                          len(m.group(2)) > 2))
    return insns, labels


# ----------------------------------------------------------------- fingerprint

SAVED = ["s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7"]
ARGS = ["a0", "a1", "a2", "a3"]


def signals(text):
    """tag -> short human witness string. Only tags that FIRE are present."""
    insns, labels = parse(text)
    sig = {}
    if not insns:
        return sig
    ops = [i.op for i in insns]
    body = "\n".join(i.op + " " + i.args for i in insns)
    n = len(insns)

    # -- jump table: jr on a register other than $ra
    jt = [i for i in insns if i.op == "jr" and "ra" not in i.regs]
    if jt:
        sig["jumptable"] = "jr on a non-$ra register x" + str(len(jt))

    # -- own rodata
    if ".section .rodata" in text or re.search(r"^dlabel D_", text, re.M) \
            or re.search(r"^dlabel jtbl_", text, re.M):
        sig["rodata"] = "own .rodata / dlabel in this listing"

    # -- loop: a branch whose target label is at a lower address
    back = 0
    for i in insns:
        if i.op in BRANCH or i.op == "j":
            m = re.search(r"(\.L[0-9A-Fa-f]{8})", i.args)
            if m and labels.get(m.group(1), 1 << 40) < i.addr:
                back += 1
    if back:
        sig["loop"] = "back-edge branch x" + str(back)

    # -- leaf / non-leaf
    if "jal" in ops:
        sig["call"] = "jal x" + str(ops.count("jal"))
    else:
        sig["leaf"] = "no jal at all"
    if "jalr" in ops:
        sig["indirect"] = "jalr x" + str(ops.count("jalr"))

    # -- muldiv
    d = [o for o in ops if o in ("div", "divu")]
    if d:
        sig["div"] = "/".join(sorted(set(d))) + " x" + str(len(d))
    mu = [o for o in ops if o in ("mult", "multu")]
    if mu:
        sig["mult"] = "/".join(sorted(set(mu))) + " x" + str(len(mu))
    # DISCARDED: an "mfhi/mflo with no mult/div" tag. It fired on 0 of 1819
    # listings -- on MIPS the result registers never appear without the
    # instruction that sets them, so the tag was a tautology of `mult`/`div`.

    # -- a %hi on a func_ symbol: an install, invisible to ovlverify
    inst = set(re.findall(r"%hi\((func_[0-9A-Fa-f]+)\)", text))
    if inst:
        sig["install"] = "%hi on " + ", ".join(sorted(inst)[:3])

    # -- callee-saved pressure
    sv = sorted(set(r for i in insns if i.op == "sw" and i.args.startswith("$")
                    for r in i.regs[:1] if r in SAVED))
    if len(sv) >= 3:
        sig["saved"] = str(len(sv)) + " of $s0-$s7 saved (" + ",".join(sv) + ")"

    # -- nops, and where they sit
    nops = [i for i in insns if i.op == "nop"]
    dnop = [i for i in nops if i.delay]
    if dnop:
        sig["delaynop"] = str(len(dnop)) + " nop(s) in a delay slot"
    load_nop = 0
    for k, i in enumerate(insns):
        if i.op == "nop" and not i.delay and k and insns[k - 1].op in LOADS:
            load_nop += 1
    if load_nop:
        sig["loadnop"] = str(load_nop) + " nop(s) in a load-delay slot"

    # -- narrow traffic
    half = [o for o in ops if o in ("sh", "lh", "lhu")]
    if half:
        sig["half"] = "halfword traffic x" + str(len(half))
    byte = [o for o in ops if o in ("sb", "lb", "lbu")]
    if byte:
        sig["byte"] = "byte traffic x" + str(len(byte))
    if re.search(r"\bsh\s+\$zero", body):
        sig["shzero"] = "sh $zero -- worth exactly 4 bytes"

    # -- sll/sra 16 pair: an s16 cast at a use
    pair = 0
    for k in range(n - 1):
        if insns[k].op == "sll" and insns[k].args.endswith("16") \
                and insns[k + 1].op == "sra" and insns[k + 1].args.endswith("16"):
            pair += 1
    if pair:
        sig["s16cast"] = "sll/sra 16 pair x" + str(pair)

    # -- addressing shapes
    if "%lo(" in text:
        sig["symaddr"] = "lui + %lo symbolic pair"
    oris = 0
    for k in range(n - 1):
        if insns[k].op == "lui" and insns[k + 1].op == "ori" \
                and "%lo" not in insns[k + 1].args:
            oris += 1
    if oris:
        sig["constaddr"] = ("lui+ori constant address x" + str(oris)
                            + " -- permuter can never score zero")

    # -- basic blocks
    nb = len(labels)
    if nb >= 6:
        sig["manyblocks"] = str(nb) + " .L labels"

    # -- parameters: read before written, and any gap
    read, wrote = {}, {}
    for k, i in enumerate(insns):
        src = i.regs if (i.op in STORE or i.op in BRANCH or i.op in JUMP) \
            else i.regs[1:]
        dst = [] if (i.op in STORE or i.op in BRANCH or i.op in JUMP) \
            else i.regs[:1]
        for r in src:
            if r in ARGS and r not in read and r not in wrote:
                read[r] = k
        for r in dst:
            if r in ARGS and r not in wrote and r not in read:
                wrote[r] = k
    used = [a for a in ARGS if a in read]
    if used:
        top = ARGS.index(used[-1])
        gap = [a for a in ARGS[:top] if a not in read]
        if gap:
            sig["deadparam"] = ("$" + ",$".join(gap) + " never read below $"
                                + used[-1] + " -- a DEAD MIDDLE parameter")

    # -- frame
    m = re.search(r"addiu\s+\$sp, \$sp, -(0x[0-9A-Fa-f]+)", body)
    if m:
        f = int(m.group(1), 16)
        if f == 0:
            sig["noframe"] = "no stack frame"
    else:
        sig["noframe"] = "no stack frame"

    # -- abs shape
    for k in range(n - 1):
        if insns[k].op in ("bgez", "bltz", "blez", "bgtz"):
            for j in (k + 1, k + 2, k + 3):
                if j >= n:
                    continue
                neg = (insns[j].op == "negu"
                       or (insns[j].op == "subu"
                           and "zero" in insns[j].regs[1:2]))
                if neg:
                    sig["abs"] = ("a signed branch within 4 insns of a "
                                  "negu/subu-from-zero -- an abs() shape")
                    break
        if "abs" in sig:
            break

    # -- ?: diamond: a short forward branch over a single move
    for k, i in enumerate(insns):
        if i.op not in BRANCH:
            continue
        m = re.search(r"(\.L[0-9A-Fa-f]{8})", i.args)
        if not m:
            continue
        tgt = labels.get(m.group(1))
        if tgt is None or not (0 < tgt - i.addr <= 12):
            continue
        skipped = [x for x in insns if i.addr < x.addr < tgt and not x.delay]
        if len(skipped) == 1 and skipped[0].op in ("addu", "or", "move", "addiu") \
                and "zero" in skipped[0].regs:
            sig["ternary"] = "short forward branch over a single move"
            break

    # -- block move
    if [o for o in ops if o in ("lwl", "lwr", "swl", "swr")]:
        sig["blockmove"] = "lwl/lwr/swl/swr -- a block move"

    # -- control-flow shape
    cond = [i for i in insns if i.op in BRANCH]
    if len(cond) >= 5:
        sig["branchy"] = str(len(cond)) + " conditional branches"
    ret = [i for i in insns if i.op == "jr" and "ra" in i.regs]
    if len(ret) > 1:
        sig["multiexit"] = str(len(ret)) + " `jr $ra` -- more than one exit"

    # -- reg-reg copies (what cse/jump2 delete and a lever keeps)
    cp = [i for i in insns
          if i.op in ("addu", "or", "move") and "zero" in i.regs[2:3]
          and i.regs[1] != "zero"]
    if len(cp) >= 3:
        sig["regcopy"] = str(len(cp)) + " reg-reg copies"

    # -- stack locals, distinct from the saved-register and $ra slots
    slots = set()
    for i in insns:
        m = re.match(r"\$(\w+), (-?0x[0-9A-Fa-f]+)\(\$sp\)", i.args)
        if m and (i.op in LOADS or i.op in STORE) \
                and m.group(1) not in SAVED and m.group(1) != "ra":
            slots.add(m.group(2))
    if slots:
        sig["stackslot"] = str(len(slots)) + " stack slot(s) not holding a $s/$ra"

    # -- a run of stores to distinct displacements off ONE base
    bases = {}
    for i in insns:
        m = re.match(r"\$\w+, (-?0x[0-9A-Fa-f]+)\(\$(\w+)\)", i.args)
        if m and i.op in STORE and m.group(2) != "sp":
            bases.setdefault(m.group(2), set()).add(m.group(1))
    runs = max([len(v) for v in bases.values()] or [0])
    if runs >= 3:
        sig["storerun"] = str(runs) + " stores to distinct offsets off one base"

    # -- a comparison the source could have written either way round
    if [o for o in ops if o in ("slt", "slti", "sltu", "sltiu")]:
        sig["compare"] = "slt/slti/sltu/sltiu present"

    # -- an argument register copied into another argument register
    fwd = [i for i in insns if i.op in ("addu", "or", "move")
           and i.regs and i.regs[0] in ARGS
           and len(i.regs) > 1 and i.regs[1] in ARGS]
    if fwd and "call" in sig:
        sig["forwardarg"] = str(len(fwd)) + " $aN-to-$aM copies -- a forward"

    # -- constants materialised in registers
    cm = 0
    for i in insns:
        if i.op == "addiu" and "zero" in i.regs[1:2]:
            cm += 1
        elif i.op in ("li", "ori") and "zero" in i.regs[1:2]:
            cm += 1
    if cm >= 3:
        sig["constmat"] = str(cm) + " constants materialised in registers"

    # -- the same load twice: a re-read, which is a cse lever
    seen, rl = {}, 0
    for i in insns:
        if i.op in LOADS and "(" in i.args:
            k = i.op + " " + i.args
            if k in seen:
                rl += 1
            seen[k] = 1
    if rl:
        sig["reload"] = str(rl) + " repeated identical load(s) -- a re-read"

    # -- read-modify-write on one address
    rmw = 0
    for k, i in enumerate(insns):
        if i.op not in LOADS:
            continue
        m = re.match(r"\$\w+, (-?0x[0-9A-Fa-f]+\(\$\w+\))", i.args)
        if not m:
            continue
        for j in range(k + 1, min(k + 5, n)):
            if insns[j].op in STORE and insns[j].args.endswith(m.group(1)):
                rmw += 1
                break
    if rmw:
        sig["rmw"] = str(rmw) + " load/store read-modify-write(s) on one address"

    # -- a scaling shift that is neither a 16-bit cast nor a word index
    sc = [i for i in insns if i.op in ("sll", "srl", "sra")
          and re.search(r", (\d+)$", i.args)
          and int(re.search(r", (\d+)$", i.args).group(1)) not in (2, 16)]
    if sc:
        sig["scaleshift"] = str(len(sc)) + " shift(s) by a count other than 2/16"

    # -- a masked bit test
    for k, i in enumerate(insns):
        if i.op == "andi" and k + 1 < n and insns[k + 1].op in BRANCH:
            sig["bittest"] = "andi feeding a branch -- a bit test"
            break

    return sig


# ------------------------------------------------------------- keyword mapping
#
# How often each signal fires, measured over all 1819 `func_*.s` listings in
# asm/nonmatchings/ovl_town (`--prevalence`). A tag's weight is log(1/p), so a
# jump table (0.9%) is worth 4.7 and a `jal` (77.6%) is worth 0.25: knowing a
# function HAS a call narrows almost nothing. Re-measure after adding a signal
# and paste the number here; a signal that fires on most listings carries no
# information and belongs at multiplier 0.0, not in the scorer.
PREVALENCE = {
    "delaynop": .885, "call": .776, "symaddr": .717, "half": .523,
    "loadnop": .438, "regcopy": .355, "byte": .328, "install": .302,
    "multiexit": .300, "saved": .260, "rmw": .236, "leaf": .224,
    "scaleshift": .222, "storerun": .217, "noframe": .208, "reload": .196,
    "compare": .194, "forwardarg": .170, "deadparam": .145,
    "stackslot": .138, "constmat": .131, "loop": .114, "branchy": .112,
    "constaddr": .091, "manyblocks": .079, "s16cast": .076, "bittest": .070,
    "shzero": .069, "mult": .044, "indirect": .034, "ternary": .033,
    "div": .028, "abs": .020, "rodata": .013, "blockmove": .011,
    "jumptable": .009,
}

# tag -> (hand multiplier, [(keyword regex, keyword weight), ...])
#
# The multiplier is a judgement on top of the measured rarity -- 0.0 DISCARDS
# the signal from scoring while leaving it visible in the brief's signal line.
# Keyword weight is how specifically that word names the tag. Matched against
# the LOWERCASED heading only: the heading is what the agent reads, so a match
# it cannot see in the heading is a match it cannot dismiss.

TAGS = {
    "jumptable": (1.0, [
        (r"jump table", 3.0), (r"case_values_threshold", 3.0),
        (r"\bswitch\b", 2.5), (r"case label", 2.5), (r"\bcase\b", 1.5),
        (r"jtbl", 2.0), (r"comparison tree", 2.0), (r"range split", 2.0),
        (r"\barms?\b", 0.6),
    ]),
    "rodata": (1.0, [
        (r"\.rodata|rodata", 3.0), (r"jump table", 2.0),
        (r"initialiser", 1.5), (r"string constant", 2.0),
        (r"align", 1.5), (r"local arrays?", 2.0),
    ]),
    "loop": (1.0, [
        (r"\bloop", 2.0), (r"\bgiv\b|\bgivs\b", 2.5), (r"induction", 2.5),
        (r"preheader", 2.5), (r"\bbiv\b", 2.5), (r"invariant", 2.0),
        (r"hoist", 1.5), (r"strength-reduce", 2.0), (r"counter", 1.5),
        (r"walked pointer|pointer walk", 2.0), (r"\bwhile\b", 1.5),
        (r"back-edge", 2.5), (r"move_movables", 2.0), (r"\bi\+\+", 2.0),
        (r"cursor", 1.2), (r"\bbreak\b", 1.0),
    ]),
    "div": (1.0, [
        (r"divid|divis|divide", 3.0), (r"quotient", 3.0),
        (r"modulo|% \(?1 <<|`% c`|/ c % c|x % ", 2.5),
        (r"floor_log2", 2.0), (r"power-of-two", 2.0),
        (r"mfhi|mflo", 2.0), (r"magic", 2.0),
    ]),
    "mult": (1.0, [
        (r"expand_mult", 3.0), (r"\bproduct\b", 2.5),
        (r"shift-and-add", 2.5), (r"mfhi|mflo", 2.5),
        (r"<< 7|\* 128|<< k|\* \(1 << k\)", 2.0), (r"\bscale", 1.5),
    ]),
    "install": (1.0, [
        (r"install", 3.0), (r"function-pointer table", 3.0),
        (r"handler", 2.5), (r"descriptor", 2.0),
        (r"address-take|address of a|address-taken", 2.0),
        (r"slot (array|table|call)|handler slot", 2.0),
    ]),
    "indirect": (1.0, [
        (r"indirect call", 3.0), (r"function-pointer table", 2.5),
        (r"jalr", 3.0), (r"slot (array|table|call)", 2.5),
        (r"handler", 1.5),
        (r"\barity\b", 1.5),      # unbounded, this matched "granularity"
    ]),
    "saved": (1.0, [
        (r"callee-saved", 3.0), (r"\bspill", 2.5), (r"global-alloc", 2.0),
        (r"allocation priority|allocno|priority", 2.0),
        (r"live (range|length)", 2.0), (r"reg_n_refs", 2.0),
        (r"\.greg|`\.greg`", 2.0), (r"\$s\b|\$sn\b|lower `\$s`", 2.0),
        (r"local-alloc", 1.5),
    ]),
    # DISCARDED (weight 0): fires on 88.5% of this unit's listings.
    "delaynop": (0.0, [
        (r"delay slot", 2.5), (r"\bnop\b", 2.5), (r"\breorg\b", 2.0),
        (r"load-delay|load delay", 2.0), (r"cross-jump", 1.0),
    ]),
    "loadnop": (1.0, [
        (r"load-delay|load delay", 3.0), (r"\bnop\b", 2.0),
        (r"delay slot", 1.5), (r"pinned", 1.2),
    ]),
    "half": (1.0, [
        (r"halfword|half-word|half stores?", 3.0),
        (r"\blhu?\b|`lhu`|`lh`", 2.5), (r"\bsh\b|`sh`", 2.5),
        (r"s16|u16|himode", 2.0), (r"\bshort\b", 1.5), (r"narrow", 1.0),
    ]),
    "byte": (1.0, [
        (r"\bbytes?\b", 2.5), (r"lbu|`lbu`", 2.5), (r"\bsb\b|`sb`", 2.5),
        (r"\bu8\b|qimode", 2.0), (r"narrow", 0.8),
    ]),
    "shzero": (1.0, [
        (r"store of zero to a half", 4.0), (r"zero to a half", 3.5),
        (r"halfword", 1.5),
    ]),
    "s16cast": (1.0, [
        (r"sign.extension|sign.extended|sign extension", 3.0),
        (r"`sll`|`sra`|sll/sra|sll`/`sra", 3.0),
        (r"narrowing|truncat", 2.0), (r"\(s16\)|\bs16\b", 1.8),
        (r"widening", 1.5), (r"zero.extend", 1.5),
    ]),
    # near-ubiquitous (71.7%): held down further, as a tie-break only.
    "symaddr": (0.5, [
        (r"%lo|%hi|lo_sum|`high`", 2.5), (r"\bsymbol\b", 2.0),
        (r"declared size", 2.0), (r"addressing", 1.5),
        (r"materialis", 1.5), (r"base pointer|pointer local", 1.2),
    ]),
    "constaddr": (1.0, [
        (r"constant address|constant-address", 3.5),
        (r"constant base|constant-base", 3.0),
        (r"out-of-image", 2.5), (r"declared size", 2.0),
        (r"`ori`|\bori\b", 2.5), (r"split", 1.5),
    ]),
    "manyblocks": (1.0, [
        (r"basic.block|block boundary|block head", 2.5),
        (r"thread_jumps", 2.5), (r"cross-jump", 2.0),
        (r"if/else|\barms?\b", 1.5), (r"\bgoto\b", 1.8),
        (r"\blabels?\b", 1.5), (r"layout|laid out", 1.5),
        (r"\bcse\b", 1.0), (r"block-local|block-scoped", 1.8),
    ]),
    "deadparam": (1.0, [
        (r"dead \*?middle\*? parameter|dead middle", 4.0),
        (r"\barity\b", 2.5), (r"forwarded parameter", 2.5),
        (r"unread|dead trailing", 2.5), (r"\bprototype\b", 2.0),
        (r"parameter", 1.5), (r"argument", 1.2),
    ]),
    "noframe": (1.0, [
        (r"\bleaf\b", 3.0), (r"prologue", 2.0), (r"frame", 2.0),
        (r"stack (slot|local|buffer|parameter)", 1.5),
    ]),
    "abs": (1.0, [
        (r"\babs\b|abs\(\)|expand_abs", 4.0),
        (r"conditional negate|negation|negate", 2.5),
    ]),
    "ternary": (1.0, [
        (r"\?:|ternary|cond_expr", 3.0), (r"store-flag|0/1 pair", 3.0),
        (r"if \(c\) x = 1", 3.0), (r"diamond", 2.0),
        (r"\bcopies?\b|reg-reg copy|register copy", 1.5),
    ]),
    "blockmove": (1.0, [
        (r"block move|block-move", 4.0), (r"struct copy|whole-struct", 3.0),
        (r"memcpy", 3.0), (r"alignment", 2.0), (r"struct-array", 1.5),
    ]),
    "leaf": (1.0, [
        (r"\bleaf\b", 3.0),
    ]),
    "call": (0.7, [
        (r"\bcalls?\b|\bjal\b", 1.5), (r"argument", 1.2),
        (r"cross-jump", 1.2), (r"callee", 1.2), (r"\bcall_insn", 1.5),
    ]),
    "branchy": (1.0, [
        (r"if/else|\barms?\b", 2.5), (r"\bgoto\b", 2.0),
        (r"thread_jumps|cross-jump", 2.0), (r"\bgate\b", 2.5),
        (r"nested ifs|one-armed", 2.5), (r"&&|\|\||de morgan", 2.5),
        (r"invert|inversion", 2.0), (r"\blabels?\b", 1.5),
        (r"layout|laid out|block order", 2.0), (r"if-convert", 2.5),
        (r"source order", 1.2), (r"\bbranch", 1.5),
    ]),
    "multiexit": (1.0, [
        (r"early `?return`?|early return", 3.0),
        (r"`return`|\breturn\b", 2.0), (r"\bexits?\b", 2.5),
        (r"laid out inline|inline", 1.5), (r"both arms leave", 3.0),
        (r"accumulator", 1.2),
    ]),
    "regcopy": (1.0, [
        (r"reg-reg copy|register copy|reg-to-reg", 3.0),
        (r"\bcopies\b|\bcopy\b", 2.0), (r"\bmove\b", 1.8),
        (r"canonicalis", 2.0), (r"jump2", 2.5), (r"\bcse\b", 1.2),
        (r"reg_uservar_p|uservar", 2.0), (r"equivalent|update_equiv", 2.0),
    ]),
    "stackslot": (1.0, [
        (r"stack slot|aggregate stack|home slot", 3.0),
        (r"declaration order", 2.5), (r"stack (local|buffer|parameter|record)", 2.5),
        (r"\blocals?\b", 1.5), (r"scratch local|temporary|\btemps?\b", 1.8),
        (r"\bspill", 1.8), (r"declared", 1.2),
    ]),
    "storerun": (1.0, [
        (r"chained assignment", 3.0), (r"right-to-left", 3.0),
        (r"disjoint displacement|distinct constant offsets|one base", 3.0),
        (r"store (pair|order|chain)|stores", 2.0),
        (r"descending|source order", 1.8), (r"\bfields?\b", 1.2),
        (r"\bstores?\b", 1.5),
    ]),
    "compare": (1.0, [
        (r"comparison|compare", 2.5), (r"operand order", 2.5),
        (r"predicate|flip", 2.5), (r"`beqz`|`blez`|`bnez`", 2.5),
        (r"\bguard\b", 2.0), (r"\bbounds?\b|range check", 2.0),
        (r"\btests?\b", 1.5), (r"store-flag|0/1 pair", 2.0),
    ]),
    "forwardarg": (1.0, [
        (r"forwarded parameter|forwarded", 3.0), (r"\barity\b", 2.5),
        (r"outgoing argument", 3.0), (r"call site", 2.0),
        (r"grandcaller", 3.0), (r"\bprototype\b", 2.0),
        (r"phantom argument", 2.5), (r"argument", 1.5),
        (r"parameter", 1.2), (r"permute its arguments", 3.0),
    ]),
    "constmat": (1.0, [
        (r"\bconstants?\b", 2.5), (r"materialis", 2.5),
        (r"two locals initialised", 3.0), (r"\bliteral", 2.0),
        (r"immediate", 2.0), (r"a local per constant", 3.0),
        (r"hoisted constant", 2.0),
    ]),
    "reload": (1.0, [
        (r"re-dereference|re-read|reload|read .* back", 3.0),
        (r"\bcache[sd]?\b", 2.5), (r"\bcse\b", 1.8),
        (r"two loads|loads of one|load count", 2.5),
        (r"invalidat", 2.5), (r"volatile", 2.0), (r"forward the store", 2.5),
    ]),
    "rmw": (1.0, [
        (r"read-modify-write", 3.5), (r"in-place|in place", 2.5),
        (r"`&=`|`\|=`|`-=`|`\+=`|compound", 2.5),
        (r"increment|decrement|\+\+|--`", 2.0), (r"\bmask", 1.5),
        (r"accumulat", 1.5),
    ]),
    "scaleshift": (1.0, [
        (r"\bshifts?\b|`sll`|`srl`|`sra`", 2.5),
        (r"mask-then-scale|shift/and/scale", 3.0),
        (r"\bscale", 2.0), (r"<< \d|<< k|\* \d|\* \(1 <<", 2.5),
        (r"\bindex\b|subscript", 1.5), (r"floor_log2", 2.0),
    ]),
    "bittest": (1.0, [
        (r"bit tests?", 3.5), (r"\bmask", 2.5), (r"andi", 2.5),
        (r"\bflag", 2.0), (r"nested ifs", 2.5), (r"`\|`|`&`|`\|\|`", 2.0),
    ]),
}

# a heading that talks about these is wrong for a function with no `jal`
LEAF_PENALTY = re.compile(
    r"\bjal\b|\bcalls?\b|callee|argument|cross-jump|forwarded|outgoing")
# ...and these are wrong for a function with no back-edge
NOLOOP_PENALTY = re.compile(
    r"\bloop|\bgiv\b|\bgivs\b|induction|preheader|\bbiv\b|move_movables"
    r"|strength-reduce|walked pointer|pointer walk")

# NOTE: matched with .search, not .match -- the un-anchored alternatives below
# (e.g. "negative results worth") silently never fired under re.match, which
# is why it is .search now.
# Sections that are not shape levers: numbered chapter heads, running-total
# bookkeeping entries that each hold several unrelated sub-results, and the
# cautions the brief's ALWAYS block already carries. None of them can be
# "the lever that applies to this target", so none is ever suggested.
SKIP_TITLE = re.compile(
    r"^\d+\.\s|^the pass order|^index|^corollary|^both halves|^\.\.\.and"
    r"|negative results worth|^three more measurements|^two smaller measurements"
    r"|^three corroborations|^citations re-anchored|^three no-ops"
    r"|^measured dead ends|^a differing count that matches"
    r"|^one cell list|^scope correction|^pass order correction"
    r"|^read the scheduler|^which of two givs")


def headings():
    """Every lever heading with its CURRENT line number, via leverindex."""
    lines = io.open(DOC, encoding="utf-8").read().split("\n")
    out = []
    for item in leverindex.build(lines):
        m = re.match(r"^\s*- (.*?)\s+`:(\d+)`$", item)
        if m:
            out.append((m.group(1), int(m.group(2))))
    return out


_SPEC = {}


def specificity(titles):
    """How much each keyword NARROWS the 390 headings.

    A pattern that matches a third of the document ("store", "register")
    carries almost nothing; one that matches two headings ("expand_abs")
    is decisive. Computed from the headings themselves so it tracks the
    document instead of my guesses about it, and cached per heading set.
    """
    key = len(titles)
    if key in _SPEC:
        return _SPEC[key]
    n = float(len(titles) or 1)
    spec = {}
    for tag, (_, kws) in TAGS.items():
        for pat, _w in kws:
            if pat in spec:
                continue
            rx = re.compile(pat)
            df = sum(1 for x in titles if rx.search(x))
            spec[pat] = math.log(n / (df + 1.0)) / math.log(n)
    _SPEC.clear()
    _SPEC[key] = spec
    return spec


# Scoring shape, tuned against the labelled set in the report (recall@8 on
# 230 doc-attested function/lever pairs). SPEC_* is how much a keyword's
# rarity ACROSS THE 390 HEADINGS counts: `thread_jumps` or `expand_abs`
# matches two headings and is near-decisive, `store` matches sixty and is
# nearly free. REST is how much the second and later signals corroborate.
SPEC_A, SPEC_B, REST = 0.25, 1.75, 0.30
FLOOR = 0.9


def score_one(title, sig, spec):
    low = title.lower()
    parts = []
    for tag, _witness in sig.items():
        mult, kws = TAGS.get(tag, (0.0, []))
        if mult <= 0:
            continue
        tw = mult * math.log(1.0 / PREVALENCE.get(tag, 0.3))
        best, bestpat = 0.0, None
        for pat, kw in kws:
            if re.search(pat, low):
                sp = max(spec.get(pat, 1.0), 0.0)
                v = kw * (SPEC_A + SPEC_B * sp)
                if v > best:
                    best, bestpat = v, pat
        if best:
            parts.append((tw * best, tag, bestpat))
    if not parts:
        return 0.0, []
    parts.sort(reverse=True)
    # The RAREST signal that speaks to this heading dominates; the rest only
    # corroborate. Summing instead rewards a long heading for containing many
    # ordinary words, which is what made the first version rank by verbosity.
    total = parts[0][0] + REST * sum(p[0] for p in parts[1:])
    if "leaf" in sig and LEAF_PENALTY.search(low):
        total -= 3.0
    if "loop" not in sig and NOLOOP_PENALTY.search(low):
        total -= 5.0
    if "jumptable" not in sig and "switch" in low:
        total -= 2.0
    # MEASURED AND DROPPED: dividing by heading length, on the theory that a
    # long heading accumulates incidental vocabulary. Both directions moved
    # recall@8 by under two labelled pairs either way -- it is noise, and the
    # knob was not worth keeping.
    return total, parts


def pick(listing_text, limit=8, floor=FLOOR):
    """-> [(score, title, lineno, [(tag, witness), ...]), ...], best first."""
    sig = signals(listing_text)
    if not sig:
        return []
    cand = [(ti, ln) for ti, ln in headings()
            if not SKIP_TITLE.search(ti.lower())]
    spec = specificity([ti.lower() for ti, _ in cand])
    out = []
    for title, ln in cand:
        s, hits = score_one(title, sig, spec)
        if s < floor:
            continue
        why = [(tag, sig[tag]) for _, tag, _ in hits[:3]]
        out.append((s, title, ln, why))
    out.sort(key=lambda r: (-r[0], r[2]))
    return out[:limit]


# ------------------------------------------------------------------- rendering

def render(targets, limit=8):
    """targets: [(name, listing_text)] -> the brief's matched-lever block.

    HEADINGS AND LINE NUMBERS ONLY. The whole point is that an agent does not
    pay for 16,690 lines, so a renderer that inlines bodies has made things
    worse; it ends with a ready-to-run `sed` for the top matches instead.
    Attribution is PER TARGET, because a nine-member group has nine shapes.
    """
    lines, tops = [], []
    for name, text in targets:
        sig = signals(text)
        rows, seen = [], set()
        for row in pick(text, limit=limit * 3):
            if row[1] in seen:      # the doc has a few duplicated headings
                continue
            seen.add(row[1])
            rows.append(row)
            if len(rows) >= limit:
                break
        shown = [k for k in sorted(sig, key=lambda k: PREVALENCE.get(k, .3))
                 if PREVALENCE.get(k, .3) <= 0.45][:7]
        lines.append("`" + name + "` -- "
                     + (", ".join(shown) if shown else "nothing distinctive"))
        if not rows:
            lines.append("  (no shape-matched lever; the ALWAYS list is all "
                         "there is, which is the expected case for a plain "
                         "leaf function)")
            continue
        for _s, title, ln, why in rows:
            trig = "/".join(tag for tag, _w in why)
            lines.append("  - `:" + str(ln) + "` " + title + "  [" + trig + "]")
        tops.append((name, rows[0][2]))
    if tops:
        lines.append("")
        seen_ln = set()
        tops = [x for x in tops
                if x[1] not in seen_ln and not seen_ln.add(x[1])]
        for name, ln in tops[:3]:
            lines.append("    sed -n '" + str(ln) + "," + str(ln + 60)
                         + "p' docs/matching-levers.md   # top match for "
                         + name)
    return "\n".join(lines)


def main():
    av = sys.argv[1:]
    if not av:
        print(__doc__)
        return 2
    if av[0] == "--prevalence":
        d = av[1]
        cnt, tot = {}, 0
        for f in sorted(os.listdir(d)):
            if not f.startswith("func_") or not f.endswith(".s"):
                continue
            s = signals(io.open(os.path.join(d, f), encoding="utf-8").read())
            if not s:
                continue
            tot += 1
            for k in s:
                cnt[k] = cnt.get(k, 0) + 1
        print(str(tot) + " listings")
        for k in sorted(cnt, key=lambda k: -cnt[k]):
            pc = 100.0 * cnt[k] / tot
            print("  " + k.ljust(12) + str(cnt[k]).rjust(5) + "  "
                  + format(pc, "5.1f") + "%")
        return 0
    only = av[0] == "--signals"
    paths = av[1:] if only else av
    for p in paths:
        text = io.open(p, encoding="utf-8").read()
        name = os.path.basename(p)[:-2]
        if only:
            for k, v in sorted(signals(text).items()):
                print(k.ljust(12) + v)
        else:
            print(render([(name, text)]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
