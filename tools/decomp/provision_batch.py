import os, re, subprocess, sys, glob, collections
sys.path.insert(0,"/home/case/projects/azure/tools/decomp")
import dngfam, dngpick
PROJ="/home/case/projects/azure"
H=re.compile(r"nonmatching (func_[0-9A-Za-z_]+), (0x[0-9A-Fa-f]+)")
sizes={}
for p in glob.glob(PROJ+"/asm/nonmatchings/*/*.s"):
    m=H.search(open(p,encoding="utf-8",errors="replace").read())
    if m: sizes[m.group(1)]=int(m.group(2),16)//4
st=dngfam.stubs() - dngpick.parked()
per=collections.defaultdict(list)
for f in st: per[re.match(r"func_([a-z0-9]+)_",f).group(1)].append((sizes.get(f,0),f))

def fragments(tag):
    """A symbol with no `jr $ra' is a fragment head; its successor is a tail;
    two or more in a small listing is several functions under one symbol."""
    bad=set(); rows=sorted(per[tag])
    for w,f in rows:
        p=os.path.join(PROJ,"asm/nonmatchings/ovl_dng_%s/%s.s"%(tag,f))
        if not os.path.exists(p): bad.add(f); continue
        t=open(p,encoding="utf-8",errors="replace").read()
        body=t[t.find("glabel"):] if "glabel" in t else t
        n=len(re.findall(r"\bjr\s+\$ra\b", body))
        if n==0 or (n>=2 and w<=48): bad.add(f)
    # and every symbol that FOLLOWS a no-jr one, by address
    addrs=sorted((int(f.split("_")[-1],16), f) for _,f in rows)
    for i,(a,f) in enumerate(addrs):
        if f in bad and i+1 < len(addrs): bad.add(addrs[i+1][1])
    return bad

# TAKE THE BATCH FROM THE COMMAND LINE.
#
# This dict used to be the ONLY way to say what to provision, and the script
# accepted no arguments at all -- so `provision_batch.py b31f s10 ... --batch
# 42' re-provisioned the PREVIOUS batch's five units and printed five rows of
# "0 fn 0w", which reads like a finished batch rather than ignored input.
# Found 2026-11-25 by running exactly that.
#
# Usage:  provision_batch.py [--first-agent hNNN] TAG[:COUNT] ...
#   provision_batch.py b31f s10 b25 s58 s19 --first-agent h186
#   provision_batch.py b31f:17 s10:9          (explicit per-unit counts)
# With no TAG arguments it falls back to LAST_BATCH below, so an accidental
# bare run still does something inspectable rather than nothing.
LAST_BATCH={"h181":("lo16",16),"h182":("s29",13),"h183":("s30",11),
            "h184":("s43",10),"h185":("b02",10)}

def _next_free_agent():
    """The lowest hNNN with no directory yet.

    THE DEFAULT USED TO BE THE LITERAL "h186", and that was a data-loss bug.
    Every agent that ran `provision_batch --units X' without --first-agent
    landed in scratch/azure-dng/agent-h186/ -- so they shared one directory,
    and the second one through OVERWROTE the first's FUNCS.txt, JTCHECK.txt,
    MYTABLES.txt, TINY.txt and TYPES.txt. The unit .c had a refuse-to-overwrite
    guard; the metadata files had none, and one agent lost all five.
    Three separate agents reported "the directory it printed already held
    another agent's work" before one of them read the source and found why.

    Allocating the next unused id removes the collision rather than reporting
    it, which is the right shape: an agent should never have to pass a flag to
    avoid clobbering a stranger.
    """
    base = os.path.join(PROJ, "scratch/azure-dng")
    used = set()
    if os.path.isdir(base):
        for n in os.listdir(base):
            m = re.match(r"^agent-h(\d+)$", n)
            if m:
                used.add(int(m.group(1)))
    n = 186
    while n in used:
        n += 1
    return "h%d" % n


def _picks_from_argv(argv):
    """-> {agent: (tag, count)}; count 0 means "every stub in the unit"."""
    first = _next_free_agent()
    tags = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--first-agent":
            first = argv[i + 1]; i += 2; continue
        if a == "--batch":          # accepted and ignored: agents are named
            i += 2; continue        # by --first-agent, not by batch number
        if a.startswith("-"):
            i += 1; continue
        tag, _, cnt = a.partition(":")
        tags.append((tag, int(cnt) if cnt else 0))
        i += 1
    if not tags:
        return dict(LAST_BATCH)
    base = int(re.match(r"h(\d+)", first).group(1))
    return {"h%d" % (base + k): (tag, cnt)
            for k, (tag, cnt) in enumerate(tags)}

PICKS=_picks_from_argv(sys.argv[1:])
print("provisioning %d unit(s): %s" % (
      len(PICKS), ", ".join("%s=%s" % (a, t) for a, (t, _) in sorted(PICKS.items()))))
