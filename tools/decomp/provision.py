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
import os, re, sys, subprocess

PROJ = '/home/case/projects/azure'
def D(unit): return os.path.join(PROJ, 'asm/nonmatchings', unit)

def listing(unit, f):
    p = os.path.join(D(unit), f + '.s')
    return open(p).read() if os.path.exists(p) else None

def words(unit, f):
    t = listing(unit, f)
    m = t and re.search(r'nonmatching \S+, (0x[0-9A-F]+)', t)
    return int(m.group(1), 16) // 4 if m else None

def defs_and_protos(src):
    txt = open(src).read()
    landed, cur, depth, body = {}, None, 0, {}
    for ln in txt.split('\n'):
        m = re.match(r'^[A-Za-z_][\w \*]*?(func_[0-9A-Fa-f]{8})\s*\(', ln)
        if m and ';' not in ln:
            cur = m.group(1); landed[cur] = ln.strip(); body[cur] = set(); depth = 0
        if cur:
            body[cur].update(re.findall(r'\b(func_[0-9A-Fa-f]{8})\s*\(', ln))
            depth += ln.count('{') - ln.count('}')
            if depth <= 0 and '}' in ln: cur = None
    protos = {}
    for m in re.finditer(r'^([A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]{8})\s*\([^;]*\);)', txt, re.M):
        protos.setdefault(m.group(2), m.group(1).strip())
    for k in body: body[k].discard(k)
    return landed, protos, body, txt

def main():
    unit, targets = sys.argv[1], sys.argv[2:]
    src = os.path.join(PROJ, 'csrc', unit + '.c')
    landed, protos, body, txt = defs_and_protos(src)
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
        print("      installs: %s" % (', '.join(ins) or 'none   <- %hi on a FUNCTION: masked, no jal'))
        print("      globals : %s" % (', '.join(gl) or 'none'))
    print("  TOTAL %d words" % tot)

    print("\n=== CALLEE CLASSIFICATION (per callee, across the group) ===")
    every = set().union(*allcallees.values()) if allcallees else set()
    for c in sorted(every):
        who = [f for f in targets if c in allcallees.get(f, ())]
        if c in landed:   cls = 'LANDED-HERE'
        elif listing(unit, c) is not None: cls = 'STUB-HERE'
        else: cls = '*** NOT IN THIS UNIT (resident) ***'
        pr = protos.get(c) or ('(definition) ' + landed[c] if c in landed else None)
        print("  %-15s %-34s called by: %s" % (c, cls, ','.join(who)))
        print("      proto: %s" % (pr if pr else '*** NO DECLARATION ANYWHERE IN THE UNIT ***'))

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
        ln = txt[:txt.index(landed[d])].count('\n') + 1
        print("  %6.3f  %-15s csrc/%s.c:%-6d vs %s" % (s, d, unit, ln, f))
        print("          callees %s   globals %s" % (','.join(sc) or '-', ','.join(sg) or '-'))
    if not hits: print("  none -- hand-decode, and say so in the brief")

    print("\n=== LANDED CALLERS (route (c): settles arity AND constant arguments) ===")
    anyc = False
    for f in targets:
        for d, dc in body.items():
            if f in dc and d not in targets:
                ln = txt[:txt.index(landed[d])].count('\n') + 1
                for m in re.finditer(re.escape(f) + r'\s*\(([^;)]*)\)', txt):
                    print("  %-15s called by LANDED %s (csrc/%s.c:%d) as %s(%s)" % (f, d, unit, ln, f, m.group(1)[:50])); anyc=True
                    break
                break
    if not anyc: print("  none -- no call site constrains arity")

if __name__ == '__main__':
    main()
