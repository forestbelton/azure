#!/usr/bin/env python3
"""provision.py UNIT FUNC... -- build an agent brief's provisioning block.

Written 2026-11-03 because MY OWN BRIEFS were the defect in every cluster of
three consecutive batches, and always in the provisioning, never the analysis.
This encodes the fixes rather than relying on remembering them:

  * per-FUNCTION callee lists, never a per-group UNION described as "shared".
    Two briefs claimed a shared callee that one member did not call, both times
    because I hand-edited a generated group and carried its intersection line.
  * callees classified LANDED-HERE / STUB-HERE / NOT-IN-THIS-UNIT. I once
    listed a resident function among "nine landed at 0 differing".
  * the %hi-on-a-FUNCTION symbols listed, not just warned about. They have no
    jal to decode and ovlverify masks them; this class has broken a build.
  * relatives found three ways, because shared-callee count alone ranked four
    groups BACKWARDS: (a) one shared callee PLUS one shared global, (b) what
    the target INSTALLS, (c) landed CALLERS.
  * prototypes printed, with "NONE" called out, since a callee being a stub
    does not mean its signature is unavailable -- and one callee with no
    declaration at all went unmentioned in a brief.
"""
import glob, io, os, re, sys, subprocess

PROJ = '/home/case/projects/azure'
def D(unit): return os.path.join(PROJ, 'asm/nonmatchings', unit)


# Symbols that are CANDIDATES for the constant-address spelling.
#
# My first version of this tested the address against the resident
# executable's range, 0x8002D000..0x8008875F, and called everything outside it
# out-of-image. That is wrong in the direction that costs most: an OVERLAY's
# OWN DATA sits just above its text -- the town overlay's at 0x800D1xxx and
# 0x800D3xxx -- and is loaded with it, so it is addressed by SYMBOL. Measured
# on the func_800B9260 group, declaring them is 0 differing and the constant
# form costs 6, 3, 1 and 22 differing on four separate symbols, plus 79 words
# against 73 on a fifth. The coarse test told an agent to do the wrong thing
# for four of the five out-of-image symbols in one group.
#
# So use the CURATED list in include/structs.h instead, which is empirical and
# maintained, and fall back to "below the resident base" only if it is gone.
# That keeps kernel/BIOS space (D_80010xxx, D_80012xxx, D_80013xxx) in and the
# overlay's own data out.
RESIDENT_LO = 0x8002D000
# ovl_town's rodata base: jtbl_800898AC is at offset 0x114C (town.bin.yaml).
RODATA_BASE = 0x80088760


def _excluded_syms():
    try:
        h = io.open(os.path.join(PROJ, 'include', 'structs.h'),
                    encoding='utf-8', errors='replace').read()
    except Exception:
        return None
    m = re.search(r'excluded, base outside the loaded image[^:]*:([^*]*)\*/', h)
    if not m:
        return None
    return set(re.findall(r'D_[0-9A-F]+', m.group(1)))


_EXCLUDED = _excluded_syms()


def _forwarding(def_line, arglist_text):
    """A suffix flag when a call site's arguments are all the CALLER's own
    parameters in order, which is the shape where the count is least
    informative: every one of them is already in the right register, so the
    source could have passed fewer and nothing would move."""
    m = re.search(r'\(([^)]*)\)\s*\{?\s*$', def_line.strip())
    if not m:
        return ''
    names = [re.findall(r'[A-Za-z_]\w*', q)[-1]
             for q in m.group(1).split(',')
             if q.strip() and q.strip() != 'void' and re.findall(r'[A-Za-z_]\w*', q)]
    args = [q.strip() for q in arglist_text.split(',') if q.strip()]
    if args and all(a in names for a in args) and \
            [names.index(a) for a in args] == sorted(names.index(a) for a in args):
        return (" -- FORWARDING CALL SITE (every argument is one of %s's own "
                "parameters, in order), so this is the WEAKEST kind of lower "
                "bound" % re.search(r'(func_[0-9A-Fa-f]{8})', def_line).group(1))
    return ''


def park_offset_is_on(park_text, sym, off):
    """True when the PARK reaches `off` off `sym` -- not merely mentions both.

    This is the other half of the 2026-11-03 false positive: tightening only
    the listing side still fired, because the park's own `+0xC8` is on the
    0x1F800000 scratchpad while it ALSO names D_80083160 elsewhere. A park
    reaches a global either directly (`D_X.unk_NN`) or through a local it
    assigns from that global (`g = &D_X;` ... `g->unk_NN`), so look for both
    and attribute nothing else.
    """
    fld = r'(?:unk_%s|pad_%s)' % (off.upper(), off.upper())
    if re.search(r'\b%s\s*\.\s*%s\b' % (sym, fld), park_text):
        return True
    locals_ = set(re.findall(
        r'\b([A-Za-z_]\w*)\s*=\s*&\s*%s\b' % sym, park_text))
    for v in locals_:
        if re.search(r'\b%s\s*->\s*%s\b' % (re.escape(v), fld), park_text):
            return True
        if re.search(r'\b%s\s*\[[^\]]*\]\s*\.\s*%s\b' % (re.escape(v), fld),
                     park_text):
            return True
    return False


def offsets_by_global(listing):
    """{global: {offsets seen as a displacement off THAT global's own base}}.

    Follows the `%lo(SYM)` line's destination register and one level of
    `addiu`/`addu` derivation from it, which is how this codebase forms a
    base-plus-offset pointer. Anything not traceable to a global's own chain is
    simply not attributed -- an offset off the scratchpad or off a parameter
    must not be reported as a field of a global, which is the defect this
    function exists to prevent.
    """
    lines = listing.split('\n')
    out = {}
    for i, ln in enumerate(lines):
        m = re.search(r'%lo\((D_[0-9A-F]+)\)', ln)
        if not m:
            continue
        sym = m.group(1)
        d = re.search(r'\b(?:addiu|lw|lhu|lh|lbu|lb)\s+\$(\w+)', ln)
        if not d:
            continue
        regs = {d.group(1)}
        # the displacement on the %lo line itself, when it is a load/store
        own = out.setdefault(sym, set())
        for ahead in lines[i + 1:i + 60]:
            # one level of derivation: addiu $new, $base, N  /  addu $new, $base, $zero
            dm = re.search(r'\b(?:addiu|addu)\s+\$(\w+),\s*\$(\w+)', ahead)
            if dm and dm.group(2) in regs:
                regs.add(dm.group(1))
            for om in re.finditer(r'0x([0-9A-Fa-f]{2,4})\(\$(\w+)\)', ahead):
                if om.group(2) in regs:
                    own.add(om.group(1))
            am = re.search(r'\baddiu\s+\$\w+,\s*\$(\w+),\s*0x([0-9A-Fa-f]{2,4})\b',
                           ahead)
            if am and am.group(1) in regs:
                own.add(am.group(2))
    return {k: v for k, v in out.items() if v}


def classify_addr_takes(listing, names):
    """Split %hi-on-a-function symbols into the ones this listing STORES (real
    installs) and the ones it only COMPARES (inspectors).

    Takes the destination register off the `%lo` line and looks ahead a few
    instructions for a `sw` of it (install) or a branch on it (test). A symbol
    that shows neither, or both, is reported as an install -- the conservative
    side, since a missed install is the expensive error.
    """
    lines = listing.split('\n')
    stored, tested = [], []
    for n in names:
        verdict = None
        for i, ln in enumerate(lines):
            if '%lo(' + n + ')' not in ln:
                continue
            m = re.search(r'\$(\w+),\s*\$\w+,\s*%lo\(', ln) or \
                re.search(r'\$(\w+),\s*%lo\(', ln)
            if not m:
                verdict = 'store'
                break
            reg = m.group(1)
            for ahead in lines[i + 1:i + 10]:
                if re.search(r'\bs[bhw]\s+\$%s\b' % reg, ahead):
                    verdict = 'store'
                    break
                if re.search(r'\b(beq|bne)\s+.*\$%s\b' % reg, ahead):
                    verdict = 'test'
                    break
            if verdict:
                break
        (tested if verdict == 'test' else stored).append(n)
    return stored, tested


def sister_of(unit, func):
    """The carved sibling unit that defines or stubs `func`, or None.

    An overlay carved into several translation units has siblings named
    `<unit>_ADDR`; a callee absent from this unit may be in one of them rather
    than in the resident executable.
    """
    base = unit.split('_')[0] + '_' + unit.split('_')[1] \
        if unit.startswith('ovl_') and len(unit.split('_')) > 1 else unit
    for fp in sorted(glob.glob(os.path.join(PROJ, 'csrc', base + '*.c'))):
        name = os.path.basename(fp)[:-2]
        if name == unit:
            continue
        try:
            o = io.open(fp, encoding='utf-8', errors='replace').read()
        except Exception:
            continue
        if re.search(r'INCLUDE_ASM\("[^"]+",\s*%s\)' % func, o) or \
           re.search(r'^[A-Za-z_][\w \*]*?\b%s\s*\([^;]*?\)\s*\{?\s*$'
                     % func, o, re.M):
            return name
    return None


def nargs(arglist_text):
    """Argument count for a call's argument text, counting only TOP-LEVEL
    commas. A naive `count(',')` overcounts as soon as one argument is itself
    a call or carries a bracketed subscript -- and a wrong arity is the worst
    provisioning error there is, because it reads as a witness."""
    s = arglist_text.strip()
    if not s or s == 'void':
        return 0
    n, depth = 1, 0
    for ch in s:
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        elif ch == ',' and depth == 0:
            n += 1
    return n


def constaddr_candidate(sym):
    """True when `sym` is a plausible integer-constant address in the original.

    The UNION of two tests, because neither alone is right. Below the resident
    base is kernel/BIOS space and always a candidate -- and the curated list
    does NOT cover all of it: it has D_80010A93 and D_80010A94 but not
    D_80010980/981/983 or D_80010AC4, which is the exact page func_8009EF88
    and func_8009E85C reach through a `(u8 *)0x80010000` local. The curated
    list then adds the high outliers an address test cannot know about
    (D_800E2968, D_801C9E40, D_801DA72C and the rest).

    What neither test may include is an overlay's OWN data, which sits above
    the resident base and is addressed by symbol.
    """
    try:
        if int(sym.split('_')[1], 16) < RESIDENT_LO:
            return True
    except (IndexError, ValueError):
        pass
    return _EXCLUDED is not None and sym in _EXCLUDED

def listing(unit, f):
    p = os.path.join(D(unit), f + '.s')
    return open(p).read() if os.path.exists(p) else None

def words(unit, f):
    t = listing(unit, f)
    m = t and re.search(r'nonmatching \S+, (0x[0-9A-F]+)', t)
    return int(m.group(1), 16) // 4 if m else None

def splat_names():
    """func_ADDR -> the real name splat knows, from symbol_addrs.

    Two callees reported as "no declaration anywhere" were SetLightMatrix and
    SetColorMatrix. A real name is worth more than a prototype: it says what
    the function DOES, which is what lets you type its arguments.
    """
    out = {}
    import glob as _g
    for fp in _g.glob(os.path.join(PROJ, 'splat', '*symbol_addrs*.txt')):
        try: txt = io.open(fp, encoding='utf-8', errors='replace').read()
        except Exception: continue
        for m in re.finditer(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]{8})\s*;', txt, re.M):
            nm, ad = m.group(1), m.group(2).upper()
            if not nm.startswith(('func_', 'D_', 'jtbl_')):
                out.setdefault('func_' + ad, nm)
    return out


