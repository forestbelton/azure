# The original identifiers in TOWN.BIN, and what they name

`town.bin` carries two symbol tables that the development build's flag viewer
used, and they are still on the retail disc: **3,587 of the original source's
own names**. This is how to read them, which of them are already attached to
addresses, and what the rest are waiting on.

`docs/town-map.md` is where the tables sit in the container.
`tools/scan-container.py --symbols FILE` dumps them.

**Fixed findings only.** The record layout, the values and the two table
extents are properties of the image. The handler names in
`csrc/ovl_town*.c` are DERIVED from them and are called out as such below.

## The record

    struct { char name[32]; s32 value; };     /* 36 bytes, NUL-padded */

| file | records |
|---|---|
| 0x80C000..0x811F58 | 678 |
| 0x812000..0x82B914 | 2,909 |

**The name comes first, and getting that backwards is the one way to read the
whole table wrong.** Reading it as `{s32 value; char name[32];}` parses, keeps
the stride, and passes every structural check -- it just pairs each name with
its PREDECESSOR's value. The error is invisible inside a run and catastrophic
at a group boundary, where a name picks up the previous enumeration's last
value. Two independent checks fix the order:

* `snf_false` is **0** and `snf_truth` is **1**. In the other order
  `snf_false` takes whatever word precedes the table.
* `DFLT_END_PSN_DM` is **22**, which is the index `ovl_town`'s own dispatcher
  starts the per-actor table at -- `-0x58` in the listing, which is `-22 * 4`.
  In the other order it is 21 and the arithmetic does not close.

By prefix, over both tables:

| prefix | n | prefix | n | prefix | n |
|---|---|---|---|---|---|
| `F*_` (`F_`, `Fr_`, `Ft_`, `Ftt_`, `Fc_`, `Fp_`) | 2,907 | `IMG_` | 79 | `PSN_` | 21 |
| `SSTP_` | 138 | `GOODS_` | 56 | `ANM*` | 13 |
| `mamonogoya_` | 128 | `S_` | 44 | `Arg_` | 3 |
| `FNO_` | 113 | `V_` | 25 | `Mode_`, `TB_` | 2 each |
| | | `sn*` | 24 | other | 32 |

The values are ordinals in a namespace, not addresses: nothing in a record
says where any code is. A name becomes useful only when something in the
image is indexed by its value.

## What is attached: the person state machine

`func_800C3988` in `csrc/ovl_town_800BF3F0.c` states the whole structure in
its own bounds:

```c
t = e->unk_00;                                  /* the person's state */
if (t >= 2 && t < DFLT_END_PSN_DM)              /* 2..21: the shared set */
    D_800D5084[t](a, b, c);
else if (t >= DFLT_END_PSN_DM)                  /* 22+: this actor's own */
    ((void (**)(...))a->unk_5C)[t - DFLT_END_PSN_DM](a, b, c);
```

and the table matches it to the entry:

```
PSN_DM_KANASIBA  1      PSN_DM_NORTHWK   9      PSN_DM_DISP_OFF  17
PSN_DM_HOMEPOS   2      PSN_DM_SOUTHWK  10      PSN_DM_DISP_ON   18
PSN_DM_TALK      3      PSN_DM_WESTWK   11      PSN_DM_DEL       19
PSN_DM_TALK2     4      PSN_DM_EASTWK   12      PSN_DM_ANG_ORG   20
PSN_DM_NORTHWAY  5      PSN_DM_JMP      13      PSN_DM_POS_ORG   21
PSN_DM_SOUTHWAY  6      PSN_DM_GOTGT    14      DFLT_END_PSN_DM  22
PSN_DM_WESTWAY   7      PSN_DM_PLWAY    15
PSN_DM_EASTWAY   8      PSN_DM_TGTWAY   16
```

`D_800D5084` is **22 entries**, so it is indexed by the state directly, and
`DFLT_END_PSN_DM` is one past its last live slot. Entries 0 and 1 are dead --
the dispatcher's lower bound is `t >= 2` -- and both hold the same pointer,
which is also the handler other code calls outright.

Every `SSTP_<CHARACTER>_*` enumeration in the table starts at **22**, which is
the same 22: a character's own states carry on where the shared set stops.
The player is not split that way. `D_800D0508` is **one 54-entry table** and
`ovl_town.c` dispatches it with no upper bound:

```c
n = (s8)D_80082660[1].unk_00;
if (n >= 2) D_800D0508[n](t, b, c);
```

