# Leads: better starting points than the park body

A park records the **best-scoring** spelling of a function plus its priced
table. Sometimes an agent finds a **different** spelling that scores WORSE but
is a better place for the next agent to start -- because it gets a structural
thing right that the park body gets wrong, so its residue is a smaller or
clearer question.

Those spellings go here. **`scratch/` is gitignored** (`.gitignore:8`), so a lead
left in an agent's working directory is one cleanup away from gone -- which is
exactly how five agents' work was lost once already. A lead is only durable if
it is in the repo.

Naming: `func_<tag>_<addr>.<agent's name for it>.c`. Each file is the agent's
own self-contained copy and compiles standalone.

**`parkcheck` globs `docs/dng-parks/func_*.c` and does not recurse**, so nothing
in this directory is measured as a park. A lead is not a park: it is not the
best figure and it is not a claim about the floor.

## Current leads

* **`func_b24_80174C64.sym6.c`** -- parks at 530/3, this leads at **530/6**.
  Why it is the better start: the park body **deletes** the target's reg-to-reg
  copy entirely (cse1's `make_regs_eqv` keeps the load's pseudo), so its 3 rows
  include a structural absence. This spelling -- symmetric
  `h = X; if (h) { q = h; ... }` in both 0xAC blocks -- **produces the copy, in
  the delay slot, and block1 comes out byte-exact.** Its whole 6-row residue is
  `q` and a `loop.c`-hoisted invariant exchanging `$v1`/`$a0`, both
  caller-saved: a wrong register WITHIN a class, with the allocation margin
  measured at 0.25 against 0.33. The agent that found it said outright that the
  next agent should start here and not from the park body.