for ag,(tag,n) in PICKS.items():
    d="/home/case/projects/azure/scratch/azure-dng/agent-%s"%ag
    os.makedirs(d, exist_ok=True)
    bad=fragments(tag)
    rows=[r for r in sorted(per[tag]) if r[1] not in bad]
    rows=rows[:n] if n else rows   # n == 0 means every stub in the unit
    unit="ovl_dng_%s"%tag
    # NEVER OVERWRITE AN EXISTING WORKING COPY. This `cp' was unconditional
    # and it DESTROYED A LIVE AGENT'S WORK on 2026-11-25: the hardcoded PICKS
    # above re-provisioned the previous batch's five units, and for the one
    # agent still running it replaced its edited ovl_dng_lo16.c with the
    # pristine csrc copy. That agent kept its own snapshots and lost nothing,
    # which was its diligence and not this script's safety.
    #
    # A provisioner's job is to SET UP a directory, so an existing unit file is
    # someone's work in progress and refusing is always right. --force is there
    # for a genuine re-provision of a finished agent's directory.
    dst = os.path.join(d, "%s.c" % unit)
    if os.path.exists(dst) and "--force" not in sys.argv:
        print("  REFUSING to overwrite %s (%d bytes, mtime %s) -- an agent may "
              "be working in it. Pass --force to replace it."
              % (dst, os.path.getsize(dst),
                 __import__("time").strftime("%H:%M",
                     __import__("time").localtime(os.path.getmtime(dst)))))
    else:
        subprocess.run(["cp",os.path.join(PROJ,"csrc","%s.c"%unit),dst],check=True)
    # AND THE METADATA FILES GET THE SAME GUARD AS THE UNIT .c. They had none,
    # which is how one agent's five .txt files were destroyed: the unit copy
    # refused to be clobbered and the metadata beside it was rewritten without
    # a word. The auto-allocated agent id above should make this unreachable,
    # so this is a backstop for an explicit --first-agent that collides.
    meta = ("FUNCS.txt", "JTCHECK.txt", "MYTABLES.txt", "TINY.txt", "TYPES.txt")
    held = [m for m in meta
            if os.path.exists(os.path.join(d, m)) and "--force" not in sys.argv]
    if held:
        print("  REFUSING to overwrite %d metadata file(s) in %s -- another "
              "agent's: %s. Pass --first-agent with an unused id (the default "
              "now picks one), or --force."
              % (len(held), d, ", ".join(held)))
        continue    # a module-level `for', not a function -- `return' here is
                    # a SyntaxError, which is how I found out
    with open(os.path.join(d,"FUNCS.txt"),"w") as fh:
        for w,f in rows: fh.write("%-30s %4dw\n"%(f,w))
    for name,cmd in (("JTCHECK",["python3","tools/decomp/jtcheck.py","--unit",unit]),
                     ("TINY",["python3","tools/decomp/dngtiny.py","--unit",tag]),
                     ("TYPES",["python3","tools/decomp/dngtypes.py","--unit",tag])):
        r=subprocess.run(cmd,cwd=PROJ,capture_output=True,text=True,timeout=1800)
        open(os.path.join(d,name+".txt"),"w").write(r.stdout+("\n[stderr]\n"+r.stderr if r.returncode else ""))
    # A TABLE DEFINED IN asm/data/ IS HOMELESS: a compiled switch emits a
    # SECOND copy and the container shifts. One such body landed at
    # 0 differing and failed the container by 76%, because ovlverify compares
    # the function's words and never sees rodata placement. Drop the owner
    # from the list rather than let an agent spend a day on it.
    homeless=set()
    for dp in glob.glob(PROJ+"/asm/data/*.s"):
        for m in re.finditer(r"dlabel (jtbl_\w+)", open(dp,errors="replace").read()):
            homeless.add(m.group(1))
    drop=set()
    for ln in open(os.path.join(d,"JTCHECK.txt")):
        t=ln.split()
        if len(t)>1 and t[1].startswith("jtbl_") and t[1] in homeless:
            drop.add(t[0])
    if drop & {f for _,f in rows}:
        rows=[r for r in rows if r[1] not in drop]
        with open(os.path.join(d,"FUNCS.txt"),"w") as fh:
            for w,f in rows: fh.write("%-30s %4dw\n"%(f,w))
    mine=[]
    for ln in open(os.path.join(d,"JTCHECK.txt")):
        t=ln.split()
        if len(t)>2 and t[0].startswith("func_") and t[1].startswith("jtbl_") and t[0] in {f for _,f in rows}:
            mine.append(" ".join(t))
    open(os.path.join(d,"MYTABLES.txt"),"w").write("\n".join(mine) or "none of my functions owns a table\n")
    print("%s %-5s %2d fn %5dw  tables:%d  fragments:%d  homeless dropped:%d" % (ag,tag,len(rows),sum(w for w,_ in rows),len(mine),len(bad),len(drop)))
