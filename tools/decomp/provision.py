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
    lines, lineno = {}, 0
    for ln in txt.split('\n'):
        lineno += 1
        m = re.match(r'^[A-Za-z_][\w \*]*?(func_[0-9A-Fa-f]{8})\s*\(', ln)
        # A DEFINITION's first line ends in `)` or `)  {`. A one-line
        # prototype ends in `;` and a MULTI-LINE prototype's first line ends in
        # `,` -- and that second case produced a FABRICATED landed caller:
        # func_800C3050's two-line declaration was read as a definition, so
        # every later declaration line was swallowed as part of its "body" and
        # reported as a call. A wrong landed caller is the worst provisioning
        # error there is, because it reads as an arity-and-constants witness.
        # Belt and braces: a name with an INCLUDE_ASM line is a stub, never
        # landed, whatever the text looks like.
        if m and ';' not in ln and re.search(r'\)\s*\{?\s*$', ln) \
                and ('INCLUDE_ASM("asm/nonmatchings/%s", %s)' % (unitname, m.group(1))) not in whole:
            cur = m.group(1); landed[cur] = ln.strip(); body[cur] = set(); depth = 0
            # Record the line HERE. Searching for the text later finds the
            # single-line PROTOTYPE instead, because the definition line is a
            # prefix of it -- that sent a brief's reader to :2206 (a
            # declaration) rather than :19205 (the definition).
            lines[cur] = lineno
        if cur:
            body[cur].update(re.findall(r'\b(func_[0-9A-Fa-f]{8})\s*\(', ln))
            depth += ln.count('{') - ln.count('}')
            if depth <= 0 and '}' in ln: cur = None
    protos = {}
    for m in re.finditer(r'^([A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]{8})\s*\([^;{]*\);)', txt, re.M):
        protos.setdefault(m.group(2), m.group(1).strip())
    for k in body: body[k].discard(k)
    return landed, protos, body, txt, lines

def main():
    unit, targets = sys.argv[1], sys.argv[2:]
    src = os.path.join(PROJ, 'csrc', unit + '.c')
    landed, protos, body, txt, lines = defs_and_protos(src, unit)
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
        gl  = sorted(set(re.findall(r'%hi\((D_[0-9A-F]+)\)', t)))
        jt  = len(re.findall(r'jr\s+\$(?!ra)', t))
        rod = 'YES' if '.section .rodata' in t or 'dlabel D_' in t else 'no'
        allcallees[f], installs[f], globs[f] = set(cal), set(ins), set(gl)
        print("  %s  %d words  jumptable=%d  owns-rodata=%s" % (f, w, jt, rod))
        print("      callees : %s" % (', '.join(cal) or 'NONE'))
        print("      installs: %s" % (', '.join(ins) + '   <- %hi on a FUNCTION: masked, no jal, VERIFY BY RELOC'
                                        if ins else 'none'))
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
            tags = []
            for g in gl:
                if re.search(r'^\s*extern\b[^;]*\b' + g + r'\b', src_txt, re.M):
                    tags.append(g)
                elif g in incglob:
                    fp, ln = incglob[g]
                    tags.append('%s (declared %s:%d -- do NOT redeclare)' % (g, fp, ln))
                else:
                    tags.append(g + ' *** NO DECLARATION, you must add one ***')
            print("      globals : %s" % ', '.join(tags))
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
        else: cls = '*** NOT IN THIS UNIT (resident) ***'
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
        i = txt.index(landed[d]); j = txt.find('\n}', i)
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
            if '\n' in inner: continue
            return inner
        return None

    witnessed = set()
    print("\n=== ROUTE (d): WHO TAKES MY TARGET'S ADDRESS (the reverse install edge) ===")
    print("    Named by two agents as the most valuable fact their brief lacked: the")
    print("    installer's own declaration settles the target's arity when nothing")
    print("    calls it. An address-taker is NOT a caller and constrains nothing by")
    print("    itself -- it is the installer's DECLARATION that is the witness.")
    anyd = False
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
        for why, nm in rel:
            sf, u, built, tgt, diff = pk[nm]
            print("  %-15s %s %s is PARKED at %d of %d words, %d DIFF (%s)"
                  % (f, why, nm, built, tgt, diff, u))
            print("                  docs/near-misses/%s" % sf)
            anyp = True
    if not anyp:
        print("  none in the call graph. %d park(s) exist overall; `ls docs/near-misses/`"
              % len(pk))
        print("  if your target looks like a family member of one of them.")

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
        # Definitions only: a line starting in column 0 with a func_ name and
        # no `;` -- the same test the sweep recipe uses, for the same reason.
        defs = [(m.group(1), m.start(), xt[:m.start()].count('\n') + 1)
                for m in re.finditer(
                    r'^[A-Za-z_][^;\n]*?\b(func_[0-9A-Fa-f]{8})\s*\([^;\n]*$',
                    xt, re.M)]
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
        i = txt.index(landed[d]); j = txt.find('\n}', i)
        body_src[d] = txt[i:j if j > 0 else i+400]
    hits = []
    for d, dc in body.items():
        if d in targets: continue
        best = None
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
            if s > 0 and (best is None or s > best[0]):
                best = (s, f, sorted(sc), sorted(sg))
        if best: hits.append((best[0], d) + best[1:])
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
                print("  %-15s called by LANDED %s (csrc/%s.c:%d) as %s(%s)"
                      % (f, d, unit, lines[d], f, a[:60]))
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
        print("      *** BUT AN ADDRESS-TAKE IS NOT NOTHING, AND THIS TOOL DOES NOT")
        print("      FOLLOW IT. If any section above names your target as INSTALLED or")
        print("      address-taken, find which SLOT the installer writes and read that")
        print("      slot\'s dispatcher. Each cast below was read out of the source, and")
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
