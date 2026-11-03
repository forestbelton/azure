#!/usr/bin/env python3
"""Shared pipeline for the decomp tools: compile a candidate, read a target
listing, and decode MIPS words into comparable fields.

This exists so `verify.py` and `advise.py` cannot drift apart on HOW a
candidate is built -- that pipeline (the compiler's include paths, maspsx's aspsx
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


# A unit left at a nonzero gas -G sends an UNDEFINED symbol whose
# `.extern SYM,size` size falls in (0, G] $gp-relative. Getting this wrong is
# silent: the function simply does not match, and nothing says why. The
# Makefile is the authority -- these are the
# `build/csrc/<unit>.o: MASPSXFLAGS := ...` lines -- so read it rather than
# guessing from the unit's name.
#
# As of 2026-11-03 NO unit has such a rule for a nonzero -G: MASPSXFLAGS_G8 and
# MASPSXFLAGS_G2 were deleted when the eleven carved gprel_* units were
# converted to file-scope definitions of their $gp objects, which get the same
# %gp_rel at the forced -G0. So this returns 0 for everything, which is right,
# and will pick a rule up again by itself if one is ever added. The `gprel_`
# prefix records why a unit was carved out, not how it is assembled.
_G_BY_UNIT = None


def _makefile_g_map(var):
    """{unit: -G} from the Makefile's per-object `VAR := $(VAR_G<n>)` rules.

    Handles PATTERN rules as well as explicit targets, and that is not
    optional: on 2026-11-03 the per-unit overlay lines were replaced by one
    `build/csrc/ovl_%.o:` pattern, this parser stopped matching, and
    cc1_g_for_unit fell back to its default of 8 for every overlay -- which is
    precisely the failure its own docstring calls "silent in the worst way".
    Two parks silently re-measured 10 words longer before it was caught.
    Explicit targets win over patterns; the longest pattern prefix wins among
    patterns.
    """
    exact, pats = {}, []
    try:
        mk = open(os.path.join(PROJ, 'Makefile')).read()
    except OSError:
        return exact, pats
    # join the backslash continuations so one rule is one line
    mk = mk.replace('\\\n', ' ')
    for m in re.finditer(r'^(build/csrc/\S+\.o(?:\s+build/csrc/\S+\.o)*)'
                         r'\s*:\s*' + var + r'\s*:?=\s*\$\(' + var + r'_G(\d+)\)',
                         mk, re.M):
        for o in m.group(1).split():
            stem = os.path.basename(o)[:-2]
            if '%' in stem:
                pats.append((stem, int(m.group(2))))
            else:
                exact[stem] = int(m.group(2))
    pats.sort(key=lambda kv: -len(kv[0]))
    return exact, pats


def _g_lookup(exact, pats, unit, default):
    if unit in exact:
        return exact[unit]
    import fnmatch
    for pat, g in pats:
        if fnmatch.fnmatchcase(unit, pat.replace('%', '*')):
            return g
    return default


def gas_g_for_unit(unit):
    """The gas -G this unit is built with, from the Makefile's own rules."""
    global _G_BY_UNIT
    if _G_BY_UNIT is None:
        _G_BY_UNIT = _makefile_g_map('MASPSXFLAGS')
    return _g_lookup(_G_BY_UNIT[0], _G_BY_UNIT[1], unit, 0)


_CC1_G_BY_UNIT = None


def cc1_g_for_unit(unit):
    """The cc1psx -G this unit is compiled with, from the Makefile's own rules.

    The resident executable is -G8 and CFLAGS says so, but the OVERLAYS are
    -G0 and carry a per-unit `build/csrc/<unit>.o: CFLAGS := $(CFLAGS_G0)`
    override. Getting this wrong is silent in the worst way: at -G8 a global
    access comes out as the assembler-expanded `lui $at` / `%lo` pair instead
    of cc1psx's own split address, so every candidate the permuter compiles is
    wrong in the same way and the search cannot converge on anything. Read the
    Makefile rather than assume CFLAGS.
    """
    global _CC1_G_BY_UNIT
    if _CC1_G_BY_UNIT is None:
        _CC1_G_BY_UNIT = _makefile_g_map('CFLAGS')
    return _g_lookup(_CC1_G_BY_UNIT[0], _CC1_G_BY_UNIT[1], unit, 8)


_GP_DEFS_BY_UNIT = {}

# A file-scope tentative definition of a small global: `u32 D_80080ADC;`,
# `u8 D_800814D0[4];`, `u8 *D_80081540;`. Anchored at column 0, so a local
# declaration inside a function body cannot match, and `extern` excluded.
_GP_DEF_RE = re.compile(
    r'^(?!extern\b)([A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*\s*\**)\s*'
    r'(D_[0-9A-Fa-f]+)\s*(\[[^\]]*\])?\s*;', re.M)
# The one remaining `.sdata` size record (see csrc/unk_264E0.c).
_GP_SIZE_RE = re.compile(r'\.size (D_[0-9A-Fa-f]+),(\d+)')


def _gp_defs_in_text(txt):
    """{symbol: the source line that puts it in maspsx's tables}."""
    out = {}
    for ty, sym, arr in _GP_DEF_RE.findall(txt):
        ty = ty.strip()
        out[sym] = '%s%s%s%s;' % (ty, '' if ty.endswith('*') else ' ', sym, arr)
    for sym, size in _GP_SIZE_RE.findall(txt):
        out.setdefault(
            sym, '__asm__(".sdata\\n\\t.size %s,%s\\n\\t.text");' % (sym, size))
    return out