def include_protos():
    """Declarations in include/, not just in the unit.

    A callee declared in include/structs.h is NOT undeclared, and saying so is
    the exact func_80053DA8 trap this tool warns about: an agent took the
    marker at face value, wrote the obvious prototype, and got an immediate
    conflicting-types error. func_8004491C is s32 (UnkNode *, u32) at
    include/structs.h:1963 and func_80064584 is s32 (s32) at :2004.
    """
    out = {}
    for d, _, fs in os.walk(os.path.join(PROJ, 'include')):
        for fn in fs:
            if not fn.endswith('.h'): continue
            fp = os.path.join(d, fn)
            try: txt = io.open(fp, encoding='utf-8', errors='replace').read()
            except Exception: continue
            for m in re.finditer(r'^([A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]{8})\s*\([^;{]*\);)',
                                 txt, re.M):
                out.setdefault(m.group(2), (m.group(1).strip(),
                                            os.path.relpath(fp, PROJ),
                                            txt[:m.start()].count('\n') + 1))
    return out


def include_globals():
    """`extern`-declared GLOBALS in include/, keyed by symbol.

    The exact bug include_protos()'s own docstring warns about, in the globals
    path instead of the callee path: on 2026-11-03 this tool told an agent that
    `D_80083160` had NO DECLARATION and it must add one, while
    include/structs.h:1800 declares it `extern Unk80083160 D_80083160;` with a
    dozen landed users. Adding a second declaration is a conflicting-types
    compile error, not a byte difference -- the agent checked instead of
    believing the tool, and said so.
    """
    out = {}
    for d, _, fs in os.walk(os.path.join(PROJ, 'include')):
        for fn in fs:
            if not fn.endswith('.h'):
                continue
            fp = os.path.join(d, fn)
            try:
                txt = io.open(fp, encoding='utf-8', errors='replace').read()
            except Exception:
                continue
            for m in re.finditer(r'^\s*extern\b[^;]*?\b(D_[0-9A-Fa-f]{6,8})\b[^;]*;',
                                 txt, re.M):
                out.setdefault(m.group(1), (os.path.relpath(fp, PROJ),
                                            txt[:m.start()].count('\n') + 1))
    return out


# Dispatcher casts that settle arity for a whole class of handlers. Each was
# read out of the source by an agent that provision.py had told there was no
# witness; the file:line is quoted so the next reader can check it rather than
# trust this table. ADD TO THIS when another install route is verified.
DISPATCHERS = [
    ("UnkNode.unk_10 (+0x10)", "func_80040190, csrc/unk_12DEC.c:567",
     "((void (*)(void *, UnkVec *, u32)) fn)((u8 *)p + 0x20, p->unk_08, p->unk_0C)",
     "first argument is the node + 0x20, i.e. the ACTOR VIEW, not the node"),
    ("func_8004491C tag", "func_800402F4, csrc/unk_12DEC.c:611",
     "void (*f)(u8 *, UnkVec *, UnkSlot *)",
     "the 'tag' passed to func_8004491C IS the callback -- but only when it is a"
     " function; the same binder also carries DATA templates, so check first"),
]


def install_slots(unit, func, landed, btext):
    """Byte offsets this function's address is STORED to, with the installer.

    An install at a dispatcher slot settles arity completely, through the
    dispatcher's own cast -- so the slot is the key to the only witness some
    functions have. This finds it from both kinds of installer:

      * a STUB installer, by reading its listing: take the `%lo(func)` line's
        destination register and look ahead for a `sw` of it;
      * a LANDED installer, by reading its body for `= func` and taking the
        offset off the store's left-hand side.

    Returns a sorted list of (offset, installer, how) tuples.
    """
    out = set()
    for d, b in btext.items():
        for m in re.finditer(r'(?:\*\s*\(\s*\w[\w \*\(\)]*\)\s*)?'
                             r'\(?\s*\(\s*u8\s*\*\s*\)?\s*(\w+)'
                             r'\s*\+\s*(0x[0-9A-Fa-f]+)\s*\)?\s*=\s*'
                             + func + r'\b', b):
            out.add((int(m.group(2), 16), d, 'landed C store'))
        for m in re.finditer(r'->\s*unk_([0-9A-Fa-f]+)\s*=\s*' + func + r'\b', b):
            out.add((int(m.group(1), 16), d, 'landed C store'))
    # stub installers: every listing in the unit that materialises this address
    for fp in sorted(glob.glob(os.path.join(PROJ, 'asm/nonmatchings', unit, '*.s'))):
        try:
            lt = io.open(fp, encoding='utf-8', errors='replace').read()
        except Exception:
            continue
        if '%lo(' + func + ')' not in lt:
            continue
        who = os.path.basename(fp)[:-2]
        lines_ = lt.split('\n')
        for i, ln in enumerate(lines_):
            if '%lo(' + func + ')' not in ln:
                continue
            m = re.search(r'\$(\w+),\s*\$\w+,\s*%lo\(', ln) or \
                re.search(r'\$(\w+),\s*%lo\(', ln)
            if not m:
                continue
            reg = m.group(1)
            for ahead in lines_[i + 1:i + 10]:
                mm = re.search(r'\bsw\s+\$%s,\s*(-?0x[0-9A-Fa-f]+|0)\s*\(' % reg, ahead)
                if mm:
                    out.add((int(mm.group(1), 16), who, 'stub listing `sw`'))
                    break
    return sorted(out)


def slot_casts(txt, off):
    """Prototyped indirect-call casts in this unit through byte offset `off`.

    This is the witness the NO-ARITY-WITNESS banner used to deny existed: a
    cast like `(*(void (**)(u8 *))(q + 0x1C))(q)` states the arity AND the
    parameter types of every handler installed at that slot. Four agents have
    now found one after the tool told them there was none.
    """
    hits = []
    pat = re.compile(r'\(\s*\*\s*\(\s*([\w ]+?)\s*\(\s*\*\*\s*\)\s*'
                     r'\(([^)]*)\)\s*\)\s*\(([^;]*?0x%X[^;]*?)\)\s*\)\s*\(' % off,
                     re.I)
    for m in pat.finditer(txt):
        hits.append((txt[:m.start()].count('\n') + 1,
                     m.group(1).strip(), m.group(2).strip()))
    return hits


def parks():
    """Every near-miss park, keyed by the function it parks.

    Added 2026-11-03. An agent's single most valuable document was the park for
    its target's CALLER, and it found that park by accident mid-run: no route
    surfaced it. It reported that four of the facts it had already paid for
    separately were already written down there. A park is not a failure note --
    it is a worked document carrying the family's field mapping, every spelling
    already measured and rejected, and what the candidate HELD FIXED.
    """
    out = {}
    d = os.path.join(PROJ, 'docs/near-misses')
    try:
        names = sorted(os.listdir(d))
    except Exception:
        return out
    for sf in names:
        if not sf.endswith('.c'):
            continue
        try:
            head = open(os.path.join(d, sf)).read(4000)
        except Exception:
            continue
        m = re.search(r'(func_[0-9A-Fa-f]{8})\s*\((\w+)\)\s*--\s*'
                      r'(\d+) of (\d+) words, (\d+) DIFF', head)
        if m:
            out[m.group(1)] = (sf, m.group(2), int(m.group(3)),
                               int(m.group(4)), int(m.group(5)))
    return out


def defs_and_protos(src, unitname):
    txt = open(src).read()
    whole = txt
    landed, cur, depth, body = {}, None, 0, {}
    defoff = {}
    in_comment = [False]

    def code_only(ln):
        '''`ln` with comment spans removed, so a name MENTIONED IN PROSE is not
        read as a call or an install.

        Route (d) reported 42 of these before 2026-11-03, and one of them
        mattered: func_8009DA50's comment says "x/z are the s16 values
        func_8009D424 is passed", which came out as
        "func_8009D424 address taken by func_8009DA50" -- it is a CALLER, not
        an installer. Comments in this tree also quote code verbatim
        (`*(u32 *)(m + 0x68) = (u32)func_8008BA00;` appears inside a comment),
        so a prose scan cannot be fixed by pattern alone.'''
        out, i = [], 0
        while i < len(ln):
            if in_comment[0]:
                e = ln.find('*/', i)
                if e < 0:
                    return ''.join(out)
                in_comment[0] = False
                i = e + 2
                continue
            s = ln.find('/*', i)
            d = ln.find('//', i)
            if d >= 0 and (s < 0 or d < s):
                out.append(ln[i:d])
                return ''.join(out)
            if s < 0:
                out.append(ln[i:])
                return ''.join(out)
            out.append(ln[i:s])
            in_comment[0] = True
            i = s + 2
        return ''.join(out)
    # Address-TAKES inside a landed body, which `body` cannot hold because it
    # matches only `func_X(`. Route (d) needs these: see the comment there.
    addr = {}
    lines, lineno = {}, 0
    pos = 0          # byte offset of `ln` within txt; see defoff[] below
    all_lines = txt.split('\n')

    def logical(i):
        r'''The first LOGICAL line of the construct starting at all_lines[i],
        joined across a wrapped parameter list, or None if it does not
        terminate within a few lines.

        A DEFINITION whose parameter list wraps has a first line ending in
        `,`, which is exactly the shape the `)\s*\{?$` test below rejects --
        that test exists to keep func_800C3050's two-line DECLARATION from
        being read as a definition.  So a wrapped definition was being
        classified as a stub with no declaration at all.  Measured
        2026-11-03: func_800A2000 (csrc/ovl_town.c:15301, a six-parameter
        definition wrapping after `void (*h10)(),`) printed as
        `STUB-HERE ... *** NO DECLARATION IN THE UNIT OR include/ ***`, and
        that signature was the entire provisioning for two targets in the
        same batch.  Joining the continuation lines first tells the two apart
        by the terminator -- `;` is a declaration, `)` or `) {` a
        definition -- instead of by where the author happened to wrap.'''
        out = all_lines[i].rstrip()
        for k in range(1, 6):
            if ';' in out or re.search(r'\)\s*\{?\s*$', out):
                return out
            if i + k >= len(all_lines):
                return None
            out = out + ' ' + all_lines[i + k].strip()
        return None

    for idx, ln in enumerate(all_lines):
        lineno += 1
        m = re.match(r'^[A-Za-z_][\w \*]*?(func_[0-9A-Fa-f]{8})\s*\(', ln)
        head = logical(idx) if m else None
        # A DEFINITION's first line ends in `)` or `)  {`. A one-line
        # prototype ends in `;` and a MULTI-LINE prototype's first line ends in
        # `,` -- and that second case produced a FABRICATED landed caller:
        # func_800C3050's two-line declaration was read as a definition, so
        # every later declaration line was swallowed as part of its "body" and
        # reported as a call. A wrong landed caller is the worst provisioning
        # error there is, because it reads as an arity-and-constants witness.
        # Belt and braces: a name with an INCLUDE_ASM line is a stub, never
        # landed, whatever the text looks like.
        if m and head and ';' not in head and re.search(r'\)\s*\{?\s*$', head) \
                and ('INCLUDE_ASM("asm/nonmatchings/%s", %s)' % (unitname, m.group(1))) not in whole:
            cur = m.group(1); landed[cur] = head.strip(); body[cur] = set()
            addr[cur] = set(); depth = 0
            # Record the line HERE. Searching for the text later finds the
            # single-line PROTOTYPE instead, because the definition line is a
            # prefix of it -- that sent a brief's reader to :2206 (a
            # declaration) rather than :19205 (the definition).
            lines[cur] = lineno
            # AND THE BYTE OFFSET, for the same reason the line number is
            # recorded here: a later `txt.index(landed[d])` finds the earlier
            # single-line PROTOTYPE, because the definition's first line is a
            # PREFIX of it (`void f(T *a, s32 b)` against `...b);`). The
            # comment above already says so for lines[], but the body-text
            # slices below were still doing the search -- so routes (c) and
            # (f) sliced the DECLARATION BLOCK, found no call in it, and
            # printed "none -- no LANDED call site constrains arity".
            # Measured 2026-11-03: 147 of 729 landed definitions in
            # csrc/ovl_town.c are prefixes of an earlier prototype, so both
            # routes were blind to 20% of the landed callers -- including both
            # of func_800ABC00's, which a plain grep finds in one command.
            defoff[cur] = pos
        code = code_only(ln)
        if cur:
            body[cur].update(re.findall(r'\b(func_[0-9A-Fa-f]{8})\s*\(', code))
            # A name NOT followed by `(` is being used as a VALUE: `(u32)func_X`,
            # `&func_X`, or a bare assignment into a slot. That is an install.
            addr[cur].update(re.findall(
                r'\b(func_[0-9A-Fa-f]{8})\b(?!\s*\()', code))
            depth += ln.count('{') - ln.count('}')
            if depth <= 0 and '}' in ln: cur = None
        pos += len(ln) + 1
    protos = {}
    for m in re.finditer(r'^([A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]{8})\s*\([^;{]*\);)', txt, re.M):
        protos.setdefault(m.group(2), m.group(1).strip())
    for k in body: body[k].discard(k)
    for k in addr: addr[k].discard(k)
    return landed, protos, body, txt, lines, addr, defoff

