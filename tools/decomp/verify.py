#!/usr/bin/env python3
"""verify.py zz_file.c UNIT FUNC -- compile, assemble via maspsx, compare .text words with the target listing.
Words carrying a relocation are compared on their upper 16 bits only."""
import re, subprocess, sys, os
SP = os.environ.get('AZURE_SCRATCH', '/tmp/azure-decomp')
os.makedirs(SP, exist_ok=True)
PROJ = os.environ.get('AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
src, unit, func = sys.argv[1:4]
base = os.path.splitext(os.path.basename(src))[0]
s = f'{SP}/{base}.s'; o = f'{SP}/{base}.o'
subprocess.run([f'{PROJ}/tools/ccpsx', '-O2', '-G8', '-Iinclude', '-S', '-o', s, src], cwd=PROJ, check=True)
asm = open(s).read()
r = subprocess.run(['python3', os.path.expanduser('~/scratch/maspsx/maspsx.py'), '--aspsx-version=2.56', '--force-stdin',
                    '--run-assembler', '--gnu-as-path=mips64-linux-gnu-as', '--use-comm-section', '-G8',
                    '-EL', '-32', '-Iinclude', '-march=r3000', '-mtune=r3000', '-no-pad-sections', '-O1', '-o', o],
                   input=asm, text=True, cwd=PROJ, capture_output=True)
if r.returncode: print(r.stderr); sys.exit(1)
# get .text bytes and relocations
d = subprocess.run(['mips64-linux-gnu-objdump', '-d', '-z', '-r', '-j', '.text', o], capture_output=True, text=True).stdout
words = []; relocs = {}
for ln in d.split('\n'):
    m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', ln)
    if m: words.append((int(m.group(1), 16), int(m.group(2), 16), m.group(3).strip())); continue
    m = re.match(r'\s*([0-9a-f]+):\s+R_MIPS_(\w+)', ln)
    if m: relocs[int(m.group(1), 16)] = m.group(2)
tgt = []
for ln in open(f'{PROJ}/asm/nonmatchings/{unit}/{func}.s'):
    m = re.match(r'\s*/\* \w+ \w+ ([0-9A-F]{8}) \*/\s+(.*)', ln)
    if m:
        b = bytes.fromhex(m.group(1)); tgt.append((int.from_bytes(b, 'little'), m.group(2).strip()))
bad = 0
print(f'built {len(words)} words, target {len(tgt)} words')
for i in range(max(len(words), len(tgt))):
    if i < len(words) and i < len(tgt):
        a, w, txt = words[i]; t, ttxt = tgt[i]
        if a in relocs:
            sh = 26 if relocs[a] == '26' else 16
            ok = (w >> sh) == (t >> sh); mark = ' ' if ok else 'X'
        else:
            ok = w == t; mark = ' ' if ok else 'X'
        if not ok: bad += 1
        print(f'{mark} {w:08x} {txt:40s} | {t:08x} {ttxt}')
    else:
        bad += 1
        print('X', words[i][2] if i < len(words) else '<none>', '|', tgt[i][1] if i < len(tgt) else '<none>')
print('RESULT:', 'MATCH' if bad == 0 else f'{bad} DIFF')
