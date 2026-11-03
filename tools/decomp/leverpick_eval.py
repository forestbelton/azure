#!/usr/bin/env python3
"""leverpick_eval.py -- measure leverpick.py, and derive its `lp:` tags.

Run with no arguments to get the recall table. There is no stored ground
truth: it is mined fresh from `docs/matching-levers.md` every run, because
each lever's section body NAMES the functions it landed, and those functions
have listings with fingerprints. That is both the labelled set and the only
honest source of tags -- a lever attested on three functions that all carry a
jump table wants the tag `jumptable`, and nobody has to decide that by hand.

    python3 tools/decomp/leverpick_eval.py                 # the recall table
    python3 tools/decomp/leverpick_eval.py --propose-tags   # the tag file
    python3 tools/decomp/leverpick_eval.py --check-index    # prove --check survives
    python3 tools/decomp/leverpick_eval.py --signal-classes # slow, see below

THE MEASUREMENT THAT MATTERS IS LEAVE-ONE-OUT. Deriving a lever's tags from
its attested functions and then scoring those same functions is circular and
reports something flattering and false; this prints that number too, labelled
CIRCULAR, purely so the size of the illusion is on the record. Under LOO a
lever's tags are derived from its attested functions MINUS the one being
scored, so a lever attested on exactly one function contributes nothing and
falls back to keywords -- and most levers are attested on exactly one
function, which is why the table breaks recall down by attestation count.

This file writes nothing except `tools/decomp/levertags.txt`, and only with
`--propose-tags`. It never touches `docs/`.
"""
import io
import math
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import leverpick as LP                                      # noqa: E402
import leverindex                                           # noqa: E402

PROJ = LP.PROJ
ASM = [os.path.join(PROJ, "asm/nonmatchings", u)
       for u in ("ovl_town", "ovl_main_80020000")]
TAGFILE = os.path.join(HERE, "levertags.txt")

# A tag is proposed when the attested functions agree on it MORE OFTEN THAN
# CHANCE WOULD GIVE, at the signal's measured prevalence. MIN_SHARE stops a
# lever attested on six functions from claiming a tag two of them happen to
# carry; MAX_P is the binomial tail. With one attested function the test still
# passes for a rare signal (a jump table is p=0.009) and still fails for a
# common one (halfword traffic is p=0.52), which is the behaviour we want and
# is not something a hand-assigned tag would have got right.
MIN_SHARE, MAX_P, MAX_TAGS = 0.67, 0.05, 2

# Only tag a lever attested by at least this many functions. Measured
# leave-one-out, gating at 2 beat gating at 1 (one attested function means
# the derivation is a single observation) and beat gating at 3 (which leaves
# so few tagged levers that the untagged ones are starved for nothing).
MIN_ATTEST = 2

# Levers deliberately left untagged: they are facts about a FAMILY or about
# the search process, not about one function's machine shape, so no listing
# fingerprint can select them and a derived tag would be a coincidence of
# whatever the attested members happened to look like. These belong in
# provision.py's output instead -- see the note printed by --propose-tags.
NO_TAG = [
    "statement order is not inherited",
    "no address-taken symbol at all",
    "differing count that matches",
    "insensitive to its declared return width",
]


def binom_sf(k, n, p):
    """P(X >= k) for X ~ Bin(n, p). n is at most a handful here."""
    if k <= 0:
        return 1.0
    if k > n:
        return 0.0
    return sum(math.comb(n, i) * p ** i * (1.0 - p) ** (n - i)
               for i in range(k, n + 1))


def listings():
    out = {}
    for d in ASM:
        if not os.path.isdir(d):
            continue
        for f in os.listdir(d):
            if f.startswith("func_") and f.endswith(".s"):
                out[f[:-2]] = os.path.join(d, f)
    return out


def sections(text=None):
    """-> [(title, lineno, body)] for every lever section in the doc."""
    lines = (text or io.open(LP.DOC, encoding="utf-8").read()).split("\n")
    heads = []
    for i, l in enumerate(lines, 1):
        m = re.match(r"^(#{2,3}) (.+?)\s*$", l)
        if m and not m.group(2).startswith("Index"):
            heads.append((i, m.group(2)))
    out = []
    for k, (i, ti) in enumerate(heads):
        end = heads[k + 1][0] if k + 1 < len(heads) else len(lines) + 1
        out.append((ti, i, "\n".join(lines[i - 1:end - 1])))
    return out


