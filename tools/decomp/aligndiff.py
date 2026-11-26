import sys, os, re, subprocess, difflib
sys.path.insert(0, '/home/case/projects/azure/tools/decomp')
import asmlib, ovlverify
# THIS TOOL TAKES EXACTLY ONE FUNCTION, AND USED TO IGNORE THE REST IN SILENCE.
# `ovlverify.py' accepts several names, so an agent that has learned its habits
# writes `aligndiff.py copy.c funcA funcB' and gets a measurement of funcA with
# funcB discarded.  agent-h113 lost a sweep of EIGHT spellings that way in
# batch 27: every one read 0, because every one measured the other body in the
# file -- and "all of these are free" is the direction that looks like success.
# That is a distinct way a measurement lies from the four already recorded: the
# tool measured a DIFFERENT FUNCTION than the one you changed.  Refuse instead.
_names = [a for a in sys.argv[3:] if not a.startswith("-")]
if _names:
    sys.exit("aligndiff takes ONE function name; %d extra given (%s).\n"
             "Run it once per function -- unlike ovlverify, it cannot take a list."
             % (len(_names), ", ".join(_names)))
src, func = sys.argv[1], sys.argv[2]

# SAME GUARD AS ovlverify.py's, AND FOR THE SAME REASON. If FUNC is still an
# INCLUDE_ASM in SRC, the stub's own assembly defines the symbol in the object
# and this tool compares the target against ITSELF -- "0 aligned differences",
# which is the exact shape of a landing with no warning attached.
#
# ovlverify.py grew this guard on 2026-11-04 after it reported a false landing
# for nine consecutive spellings. aligndiff did not, and on 2026-11-16 an agent
# running a parallel batch reused a measurement directory whose copy had been
# re-spliced for a DIFFERENT function; this printed
# "109 built / 109 target, 0 aligned differences" for a function that was back
# to being a stub. Re-splice before every measurement -- and now the tool says
# so rather than leaving it to discipline.
if '--allow-stub' not in sys.argv:
    # ASK cfuncs, WHICH STRIPS COMMENTS FIRST. This used to re.search the raw
    # text for the macro, so a park header or a body comment QUOTING
    # `INCLUDE_ASM("...", func_x)' would match and the tool would refuse to
    # measure a body that is really spliced in. The same raw-text mistake, as
    # `grep -c INCLUDE_ASM', once reported one stub for a unit with none.
    try:
        import cfuncs as _cf
        _still_stub = func in _cf.stubs(src)
    except OSError:
        _still_stub = False
    if _still_stub:
        print("%s: STILL AN INCLUDE_ASM STUB in %s -- refusing to measure the "
              "stub against itself (it would report 0 aligned differences). "
              "Re-splice the body first, or pass --allow-stub." % (func, src))
        sys.exit(1)

words, relocs = asmlib.build(src, gasG=0, cflags=('-G0',))
o = os.path.join(asmlib.procdir(), os.path.splitext(os.path.basename(src))[0] + ".o")
syms = {}
for ln in subprocess.run(["mips64-linux-gnu-nm","-n",o],capture_output=True,text=True).stdout.split("\n"):
    m = re.match(r"([0-9a-f]+) [Tt] (\S+)", ln)
    if m: syms[m.group(2)] = int(m.group(1),16)
start = syms[func]
ends = sorted(v for v in syms.values() if v > start)
end = ends[0] if ends else 1 << 30
got = [(a,w,t) for (a,w,t) in words if start <= a < end]
tgt = ovlverify.target(func)
RELOC = ovlverify.RELOC
def key(w, rel):
    op = w >> 26
    if op in (2, 3):
        return (op,)
    if op == 0:
        return (0, w & 0x7FF, (w >> 6) & 0x1F)
    if op in (0x10, 0x11, 0x12, 0x13):
        return (op, w & 0x3FFFFFF)
    imm = w & 0xFFFF
    if rel or op in (1, 4, 5, 6, 7):
        imm = None
    return (op, imm)

grel = set(relocs)
G = [key(w, (a in grel) or bool(RELOC.search(t))) for (a,w,t) in got]
def trel(t):
    if re.search(r'%(?:hi|lo)\(D_1F8[0-9A-F]{5}\)', t):
        return False          # splat pseudo-symbol for a computed scratchpad address
    return bool(RELOC.search(t))
T = [key(w, trel(t)) for (w,t) in tgt]
# The target's own first address, read out of its listing.  This used to be a
# hardcoded 0x800AC4C4, so every run printed addresses for the wrong function.
TBASE = 0
_lst = ovlverify.listing(func)
if _lst:
    _seen = False
    for _ln in open(_lst):
        if _ln.startswith("glabel "):
            _seen = _ln.split()[1].strip() == func
            continue
        if _seen:
            _m = re.search(r'/\* [0-9A-F]+ ([0-9A-F]{8})', _ln)
            if _m:
                TBASE = int(_m.group(1), 16)
                break

sm = difflib.SequenceMatcher(None, G, T, autojunk=False)
out=[]
def wordeq(gi,tj):
    gw, gt = got[gi][1], got[gi][2]
    tw, tt = tgt[tj][0], tgt[tj][1]
    rel = (got[gi][0] in grel) or bool(RELOC.search(gt)) or bool(RELOC.search(tt))
    if re.search(r'%(?:hi|lo)\(D_1F8[0-9A-F]{5}\)', tt):
        rel = False
    if rel:
        op = gw>>26
        if op in (2,3): return (tw>>26) in (2,3)
        return (gw & 0xFFFF0000) == (tw & 0xFFFF0000)
    return gw == tw
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        for k in range(i2-i1):
            gi, tj = i1+k, j1+k
            out.append(('=' if wordeq(gi,tj) else 'R', gi, tj))
    elif tag=='replace':
        for k in range(max(i2-i1,j2-j1)):
            gi = i1+k if i1+k<i2 else None
            tj = j1+k if j1+k<j2 else None
            # A raw constant address in the candidate has NO relocation, so
            # key() keeps its immediate, while the listing's %hi(D_...) side
            # drops it -- the keys differ and difflib calls it a 'replace'
            # even though the two WORDS are identical.  That false class cost
            # an agent most of a budget on 2026-11-15 (six of its twenty-five
            # reported differences were matching code), so check the bytes
            # before reporting a replace.
            tag2 = 'X'
            if gi is not None and tj is not None and wordeq(gi, tj):
                tag2 = '='
            out.append((tag2, gi, tj))
    elif tag=='delete':
        for gi in range(i1,i2): out.append(('+', gi, None))
    elif tag=='insert':
        for tj in range(j1,j2): out.append(('-', None, tj))
nd = sum(1 for r in out if r[0]!='=')
print("built %d words, target %d words, %d aligned differences" % (len(got), len(tgt), nd))
show = '-v' in sys.argv
ctx = 0
for i,(tag,gi,tj) in enumerate(out):
    if tag=='=' and not show: continue
    gt = re.sub(r'\s+',' ',got[gi][2].strip()) if gi is not None else ''
    tt = re.sub(r'\s+',' ',tgt[tj][1].strip()) if tj is not None else ''
    ta = ('%08X'%(TBASE + 4*tj)) if (tj is not None and TBASE) else '        '
    print("%s %4s %4s %s  %-40s %s" % (tag, gi if gi is not None else '', tj if tj is not None else '', ta, gt, tt))
