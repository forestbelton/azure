#!/usr/bin/env python3
"""verify.py zz_file.c UNIT FUNC -- compile, assemble via maspsx, compare .text words with the target listing.

Words carrying a relocation are compared on their upper 16 bits only, because
the linker supplies the rest. The low half is still checked against the addend
the splat listing implies and reported separately as `?`: a mismatch there is
usually real, but not always (a different symbol with a compensating addend
resolves to the same bytes), so it is a warning rather than a diff.

`make check` remains the only authority. In particular verify.py cannot see a
jump table landing at the wrong address.
"""
import re, subprocess, sys, os
SP = os.environ.get('AZURE_SCRATCH', '/tmp/azure-decomp')
os.makedirs(SP, exist_ok=True)
PROJ = os.environ.get('AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
src, unit, func = sys.argv[1:4]
base = os.path.splitext(os.path.basename(src))[0]
s = f'{SP}/{base}.s'; o = f'{SP}/{base}.o'
try:
    os.remove(s)
except OSError:
    pass
subprocess.run([f'{PROJ}/tools/ccpsx', '-O2', '-G8', '-Iinclude', '-S', '-o', s, src], cwd=PROJ, check=True)
if not os.path.exists(s):
    # wine's filesystem is case-insensitive, so if some other file in SP
    # differs from this one only in case, ccpsx wrote that one and exited 0.
    clash = [f for f in os.listdir(SP)
             if f.lower() == os.path.basename(s).lower() and f != os.path.basename(s)]
    print(f'ccpsx exited 0 but wrote no {s}')
    if clash:
        print(f'  it wrote {SP}/{clash[0]} instead: wine is case-insensitive, so those')
        print(f'  two names are one file. Rename {src} so it differs by more than case.')
    else:
        print('  no case clash found in the scratch directory; try again (concurrent')
        print('  ccpsx runs share one wine prefix and can lose an output).')
    sys.exit(2)
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

def listing_addend(text):
    """The addend splat's %hi/%lo operand implies, or None if there is none."""
    m = re.search(r'%(?:hi|lo)\(([^)]*)\)', text)
    if not m:
        return None
    inner = m.group(1)
    a = re.search(r'([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*$', inner)
    if not a:
        return 0
    v = int(a.group(2), 16 if a.group(2).lower().startswith('0x') else 10)
    return v if a.group(1) == '+' else -v

bad = 0; warn = 0
print(f'built {len(words)} words, target {len(tgt)} words')
for i in range(max(len(words), len(tgt))):
    if i < len(words) and i < len(tgt):
        a, w, txt = words[i]; t, ttxt = tgt[i]
        if a in relocs:
            sh = 26 if relocs[a] == '26' else 16
            ok = (w >> sh) == (t >> sh); mark = ' ' if ok else 'X'
            if ok and sh == 16:
                want = listing_addend(ttxt)
                if want is not None:
                    got = w & 0xFFFF
                    if got >= 0x8000 and want < 0:
                        got -= 0x10000
                    if got != want:
                        mark = '?'; warn += 1
        else:
            ok = w == t; mark = ' ' if ok else 'X'
        if not ok: bad += 1
        print(f'{mark} {w:08x} {txt:40s} | {t:08x} {ttxt}')
    else:
        bad += 1
        print('X', words[i][2] if i < len(words) else '<none>', '|', tgt[i][1] if i < len(tgt) else '<none>')
tail = '' if not warn else f' (+{warn} relocated word(s) whose addend differs from the listing, marked ?; run make check)'
print('RESULT:', ('MATCH' if bad == 0 else f'{bad} DIFF') + tail)
