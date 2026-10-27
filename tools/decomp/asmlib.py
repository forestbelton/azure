#!/usr/bin/env python3
"""Shared pipeline for the decomp tools: compile a candidate, read a target
listing, and decode MIPS words into comparable fields.

This exists so `verify.py` and `advise.py` cannot drift apart on HOW a
candidate is built -- that pipeline (ccpsx's include paths, maspsx's aspsx
version and -G, gas's flags) is the part that has broken silently before.
Adding `-Ipsyq/include` to one caller and not the others is exactly the bug
this file prevents a repeat of.

Everything here is pure: no printing, no argv parsing.
"""
import os
import re
import subprocess

SP = os.environ.get('AZURE_SCRATCH', '/tmp/azure-decomp')
MASPSX = os.path.join(os.environ.get('MASPSX_DIR', 'vendor/maspsx'), 'maspsx.py')
PROJ = os.environ.get(
    'AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

# cc1psx needs BOTH include roots: include/common.h pulls in sys/types.h,
# libcd.h, libgte.h and libgpu.h, which live only under psyq/include. This is
# the Makefile's CFLAGS, minus -S/-o.
CFLAGS = ['-O2', '-G8', '-Iinclude', '-Ipsyq/include']


# A unit built with MASPSXFLAGS_G8 or _G2 leaves gas at a nonzero -G, so an
# UNDEFINED symbol whose `.extern SYM,size` size falls in (0, G] goes
# $gp-relative. Getting this wrong is silent: the function simply does not
# match, and nothing says why. The Makefile is the authority -- these are the
# `build/csrc/<unit>.o: MASPSXFLAGS := ...` lines -- so read it rather than
# guessing from the unit's name. A unit with no such rule gets the default
# MASPSXFLAGS, which forces -G0, and that includes some gprel_ units: the
# prefix records why the unit was carved out, not how it is assembled.
_G_BY_UNIT = None


def gas_g_for_unit(unit):
    """The gas -G this unit is built with, from the Makefile's own rules."""
    global _G_BY_UNIT
    if _G_BY_UNIT is None:
        _G_BY_UNIT = {}
        try:
            mk = open(os.path.join(PROJ, 'Makefile')).read()
        except OSError:
            mk = ''
        # join the backslash continuations so one rule is one line
        mk = mk.replace('\\\n', ' ')
        for m in re.finditer(r'^(build/csrc/\S+\.o(?:\s+build/csrc/\S+\.o)*)'
                             r'\s*:\s*MASPSXFLAGS\s*:?=\s*\$\(MASPSXFLAGS_G(\d+)\)',
                             mk, re.M):
            for o in m.group(1).split():
                _G_BY_UNIT[os.path.basename(o)[:-2]] = int(m.group(2))
    return _G_BY_UNIT.get(unit, 0)


_SIZES_BY_UNIT = {}


def size_syms_for_unit(unit):
    """The symbols the unit gives a `.size` to, from its own __asm__ block.

    cc1psx emits `.extern SYM,size` from a DECLARED type, and gas goes
    $gp-relative only for a symbol whose size is in (0, G]. A carved unit
    therefore carries an `__asm__(".sdata\n\t.size SYM,4\n\t.text")` block
    naming every small global it wants $gp-relative. A park file compiled
    standalone does NOT inherit that block, so every reference to one of
    these symbols costs a word more than it does in the unit -- which is a
    whole word of mis-measurement, and it bit func_8004DE88 for two sessions.
    """
    if unit not in _SIZES_BY_UNIT:
        try:
            txt = open(os.path.join(PROJ, 'csrc', unit + '.c')).read()
        except OSError:
            txt = ''
        _SIZES_BY_UNIT[unit] = set(re.findall(r'\.size (\w+),', txt))
    return _SIZES_BY_UNIT[unit]


def missing_size_syms(src, unit):
    """Symbols `src` references that its unit sizes and `src` does not."""
    try:
        body = open(src).read()
    except OSError:
        return []
    if '.size' in body:
        return []
    return sorted(s for s in size_syms_for_unit(unit)
                  if re.search(r'\b' + s + r'\b', body))


class CompileError(Exception):
    pass


def compile_asm(src, cflags=()):
    """ccpsx -S the candidate; return the assembly text.

    Raises CompileError, including for wine's case-insensitivity trap: with
    zz_gA.s already in the scratch directory, ccpsx writes THAT file for
    zz_ga.c, exits 0, and the output we want never appears.
    """
    os.makedirs(SP, exist_ok=True)
    base = os.path.splitext(os.path.basename(src))[0]
    s = os.path.join(SP, base + '.s')
    try:
        os.remove(s)
    except OSError:
        pass
    r = subprocess.run([os.path.join(PROJ, 'tools/ccpsx')] + CFLAGS + list(cflags)
                       + ['-S', '-o', s, src], cwd=PROJ,
                       capture_output=True, text=True)
    if r.returncode:
        raise CompileError(r.stdout + r.stderr)
    if not os.path.exists(s):
        clash = [f for f in os.listdir(SP)
                 if f.lower() == os.path.basename(s).lower() and f != os.path.basename(s)]
        msg = 'ccpsx exited 0 but wrote no %s' % s
        if clash:
            msg += ('\n  it wrote %s/%s instead: wine is case-insensitive, so those'
                    '\n  two names are one file. Rename %s so it differs by more than case.'
                    % (SP, clash[0], src))
        else:
            msg += ('\n  no case clash in the scratch directory; try again (concurrent'
                    '\n  ccpsx runs share one wine prefix and can lose an output).')
        raise CompileError(msg)
    return open(s).read()


def assemble(asm, name, gasG=0):
    """maspsx + gas the assembly; return the object path.

    gasG models a unit carved out with MASPSXFLAGS_G8/_G2 -- gas then emits
    GPREL16 for a symbol whose `.extern SYM,size` size is in (0, G].
    """
    o = os.path.join(SP, name + '.o')
    cmd = ['python3', MASPSX, '--aspsx-version=2.56', '--force-stdin',
           '--run-assembler', '--gnu-as-path=mips64-linux-gnu-as',
           '--use-comm-section', '-G%d' % (gasG or 8)]
    if gasG:
        cmd.append('--dont-force-G0')
    cmd += ['-EL', '-32', '-Iinclude', '-march=r3000', '-mtune=r3000',
            '-no-pad-sections', '-O1', '-o', o]
    r = subprocess.run(cmd, input=asm, text=True, cwd=PROJ, capture_output=True)
    if r.returncode:
        raise CompileError(r.stderr)
    return o


def object_words(o):
    """(words, relocs) for an object's .text.

    words is [(addr, word, text)]; relocs maps addr -> R_MIPS_* suffix.
    -z keeps runs of zero words, which is what shows the hazard nops maspsx
    inserts after mflo/mult.
    """
    d = subprocess.run(['mips64-linux-gnu-objdump', '-d', '-z', '-r', '-j', '.text', o],
                       capture_output=True, text=True).stdout
    words, relocs = [], {}
    for ln in d.split('\n'):
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', ln)
        if m:
            words.append((int(m.group(1), 16), int(m.group(2), 16), m.group(3).strip()))
            continue
        m = re.match(r'\s*([0-9a-f]+):\s+R_MIPS_(\w+)', ln)
        if m:
            relocs[int(m.group(1), 16)] = m.group(2)
    return words, relocs


def build(src, gasG=0, cflags=()):
    """compile + assemble + disassemble a candidate."""
    asm = compile_asm(src, cflags)
    name = os.path.splitext(os.path.basename(src))[0]
    return object_words(assemble(asm, name, gasG))


def target_words(unit, func):
    """[(word, text)] from splat's nonmatchings listing."""
    path = '%s/asm/nonmatchings/%s/%s.s' % (PROJ, unit, func)
    out = []
    for ln in open(path):
        m = re.match(r'\s*/\* \w+ \w+ ([0-9A-F]{8}) \*/\s+(.*)', ln)
        if m:
            b = bytes.fromhex(m.group(1))
            out.append((int.from_bytes(b, 'little'), m.group(2).strip()))
    if not out:
        raise CompileError('no words parsed from %s (run make splat?)' % path)
    return out


def target_vram(unit, func):
    """The function's vram start, from the listing's first row."""
    path = '%s/asm/nonmatchings/%s/%s.s' % (PROJ, unit, func)
    for ln in open(path):
        m = re.match(r'\s*/\* \w+ (\w+) [0-9A-F]{8} \*/', ln)
        if m:
            return int(m.group(1), 16)
    return 0


def listing_addend(text):
    """The addend splat's %hi/%lo operand implies, or None if there is none."""
    m = re.search(r'%(?:hi|lo)\(([^)]*)\)', text)
    if not m:
        return None
    a = re.search(r'([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*$', m.group(1))
    if not a:
        return 0
    v = int(a.group(2), 16 if a.group(2).lower().startswith('0x') else 10)
    return v if a.group(1) == '+' else -v


# ---------------------------------------------------------------- decoding
#
# Classification works on the decoded fields, never on disassembly text: the
# two sides are printed by different tools (objdump says `li v0,-1` and
# `move a2,a0` where splat says `addiu $v0, $zero, -0x1` and
# `addu $a2, $a0, $zero`), and branch operands are a label on one side and an
# address on the other. The encoded word is the only common ground.

REGS = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']

OPS = {0x02: 'j', 0x03: 'jal', 0x04: 'beq', 0x05: 'bne', 0x06: 'blez',
       0x07: 'bgtz', 0x08: 'addi', 0x09: 'addiu', 0x0a: 'slti', 0x0b: 'sltiu',
       0x0c: 'andi', 0x0d: 'ori', 0x0e: 'xori', 0x0f: 'lui',
       0x20: 'lb', 0x21: 'lh', 0x22: 'lwl', 0x23: 'lw', 0x24: 'lbu',
       0x25: 'lhu', 0x26: 'lwr', 0x28: 'sb', 0x29: 'sh', 0x2a: 'swl',
       0x2b: 'sw', 0x2e: 'swr'}

FUNCTS = {0x00: 'sll', 0x02: 'srl', 0x03: 'sra', 0x04: 'sllv', 0x06: 'srlv',
          0x07: 'srav', 0x08: 'jr', 0x09: 'jalr', 0x10: 'mfhi', 0x11: 'mthi',
          0x12: 'mflo', 0x13: 'mtlo', 0x18: 'mult', 0x19: 'multu',
          0x1a: 'div', 0x1b: 'divu', 0x20: 'add', 0x21: 'addu',
          0x22: 'sub', 0x23: 'subu', 0x24: 'and', 0x25: 'or', 0x26: 'xor',
          0x27: 'nor', 0x2a: 'slt', 0x2b: 'sltu'}

# op == 1 is REGIMM; the branch is in the rt field.
REGIMM = {0x00: 'bltz', 0x01: 'bgez', 0x10: 'bltzal', 0x11: 'bgezal'}

LOADS = set('lb lh lwl lw lbu lhu lwr'.split())
STORES = set('sb sh swl sw swr'.split())
BRANCHES = set('beq bne blez bgtz bltz bgez bltzal bgezal'.split())


class Insn:
    """One decoded MIPS word.

    `shape` is what alignment keys on: the mnemonic plus the immediate, with
    registers left out, so a pure register difference still ALIGNS (and is
    then reported as such) while a different constant does not.
    """

    __slots__ = ('word', 'text', 'op', 'rs', 'rt', 'rd', 'shamt', 'imm',
                 'simm', 'rel')

    def __init__(self, word, text):
        self.word = word
        self.text = text
        self.rel = False
        o = word >> 26
        self.rs = (word >> 21) & 31
        self.rt = (word >> 16) & 31
        self.rd = (word >> 11) & 31
        self.shamt = (word >> 6) & 31
        self.imm = word & 0xFFFF
        self.simm = self.imm - 0x10000 if self.imm & 0x8000 else self.imm
        if word == 0:
            self.op = 'nop'
        elif o == 0:
            self.op = FUNCTS.get(word & 63, 'r%02x' % (word & 63))
        elif o == 1:
            self.op = REGIMM.get(self.rt, 'regimm%02x' % self.rt)
        else:
            self.op = OPS.get(o, 'op%02x' % o)

    @property
    def regs(self):
        """The register fields this mnemonic actually uses, in operand order."""
        o = self.op
        if o == 'nop':
            return ()
        if o in ('sll', 'srl', 'sra'):
            return (self.rd, self.rt)
        if o in ('sllv', 'srlv', 'srav'):
            return (self.rd, self.rt, self.rs)
        if o in ('mult', 'multu', 'div', 'divu'):
            return (self.rs, self.rt)
        if o in ('mfhi', 'mflo'):
            return (self.rd,)
        if o in ('mthi', 'mtlo', 'jr'):
            return (self.rs,)
        if o in FUNCTS.values():
            return (self.rd, self.rs, self.rt)
        if o == 'lui':
            return (self.rt,)
        if o in LOADS or o in STORES:
            return (self.rt, self.rs)
        if o in ('beq', 'bne'):
            return (self.rs, self.rt)
        if o in BRANCHES:
            return (self.rs,)
        if o in ('j', 'jal'):
            return ()
        return (self.rt, self.rs)

    @property
    def imm_key(self):
        """The constant part of the shape, or None where it is not comparable.

        A branch displacement and a jump target are deliberately excluded: the
        same source can place a block at a different offset, and the SHIFT is
        what we want to detect separately rather than have it desynchronise
        every later alignment.
        """
        if self.op in BRANCHES or self.op in ('j', 'jal'):
            return None
        if self.op in ('sll', 'srl', 'sra'):
            return self.shamt
        if self.op in ('addiu', 'addi', 'slti', 'sltiu'):
            return self.simm
        if self.op in ('andi', 'ori', 'xori', 'lui'):
            return self.imm
        if self.op in LOADS or self.op in STORES:
            return self.simm
        return None

    @property
    def shape(self):
        return (self.op, self.imm_key)

    def __repr__(self):
        return '%s %s' % (self.op, ','.join(REGS[r] for r in self.regs))


def decode_built(words):
    return [Insn(w, t) for _, w, t in words]


def decode_target(words):
    return [Insn(w, t) for w, t in words]
