#!/usr/bin/env python3
"""Compare a built file against the reference and localise the first difference.

A whole-file checksum only says yes or no. What you need when a link comes out
wrong is *where*, and which subsegment of the splat config owns that offset --
that turns a failed build into a specific line of YAML to fix.
"""
import argparse
import hashlib
import pathlib
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent.parent / "src"))

import yaml


def subsegments(path: pathlib.Path):
    """[(start, end, kind, name)] over every code-segment subsegment."""
    doc = yaml.safe_load(path.read_text())
    segments = doc.get("segments") or []
    out = []
    for i, seg in enumerate(segments):
        if not isinstance(seg, dict) or not seg.get("subsegments"):
            continue
        tail = next((s[0] for s in segments[i + 1:]
                     if isinstance(s, list) and s), None)
        entries = [e for e in seg["subsegments"] if isinstance(e, list) and e]
        for j, e in enumerate(entries):
            start = e[0]
            kind = e[1] if len(e) > 1 else "?"
            name = e[2] if len(e) > 2 else ""
            end = entries[j + 1][0] if j + 1 < len(entries) else tail
            out.append((start, end, kind, name))
    return out


def owner(subs, off):
    for start, end, kind, name in subs:
        if end is not None and start <= off < end:
            return start, end, kind, name
    return None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("built", type=pathlib.Path)
    ap.add_argument("reference", type=pathlib.Path)
    ap.add_argument("--yaml", type=pathlib.Path,
                    help="splat config, to name the subsegment that differs")
    ap.add_argument("--context", type=int, default=16,
                    help="bytes of hex context to show (default 16)")
    args = ap.parse_args()

    if not args.built.exists():
        print(f"FAIL  {args.built} does not exist")
        return 1
    a = args.built.read_bytes()
    b = args.reference.read_bytes()

    ha = hashlib.sha1(a).hexdigest()
    hb = hashlib.sha1(b).hexdigest()
    print(f"built     {args.built}  {len(a):#x} bytes  {ha}")
    print(f"reference {args.reference}  {len(b):#x} bytes  {hb}")

    if a == b:
        print("\nOK  byte-identical")
        return 0

    if len(a) != len(b):
        print(f"\nsize differs by {len(a) - len(b):+#x}")

    n = min(len(a), len(b))
    first = next((i for i in range(n) if a[i] != b[i]), None)
    if first is None:
        print(f"\nFAIL  common prefix of {n:#x} bytes matches; lengths differ")
        return 1

    diff = sum(1 for i in range(n) if a[i] != b[i])
    print(f"\nFAIL  {diff:#x} of {n:#x} bytes differ "
          f"({100 * diff / n:.2f}%); first at {first:#08x}")

    if args.yaml and args.yaml.exists():
        subs = subsegments(args.yaml)
        hit = owner(subs, first)
        if hit:
            start, end, kind, name = hit
            print(f"      inside subsegment [{start:#07x}, {kind}"
                  f"{', ' + name if name else ''}]  "
                  f"covering {start:#x}-{end:#x}  (+{first - start:#x} into it)")
        else:
            print(f"      {first:#08x} is not covered by any subsegment")

    lo = max(0, (first - args.context) & ~0xF)
    hi = min(n, lo + args.context * 3)
    print()
    for off in range(lo, hi, 16):
        ba, bb = a[off:off + 16], b[off:off + 16]
        mark = "  <<<" if off <= first < off + 16 else ""
        print(f"  {off:08x}  built {ba.hex(' ')}{mark}")
        if ba != bb:
            print(f"  {'':8}  ref   {bb.hex(' ')}")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
