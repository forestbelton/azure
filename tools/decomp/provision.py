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
import io, os, re, sys, subprocess

PROJ = '/home/case/projects/azure'
def D(unit): return os.path.join(PROJ, 'asm/nonmatchings', unit)

def listing(unit, f):
    p = os.path.join(D(unit), f + '.s')
    return open(p).read() if os.path.exists(p) else None

def words(unit, f):
    t = listing(unit, f)
    m = t and re.search(r'nonmatching \S+, (0x[0-9A-F]+)', t)
    return int(m.group(1), 16) // 4 if m else None

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
    inc = include_protos()
    allcallees, installs, globs = {}, {}, {}
    print("=== PER-FUNCTION FACTS (never describe a union as shared) ===")
    tot = 0
    for f in targets:
        t = listing(unit, f)
        if t is None:
            print("  %-15s *** NO LISTING IN %s ***" % (f, unit)); continue
        w = words(unit, f); tot += w
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
        print("      globals : %s" % (', '.join(gl) or 'none'))
    print("  TOTAL %d words" % tot)

    print("\n=== CALLEE CLASSIFICATION (per callee, across the group) ===")
    every = set().union(*allcallees.values()) if allcallees else set()
    for c in sorted(every):
        who = [f for f in targets if c in allcallees.get(f, ())]
        if c in landed:   cls = 'LANDED-HERE'
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
            st = 'LANDED at ' + (landed[h][:60] if h in landed else '') if h in landed else ('stub' if listing(unit,h) else 'NOT IN UNIT')
            print("  %s installs %-15s -> %s" % (f, h, st))
    if not anyi: print("  none")

    # Rank by RARITY, not by count. func_80095388 is called by ~30 landed
    # functions, so sharing it means nothing; a callee called by two means a
    # great deal. Counting shared callees is what ranked four groups BACKWARDS.
    freq = {}
    for dc in body.values():
        for c in dc: freq[c] = freq.get(c, 0) + 1
    print("\n=== LANDED RELATIVES, route (a): shared callees+globals, WEIGHTED BY RARITY ===")
    print("    (score = sum 1/freq over shared callees, +2/freq per shared global;")
    print("     a callee the whole unit calls scores ~0, a rare one dominates)")
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
            sg = set(g for g in globs.get(f, ()) if g in body_src.get(d, ''))
            s = sum(1.0/freq.get(c, 1) for c in sc) + sum(2.0/max(1, sum(1 for b in body_src.values() if g in b)) for g in sg)
            if s > 0 and (best is None or s > best[0]):
                best = (s, f, sorted(sc), sorted(sg))
        if best: hits.append((best[0], d) + best[1:])
    ranked = sorted(hits, reverse=True)
    if ranked and ranked[0][0] < 0.15:
        print("  *** ROUTE (a) FOUND NOTHING. Best score %.3f is noise (a callee the"
              % ranked[0][0])
        print("      whole unit calls). Do NOT name these in a brief as relatives --")
        print("      that is the mistake that ranked four groups backwards. Use route")
        print("      (b)/(c) above, or say plainly that there is no template.")
    for s, d, f, sc, sg in ranked[:8]:
        ln = lines[d]
        print("  %6.3f  %-15s csrc/%s.c:%-6d vs %s" % (s, d, unit, ln, f))
        print("          callees %s   globals %s" % (','.join(sc) or '-', ','.join(sg) or '-'))
    if not hits: print("  none -- hand-decode, and say so in the brief")

    print("\n=== LANDED CALLERS (route (c): settles arity AND constant arguments) ===")
    anyc = False
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

    for f in targets:
        for d, dc in body.items():
            if f in dc and d not in targets:
                a = arglist(btext.get(d, ''), f)
                if a is None: continue
                print("  %-15s called by LANDED %s (csrc/%s.c:%d) as %s(%s)"
                      % (f, d, unit, lines[d], f, a[:60]))
                anyc = True
                break
    if not anyc: print("  none -- no call site constrains arity")

if __name__ == '__main__':
    main()
