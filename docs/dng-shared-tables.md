# Jump tables shared across carved dungeon units

The carved DUNGEON.BIN units are replicated code, so one jump table's shape
recurs across several units' stubs. `jtcheck` finds these by entry count and
distinct-target count. **Solving one body gives the others their minval, their
pad-versus-arm split and their arm order for free** — which is most of the work
on a table owner, and all of it is invisible to `ovlverify` (see
`docs/matching-levers.md`, "`ovlverify` IS BLIND TO A JUMP TABLE'S ARM ORDER").

## The 12-entry family — SOLVED 2026-11-25

Carried by `jtbl_*_80170820` and equivalents. Members found by `jtcheck`:

| body | unit | state |
|---|---|---|
| `func_b42_801714B8` | b42 | **landed** earlier — the first reference, 481 words |
| `func_b39_80170F6C` | b39 | **landed** 2026-11-25, 617/617, 0 differing |
| `func_b07_80170E54` | b07 | in progress |
| `func_b08_80170E7C` | b08 | **still a stub** |
| `func_b22_801724BC` | b22 | **still a stub** |

**The map, confirmed against the b39 listing and by a standalone
`tools/cdkgcc -S` compile:**

* **minval is 1** — the dispatch is `addiu $v1, $v0, -0x1` then
  `sltiu $v1, 0xC`, so the case set is 1..12.
* **Entries 4, 10 and 11 are gcc's pad, not arms.** All three hold the
  `default` label, which is also the range-check `beqz`'s target. So there are
  **nine real cases**, and 4, 10 and 11 must NOT be written as empty `break`
  arms.
* **Arm/source order:** `{8,9}` → `{5,6,7}` → `{1,2,3}` → `default`, with
  **`case 12:` nested inside the `w[0x9A] != 0x11` then-arm** rather than as a
  top-level arm.
* **The axis the family varies on:** b39 merges cases 8 and 9 into one arm
  where b42 splits them. That was the only entry-map difference between those
  two, so check it rather than assuming either.

**The positional verification, in full, because it is the method as much as the
result.** The standalone compile emits gcc's own `$L` labels, so the check is a
substitution: order the emitted arm blocks by address, order the ROM's arm
labels by address, match them up, and the emitted entry sequence should become
the ROM's entry sequence.

    emitted entries   [$L84, $L84, $L84, $L85, $L79, $L79, $L79,
                       $L73, $L73, $L85, $L85, $L81]
    emitted arm-block addresses (.s lines)
                      $L73=1246 < $L79=1293 < $L81=1315
                                 < $L84=1324 < $L85=1341
    ROM arm addresses 16D8 < 1740 < 1784 < 1794 < 17C0
    substitution      $L73=16D8  $L79=1740  $L81=1784
                      $L84=1794  $L85=17C0
    emitted, resolved [1794, 1794, 1794, 17C0, 1740, 1740, 1740,
                       16D8, 16D8, 17C0, 17C0, 1784]  == the ROM's

> **Writing the arms ascending by case value would permute this table**, and
> `ovlverify` would still read 0. The substitution above is the only check that
> sees it short of `make containers`.

The same unit's 5-case table checks the same way and is positionally 1:1:
emitted `$L59 = [$L46, $L48, $L52, $L55, $L57]` at strictly ascending block
addresses, against the ROM's five strictly ascending `.Lb39_*` labels.

**Why the standalone word count differs from the ROM's by one, and it is not a
fault:** the 5-case table emits FIVE words standalone where the ROM has six.
The sixth ROM word is a real `0x00000000` alignment pad — an odd case count
followed by an 8-aligned rodata object (the 12-entry table itself) — and a
standalone compile has no following object because `asmlib` does not expand
`INCLUDE_ASM`/`INCLUDE_RODATA`. **So compare the ENTRIES, not the word count,
and expect a missing trailing pad.**

**A guess in the recon that turned out wrong**, recorded so it is not inherited:
b39's OTHER table, the 5-case `jtbl_b39_80170808`, was expected to be b42's
inlined `d + 0x46` dispatch. It is not. It is a separate
`*(s16 *)(p + 0xA8)` state machine gated on `*(s32 *)(d + 0x14) & 0x20000`,
minval 0, cases 0..4, and its sixth word is a real `0x00000000` alignment pad
rather than a label.

## How to use this file

When provisioning a unit whose stub owns a table, grep this file for the entry
count first. If the family is listed, put the map in the agent's brief verbatim
— that is what turned a 617-word body into a landing with no donor search, and
what let the b07 agent skip deriving the same minval a third time. If the
family is not listed and you solve one, add it here.
