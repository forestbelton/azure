#!/usr/bin/env python3
"""verify.py zz_file.c UNIT FUNC -- compile, assemble via maspsx, compare .text words with the target listing.

Words carrying a relocation are compared on their upper 16 bits only, because
the linker supplies the rest. The low half is still checked against the addend
the splat listing implies and reported separately as `?`: a mismatch there is
usually real, but not always (a different symbol with a compensating addend
resolves to the same bytes), so it is a warning rather than a diff.

`make check` remains the only authority. In particular verify.py cannot see a
jump table landing at the wrong address.

Output is abbreviated by default: differing words, two words of context either
side, and runs of matching words collapsed to one line. Pass --full for the
whole side-by-side. The abbreviation matters -- the full form of a 437-word
function is about 10k tokens, and an agent iterating on it pays that again on
every subsequent turn.
"""
import re, subprocess, sys, os
SP = os.environ.get('AZURE_SCRATCH', '/tmp/azure-decomp')
# maspsx is vendored at vendor/maspsx; MASPSX_DIR overrides it, which is the
# same variable the Makefile honours, so the two cannot drift apart.
MASPSX = os.path.join(os.environ.get('MASPSX_DIR', 'vendor/maspsx'), 'maspsx.py')
os.makedirs(SP, exist_ok=True)
PROJ = os.environ.get('AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
argv = [a for a in sys.argv[1:] if a != '--full']
full = len(argv) != len(sys.argv) - 1
src, unit, func = argv[0:3]
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
# --g8 models a unit carved out with MASPSXFLAGS_G8 (gas left at -G8 rather
# than maspsx's forced -G0), which is how gprel_12DD4 and gprel_14910 are
# built: gas then assumes an *undefined* symbol is small data and emits
# GPREL16. Use it to check a function before moving it into a carved unit.
g8 = '--g8' in sys.argv
maspsx = ['python3', MASPSX, '--aspsx-version=2.56', '--force-stdin',
          '--run-assembler', '--gnu-as-path=mips64-linux-gnu-as', '--use-comm-section', '-G8']
if g8:
    maspsx.append('--dont-force-G0')
r = subprocess.run(maspsx +
                   ['-EL', '-32', '-Iinclude', '-march=r3000', '-mtune=r3000', '-no-pad-sections', '-O1', '-o', o],
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
rows = []
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
        rows.append((mark, f'{mark} {w:08x} {txt:40s} | {t:08x} {ttxt}'))
    else:
        bad += 1
        rows.append(('X', 'X ' + (words[i][2] if i < len(words) else '<none>')
                     + ' | ' + (tgt[i][1] if i < len(tgt) else '<none>')))

print(f'built {len(words)} words, target {len(tgt)} words')
if full:
    for _, line in rows:
        print(line)
else:
    CTX = 2
    keep = set()
    for i, (mark, _) in enumerate(rows):
        if mark != ' ':
            for j in range(max(0, i - CTX), min(len(rows), i + CTX + 1)):
                keep.add(j)
    run = 0
    for i, (_, line) in enumerate(rows):
        if i in keep:
            if run:
                print(f'  ... {run} matching word(s) ...')
                run = 0
            print(line)
        else:
            run += 1
    if run:
        print(f'  ... {run} matching word(s) ...')
    if len(keep) < len(rows):
        print('  (--full for the whole side-by-side)')

tail = '' if not warn else f' (+{warn} relocated word(s) whose addend differs from the listing, marked ?; run make check)'
print('RESULT:', ('MATCH' if bad == 0 else f'{bad} DIFF') + tail)
