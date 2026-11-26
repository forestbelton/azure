# The DUNGEON.BIN endgame: the remaining work is PARKS, not fresh bodies

Written 2026-11-26 at 87.8% (6,563 of 6,825 functions, 262 stubs, 78 parks),
because the dispatch pattern that got here stops working shortly and it is
better to say so before the briefs start coming back empty.

## What is actually left

**Fresh, unparked, unheld work: about 28 functions in eight units.** Measured
with `provision_batch.py <unit> --dry-run`, which drops parks and fragments, so
these are the numbers that matter rather than the stub counts:

| unit | functions | words |
|---|---|---|
| b57 | 3 | 1,993 |
| s37 | 4 | 1,224 |
| s33 | 4 | 923 |
| s70 | 4 | 862 |
| b38 | 3 | 832 |
| b23 | 3 | 809 |
| b19 | 2 | 540 |
| s25 | 5 | 306 |

Plus whatever the five live agents do not land.

**Everything else -- the large majority of the 262 remaining stubs -- is parked
or fragmented**, and three units that look like big blocks are not:

* `lo20`: 40 stubs, but **33 are fragments** and provisioning yields nothing.
* `lo16`: 20 stubs, **19 parked** (and all six that could not even be
  measured are now measurable; two are word-exact).
* `b31f`: 11 stubs, **10 parked**, and the eleventh is the 849-word body whose
  jump table is 4-byte aligned and therefore cannot meet the gate at all.
* `b29`, `s56`, `b22`: 0 provisionable each. Their remaining stubs are all
  parks.

**So the stub count overstates the available work by roughly eight to one.**
Pick targets with the dry-run, never off `progress.py --by-unit`'s stub column
-- its `parked` column reads 0 even for units with nineteen park files, which
is how a whole afternoon's worth of bad picks happens.

## Why the brief has to change shape

A unit brief says "land these N stubs, here is your donor route". That is the
right shape while fresh bodies exist. For a park it is the wrong shape, because
a park already HAS a body, a measured residue and a priced table of rejected
spellings -- the work is not writing C, it is finding the one axis nobody has
probed.

**What has actually cracked parks today**, in order of how often:

1. **A lever that did not exist when the park was written.** The b07 park sat at
   one row with 26 spellings measured flat and a compiler-source argument that
   it was unreachable; it landed on two lines once someone read the *whole*
   predicate in `cse.c:838` and found the second disjunct. **So a park's
   "closed" axes are only closed against the levers of the day it was written.**
2. **A sibling landing in the same family.** The same park's residue was closed
   by an agent working the equivalent body in a different unit, who met the row
   fresh.
3. **A tool fix.** Six lo16 parks were unmeasurable for reasons that were in
   `parksplice`, not in the parks.

**None of the three is "think harder about the park".** A park brief should
therefore carry: the park's own priced table (so it is not re-run), the levers
added since its date, and the names of its family's landed siblings.

## The three bodies that can never meet the gate

Not parks -- held, and they should not be handed to anyone as work:

* `func_b31f_801730AC` (849w) -- its jump table is 4-byte aligned, so a C
  `switch` cannot place it.
* `func_b29_8017088C` (150w) -- splat labels a Shift-JIS rodata word as the
  function's first instruction, so the target word count is 151 and a
  byte-correct body can never measure 0.
* one b31 body with its table in `asm/data/` -- a compiled `switch` emits a
  second copy and the container shifts.

Everything else is reachable as far as anyone has shown.
