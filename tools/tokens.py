#!/usr/bin/env python3
"""tokens.py -- what a session cost, read out of Claude Code's own transcripts.

Transcripts live in ~/.claude/projects/<slugified-cwd>/<session>.jsonl, with
subagent transcripts under <session>/subagents/agent-*.jsonl. Every assistant
message carries a `usage` block; this sums them.

    python3 tools/tokens.py                      # every session, newest first
    python3 tools/tokens.py --live               # modified in the last 30 min
    python3 tools/tokens.py 5441f4af             # one session, with subagents
    python3 tools/tokens.py 5441f4af --agents    # break out each subagent
    python3 tools/tokens.py --since 2026-10-28

COLUMNS. `in` is uncached input, `w5m`/`w1h` are cache writes at each TTL,
`read` is cache reads, `out` is output (thinking included; `think` breaks it
out). A turn is one assistant message.

TOTALS. `raw` adds every token the API saw. `in-eq` weights them the way
caching bills input -- 1x uncached, 1.25x a 5-minute cache write, 2x a
1-hour one, 0.1x a cache read -- and leaves output out, because output is
priced on a different scale. Neither is dollars; multiply by the rate for the
model if you want money.

Records are deduplicated by uuid: a resumed or retried session writes the same
assistant message more than once, which otherwise double-counts it.
"""
import io
import json
import os
import re
import sys
import time

HOME = os.path.expanduser("~")
HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(HERE))


def project_dir(cwd=None):
    """Claude Code slugifies the cwd: /home/case/projects/azure -> -home-case-projects-azure."""
    cwd = cwd or PROJ
    slug = re.sub(r"[^A-Za-z0-9]", "-", cwd)
    return os.path.join(HOME, ".claude", "projects", slug)


class Tally(object):
    def __init__(self, name):
        self.name = name
        self.turns = 0
        self.inp = self.w5m = self.w1h = self.read = self.out = self.think = 0
        self.t0 = self.t1 = None
        self.models = set()

    def add(self, u, ts, model):
        self.turns += 1
        self.inp += u.get("input_tokens", 0) or 0
        self.read += u.get("cache_read_input_tokens", 0) or 0
        self.out += u.get("output_tokens", 0) or 0
        det = u.get("output_tokens_details") or {}
        self.think += det.get("thinking_tokens", 0) or 0
        cc = u.get("cache_creation") or {}
        w5 = cc.get("ephemeral_5m_input_tokens")
        w1 = cc.get("ephemeral_1h_input_tokens")
        if w5 is None and w1 is None:
            # older transcripts only have the total
            self.w5m += u.get("cache_creation_input_tokens", 0) or 0
        else:
            self.w5m += w5 or 0
            self.w1h += w1 or 0
        if ts:
            self.t0 = ts if self.t0 is None or ts < self.t0 else self.t0
            self.t1 = ts if self.t1 is None or ts > self.t1 else self.t1
        if model:
            self.models.add(model)

    @property
    def raw(self):
        return self.inp + self.w5m + self.w1h + self.read + self.out

    @property
    def in_eq(self):
        return int(self.inp + 1.25 * self.w5m + 2.0 * self.w1h + 0.1 * self.read)

    def merge(self, other):
        for f in ("turns", "inp", "w5m", "w1h", "read", "out", "think"):
            setattr(self, f, getattr(self, f) + getattr(other, f))
        self.models |= other.models
        if other.t0 and (self.t0 is None or other.t0 < self.t0):
            self.t0 = other.t0
        if other.t1 and (self.t1 is None or other.t1 > self.t1):
            self.t1 = other.t1


def scan(path, tally, seen):
    for line in io.open(path, encoding="utf-8", errors="replace"):
        line = line.strip()
        if not line:
            continue
        try:
            d = json.loads(line)
        except Exception:
            continue
        m = d.get("message")
        if not isinstance(m, dict):
            continue
        u = m.get("usage")
        if not u:
            continue
        key = d.get("uuid") or (d.get("requestId"), d.get("timestamp"))
        if key in seen:
            continue
        seen.add(key)
        tally.add(u, d.get("timestamp"), m.get("model"))
    return tally


def human(n):
    for unit, div in (("B", 1e9), ("M", 1e6), ("K", 1e3)):
        if abs(n) >= div:
            return "%.1f%s" % (n / div, unit)
    return str(int(n))


def sessions(d, pick=None, since=None, live_min=None):
    out = []
    if not os.path.isdir(d):
        return out
    now = time.time()
    for name in os.listdir(d):
        if not name.endswith(".jsonl"):
            continue
        sid = name[:-6]
        if pick and not sid.startswith(pick):
            continue
        p = os.path.join(d, name)
        mt = os.path.getmtime(p)
        if live_min and now - mt > live_min * 60:
            continue
        if since and time.strftime("%Y-%m-%d", time.localtime(mt)) < since:
            continue
        subs = []
        sdir = os.path.join(d, sid, "subagents")
        if os.path.isdir(sdir):
            subs = [os.path.join(sdir, f) for f in sorted(os.listdir(sdir))
                    if f.endswith(".jsonl")]
        out.append((sid, p, mt, subs))
    return sorted(out, key=lambda r: r[2], reverse=True)


HDR = "%-10s %6s %8s %8s %8s %9s %8s   %8s %8s"
ROW = "%-10s %6d %8s %8s %8s %9s %8s   %8s %8s"


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    flags = [a for a in sys.argv[1:] if a.startswith("--")]
    pick = args[0] if args else None
    since = None
    for f in flags:
        if f.startswith("--since="):
            since = f.split("=", 1)[1]
    if "--since" in flags and len(args) > 1:
        since = args[1]
    live = 30 if "--live" in flags else None
    show_agents = "--agents" in flags

    d = project_dir()
    rows = sessions(d, pick, since, live)
    if not rows:
        print("no transcripts under %s" % d)
        return 1

    print(HDR % ("session", "turns", "in", "w5m", "w1h", "read", "out", "raw", "in-eq"))
    grand = Tally("TOTAL")
    for sid, path, _mt, subs in rows:
        seen = set()
        t = scan(path, Tally(sid[:8]), seen)
        agents = []
        for s in subs:
            a = scan(s, Tally(os.path.basename(s)[:-6]), set())
            agents.append(a)
            t.merge(a)
        print(ROW % (t.name, t.turns, human(t.inp), human(t.w5m), human(t.w1h),
                     human(t.read), human(t.out), human(t.raw), human(t.in_eq)))
        if show_agents:
            for a in agents:
                print(ROW % ("  " + a.name[:8], a.turns, human(a.inp),
                             human(a.w5m), human(a.w1h), human(a.read),
                             human(a.out), human(a.raw), human(a.in_eq)))
        grand.merge(t)

    if len(rows) > 1:
        print(ROW % (grand.name, grand.turns, human(grand.inp), human(grand.w5m),
                     human(grand.w1h), human(grand.read), human(grand.out),
                     human(grand.raw), human(grand.in_eq)))

    if len(rows) == 1:
        t = grand
        span = ""
        if t.t0 and t.t1:
            span = "  %s -> %s" % (t.t0[11:19], t.t1[11:19])
        print()
        print("%d turns%s, model(s): %s" % (
            t.turns, span, ", ".join(sorted(t.models)) or "?"))
        print("thinking %s of %s output; %s per turn raw" % (
            human(t.think), human(t.out), human(t.raw // max(1, t.turns))))
    return 0


if __name__ == "__main__":
    sys.exit(main())