def attested(have):
    """-> {lever title: set(function)} from the doc's own section bodies."""
    out, dropped = {}, 0
    for ti, _ln, body in sections():
        fns = set(re.findall(r"\bfunc_[0-9A-Fa-f]{8}\b", body)) & set(have)
        if LP.SKIP_TITLE.search(ti.lower()):
            dropped += len(fns)
            continue
        if fns:
            out.setdefault(ti, set()).update(fns)
    return out, dropped


def fingerprints(have, funcs):
    return {f: LP.signals(io.open(have[f], encoding="utf-8").read())
            for f in funcs}


def derive(funcs, fps):
    """-> {tag: (support, n, pvalue)} for one lever's attested functions."""
    n = len(funcs)
    if not n:
        return {}
    got = {}
    for tag, p in LP.PREVALENCE.items():
        if LP.TAGS.get(tag, (0.0, []))[0] <= 0:
            continue
        k = sum(1 for f in funcs if tag in fps[f])
        if not k or k / float(n) < MIN_SHARE:
            continue
        pv = binom_sf(k, n, p)
        if pv <= MAX_P:
            got[tag] = (k, n, pv)
    best = sorted(got, key=lambda x: got[x][2])[:MAX_TAGS]
    return {x: got[x] for x in best}


def confidence(pv):
    return "high" if pv <= 0.001 else ("medium" if pv <= 0.01 else "low")


def ranked(sig, cand, spec, tagmap):
    """-> ordered, de-duplicated lever titles, from the SHIPPING ordering.

    Calls leverpick.rank_rows so the measurement cannot drift away from what
    a brief actually prints -- including the TAG_QUOTA reservation, which is
    the whole reason the untagged majority stays reachable.
    """
    order, seen = [], set()
    for row in LP.rank_rows(sig, cand, spec, tagmap):
        if row[1] not in seen:
            seen.add(row[1])
            order.append(row[1])
    return order