def main():
    unit, targets = sys.argv[1], sys.argv[2:]
    src = os.path.join(PROJ, 'csrc', unit + '.c')
    landed, protos, body, txt, lines, addr, defoff = defs_and_protos(src, unit)
    src_txt = txt
    inc = include_protos()
    snames = splat_names()
    incglob = include_globals()
    incasm = {}
    for m in re.finditer(r'INCLUDE_ASM\("[^"]*", *(func_[0-9A-Fa-f]{8})\)', txt):
        incasm[m.group(1)] = txt[:m.start()].count('\n') + 1
    allcallees, installs, globs = {}, {}, {}
    print("=== PER-FUNCTION FACTS (never describe a union as shared) ===")
    tot = 0
    for f in targets:
        t = listing(unit, f)
        if t is None:
            print("  %-15s *** NO LISTING IN %s ***" % (f, unit)); continue
        # words() returns None when a listing carries no sized `nonmatching`
        # header. Adding that to `tot` crashed the whole tool AFTER it had
        # printed partial output, which is the worst failure mode for something
        # agents read top to bottom. Say so and keep going.
        w = words(unit, f)
        if w is None:
            print("  %-15s *** LISTING HAS NO SIZED `nonmatching` HEADER -- read it by"
                  " hand, every word count below is suspect ***" % f)
            w = 0
        tot += w
        cal = sorted(set(re.findall(r'jal\s+(\w+)', t)))
        ins = sorted(set(re.findall(r'%hi\((func_[0-9A-Fa-f]+)\)', t)))
        stored, tested = classify_addr_takes(t, ins)
        # BOTH %hi AND %lo. Matching only %hi loses every symbol that shares
        # one materialised base with another -- which is exactly the
        # constant-base idiom this overlay is full of, where one `lui` serves
        # several `%lo` displacements. On 2026-11-03 this reported
        # D_80010983 for func_8009EF88 and omitted D_80010AC4, the arm's whole
        # payload; for func_8009E85C it reported D_80010980 and omitted
        # D_80010981, D_80010983 and D_80010A94 -- three of its four accesses,
        # including the one supplying a callee's fourth argument. Two agents
        # reported it independently.
        gl  = sorted(set(re.findall(r'%(?:hi|lo)\((D_[0-9A-F]+)\)', t)))
        jt  = len(re.findall(r'jr\s+\$(?!ra)', t))
        # A JUMP TABLE IS NOT "OWNING RODATA". Every jump-table listing has a
        # `.section .rodata` block for the table itself, so the old test
        # (`'.section .rodata' in t or 'dlabel D_' in t`) said YES for any
        # function whose only rodata IS its table -- and jumptable + own-rodata
        # is exactly the conjunction the func_800BF1C8 park was blocked on, so
        # the tool reported the blocking shape for functions that cannot have
        # it. func_800A4A58 landed byte-identical with its table on
        # 2026-11-03; its rodata needed nothing but the right alignment.
        #
        # Own rodata means a `dlabel D_...` -- a DATA label migrated into this
        # listing. Anchored, because `enddlabel` contains `dlabel`, which is
        # the trap docs/town-sweep.md already records for the ad-hoc grep.
        own_rod = bool(re.search(r'^dlabel D_', t, re.M))
        jt_rod = bool(re.search(r'^dlabel jtbl_', t, re.M))
        rod = 'YES' if own_rod else ('jumptable only' if jt_rod else 'no')
        # `installs` feeds the group-level install heuristics, so it must hold
        # only the STORED ones.
        allcallees[f], installs[f], globs[f] = set(cal), set(stored), set(gl)
        print("  %s  %d words  jumptable=%d  owns-rodata=%s" % (f, w, jt, rod))
        if jt_rod and not own_rod:
            a = re.search(r'^dlabel (jtbl_[0-9A-Fa-f]+)', t, re.M).group(1)
            off = int(a.split('_')[1], 16) - RODATA_BASE
            print("      %s is at rodata offset 0x%X, %d mod 8 -- %s"
                  % (a, off, off % 8,
                     "`.align 3` is a NO-OP, the table costs nothing"
                     if off % 8 == 0 else
                     "*** 4 mod 8: cc1psx's `.align 3` will PAD it and the "
                     "container comes out long. This is the class that forced "
                     "the three-unit carve; see splat/town.bin.yaml ***"))
        print("      callees : %s" % (', '.join(cal) or 'NONE'))
        if ins:
            # AN ADDRESS-TAKE IS NOT NECESSARILY AN INSTALL. A function-address
            # COMPARE (`beq`/`bne` against the materialised address) is
            # indistinguishable from a STORE in a %hi/%lo grep, and this line
            # used to call both "installs". On 2026-11-03 that made
            # func_800A0F10 -- which only TESTS the camera task's current
            # handler against two addresses -- look like a second installer of
            # a handler func_80093ED8 really does install, and the bogus
            # "two installers of one handler" pairing went into a brief as its
            # strongest lead. It cost the agent a detour.
            if stored:
                print("      installs: %s   <- %%hi on a FUNCTION: masked, "
                      "no jal, VERIFY BY RELOC" % ', '.join(stored))
            else:
                print("      installs: none")
            if tested:
                print("      TESTS a handler address (NOT an install): %s"
                      % ', '.join(tested))
                print("          It materialises the address and COMPARES it"
                      " -- beq/bne, no store. That constrains nothing about")
                print("          the named function and makes this an"
                      " inspector, not an installer. Do not pair it with a")
                print("          real installer of the same handler.")
        else:
            print("      installs: none")
        # AN ADDRESS-TAKE DOES NOT RIDE AN IMPLICIT DECLARATION the way a call
        # does. This unit rides 18 implicit declarations for CALLS to functions
        # defined below their call site, and none of them covers taking an
        # address: the compile dies with "undeclared" at the install. Cost an
        # agent a compile on func_800BEBA4 (installing func_800BF15C, defined
        # below), and it reported that the tool flags the install but not the
        # declaration it needs.
        mypos = incasm.get(f) or lines.get(f) or 0
        need = [i for i in ins
                if (lines.get(i) or incasm.get(i) or 0) > mypos]
        if need:
            print("                *** EACH OF THESE IS DEFINED BELOW YOU, SO THE INSTALL")
            print("                NEEDS A FORWARD DECLARATION -- an address-take does NOT")
            print("                ride an implicit declaration the way a CALL does, and")
            print("                this unit's 18 implicit-declaration rides are all calls:")
            for i in need:
                pr = protos.get(i)
                print("                  %-15s %s" % (
                    i, pr.strip() if pr else 'no declaration in the unit either -- write one'))
        if gl:
            tags, outside, needdecl = [], [], []
            for g in gl:
                if re.search(r'^\s*extern\b[^;]*\b' + g + r'\b', src_txt, re.M):
                    tags.append(g)
                elif g in incglob:
                    fp, ln = incglob[g]
                    tags.append('%s (declared %s:%d -- do NOT redeclare)' % (g, fp, ln))
                elif constaddr_candidate(g):
                    # DEMANDING A DECLARATION HERE IS THE WRONG INSTRUCTION,
                    # and following it changes the addressing. A global whose
                    # base is outside the loaded image is usually an integer
                    # constant cast to a pointer in the original, reached
                    # through a `(u8 *)0x...` local with the offset in the
                    # index. Declare the symbol instead and the address
                    # becomes plus(symbol_ref, reg) rather than
                    # plus(reg, const_int), which changes the instruction
                    # count. The tell in the listing is whether the `lui`'s
                    # destination register EQUALS the load's: if it does, only
                    # the constant form reaches it. Two agents hit this on the
                    # same day -- D_80012D5C in func_800B180C (2 differing as
                    # a symbol) and the D_80010xxx page in func_8009EF88.
                    outside.append(g)
                    tags.append(g + ' [MAYBE A CONSTANT]')
                else:
                    needdecl.append(g)
                    tags.append(g + ' [NEEDS A DECLARATION]')
            print("      globals : %s" % ', '.join(tags))
            if needdecl:
                # THE MARKER USED TO BE APPENDED TO THE COMMA-JOINED LIST, so
                # it read as applying to every symbol on the line. Two agents
                # reported it on 2026-11-03: one was told three globals needed
                # a declaration when only the trailing one did, and said that
                # believing it would have walked them into the by-value/decay
                # trap the sweep doc records. The tool already does this right
                # in the declared case ("declared include/structs.h:1636 -- do
                # NOT redeclare"), so the per-symbol form is carried into the
                # undeclared case and the ones that need it are named again on
                # their own line.
                print("      *** NEEDS A DECLARATION -- exactly these %d, not the"
                      " whole line above: %s ***"
                      % (len(needdecl), ', '.join(needdecl)))
            if outside:
                # One note for all of them, not one per symbol: this used to
                # repeat a whole paragraph per global and the line became
                # unreadable, which defeats the point for something agents
                # read top to bottom.
                base = min(int(g.split('_')[1], 16) for g in outside)
                print("      [MAYBE A CONSTANT] %s -- outside the resident"
                      " image, so MEASURE BOTH SPELLINGS."
                      % ', '.join(outside))
                print("          These are often integer constants cast to"
                      " pointers in the original: try a `(u8 *)0x%08X` base"
                      % (base & 0xFFFF0000))
                print("          local with the offsets in the index as well"
                      " as the declared symbol. Declaring it makes the")
                print("          address plus(symbol_ref, reg) rather than"
                      " plus(reg, const_int), which can change the")
                print("          instruction count -- but it often does NOT,"
                      " so this is a lever to try, not an instruction.")
                # The SHARPEST tell, and the only one that works from a
                # single instruction pair. Added 2026-11-04 after it decided
                # func_8009F8EC with no measurement at all (the declared
                # symbol was 19 words / 2 differing, the constant 19 / 0).
                if re.search(r'lui +\$at', t):
                    print("          *** THIS LISTING HAS A `lui $at` -- the"
                          " SHARPEST tell, and the only one that works")
                    print("          from a single instruction pair."
                          " `$at` is FIXED_REGISTERS[1] in the mips port, so")
                    print("          cc1psx CANNOT allocate it: a `lui $at`"
                          " carrying the memory reference's own `%lo`,")
                    print("          with no `ori`/`addiu` anywhere, is the"
                          " ASSEMBLER expanding a macro-form")
                    print("          `sb/sw reg, <address>`, and at -G0 only a"
                          " CONSTANT ADDRESS emits that macro form.")
                    print("          Write the constant. Index of the others:"
                          " grep -lE 'lui +[$]at'"
                          " asm/nonmatchings/%s/*.s -- note the `+`, a" % unit)
                    print("          single-space pattern silently returns"
                          " nothing. ***")
                print("          Tells, in order of reliability: a `lui $at`"
                      " (above) is decisive; an `ori` where the target has")
                print("          `addiu` means you declared a symbol that"
                      " wants to be a constant; several")
                print("          %lo off one %hi is the constant-base idiom."
                      " The")
                print("          lui-destination-equals-the-load test is NOT"
                      " reliable on a short leaf -- on func_800B9034 the")
                print("          registers tie and all three spellings are"
                      " byte-exact, so there the tying is a consequence of")
                print("          the surrounding allocation, not of"
                      " constant-vs-symbol.")
        else:
            print("      globals : none")
    print("  TOTAL %d words" % tot)

    print("\n=== YOUR TARGETS' OWN EXISTING DECLARATIONS ===")
    print("    An arity change must be made in BOTH places or the compile fails on a")
    print("    type mismatch instead of on the thing you are testing. One agent's")
    print("    probe died exactly this way.")
    anyt = False
    for f in targets:
        d = protos.get(f)
        if d:
            print("  %-15s already declared: %s" % (f, d)); anyt = True
    if not anyt: print("  none of your targets is declared in the unit")

    print("\n=== CALLEE CLASSIFICATION (per callee, across the group) ===")
    every = set().union(*allcallees.values()) if allcallees else set()
    for c in sorted(every):
        who = [f for f in targets if c in allcallees.get(f, ())]
        if c in landed:
            # An arity mismatch on a callee defined BELOW its call site rides an
            # implicit declaration (this unit does that 18 times); one defined
            # ABOVE is a hard "too many arguments to function". Same mismatch,
            # opposite consequence, so print which.
            # The call site is the TARGET's position, and a target is still a
            # stub -- so look up its INCLUDE_ASM line, not `lines`, which only
            # holds landed definitions. Getting this backwards reported a hard
            # error as benign.
            sites = [incasm[x] for x in who if x in incasm]
            first = min(sites) if sites else None
            cls = ('LANDED-ABOVE (mismatch = HARD ERROR)'
                   if first is not None and lines[c] < first
                   else 'LANDED-BELOW (mismatch rides implicit decl)')
        elif listing(unit, c) is not None: cls = 'STUB-HERE'
        else:
            # A CARVE CONSEQUENCE I did not anticipate: this used to call
            # anything absent from csrc/<unit>.c "resident", which is wrong for
            # every address in a SISTER CARVED UNIT. The label matters -- it is
            # what tells an agent to look in town.bin_undefined_funcs_auto.txt
            # and to weigh the `D_`-spelling trap, and both are the wrong move
            # for a sister-unit symbol. Reported 2026-11-03 for func_800C1D20,
            # which is an INCLUDE_ASM stub in csrc/ovl_town_800BF3F0.c and in
            # NEITHER auto file.
            sis = sister_of(unit, c)
            cls = ('*** IN A SISTER UNIT (%s) ***' % sis if sis
                   else '*** NOT IN THIS UNIT (resident) ***')
        pr = protos.get(c)
        if not pr and c in landed:
            d = landed[c]
            pr = '(definition) ' + (d[:d.rindex(')')+1] if ')' in d else d)
        print("  %-15s %-34s called by: %s" % (c, cls, ','.join(who)))
        if pr:
            print("      proto: %s" % pr)
        elif c in inc:
            d, f, l = inc[c]
            print("      proto: %s   <- from %s:%d, NOT the unit" % (d, f, l))
        elif c in snames:
            print("      proto: *** NO DECLARATION *** but splat NAMES it: %s"
                  % snames[c])
            print("             -- a real name says what it DOES, which is how you")
            print("                type its arguments. grep it in include/ and psyq/.")
        else:
            print("      proto: *** NO DECLARATION IN THE UNIT OR include/ ***")

    # The defect class that has cost this sweep the most, hit independently by
    # two agents in one batch: a callee whose PROTOTYPE IS NARROWER than the
    # call site. It compiles silently when the definition sits after the call
    # (implicit declaration) and is a hard error when it sits before, and it is
    # invisible to ovlverify and to families.py. Count the $aN registers the
    # caller sets before each jal and compare.
    print("\n=== ARITY LEADS -- THIS OVER-COUNTS. A LEAD, NOT A WITNESS. ===")
    print("    A narrower prototype is checklist item 3: it either needs widening or")
    print("    rides an implicit declaration, and getting it wrong is a hard error or")
    print("    a silent one depending on whether the definition precedes the call.")
    print("    BUT a backward scan over-counts, for three reasons already in the")
    print("    sweep doc, and an agent confirmed four false 3-arg callees in one pair:")
    print("      * reorg parks an unrelated $aN set in a branch delay slot, so the")
    print("        scan picks up the NEXT call's argument;")
    print("      * cross-jumping hoists an $a0 set out of the block entirely, so a")
    print("        correct count here can be right only by accident of layout;")
    print("      * the scan crosses basic blocks, the same reason sched cannot be")
    print("        blamed for a moved insn.")
    print("    So VERIFY each line below by walking the control flow to the jal before")
    print("    acting on it. Do not put one of these in a brief as a witness.")
    flagged = False
    for f in targets:
        lt = listing(unit, f)
        if lt is None: continue
        rows = lt.split('\n')
        jal_at = [(i, m.group(1)) for i, ln in enumerate(rows)
                  for m in [re.search(r'jal\s+(func_\w+)', ln)] if m]
        for i, callee in jal_at:
            # Scan back to the previous jal OR the previous label, whichever
            # is nearer: a $aN write before either is another call's setup.
            regs = set()
            for j in range(max(0, i - 14), i + 2):          # +2: the delay slot
                if j != i and (re.search(r'jal\s+func_', rows[j])
                               or re.match(r'\s*(\.L|glabel|endlabel)', rows[j])):
                    regs = set()
                m = re.search(r'^\s*/\*[^*]*\*/\s+\S+\s+\$(a[0-3])\s*,', rows[j])
                if m: regs.add(m.group(1))
            # Infer arity from the HIGHEST register set, not the count: $a0 is
            # often already live and never moved (the documented
            # missing-argument-move trap), so {a1,a2} still means three args.
            pr = protos.get(callee) or (landed.get(callee) or '')
            if callee in inc and not protos.get(callee): pr = inc[callee][0]
            if not pr or '(' not in pr: continue
            args = pr[pr.index('(') + 1:pr.rindex(')')].strip()
            n = 0 if args in ('', 'void') else args.count(',') + 1
            if args == '' and '()' in pr:
                continue                                     # K&R, unconstrained
            implied = (max(int(r[1]) for r in regs) + 1) if regs else 0
            if implied and n < implied:
                flagged = True
                print("  *** %s calls %s with >=%d arg(s) (sets %s) but its prototype"
                      % (f, callee, implied, ','.join('$' + r for r in sorted(regs))))
                print("      declares %d: %s" % (n, pr))
    if not flagged:
        print("  no narrower prototype found among the callees with real prototypes")

    print("\n=== INSTALLED HANDLERS (route (b): what the target installs) ===")
    anyi = False
    for f in targets:
        for h in sorted(installs.get(f, ())):
            anyi = True
            if h in landed:
                d0 = landed[h]
                st = 'LANDED at ' + (d0[:d0.rindex(')') + 1] if ')' in d0 else d0)
            else:
                st = 'stub' if listing(unit, h) else 'NOT IN UNIT'
            print("  %s installs %-15s -> %s" % (f, h, st))
    if not anyi: print("  none")

    # Rank by RARITY, not by count. func_80095388 is called by ~30 landed
    # functions, so sharing it means nothing; a callee called by two means a
    # great deal. Counting shared callees is what ranked four groups BACKWARDS.
    freq = {}
    for dc in body.values():
        for c in dc: freq[c] = freq.get(c, 0) + 1
    # Extract each landed body's TEXT once, so the argument list is read from
    # the CALLER'S BODY and not from the whole file. Searching `txt` made the
    # first match win, and the first match is normally the target's own forward
    # DECLARATION -- which rendered as `as func_X()` and read as a measured
    # ZERO-ARITY call site. That is worse than omitting the field: an agent
    # that believed it would declare `void func_X(void)`, reach 0 differing
    # (the callee cannot see its own arity), and fail the container build with
    # "too many arguments to function".
    btext = {}
    for d in landed:
        # NOT `defoff.get(d, txt.index(...))`: Python evaluates a default
        # EAGERLY, so the fallback search ran even when defoff had the key --
        # and once landed[] started holding a definition's first LOGICAL line
        # (joined across a wrapped parameter list) that search raises
        # ValueError, because the joined text appears nowhere in the file.
        i = defoff[d] if d in defoff else txt.index(landed[d])
        j = txt.find('\n}', i)
        btext[d] = txt[i:j if j > 0 else i + 2000]

    def arglist(hay, name):
        """The full argument text, with parens balanced.

        `[^;)]*` cannot cross a nested `)`, so it truncated
        `a, *((u8 *)a + 0x96))` to `a, *((u8 *` -- enough to count a comma but
        not enough to settle a constant argument, which is half of what this
        section claims to do."""
        for m in re.finditer(re.escape(name) + r'\s*\(', hay):
            k = m.end(); depth = 1
            while k < len(hay) and depth:
                if hay[k] == '(': depth += 1
                elif hay[k] == ')': depth -= 1
                k += 1
            if depth: continue
            inner = hay[m.end():k-1]
            # A MULTI-LINE ARGUMENT LIST IS STILL AN ARGUMENT LIST. This used
            # to `continue` on any newline, so route (c) printed "none -- no
            # LANDED call site constrains arity" for a call it had already
            # found. On 2026-11-03 that hid func_8009DA50's ten-argument call
            # of func_8009D424 -- the single most load-bearing provisioning
            # fact in its group, giving the arity, the argument order and
            # every parameter type -- and the agent recovered it with a plain
            # grep in its first minute. Collapse the whitespace instead.
            return re.sub(r'\s+', ' ', inner).strip()
        return None

    witnessed = set()
    print("\n=== ROUTE (d): WHO TAKES MY TARGET'S ADDRESS (the reverse install edge) ===")
    print("    Named by two agents as the most valuable fact their brief lacked.")
    print("    An address-taker is NOT a caller and constrains nothing by itself.")
    print("    READ WHAT THE INSTALLER ASSIGNS INTO BEFORE TRUSTING IT FOR ARITY.")
    print("    This route's wording used to say flatly that the installer's own")
    print("    declaration is the arity witness. That is TRUE only when the")
    print("    installer calls your target, or assigns it into a slot declared with")
    print("    a PROTOTYPE. In this codebase the slot is usually `void (*unk_00)();`")
    print("    -- UNPROTOTYPED, so ANY arity compiles and the install settles")
    print("    nothing about it. On 2026-11-03 that wording was echoed into a brief")
    print("    for func_80091CE0/D6C/EB4/920F4, whose four installers are all")
    print("    one-parameter functions that only store the address; the agent")
    print("    falsified it and got arity from the listings instead -- every one")
    print("    reads $a0, $a1 and $a2 into a saved register before writing any of")
    print("    them, and $a3 is never touched. docs/town-sweep.md records the trap.")
    print("    What an install DOES settle: which slot, and the first parameter's")
    print("    type by family convention.")
    anyd = False
    # Capture the C file's name BEFORE the loop below, which rebinds `src` to
    # the string 'declared' and would otherwise print that as the filename.
    cfile = os.path.basename(src)
    for f in targets:
        for sf in sorted(os.listdir(D(unit))):
            if not sf.endswith('.s') or sf[:-2] in targets: continue
            try: st = open(os.path.join(D(unit), sf)).read()
            except Exception: continue
            if '%hi(' + f + ')' in st:
                nm = sf[:-2]
                decl = protos.get(nm)
                src = 'declared'
                if not decl and nm in landed:
                    d0 = landed[nm]
                    decl = d0[:d0.rindex(')') + 1] if ')' in d0 else d0
                    src = 'LANDED definition'
                if not decl and nm in inc:
                    decl, fn, l0 = inc[nm][0], inc[nm][1], inc[nm][2]
                    src = 'from %s:%d' % (fn, l0)
                print("  %-15s address taken by %-15s %s" % (f, nm,
                      ('%s: %s' % (src, decl)) if decl
                      else 'NO DECLARATION ANYWHERE -- read its listing'))
                anyd = True; witnessed.add(f)
        # AND THE LANDED C, which the loop above cannot see. splat emits a
        # listing only for a function still carried by INCLUDE_ASM and prunes
        # it on the next run, so a LANDED installer has no `.s` file at all and
        # this route printed `none` for it. On 2026-11-03 that lost
        # func_800A1C94 -- landed C, 23 lines above its targets' own
        # INCLUDE_ASMs, installing BOTH func_800A1D1C and func_800A1F00 -- for
        # the group whose arity it settles. Route (d)'s own header calls this
        # the most valuable fact agents said their brief lacked, so printing
        # `none` when the answer is in the same file is the worst case.
        for nm in sorted(addr):
            if nm in targets or f not in addr[nm]:
                continue
            d0 = landed[nm]
            decl = d0[:d0.rindex(')') + 1] if ')' in d0 else d0
            print("  %-15s address taken by %-15s LANDED C %s:%d: %s" % (
                f, nm, cfile, lines.get(nm, 0), decl))
            anyd = True; witnessed.add(f)
    if not anyd: print("  none")

    print("\n=== ROUTE (e): ASM CALLERS (route (c) only sees LANDED callers) ===")
    anye = False
    callers = {}
    for f in targets:
        for sf in sorted(os.listdir(D(unit))):
            if not sf.endswith('.s') or sf[:-2] in targets: continue
            try: st = open(os.path.join(D(unit), sf)).read()
            except Exception: continue
            if re.search(r'jal\s+' + f + r'\b', st):
                nm = sf[:-2]
                callers.setdefault(f, set()).add(nm)
                if nm in landed:
                    d0 = landed[nm]
                    print("  %-15s jal'd by LANDED %-15s csrc/%s.c:%d -- read the C, it has"
                          % (f, nm, unit, lines[nm]))
                    print("                  argument TYPES a listing does not: %s"
                          % (d0[:d0.rindex(')') + 1] if ')' in d0 else d0))
                else:
                    print("  %-15s jal'd by STUB %-15s -- read %s/%s to settle arity"
                          % (f, nm, D(unit).replace(PROJ + '/', ''), sf))
                anye = True; witnessed.add(f)
    if not anye: print("  none")

    print("\n=== ROUTE (i): PARKS IN YOUR CALL GRAPH -- READ THESE BEFORE MEASURING ===")
    print("    A park is a worked document, not a failure note: the family's field")
    print("    mapping, every spelling already measured AND REJECTED, and what the")
    print("    candidate held FIXED. The agent on func_800AD138 found its caller's park")
    print("    by accident mid-run and reported that four facts it had already paid for")
    print("    separately were written down in it. A park on a CALLER or CALLEE usually")
    print("    shares the record layout; a park on the TARGET is your baseline to beat.")
    pk = parks()
    anyp = False
    edge_reported = {}
    for f in targets:
        rel = []
        if f in pk:
            rel.append(('** THE TARGET ITSELF **', f))
        for c in sorted(allcallees.get(f, ())):
            if c in pk:
                rel.append(('a CALLEE', c))
        for c in sorted(callers.get(f, ())):
            if c in pk:
                rel.append(('a CALLER', c))
        edge_reported[f] = {nm for _, nm in rel}
        for why, nm in rel:
            sf, u, built, tgt, diff = pk[nm]
            print("  %-15s %s %s is PARKED at %d of %d words, %d DIFF (%s)"
                  % (f, why, nm, built, tgt, diff, u))
            print("                  docs/near-misses/%s" % sf)
            anyp = True
    if not anyp:
        print("  none in the call graph.")

    # A PARK ON A SIBLING -- same record, same source fragment, NO CALL-GRAPH
    # EDGE -- was invisible to every route, and on 2026-11-03 that relationship
    # produced the single most valuable lead of the campaign: func_800B70EC
    # shares no caller and no callee with func_800B6FB8 and func_800B72B8, so
    # route (i) correctly reported "none in the call graph" and route (a)
    # buried the relation at 0.033, yet landing it RETIRED BOTH PARKS (+100
    # words and the answer they had each spent hundreds of compiles failing to
    # find). The relation was a shared FIELD OFFSET off a shared global, which
    # is exactly what route (m) already computes. The agent asked for this
    # route by name; here it is.
    print("\n=== ROUTE (i2): PARKS THAT SHARE A FIELD OFFSET WITH YOU (NO EDGE NEEDED) ===")
    print("    A park with no call-graph edge to you can still be the same source")
    print("    fragment over the same record. That is how the func_800B6FB8 and")
    print("    func_800B72B8 parks were retired -- by a function that shares neither")
    print("    a caller nor a callee with either of them.")
    anyp2 = False
    for f in targets:
        lp = os.path.join(D(unit), f + '.s')
        try:
            ls = open(lp).read()
        except Exception:
            continue
        # THE OFFSET MUST BE ON THAT GLOBAL'S OWN ADDRESSING CHAIN. My first
        # version matched the global and the offset INDEPENDENTLY, so any
        # function mentioning D_80083160 anywhere and 0xC8 anywhere scored --
        # and on 2026-11-03 that sent an agent to read two parks in full whose
        # +0xC8 is on the 0x1F800000 SCRATCHPAD, a different record entirely.
        # Neither park reads that global at +0xC8 at all. The precision figure
        # I quoted when adding the route (6 of 60 stubs, 2 parks each) measured
        # selectivity, not correctness.
        bysym = offsets_by_global(ls)
        mysyms = set(bysym)
        for nm, (sf, u, built, tgt, diff) in sorted(pk.items()):
            if nm in edge_reported.get(f, ()):
                continue   # route (i) already reported it on the call graph
            try:
                pt = io.open(os.path.join(PROJ, 'docs/near-misses', sf),
                             encoding='utf-8', errors='replace').read()
            except Exception:
                continue
            psyms = set(re.findall(r'\b(D_[0-9A-F]+)\b', pt))
            # Report only a (global, offset) pair where BOTH the global and
            # that offset OFF THAT GLOBAL appear in the park.
            pairs = []
            for sym in sorted(mysyms & psyms):
                for o in sorted(bysym[sym]):
                    if int(o, 16) < 0x40:
                        continue
                    if park_offset_is_on(pt, sym, o):
                        pairs.append((sym, o))
            if pairs:
                print("  %-15s %s is PARKED at %d of %d words, %d DIFF (%s)"
                      % (f, nm, built, tgt, diff, u))
                for sym, o in pairs[:4]:
                    print("      %s + 0x%s -- on THAT global's own addressing"
                          " chain in your listing, and reached off THAT global"
                          " in the park too" % (sym, o.upper()))
                print("      docs/near-misses/%s" % sf)
                anyp2 = True
    if not anyp2:
        print("  none. %d park(s) exist overall; `ls docs/near-misses/` if your target"
              % len(pk))
        print("  looks like a family member of one of them.")

    # UNCONDITIONAL. This advice used to be gated behind a LOW top score, so a
    # group scoring 1.959 never saw it -- and that was the group whose decisive
    # relatives the rows[:4] cut had hidden. The two routes below have found the
    # right template SIX times where the metric did not.
    print("\n=== DO THIS REGARDLESS OF WHAT ROUTE (a) RANKED ===")
    print("    The score cannot see POSITION (a first-call entry point scores like any")
    print("    other) or LINE-LEVEL IDENTITY (a shared call expression WITH its constant")
    print("    arguments). Two cheap routes have beaten it six times:")
    for f in targets:
        n = incasm.get(f)
        if n: print("      sed -n '%d,%dp' csrc/%s.c    # %s's C-file neighbours"
                    % (max(1, n - 40), n + 40, unit, f))
    print("      ...and an IDIOM GREP: grep a distinctive STATEMENT shape out of the")
    print("      listing (a field expression, a constant, a global) rather than a callee")
    print("      name. e.g. grep -n 'D_800814A0 |= 0x8000' csrc/%s.c" % unit)

    # ------------------------------------------------------------------ (n)
    print("\n=== ROUTE (n): WHAT THIS UNIT ALREADY SAYS ABOUT YOUR TARGET ===")
    print("    Every mention of the target's own name in the C file, except its")
    print("    INCLUDE_ASM. This is one grep and no route scores it, and TWO agents")
    print("    asked for it independently after finding their decisive lead in a")
    print("    COMMENT that names the target in prose. One of them had the fixed")
    print("    neighbour window printed above START TWENTY LINES BELOW the template,")
    print("    so the load-bearing comment block was outside it; the other found a")
    print("    sentence that gave it both callees and the table base, and its")
    print("    function landed on the first compile from that one line.")
    # Classify each line by tracking block-comment state across the file. A
    # line's own text cannot tell you: a continuation line inside a /* */ block
    # starts with ordinary prose, and my first version labelled those "code".
    in_c, lineclass = False, {}
    for i, ln in enumerate(txt.split('\n'), 1):
        lineclass[i] = 'comment' if in_c else 'code'
        j = 0
        while j < len(ln):
            if in_c:
                e = ln.find('*/', j)
                if e < 0:
                    break
                in_c = False
                j = e + 2
            else:
                s = ln.find('/*', j)
                d = ln.find('//', j)
                if d >= 0 and (s < 0 or d < s):
                    break
                if s < 0:
                    break
                if lineclass[i] == 'code' and not ln[:s].strip():
                    lineclass[i] = 'comment'
                in_c = True
                j = s + 2
    anyn = False
    for f in targets:
        hits = []
        for i, ln in enumerate(txt.split('\n'), 1):
            if f not in ln or 'INCLUDE_ASM' in ln:
                continue
            kind = lineclass[i]
            if kind == 'code' and re.match(
                    r'^[A-Za-z_][\w \*]*\b%s\s*\([^;]*\);' % f, ln):
                kind = 'declaration'
            elif kind == 'code' and '/*' in ln.split(f)[0]:
                kind = 'comment'
            hits.append((i, kind, ln.strip()))
        if hits:
            anyn = True
            print("  %s -- %d mention(s):" % (f, len(hits)))
            for i, kind, ln in hits[:8]:
                print("      :%-6d %-11s %s" % (i, kind, ln[:92]))
            if len(hits) > 8:
                print("      ... %d more; grep -n '%s' csrc/%s.c"
                      % (len(hits) - 8, f, unit))
    if not anyn:
        print("  none -- this unit never names any of your targets outside its"
              " INCLUDE_ASM")

    print("\n=== ROUTE (h): YOUR RAREST GLOBALS, ACROSS THE WHOLE csrc/ TREE ===")
    print("    A global that is ABSENT from this unit is the strongest signal that")
    print("    the template lives in ANOTHER unit -- and neither the C-file-neighbour")
    print("    route nor an idiom grep can reach it, because both are unit-scoped.")
    print("    One agent's decisive template (a dual-view rule plus a declaration-order")
    print("    requirement) was in csrc/unk_154BC.c and was found exactly this way.")
    anyh = False
    for f in targets:
        for g in sorted(globs.get(f, ())):
            here = len(re.findall(r'\b' + g + r'\b', txt))
            hits = []
            for d2, _, fs in os.walk(os.path.join(PROJ, 'csrc')):
                for fn in fs:
                    if not fn.endswith('.c') or fn == unit + '.c': continue
                    fp = os.path.join(d2, fn)
                    try: o = io.open(fp, encoding='utf-8', errors='replace').read()
                    except Exception: continue
                    n = len(re.findall(r'\b' + g + r'\b', o))
                    if n: hits.append((n, os.path.relpath(fp, PROJ)))
            if hits and here <= 2:
                anyh = True
                print("  %-15s %s: %d use(s) HERE, elsewhere -> %s"
                      % (f, g, here, ', '.join('%s (%d)' % (p2, n) for n, p2 in sorted(hits, reverse=True)[:3])))
                print("      grep -rn %s include/ csrc/" % g)
    if not anyh:
        print("  none of your globals is rare here AND present elsewhere")

    # ------------------------------------------------------------------ (o)
    print("\n=== ROUTE (o): A GLOBAL THAT IS RARE *IN THIS UNIT* AND CARRIED BY PROSE ===")
    print("    Route (h) asks whether a global lives in ANOTHER unit, which is a")
    print("    different question and answers `none` for the case that matters most:")
    print("    a global with one or two LANDED users right here, whose comment block")
    print("    or INCLUDE_RODATA line is the template. That is where func_8009F77C's")
    print("    whole head came from -- `u8 s[4]; memcpy(s, <rodata>, 4);` with the")
    print("    alignment-1 lwl/lwr copy -- and route (h) said `none of your globals is")
    print("    rare here AND present elsewhere`, which was true and useless. The agent")
    print("    found it with one grep and asked for this route by name.")
    anyo = False
    # Every INCLUDE_RODATA line in the unit, by symbol.
    inc_rod = {}
    for m in re.finditer(r'^.*INCLUDE_RODATA\([^)]*?(D_[0-9A-Fa-f]+)\s*\).*$', txt, re.M):
        inc_rod.setdefault(m.group(1), txt[:m.start()].count('\n') + 1)
    for f in targets:
        for g in sorted(globs.get(f, ())):
            # USERS IN THIS UNIT = landed bodies that reference it. Not a raw
            # count over `txt`, which double-counts a declaration and every
            # mention in prose.
            users = sorted(d for d, b in btext.items() if re.search(r'\b' + g + r'\b', b))
            if len(users) > 3:
                continue
            # Does any LANDED COMMENT name it? Comments are where this unit
            # keeps its field maps, and a comment is not a body, so `users`
            # cannot see one.
            prose = []
            for m in re.finditer(r'/\*(?:[^*]|\*(?!/))*\*/', txt, re.S):
                if re.search(r'\b' + g + r'\b', m.group(0)):
                    prose.append(txt[:m.start()].count('\n') + 1)
            rod = inc_rod.get(g)
            if not users and not prose and rod is None:
                continue
            anyo = True
            print("  %-15s %s: %d LANDED user(s) in this unit%s"
                  % (f, g, len(users),
                     (' -- ' + ', '.join(users)) if users else ''))
            if rod is not None:
                print("      *** INCLUDE_RODATA at csrc/%s.c:%d -- the BYTES are in"
                      " this unit, so the template is a copy, not a reference ***"
                      % (unit, rod))
            if prose:
                print("      named in %d comment block(s): %s"
                      % (len(prose), ', '.join('csrc/%s.c:%d' % (unit, n)
                                               for n in prose[:6])))
                print("      READ THOSE BLOCKS. A comment naming your global is prose a")
                print("      previous landing wrote about this exact record, and it has")
                print("      carried the decisive lead five times.")
            print("      grep -n %s csrc/%s.c" % (g, unit))
    if not anyo:
        print("  no global of yours is rare in this unit AND carried by prose or rodata")

    # ------------------------------------------------------------------ (p)
    print("\n=== ROUTE (p): CONSTANT ADDRESSES -- WHO ELSE USES THE SAME RAW HEX ===")
    print("    Every scored route keys on a DECLARED SYMBOL, so a function whose")
    print("    shared thing is a CONSTANT ADDRESS is invisible to all of them. That")
    print("    is a structural hole, not a tuning problem: func_800B8A40 is a")
    print("    line-for-line template for func_800B9830 -- same bound, same")
    print("    `base[i*2 + 0x33A5]` pair, same second block -- and appeared in NO")
    print("    route at any score, because 0x800133A4 is not a symbol. This route")
    print("    is the grep that finds it.")
    anyp = False
    for f in targets:
        lt = listing(unit, f)
        if not lt:
            continue
        # The addresses a CONSTANT-ADDRESS function materialises, as raw hex.
        # Take them from the out-of-image globals the listing names, since
        # splat renders a constant the same way it renders a symbol.
        addrs = sorted(set(
            int(g[2:], 16) for g in re.findall(r'%(?:hi|lo)\((D_[0-9A-Fa-f]+)\)', lt)
            if constaddr_candidate(g)))
        if not addrs:
            continue
        for a in addrs:
            # SKIP THE MIS-SYMBOLISATION CLASS. splat pairs a `lui` with any
            # following `addiu`/load displacement, so "D_80010004" can be the
            # integer 4 and "D_80010002" the integer 2 -- both measured in
            # this unit, and declaring either blocks the landing. A low half
            # under 0x100 is that class far more often than it is an address,
            # and searching for `0x4` matched 178 bodies and 162 comment
            # blocks, which is noise that also legitimises a fake symbol.
            if (a & 0xFFFF) < 0x100:
                print("  %-15s 0x%08X: low half 0x%X is almost certainly a splat"
                      " MIS-SYMBOLISATION" % (f, a, a & 0xFFFF))
                print("      (splat paired one `lui` with a small immediate or"
                      " displacement). Do NOT declare it; read the instruction.")
                continue
            # Search for the raw hex in the spellings an author would write:
            # the full address, and the low half only when it is wide enough
            # to be distinctive.
            pats = ['0x%08X' % a, '0x%X' % a]
            if (a & 0xFFFF) >= 0x1000:
                pats.append('0x%X' % (a & 0xFFFF))
            users = {}
            for d, b in btext.items():
                for pat in pats:
                    if re.search(re.escape(pat), b, re.I):
                        users.setdefault(d, set()).add(pat)
            # And comment text, which is where this unit keeps its field maps
            # and where route (n) cannot look, because it greps NAMES.
            prose = []
            for m in re.finditer(r'/\*(?:[^*]|\*(?!/))*\*/', txt, re.S):
                if any(re.search(re.escape(pt), m.group(0), re.I) for pt in pats):
                    prose.append(txt[:m.start()].count('\n') + 1)
            if not users and not prose:
                continue
            # A hex half the unit uses is not a discriminator. Say so rather
            # than printing a page of names: route (a)'s own lesson.
            if len(users) > 20:
                print("  %-15s 0x%08X: %d landed bodies use that hex -- TOO COMMON"
                      " to discriminate, not listed" % (f, a, len(users)))
                continue
            anyp = True
            print("  %-15s 0x%08X (%s):" % (f, a, ', '.join(pats)))
            if users:
                print("      %d landed body/bodies use that hex: %s"
                      % (len(users), ', '.join('%s (:%d)' % (d, lines.get(d, 0))
                                               for d in sorted(users)[:6])))
                if len(users) > 6:
                    print("      ... and %d MORE, NOT shown" % (len(users) - 6))
            if prose:
                print("      named in %d comment block(s): %s"
                      % (len(prose), ', '.join('csrc/%s.c:%d' % (unit, n)
                                               for n in prose[:6])))
            print("      grep -n '0x%X' csrc/%s.c" % (a & 0xFFFF, unit))
    if not anyp:
        print("  no target of yours materialises a constant address with another user")

    # ------------------------------------------------------------------ (m)
    print("\n=== ROUTE (m): RARE FIELD OFFSETS -- THE DISCRIMINATOR ROUTE (h) CANNOT SEE ===")
    print("    Route (h) ranks the SYMBOL. When your target's only global is a")
    print("    COMMON one, every scored route reports nothing -- and the")
    print("    discriminator is then the DISPLACEMENT, not the symbol. On")
    print("    2026-11-03 func_800BDC98 and func_800BDDD0 reached D_80083160 (39")
    print("    users, so route (h) said nothing) at +0x1DC, which has six users")
    print("    all in ONE landed family; csrc/ovl_town.c already typed that record")
    print("    and wrote the index idiom out verbatim. The agent found it by")
    print("    grepping the offset by hand after route (a), (h), (j) and (k) all")
    print("    came back empty. This route is that grep.")
    anym = False
    for f in targets:
        lp = os.path.join(D(unit), f + '.s')
        try: ls = open(lp).read()
        except Exception: continue
        # Displacements and addiu constants big enough to be a field offset
        # rather than a small immediate or a frame slot.
        # TWO SOURCES, AND THEY ARE NOT EQUALLY TRUSTWORTHY. A displacement
        # `0xNNN(reg)` in a load or store IS a field offset. An `addiu`
        # immediate MIGHT be one -- it is how a base+offset pointer is formed,
        # which is the +0x1DC case this route exists for -- but it is equally
        # an arithmetic addend. On 2026-11-03 this route reported
        # "func_800A378C +0x2000: 3 landed function(s) use it" where 0x2000 is
        # the addend in `(x * 2 + 0x2000) * 16` and the three users all test it
        # as a FLAG. Nothing in the instruction tells them apart, so report
        # which source each came from and let the reader discount accordingly.
        disp, arith = set(), set()
        for m in re.finditer(r'0x([0-9A-Fa-f]{2,4})\(', ls):
            disp.add(int(m.group(1), 16))
        # NOT `$zero` as the source: `addiu $a1, $zero, 0x2000` is a CONSTANT
        # LOAD, not a field offset, and the first version of this route ranked
        # one of those top for func_800B72B8 and never printed the +0x1DC it
        # was written to find.
        for m in re.finditer(
                r'addiu\s+\$\w+,\s*\$(?!zero\b)\w+,\s*0x([0-9A-Fa-f]{2,4})\b',
                ls):
            arith.add(int(m.group(1), 16))
        arith -= disp
        offs = sorted(disp) + sorted(arith)
        rows = []
        for o in offs:
            if o < 0x40 or o % 2:
                continue
            pat = r'(?:0x%X\b|\bunk_%02X\b|\bpad_%02X\b)' % (o, o, o)
            # Rank by how many LANDED FUNCTIONS use it, not by how many times
            # the text occurs. A field name appears once per declaration and
            # once per use, so +0x1DC reads as 18 textual hits while being the
            # property of six functions -- and a raw-text threshold filtered
            # out the very case this route exists for.
            sites = []
            for nm in landed:
                if nm in targets: continue
                i = txt.find(landed[nm])
                if i < 0: continue
                j = txt.find('\n}', i)
                if j < 0: j = i
                if re.search(pat, txt[i:j], re.I):
                    sites.append(nm)
            if sites and len(sites) <= 8:
                # Displacement-derived first: those are certainly offsets.
                rows.append((0 if o in disp else 1, len(sites), o, sites))
        ranked_m = sorted(rows)
        for kind, n, o, sites in ranked_m[:4]:
            anym = True
            # SAY WHEN THE LIST IS CUT. This printed "+0x74: 8 landed
            # function(s) use it:" followed by FOUR names, so the count and
            # the list disagreed by construction and silently -- and this
            # route exists precisely for the case where the fifth name is the
            # discriminator. Route (a) already says "N MORE below the cut";
            # this one did not.
            shown = sites[:4]
            print("  %-15s +0x%X: %d landed function(s) use it%s: %s%s"
                  % (f, o, n,
                     '' if kind == 0 else
                     '  [from an addiu IMMEDIATE, so it may be an arithmetic'
                     ' addend rather than a field offset -- check the opcode]',
                     ', '.join('%s (:%d)' % (s, lines.get(s, 0))
                               for s in shown),
                     '' if len(sites) <= len(shown) else
                     '  ... and %d MORE, NOT shown -- ask for them if these'
                     ' do not explain your target' % (len(sites) - len(shown))))
            print("      grep -nE 'unk_%02X|0x%X' csrc/%s.c" % (o, o, unit))
        if len(ranked_m) > 4:
            print("      ... %d MORE OFFSET(S) for %s below the cut, NOT shown."
                  % (len(ranked_m) - 4, f))
    if not anym:
        print("  no rare field offset with a landed user")
    print("    NOTE: for a SCRATCHPAD record (a 0x1F800000 base) this route's")
    print("    offset grep crosses into whatever the unit's busiest struct")
    print("    uses at the same offsets and returns noise. One group's")
    print("    +0x74/+0x76/+0x7A/+0x84 hits were all UnkTownActor fields in")
    print("    unrelated functions, nowhere near the record being typed.")

    print("\n=== ROUTE (f): LANDED CALL SITES OF YOUR CALLEES, WITH THEIR ARGUMENTS ===")
    print("    Line-level identity, which the rarity score cannot see: a landed")
    print("    function that passes the SAME CONSTANTS to the same callee is a")
    print("    template even when it shares nothing else. One such pair was scored")
    print("    0.225 while a wrong-shape relative scored 2.000.")
    anyf = False
    for c in sorted(every):
        rows = []
        for d in landed:
            if d in targets or c not in body.get(d, ()): continue
            a = arglist(btext.get(d, ''), c)
            if a is None: continue
            rows.append((d, a))
        # only interesting when a constant or global appears in the arguments
        rows = [(d, a) for d, a in rows if re.search(r'\b(0x[0-9A-Fa-f]+|D_[0-9A-F]{6,8})\b', a)]
        if not rows: continue
        anyf = True
        print("  %s, as called by landed code:" % c)
        for d, a in rows[:5]:
            print("      csrc/%s.c:%-6d %s(%s)" % (unit, lines[d], c, a[:66]))
    if not anyf: print("  no landed call site of any of your callees passes a constant")

    print("\n=== ROUTE (l): LANDED SIBLINGS DRIVEN BY THE SAME CALLER ===")
    print("    A function that drives your target AND a landed function from the SAME")
    print("    BODY is very often driving two cut-downs of one recipe. No other route")
    print("    scores that edge. It was decisive for func_800A2AF8, whose template")
    print("    func_800AA10C route (a) buried in a '50 MORE below the cut, NOT shown'")
    print("    bucket -- and route (k) would have missed it too, because the two call")
    print("    different projection helpers so neither callee set contains the other.")
    anyl = False
    for f in targets:
        seen = []
        for d in sorted(callers.get(f, ())):
            if d in landed:
                dc = set(body.get(d, ()))
                where = 'LANDED caller csrc/%s.c:%d' % (unit, lines.get(d, 0))
            else:
                dt = listing(unit, d)
                if dt is None:
                    continue
                dc = set(re.findall(r'jal\s+(\w+)', dt))
                where = 'STUB caller, read %s/%s.s' % (
                    D(unit).replace(PROJ + '/', ''), d)
            sibs = sorted(c for c in dc
                          if c != f and c not in targets and c in landed)
            if sibs:
                seen.append((d, where, sibs))
        for d, where, sibs in seen[:3]:
            print("  %-15s driven by %s alongside:" % (f, d))
            print("                  %s" % where)
            for c in sibs[:4]:
                print("                  LANDED SIBLING %-15s csrc/%s.c:%d"
                      % (c, unit, lines.get(c, 0)))
            if len(sibs) > 4:
                print("                  ... and %d more: %s"
                      % (len(sibs) - 4, ', '.join(sibs[4:])))
            anyl = True
    if not anyl:
        print("  no caller of any target also calls a landed function")

    print("\n=== ROUTE (k): CALLEE-SET CONTAINMENT, WHICH RARITY CANNOT SEE ===")
    print("    Route (a) weights each shared callee by 1/freq, so a family whose callees")
    print("    are all COMMON weights to ~0 and the metric reports nothing. That has now")
    print("    been reported ELEVEN times. The worst case: route (a) said 'NO RELATIVE AT")
    print("    ANY SCORE' for func_80091000 while its head was func_80091260's verbatim")
    print("    and its whole body a strict SUBSET of its own sibling's. Containment is a")
    print("    structural fact and needs no weighting, so it is reported unweighted here.")
    anyk = False
    for f in targets:
        mine = allcallees.get(f, set())
        if not mine:
            continue
        rows = []
        for d, dc in body.items():
            if d in targets or not dc:
                continue
            dc = set(dc)
            # FLOOR, added after the route's first noise report. On a target
            # with eight callees, containment-by-one names everything that
            # calls the unit's commonest callee: func_800BD688's set is
            # {func_80069EF8}, i.e. rand(), which is containment by the single
            # most common symbol in the unit and carries no information.
            # Unweighted is the POINT of this route, so the fix is a size
            # floor, not a weight: a contained set of one says nothing.
            # Checked against the case the route was built for, where the
            # decisive contained set has seven members.
            if min(len(mine), len(dc)) < 2:
                continue
            if mine <= dc:
                rows.append((0, len(dc) - len(mine), 'YOURS IS A SUBSET of   ', d, ''))
            elif dc <= mine:
                rows.append((0, len(mine) - len(dc), 'IS A SUBSET of yours:  ', d, ''))
            else:
                # NEAR-SUBSET: all but ONE of your callees is covered. The
                # tolerance is on `mine - theirs` ALONE, deliberately -- a first
                # version tested the SYMMETRIC difference, which can never fire
                # for the case that motivated this route (func_80091000 has 7
                # callees, func_80091260 has 26, and their head is verbatim
                # identical; the symmetric difference is 20). What matters is
                # that a template covers all but one of YOUR calls; how much
                # else it does is irrelevant.
                only_mine = mine - dc
                if len(only_mine) == 1:
                    d1 = only_mine.pop()
                    rows.append((1, len(dc - mine), 'ALL BUT ONE OF YOURS:  ', d,
                                 '  (not covered: %s)' % d1))
        # exact containment first, then near; within each, the tightest fit
        rows.sort(key=lambda r: (r[0], r[1]))
        for _, _, why, d, extra in rows[:5]:
            print("  %-15s %s %s   csrc/%s.c:%d%s"
                  % (f, why, d, unit, lines.get(d, 0), extra))
            print("                  sed -n '%d,%dp' csrc/%s.c"
                  % (lines.get(d, 1), lines.get(d, 1) + 60, unit))
            anyk = True
        # Say what is below the cut. Measured on the case that motivated this
        # route: 9 candidates, the sibling the agent actually diffed against at
        # rank 4 (printed) and the two family HEADS at ranks 8 and 9 (not).
        # Route (a) reported zero candidates for the same function.
        if len(rows) > 5:
            print("                  ... %d MORE below the cut, and route (a) scored this"
                  % (len(rows) - 5))
            print("                  function at NOTHING -- read them if the five miss:")
            print("                  %s" % ', '.join(r[3] for r in rows[5:]))
    if not anyk:
        print("  no landed function's callee set contains, or is contained by, any target's")

    print("\n=== ROUTE (j): CROSS-UNIT RELATIVES BY SHARED CALLEE SET ===")
    print("    Route (f) and route (a) are UNIT-SCOPED, and for code that mostly calls")
    print("    RESIDENT functions that is the wrong boundary. On func_8008B818 the")
    print("    decisive template was csrc/unk_72A8.c:1871 -- line for line, eight")
    print("    statements -- and route (a) scored 0.101, route (h) correctly reported")
    print("    no rare shared global, and route (f) printed this unit's call sites and")
    print("    stopped at the unit boundary. The agent found it by grepping a callee")
    print("    across csrc/*.c by hand. This is that grep, scored.")
    xfun, xfreq = {}, {}
    for fp in sorted(glob.glob(os.path.join(PROJ, 'csrc', '*.c'))):
        if os.path.basename(fp) == unit + '.c':
            continue
        try:
            xt = io.open(fp, encoding='utf-8', errors='replace').read()
        except Exception:
            continue
        rel = os.path.relpath(fp, PROJ)
        # DEFINITIONS ONLY, and "no `;`" is not enough. A MULTI-LINE
        # DECLARATION's first line ends in `,` and has no `;` either, so the
        # old test matched it -- and two agents reported route (j) ranking
        # `func_800C3050` at csrc/ovl_town_800BF3F0.c:1278 as a top cross-unit
        # relative, which is that name's DECLARATION in the duplicated
        # preamble, for a function that is an INCLUDE_ASM STUB in the same
        # file. Route (j) sits beside landed-only routes, so the row reads as
        # landed C. Same declaration-line class b5c57a6 fixed for routes (c)
        # and (f).
        #
        # Two gates now: the line must END in `)` or `) {`, which a
        # declaration's first line cannot, and the name must not be a stub in
        # that file.
        stubs_there = set(re.findall(
            r'INCLUDE_ASM\("[^"]+",\s*(func_[0-9A-Fa-f]{8})\)', xt))
        defs = [(m.group(1), m.start(), xt[:m.start()].count('\n') + 1)
                for m in re.finditer(
                    r'^[A-Za-z_][^;\n]*?\b(func_[0-9A-Fa-f]{8})\s*\('
                    r'[^;\n]*\)\s*\{?\s*$', xt, re.M)
                if m.group(1) not in stubs_there]
        for i, (nm, st, ln) in enumerate(defs):
            end = defs[i + 1][1] if i + 1 < len(defs) else len(xt)
            called = set(re.findall(r'\b(func_[0-9A-Fa-f]{8})\s*\(', xt[st:end]))
            called.discard(nm)
            if called:
                xfun[(rel, nm, ln)] = called
                for c in called:
                    xfreq[c] = xfreq.get(c, 0) + 1
    anyj = False
    for f in targets:
        mine = allcallees.get(f, set())
        if not mine:
            continue
        rows = []
        for (rel, nm, ln), called in xfun.items():
            sh = mine & called
            if not sh:
                continue
            rows.append((sum(1.0 / xfreq[c] for c in sh), rel, nm, ln, sh))
        rows.sort(reverse=True)
        if not rows:
            continue
        anyj = True
        print("  %s:" % f)
        for sc, rel, nm, ln, sh in rows[:3]:
            print("      %.3f  %s:%-5d %s" % (sc, rel, ln, nm))
            print("             shares: %s" % ', '.join(sorted(sh)))
            print("             sed -n '%d,%dp' %s" % (ln, ln + 40, rel))
    if not anyj:
        print("  none of your callees is called by a landed function in another unit")

    print("\n=== LANDED RELATIVES, route (a): shared callees+globals, WEIGHTED BY RARITY ===")
    print("    (score = sum 1/freq over shared callees, +2/freq per shared global;")
    print("     a callee the whole unit calls scores ~0, a rare one dominates)")
    # Macro aliases for globals: `#define NAME (... D_XXXXXXXX ...)`. Without
    # these, a landed body that reaches a global through a macro scores 0 on
    # that global, which hid the decisive template for one group entirely.
    alias = {}
    for m in re.finditer(r'^#define\s+(\w+)\s+(.*)$', txt, re.M):
        for g in re.findall(r'\b(D_[0-9A-F]{6,8})\b', m.group(2)):
            alias.setdefault(g, set()).add(m.group(1))

    body_src = {}
    for d in landed:
        # NOT `defoff.get(d, txt.index(...))`: Python evaluates a default
        # EAGERLY, so the fallback search ran even when defoff had the key --
        # and once landed[] started holding a definition's first LOGICAL line
        # (joined across a wrapped parameter list) that search raises
        # ValueError, because the joined text appears nowhere in the file.
        i = defoff[d] if d in defoff else txt.index(landed[d])
        j = txt.find('\n}', i)
        body_src[d] = txt[i:j if j > 0 else i+400]
    hits = []
    for d, dc in body.items():
        if d in targets: continue
        for f in targets:
            sc = dc & allcallees.get(f, set())
            bs = body_src.get(d, '')
            sg = set(g for g in globs.get(f, ())
                     if g in bs or any(a in bs for a in alias.get(g, ())))
            gfreq = {g: max(1, sum(1 for b in body_src.values()
                                    if g in b or any(a in b for a in alias.get(g, ()))))
                     for g in sg}
            s = (sum(1.0 / freq.get(c, 1) for c in sc)
                 + sum(2.0 / gfreq[g] for g in sg))
            # CONJUNCTION BONUS: how many landed bodies contain BOTH a shared
            # callee and a shared global? That pair is what makes a template,
            # and it is far rarer than either element.
            if sc and sg:
                pair = sum(1 for b in body_src.values()
                           if any(c in b for c in sc) and any(g in b for g in sg))
                s += 3.0 / max(1, pair)
            # ONE ROW PER (relative, target), not one per relative. The
            # earlier code kept only each relative's single best target and
            # broke ties with a strict `>`, which went to whichever target
            # came FIRST on the command line. So a target whose relative set
            # TIES or is a SUBSET of a sibling's printed "NO RELATIVE AT ANY
            # SCORE" while its line-for-line template sat in the sibling's
            # list, and reversing the argument order moved the whole section.
            # Reported independently by two agents in one batch:
            # func_800951B4 (ties with func_80094DA8 at 1.364 on
            # func_8009567C, the verbatim 4-statement head of all three) and
            # func_800B1960 (identical global set to func_800B3EEC, every
            # relative tying at 0.487, including func_800B67A4, its own
            # line-for-line template). This is a located cause for the
            # "ranking reported inverted or empty" class.
            if s > 0:
                hits.append((s, d, f, sorted(sc), sorted(sg)))
    # Emit top-N PER TARGET, not a flat global top-N. As a flat list, eight rows
    # for seven targets named only TWO of them -- five targets got no relative at
    # all, and the cut discarded a line-for-line one-word-away template that
    # scored identically to the rows that were printed. The section's whole
    # purpose is a relative per target.
    per = {}
    for s, d, f, sc, sg in sorted(hits, reverse=True):
        per.setdefault(f, []).append((s, d, sc, sg))
    ranked = sorted(hits, reverse=True)
    if ranked and ranked[0][0] < 0.15:
        # The earlier wording here said "ROUTE (a) FOUND NOTHING ... do NOT name
        # these as relatives". That produced a FALSE NEGATIVE as costly as the
        # false positives it was added to prevent: on one group the 0.156 top
        # hit WAS the line-for-line template, and saying so landed two
        # functions on the first compile. A low score means the METRIC has
        # nothing, not that the unit has nothing.
        print("  *** LOW SCORES (best %.3f). The metric has nothing here -- but that is"
              % ranked[0][0])
        print("      NOT proof the unit has nothing. It cannot see two things:")
        print("        POSITION -- a callee that is the FIRST call, before any gate, is")
        print("          a family's shared entry point and far more diagnostic than a")
        print("          rare callee in a tail, yet it scores 1/freq like any other.")
        print("        LINE-LEVEL IDENTITY -- what makes a function a template is often")
        print("          a shared call expression WITH ITS CONSTANT ARGUMENTS. One")
        print("          global had 11 landed call sites, so it scored as 'common',")
        print("          but only 2 passed the same `(x - 0x20)` form.")
        print("      So READ the top entries anyway.")
    for f in targets:
        rows = per.get(f, [])
        print("  -- for %s --%s" % (f, '' if rows else '  NO RELATIVE AT ANY SCORE'))
        for s, d, sc, sg in rows[:6]:
            print("  %6.3f  %-15s csrc/%s.c:%-6d" % (s, d, unit, lines[d]))
            print("          callees %s   globals %s" % (','.join(sc) or '-', ','.join(sg) or '-'))
        if len(rows) > 6:
            print("      ... %d MORE below %.3f -- NOT shown. The cut has already hidden"
                  % (len(rows) - 6, rows[5][0]))
            print("      functions whose comments carried the decisive lever, so if the")
            print("      shown rows do not explain your target, ask for more rather than")
            print("      concluding the metric has nothing.")
    if not hits: print("  none -- hand-decode, and say so in the brief")

    print("\n=== LANDED CALLERS (route (c): settles arity AND constant arguments) ===")
    anyc = False
    for f in targets:
        for d, dc in body.items():
            if f in dc and d not in targets:
                a = arglist(btext.get(d, ''), f)
                if a is None: continue
                # 60 characters truncated a ten-argument call to six, which
                # hides the ARITY -- the thing this route exists to settle.
                print("  %-15s called by LANDED %s (csrc/%s.c:%d) as %s(%s)"
                      % (f, d, unit, lines[d], f,
                         a if len(a) <= 240 else a[:240] + ' ...TRUNCATED'))
                # THE COUNT IS A LOWER BOUND, NOT THE ARITY. A parameter
                # the caller already holds in the right ABI register emits no
                # setup move, so it is invisible at the call site AND costs
                # nothing to omit from the source. Measured 2026-11-03:
                # func_800B0134 takes TWO parameters, func_800B0158 calls it
                # as `func_800B0134(s)`, and the caller measures 16 words / 0
                # differing with and without the second argument -- because
                # its own `void *p` is already in $a1. A one-parameter
                # func_800B0134 that reads the pointer back out of s + 0x18
                # is 8 differing. The sweep doc records this trap in the
                # CALLEE direction; this is the CALLER direction.
                n = nargs(a)
                fwd = _forwarding(landed.get(d, ''), a)
                print("      %d argument(s) at that call site -- a LOWER"
                      " BOUND on arity, not the arity%s" % (n, fwd))
                anyc = True; witnessed.add(f)
    if not anyc: print("  none -- no LANDED call site constrains arity")
    print("\n=== ROUTE (g): YOUR OWN GROUP MEMBERS AS WITNESSES ===")
    print("    A group member that calls another is a witness, and the search used to")
    print("    miss it entirely -- one batch had a member calling a sibling with four")
    print("    arguments while the sibling was listed as having no witness at all.")
    anyg = False
    for f in targets:
        for o in targets:
            if o == f: continue
            lt = listing(unit, o)
            if lt and re.search(r'jal\s+' + f + r'\b', lt):
                rel = ('BELOW it in the file, so a too-narrow spelling is a HARD ERROR'
                       if incasm.get(o, 0) > incasm.get(f, 0) else 'above it')
                print("  %-15s is jal'd by group member %-15s (%s)" % (f, o, rel))
                anyg = True; witnessed.add(f)
    if not anyg: print("  no group member calls another")

    missing = [f for f in targets if f not in witnessed and listing(unit, f)]
    if missing:
        print("\n  *** NO ARITY WITNESS ANYWHERE for: %s" % ', '.join(missing))
        # The old wording said "no %hi", which CONTRADICTED this tool's own
        # INSTALLED HANDLERS section four lines earlier when a function is
        # address-taken. The conclusion was right and the stated reason was
        # false, which is worse than saying less.
        print("      No jal and no C call site in this unit.")
        # CORRECTED AGAIN 2026-11-03. This block used to say an address-take
        # "constrains nothing about arity" and that the absence of a witness
        # was "a FACT, not a gap in the search". BOTH ARE FALSE, and three
        # agents reported it wrong in one day -- each of them found the witness
        # the tool had declared not to exist. An install at a known dispatcher
        # slot constrains arity COMPLETELY, through the dispatcher's own cast,
        # and this tool does not follow that edge out of the unit.
        # FOLLOW THE EDGE instead of only telling the agent to. This banner
        # has now been wrong for FOUR agents, each of whom found the witness
        # the tool said did not exist -- the last one through
        # `(*(void (**)(u8 *))(q + 0x1C))(q);` at csrc/ovl_town.c:18351, a
        # prototyped cast through the very slot route (b) had already named as
        # the install target. The slot was known; nothing looked for the cast.
        found_any = False
        for f in missing:
            for off, who, how in install_slots(unit, f, landed, btext):
                casts = slot_casts(txt, off)
                if not casts:
                    print("      %s is installed at +0x%X by %s (%s) -- no"
                          " prototyped cast through that offset in this unit"
                          % (f, off, who, how))
                    continue
                found_any = True
                print("      *** WITNESS FOUND: %s is installed at +0x%X by %s"
                      " (%s), and this unit dispatches that slot through a"
                      " PROTOTYPED cast:" % (f, off, who, how))
                for ln, ret, args in casts:
                    print("            csrc/%s.c:%-6d  %s (*)(%s)"
                          % (unit, ln, ret, args or 'void'))
                print("          A prototyped cast states the arity AND the"
                      " parameter types of every handler at that slot, so this"
                      " IS an arity witness. Read the dispatcher before")
                print("          falling back to register reads.")
        if found_any:
            print("      (The banner above is therefore too strong for at least"
                  " one of those names.)")
        print("      *** AN ADDRESS-TAKE IS NOT NOTHING. If any section above names")
        print("      your target as INSTALLED or address-taken, find which SLOT the")
        print("      installer writes and read that slot's dispatcher. The search")
        print("      above does this automatically now; the casts below are the ones")
        print("      known by hand, each read out of the source, and")
        print("      each settles arity AND parameter types for every handler in its")
        print("      class. Three agents in one day found a witness here after this")
        print("      tool told them none existed:")
        for slot, where, sig, note in DISPATCHERS:
            print("        %-22s %s" % (slot, where))
            print("            %s" % sig)
            print("            %s" % note)
        print("      Also: a group member that `jalr`s through a slot it was handed by")
        print("      another member IS a call site, and route (g) only looks for a direct")
        print("      `jal`, so it does not see that either.")
        print("      THEN fall back to register reads in the target\'s own body (an")
        print("      argument register READ before it is written is a real parameter).")
        print("      A dead MIDDLE parameter IS witnessed, by the gap it leaves in the")
        print("      $aN sequence; only a dead TRAILING parameter is invisible.")

if __name__ == '__main__':
    main()
