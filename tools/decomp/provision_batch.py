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

PICKS={"h181":("lo16",16),"h182":("s29",13),"h183":("s30",11),
       "h184":("s43",10),"h185":("b02",10)}
for ag,(tag,n) in PICKS.items():
    d="/home/case/scratch/azure-dng/agent-%s"%ag
    os.makedirs(d, exist_ok=True)
    bad=fragments(tag)
    rows=[r for r in sorted(per[tag]) if r[1] not in bad][:n]
    unit="ovl_dng_%s"%tag
    subprocess.run(["cp",os.path.join(PROJ,"csrc","%s.c"%unit),os.path.join(d,"%s.c"%unit)],check=True)
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