def table(ranks, label, ks=(5, 8, 15)):
    n = len(ranks) or 1
    cols = []
    for k in ks:
        h = sum(1 for r in ranks if r and r <= k)
        cols.append("@" + str(k) + " " + format(100.0 * h / n, "5.1f") + "%")
    a = sum(1 for r in ranks if r)
    cols.append("@any " + format(100.0 * a / n, "5.1f") + "%")
    hits = [r for r in ranks if r and r <= 8]
    med = sorted(hits)[len(hits) // 2] if hits else 0
    print("  " + label.ljust(34) + "n=" + str(len(ranks)).rjust(4) + "  "
          + "  ".join(cols) + "   median rank of an @8 hit "
          + (str(med) if hits else "-"))


def evaluate():
    have = listings()
    att, dropped = attested(have)
    funcs = sorted(set().union(*att.values())) if att else []
    fps = fingerprints(have, funcs)
    pairs = [(ti, f) for ti, fs in att.items() for f in sorted(fs)]
    # use_profiles=False: this harness supplies every tag itself, so the
    # baseline row is a true keyword-only baseline even once levertags.txt
    # exists, and the LOO row cannot read its own answer out of that file.
    cand = [(ti, ln, tg) for ti, ln, tg in LP.headings(use_profiles=False)
            if not LP.SKIP_TITLE.search(ti.lower())]
    spec = LP.specificity([ti.lower() for ti, _, _ in cand])
    doctags = {ti: tg for ti, _ln, tg in cand if tg}

    print("GROUND TRUTH, mined fresh from docs/matching-levers.md")
    print("  " + str(len(pairs)) + " lever/function pairs over "
          + str(len(funcs)) + " functions and " + str(len(att)) + " levers")
    print("  " + str(dropped) + " pairs dropped as meta/bookkeeping sections")
    print("  tags already IN the doc: " + str(len(doctags)) + " lever(s)")
    hist = {}
    for ti, fs in att.items():
        k = len(fs) if len(fs) < 4 else 4
        hist[k] = hist.get(k, 0) + 1
    print("  levers by attested-function count: "
          + ", ".join(str(k) + ("+" if k == 4 else "") + "->" + str(hist[k])
                      for k in sorted(hist)))
    print()

    # cache a ranking per (function, tag-regime) so each is computed once
    def run(tagmap):
        order = {}
        for f in funcs:
            order[f] = ranked(fps[f], cand, spec, tagmap)
        return [(order[f].index(ti) + 1 if ti in order[f] else None)
                for ti, f in pairs]

    print("RECALL OF THE LEVER THAT ACTUALLY PAID")
    base = run(doctags)
    table(base, "keywords only (baseline)")
    base_n = {}
    for (ti, _f), r in zip(pairs, base):
        base_n.setdefault(min(len(att[ti]), 4), []).append(r)

    full = run({ti: list(derive(fs, fps)) for ti, fs in att.items()
                if len(fs) >= MIN_ATTEST and not skip_tagging(ti)})
    table(full, "shape profiles, CIRCULAR (ignore)")

    # LEAVE-ONE-OUT, done per FUNCTION rather than per pair. Every lever in
    # the document is re-tagged from its own attested functions MINUS this
    # one, not just the lever under test -- tagging only the lever under test
    # and leaving its 358 competitors untagged hands it a TAG_BONUS nothing
    # can outrank, which measured 10 points higher and was wrong.
    loo_ranks, by_n = [], {}
    cache = {}
    for f in funcs:
        tm = dict(doctags)
        for ti, fs in att.items():
            if len(fs) < MIN_ATTEST or skip_tagging(ti):
                continue
            rest = fs - {f}
            key = (ti, tuple(sorted(rest)))
            if key not in cache:
                cache[key] = list(derive(sorted(rest), fps))
            if cache[key]:
                tm[ti] = cache[key]
        order = ranked(fps[f], cand, spec, tm)
        for ti in sorted(att):
            if f not in att[ti]:
                continue
            rank = order.index(ti) + 1 if ti in order else None
            loo_ranks.append(rank)
            by_n.setdefault(min(len(att[ti]), 4), []).append(rank)
    table(loo_ranks, "shape profiles, LEAVE-ONE-OUT")
    print()
    print("BROKEN DOWN BY HOW MANY FUNCTIONS ATTEST THE LEVER")
    print("  (the only fair comparison: tags can only help where there is")
    print("   something left to learn from after removing the test function)")
    for k in sorted(by_n):
        suf = str(k) + ("+" if k == 4 else "")
        table(base_n.get(k, []), "  n=" + suf + " baseline, keywords")
        lab = "  n=" + suf + " LOO, profiles"
        if k == 1:
            lab += " (derives nothing)"
        table(by_n[k], lab)
    print()
    prop = {ti: derive(fs, fps) for ti, fs in att.items()}
    tagged = [ti for ti in prop if prop[ti] and not skip_tagging(ti)
              and len(att[ti]) >= MIN_ATTEST]
    cov = sum(1 for ti, f in pairs if ti in tagged)
    print("WHAT --propose-tags WRITES INTO tools/decomp/levertags.txt")
    print("  " + str(len(tagged)) + " of " + str(len(att))
          + " attested levers get a shape profile ("
          + str(len(cand)) + " levers exist in all)")
    print("  those levers cover " + str(cov) + " of " + str(len(pairs))
          + " labelled pairs; the other " + str(len(cand) - len(tagged))
          + " levers rank on keywords alone, as before")
    print("  TAG_QUOTA=" + str(int(LP.TAG_QUOTA)) + " of " + str(8)
          + " printed slots may go to a profile match, so those "
          + str(len(cand) - len(tagged)) + " stay reachable")
    return base, loo_ranks


def skip_tagging(title):
    low = title.lower()
    return any(x in low for x in NO_TAG)


def propose():
    have = listings()
    att, _ = attested(have)
    funcs = sorted(set().union(*att.values())) if att else []
    fps = fingerprints(have, funcs)
    cand = [(ti, ln, tg) for ti, ln, tg in LP.headings(use_profiles=False)
            if not LP.SKIP_TITLE.search(ti.lower())]
    spec = LP.specificity([ti.lower() for ti, _, _ in cand])
    named = {ti: LP.match_map(ti, spec) for ti, _l, _t in cand}
    rows, held = [], []
    for ti in sorted(att):
        got = derive(att[ti], fps)
        if skip_tagging(ti):
            held.append((ti, len(att[ti]), sorted(got), "family/process fact"))
            continue
        if len(att[ti]) < MIN_ATTEST:
            if got:
                held.append((ti, len(att[ti]), sorted(got),
                             "one attested function: a single observation"))
            continue
        if not got:
            continue
        order = sorted(got, key=lambda x: got[x][2])
        tags = " ".join(order)
        support = " ".join(x + "=" + str(got[x][0]) + "/" + str(got[x][1])
                           for x in order)
        conf = confidence(min(got[x][2] for x in order))
        mm = named.get(ti, {})
        corr = " ".join(("heading-names:" if x in mm else "similarity-only:")
                        + x for x in order)
        rows.append((ti, tags, support, conf, corr))
    out = []
    out.append("# levertags.txt -- PROPOSED `lp:` tags for "
               "docs/matching-levers.md.")
    out.append("# Generated by tools/decomp/leverpick_eval.py "
               "--propose-tags. Not applied: I cannot write to docs/.")
    out.append("#")
    out.append("# Apply by inserting the comment on the line AFTER the "
               "heading, then RE-RUN")
    out.append("# `python3 tools/decomp/leverindex.py` (the regenerating "
               "form) because every")
    out.append("# inserted line shifts the heading line numbers the index "
               "records. The")
    out.append("# comment itself is invisible to leverindex.build(), which "
               "matches only")
    out.append("# `^#{2,3} ` -- verified by `leverpick_eval.py "
               "--check-index`.")
    out.append("#")
    out.append("#     ### A `switch` ON A `short` SUBJECT PUTS ...")
    out.append("#     <!-- lp: jumptable s16cast -->")
    out.append("#")
    out.append("# Tags are DERIVED, not judged: a lever's section body names "
               "the functions")
    out.append("# it landed, each of those has a fingerprint, and a tag is "
               "proposed when")
    out.append("# they agree on a signal more often than its measured "
               "prevalence would")
    out.append("# give (share >= " + str(MIN_SHARE) + ", binomial P(X>=k) <= "
               + str(MAX_P) + ", at most " + str(MAX_TAGS) + " tags).")
    out.append("# support is k/n: k of the n attested functions carry it. "
               "confidence is")
    out.append("# the binomial tail: high <= 0.001, medium <= 0.01, "
               "low <= 0.05.")
    out.append("#")
    out.append("# A LOW-CONFIDENCE ROW ON ONE ATTESTED FUNCTION IS ONE "
               "OBSERVATION. It is")
    out.append("# there because the signal is rare enough that one sighting "
               "beats chance,")
    out.append("# not because anything corroborated it -- apply those last, "
               "or not at all,")
    out.append("# and re-run the eval either way.")
    out.append("#")
    out.append("# FORMAT: one record per lever, tab-separated fields on "
               "four lines:")
    out.append("#   lever<TAB>heading text")
    out.append("#   tags<TAB>space-separated")
    out.append("#   support<TAB>tag=k/n ...")
    out.append("#   confidence<TAB>high|medium|low")
    out.append("")
    for ti, tags, support, conf, corr in rows:
        out.append("lever\t" + ti)
        out.append("shape\t" + tags)
        out.append("support\t" + support)
        out.append("confidence\t" + conf)
        out.append("kind\t" + corr)
        out.append("")
    out.append("# DELIBERATELY LEFT UNTAGGED -- these are facts about a "
               "FAMILY or about the")
    out.append("# search, not about one function's machine shape. No "
               "fingerprint can select")
    out.append("# them, and the tags the derivation offers (listed) are "
               "coincidences of")
    out.append("# whatever the attested members happened to look like. They "
               "belong in")
    out.append("# provision.py's output, not here.")
    for ti, n, got, why in sorted(held, key=lambda r: r[3]):
        out.append("# untagged\t" + ti)
        out.append("#   " + why + "; attested on " + str(n)
                   + " function(s); derivation offered: "
                   + (" ".join(got) or "nothing"))
    io.open(TAGFILE, "w", encoding="utf-8").write("\n".join(out) + "\n")
    print("wrote " + TAGFILE)
    print("  " + str(len(rows)) + " levers tagged, " + str(len(held))
          + " deliberately held back")
    for lab in ("high", "medium", "low"):
        print("  " + lab + ": " + str(sum(1 for r in rows if r[3] == lab)))


def check_index():
    """Prove a tag comment is invisible to leverindex, on a COPY of the doc."""
    tmp = tempfile.mkdtemp(prefix="lp_idx_")
    try:
        os.makedirs(os.path.join(tmp, "docs"))
        dst = os.path.join(tmp, "docs/matching-levers.md")
        shutil.copy(LP.DOC, dst)
        text = io.open(dst, encoding="utf-8").read()
        before = leverindex.build(text.split("\n"))
        lines = text.split("\n")
        out, added = [], 0
        for l in lines:
            out.append(l)
            if re.match(r"^#{2,3} ", l) and not l.startswith("## Index"):
                out.append("<!-- lp: jumptable s16cast -->")
                added += 1
        io.open(dst, "w", encoding="utf-8").write("\n".join(out))
        after = leverindex.build("\n".join(out).split("\n"))
        env = dict(os.environ, AZURE_ROOT=tmp)
        chk = subprocess.run(
            [sys.executable, os.path.join(HERE, "leverindex.py"), "--check"],
            capture_output=True, text=True, env=env)
        gen = subprocess.run(
            [sys.executable, os.path.join(HERE, "leverindex.py")],
            capture_output=True, text=True, env=env)
        chk2 = subprocess.run(
            [sys.executable, os.path.join(HERE, "leverindex.py"), "--check"],
            capture_output=True, text=True, env=env)
        strip = lambda rows: [re.sub(r"\s+`:\d+`$", "", r) for r in rows]
        print("inserted " + str(added) + " tag comments into a COPY of the doc")
        print("  levers seen before: " + str(len(before))
              + "   after: " + str(len(after)))
        print("  heading TEXT identical: "
              + str(strip(before) == strip(after)))
        print("  --check on the tagged copy (expected STALE, line numbers "
              "moved): rc=" + str(chk.returncode) + " " + chk.stdout.strip())
        print("  regenerate:                                              "
              + gen.stdout.strip())
        print("  --check again (expected clean):                      rc="
              + str(chk2.returncode) + " " + chk2.stdout.strip())
        ok = (strip(before) == strip(after) and len(before) == len(after)
              and chk2.returncode == 0)
        os.environ.pop("AZURE_ROOT", None)
        print("VERDICT: " + ("tag comments are invisible to leverindex; "
                            "--check stays green after regenerating"
                            if ok else "SOMETHING IS WRONG -- do not tag"))
        return 0 if ok else 1
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def signal_classes():
    """Does a function carrying signal X get a lever naming X in its top 8?"""
    have = listings()
    town = [f for f in sorted(have) if "ovl_town" in have[f]]
    buckets = {}
    for f in town:
        for tag in LP.signals(io.open(have[f], encoding="utf-8").read()):
            buckets.setdefault(tag, []).append(f)
    for tag in sorted(buckets, key=lambda t: LP.PREVALENCE.get(t, 1.0)):
        if LP.TAGS.get(tag, (0.0, []))[0] <= 0:
            continue
        fs, ok = buckets[tag][:60], 0
        pats = [p for p, _w in LP.TAGS[tag][1]]
        for f in fs:
            top = [r[1].lower()
                   for r in LP.pick(io.open(have[f], encoding="utf-8").read())]
            if any(any(re.search(p, ti) for p in pats) for ti in top):
                ok += 1
        print("  " + tag.ljust(12) + str(ok).rjust(3) + "/"
              + str(len(fs)).ljust(4) + format(100.0 * ok / len(fs), "5.0f")
              + "%")


def main():
    av = sys.argv[1:]
    if "--check-index" in av:
        return check_index()
    if "--propose-tags" in av:
        propose()
        return 0
    if "--signal-classes" in av:
        signal_classes()
        return 0
    evaluate()
    return 0


if __name__ == "__main__":
    sys.exit(main())