`SSTP_PLT_DM_*` is **32 contiguous values, 22..53**. 22 + 32 = 54, which is
the table's length exactly, and the first 22 entries of `D_800D0508` have the
same handler layout as `D_800D5084`'s -- the same shared states compiled for a
second actor. So slot 1 of `D_80082660` is the player.

### Two checks that are not about counting

Counting tables against enumerations is how this was found, not why it is
believed. The handlers themselves confirm it:

* **The four `*WAY` handlers differ in exactly one constant, and it is an
  angle.** 25 words each; `NORTHWAY` sets 0x800, `WESTWAY` 0xc00, `EASTWAY`
  0x400, and `SOUTHWAY` sets nothing at all because its angle is 0 and the
  `ori` is elided, which is why that one is a word shorter. The engine's
  angles wrap at 0x1000, so **the opposite names get opposite angles** --
  north/south 0x800 apart, west/east 0x800 apart. Four names, four quarter
  turns, in enumeration order.
* **The four `*WK` handlers differ in exactly one word**, an `addiu` of
  0x3c7c / 0x3d08 / 0x3d94 / 0x3e20 -- four per-direction records 0x8C apart.

### What that renamed

72 states, 72 distinct handlers, no two sharing one: **20 shared handlers**
(`psn_dm_*`, in `ovl_town_800BF3F0.c`), **20 player copies of the same states**
and **32 player-specific handlers** (`plt_dm_*`, both in `ovl_town.c`).
`include/town.h` holds the recovered constants and the `DFLT_END_PSN_DM` bound
is now spelled at the dispatcher instead of `0x16`.

**The handler names are derived, not recovered.** `psn_dm_homepos` means "the
`D_800D5084` entry `PSN_DM_HOMEPOS` selects". The state names are the
evidence; the handler names are bookkeeping on top of it. All four containers
stay byte-identical, `progress.py` and `guard.py` are unchanged.

## What is not attached, and what each one needs

| group | n | what it indexes | what is missing |
|---|---|---|---|
| `F*_` | 2,907 | scenario flags, `F_zero` = 1002 upward; `Fr_`/`Ft_`/`Ftt_` callbacks | the flag get/set pair. Its callers pass the index as a literal, so finding the pair names every call site at once. |
| `FNO_` | 113 | **values 100..212** -- "function number", a dispatch table indexed from 100 | `ovl_town` has no table or switch of that length (largest jump table 97, largest switch 32 cases), so the dispatcher is in the tier-2 overlays. |
| `SSTP_` | 138 | the other 25 characters' `unk_5C` tables | each character's table needs tying to its character. The sizes are available (`NIKO` 13 entries, `WEDY` 9, `FAR`/`MIA` 8, `MAM`/`PATY` 7, ...) and `ovl_town`'s data holds unassigned function-pointer tables of 6, 7 (x5), 11 (x2) and 12 entries, but five tables of the same size cannot be told apart by length -- the install site has to do it. |
| `IMG_` | 79 | per-character image ids, each restarting at 0 | the per-character image table. `docs/image-blobs.md`'s `D_8006CDC8[]` is 27 entries and is not it. |
| `GOODS_` | 56 | item ids, 5,2xx | an item table of 56 entries. |
| `mamonogoya_` | 128 | the monster shed, values 0x8002..0x8081 | the 0x8000 bit says these are a different space from the plain flags. |
| `S_`, `V_`, `Arg_`, `Mode_`, `TB_` | 76 | scenario script arguments and modes | the script interpreter, which is tier-2. |

The pattern is consistent: **what the names index mostly lives in the 48
tier-2 overlays that are not decompiled** (`main.c`, `lshop.c`, `tobjs.c`,
`sn_main.c`, `player.c`, `flgtst.c` -- `flgtst.c` is almost certainly the flag
viewer these tables were built for). The person state machine is attachable
because that one dispatcher happens to live in `ovl_town`.

`dungeon.bin` carries five smaller tables of the same shape (166 records, all
`SSTP_PLT_DM_*`-style animation states); `main.bin` and `slus_006.14` carry
none.

## The trap, restated

A table value is a small integer and the value space is dense, so **matching a
literal in the code against the table finds nothing but coincidence.** Scoring
every `func_*(N)` call site in `csrc/` against the 3,587 values gives four
functions whose literal arguments are "100% table values", and all four are
noise: one of them is called from `memory.c` and `card_mes.c` with 512, 768 and
1282, which are sizes. The names only attach through a table whose LENGTH, or
a dispatcher whose BOUNDS, independently say which enumeration indexes it.
