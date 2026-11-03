import sys, os, re, subprocess, difflib
sys.path.insert(0, '/home/case/projects/azure/tools/decomp')
import asmlib, ovlverify
src, func = sys.argv[1], sys.argv[2]
words, relocs = asmlib.build(src, gasG=0, cflags=('-G0',))
o = os.path.join(asmlib.SP, os.path.splitext(os.path.basename(src))[0] + ".o")
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
            out.append(('X', gi, tj))
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
    ta = ('%08X'%(0x800AC4C4 + 4*tj)) if tj is not None else '        '
    print("%s %4s %4s %s  %-40s %s" % (tag, gi if gi is not None else '', tj if tj is not None else '', ta, gt, tt))