def gp_defs_for_unit(unit):
    """{symbol: definition line} for the small globals the unit DEFINES.

    cc1psx emits a bare-symbol load or store for a small global and leaves the
    addressing to the assembler. maspsx rewrites it to %gp_rel only for a
    symbol it saw defined in this translation unit: a C tentative definition
    becomes `.comm`, which lands in its `sbss_entries`, and a `.size` inside
    `.sdata` lands in `sdata_entries`. A unit therefore DEFINES every small
    global it wants $gp-relative -- which is what the original translation
    unit did. A park file compiled standalone has none of those definitions,
    so every reference to one costs a word more than it does in the unit:
    a whole word of mis-measurement, and it bit func_8004DE88 for two
    sessions.
    """
    if unit not in _GP_DEFS_BY_UNIT:
        try:
            txt = open(os.path.join(PROJ, 'csrc', unit + '.c')).read()
        except OSError:
            txt = ''
        _GP_DEFS_BY_UNIT[unit] = _gp_defs_in_text(txt)
    return _GP_DEFS_BY_UNIT[unit]


def missing_gp_defs(src, unit):
    """{symbol: line} that `src` references, its unit defines, and `src` lacks."""
    try:
        body = open(src).read()
    except OSError:
        return {}
    have = _gp_defs_in_text(body)
    return {s: line for s, line in sorted(gp_defs_for_unit(unit).items())
            if s not in have and re.search(r'\b' + s + r'\b', body)}


def size_syms_for_unit(unit):
    """Back-compatible alias: just the symbol names."""
    return set(gp_defs_for_unit(unit))


def missing_size_syms(src, unit):
    """Back-compatible alias: just the symbol names."""
    return sorted(missing_gp_defs(src, unit))


class CompileError(Exception):
    pass


def compile_asm(src, cflags=()):
    """Compile the candidate to assembly with tools/cdkgcc; return the text.

    This used to carry two workarounds for driving cc1psx under wine, both of
    which the native compiler makes unnecessary. Wine's filesystem is
    case-insensitive, so the name it wrote was not always the name we asked
    for -- `I1.c` came back as `i1.s` -- and concurrent runs sharing one wine
    prefix could leave a compile exiting 0 having written nothing at all. A
    native compiler writes exactly the path given to -o, so a missing output
    is now a real error rather than something to retry.

    Raises CompileError if the compile fails or writes nothing.
    """
    os.makedirs(SP, exist_ok=True)
    base = os.path.splitext(os.path.basename(src))[0]
    s = os.path.join(SP, base + '.s')
    r = subprocess.run([os.path.join(PROJ, 'tools/cdkgcc')] + CFLAGS + list(cflags)
                       + ['-S', '-o', s, src], cwd=PROJ,
                       capture_output=True, text=True)
    if r.returncode:
        raise CompileError(r.stdout + r.stderr)
    if not os.path.exists(s):
        raise CompileError('the compiler exited 0 but wrote no %s' % s)
    return open(s).read()


def assemble(asm, name, gasG=0):
    """maspsx + gas the assembly; return the object path.

    gasG models a unit left at a nonzero gas -G -- gas then emits GPREL16 for
    a symbol whose `.extern SYM,size` size is in (0, G]. No unit is built that
    way any more; see the note on gas_g_for_unit above.
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


def _listing_rows(unit, func):
    """The listing rows that belong to FUNC itself.

    A nonmatchings listing is not all .text: splat emits a switch's jump table
    into the same file, as `.word .L...` rows BEFORE the `glabel`, and those
    rows carry the same `/* rom vram WORD */` comment as an instruction. Taking
    every row shifts the whole comparison by the size of the table -- which is
    why func_8002168C read as "built 82 words, target 87" with every word
    wrong, and why no function with a jump table could ever verify.

    So rows are taken from `glabel FUNC` up to the next `glabel`. A listing
    with no glabel at all falls back to every row.
    """
    path = '%s/asm/nonmatchings/%s/%s.s' % (PROJ, unit, func)
    if not os.path.exists(path):
        # An overlay split across several translation units keeps each
        # function's listing under its UNIT's directory, not the overlay's.
        # ovl_town was carved into three on 2026-11-03 and every caller that
        # passes the overlay name stopped resolving; the park for
        # func_800CE2D4 failed with a bare FileNotFoundError.
        import glob as _glob
        alt = sorted(_glob.glob('%s/asm/nonmatchings/%s_*/%s.s'
                                % (PROJ, unit, func)))
        if alt:
            path = alt[0]
    lines = open(path).readlines()
    start = next((i + 1 for i, ln in enumerate(lines)
                  if re.match(r'\s*glabel\s+%s\s*$' % re.escape(func), ln)), None)
    if start is None:
        return lines, path
    end = next((i for i, ln in enumerate(lines[start:], start)
                if re.match(r'\s*glabel\s', ln)), len(lines))
    return lines[start:end], path


def target_words(unit, func):
    """[(word, text)] from splat's nonmatchings listing, FUNC's rows only."""
    rows, path = _listing_rows(unit, func)
    out = []
    for ln in rows:
        m = re.match(r'\s*/\* \w+ \w+ ([0-9A-F]{8}) \*/\s+(.*)', ln)
        if m:
            b = bytes.fromhex(m.group(1))
            out.append((int.from_bytes(b, 'little'), m.group(2).strip()))
    if not out:
        raise CompileError('no words parsed from %s (run make splat?)' % path)
    return out


def target_vram(unit, func):
    """The function's vram start, from its first row (not a jump table's)."""
    rows, _ = _listing_rows(unit, func)
    for ln in rows:
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
