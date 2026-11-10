# The node payload thread

Legibility work, not matching work: the project is at 100% and nothing here
is allowed to change a byte. Picked up 2026-11-09, left unfinished on
purpose.

## What is established

`UnkNode` (`include/structs.h`) is a 0x20-byte HEADER. Every user allocates
0x20 plus a payload of its own and reaches that payload off the node. The
witnesses and the two codegen measurements are in the struct's own comment;
the short version is that `func_8003FE78`'s size argument is the whole object
in words and varies per caller, each `bzero` after a reuse clears that size
minus 0x20, and `csrc/gprel_72A8.c` converts the other way with
`(UnkNode *)(p - 0x20)`.

So the node now ends with `u8 payload[0]` -- gcc's zero-length-array
extension, NOT the C99 flexible array member `u8 payload[]`, which cc1psx
rejects with "field `payload' has incomplete type". Neither form contributes
to `sizeof`, so the header is still 0x20 and `n + 1` still advances by it.
Measured at `-O2 -G0`: `n->payload`, `(u8 *)n + 0x20` and `(s32 *)(n + 1)` all
compile to the same `addu $2,$4,32`.

## What landed

`faa8e5f` converted the eight sites whose base was already declared
`UnkNode *`, so no judgement about the pointer was needed: the four list
walks (`func_80033D54`, `func_80045340`, `func_80044BB0`, `func_80044C54`),
the two dispatchers that hand a payload to a callback (`func_80040190`,
`func_800402F4`), and the two `(n + 1)` sites in `func_800525D4`. All eight
were free.

## What is left

142 code sites, as of 2026-11-09:

| unit                      | sites |
|---------------------------|-------|
| `ovl_town.c`              | 108   |
| `ovl_town_800BF3F0.c`     | 17    |
| `unk_72A8.c`              | 12    |
| `unk_19434.c`             | 2     |
| `unk_1C540.c`             | 2     |
| `unk_12DEC.c`             | 1     |

**Count them with this and nothing looser**, because two sloppier queries
each gave a wrong number -- 190 and 178 -- and the wrong numbers are in the
session that started this file:

```python
re.compile(r'\(u8 \*\)\s*[A-Za-z_][A-Za-z0-9_]* \+ 0x20(?![0-9A-Fa-f])')
```

The negative lookahead is what stops `+ 0x204` matching, and block comments
have to be stripped first or the prose quoting these shapes is counted as
code.

**The work is reading, not measuring.** A `+ 0x20` off a pointer that is not
a node reads identically, and in `ovl_town.c` the bases are spelled `o` (63),
`n` (50), `p` (23), `a`, `node` and `g`. Each one needs its base established
as a node before it becomes `n->payload`. The measuring turned out to be the
cheap half: all eight of the first pass were free.

## The loop

1. Establish the base is a node. A declared `UnkNode *` is enough; otherwise
   follow it back to a `func_8003FE78`/`func_8003FC64` result or a
   `- 0x20`/`- 8` conversion.
2. Convert to `n->payload`, keeping the SAME base variable -- see below.
3. `rm` the unit's `.o`. **A header is not a make dependency here**, so
   editing `structs.h` or a unit's header and running `make` relinks stale
   objects and reports undefined symbols or a stale match. This cost a
   detour already.
4. `make containers`. Four `OK byte-identical` is the whole test: every one
   of these units is linked into a container, so a codegen change cannot
   hide. Back out any site that moves a byte and note why.
5. `clang-format --dry-run <unit>` before and after. Several of these units
   are clean and should stay that way; `gprel_*` units write casts as
   `(u8 *) p`, with the space, so match the local style.

**Do not collapse a converted site onto an equal alias.** `func_800525D4`
holds `s = n->payload` and then writes `*(u16 *)n->payload = 0`, where `s`
and `n->payload` are the same address and the second could read `*(u16 *)s`.
The comment above that function records how narrowly `s` wins `$s1` against
five block-scoped handles, 45 differing words if it flips, so adding a
reference to `s` is a separate measurement rather than a tidy-up.

## The other end of the same fact

The header side has the same problem mirrored, and the same fix:

| spelling | is | sites |
|---|---|---|
| `p[-4] = f` | `((UnkNode *)p - 1)->unk_10` | 57 in `ovl_main_80020000.c` |
| `((u16 *)p)[-1]` | `...->unk_1E`, the flags | 5 in `ovl_main_80020000.c` |

Measured free in isolation: `p[-4] = v` and `((UnkNode *)p - 1)->unk_10 = v`
both compile to `sw $5,-16($4)`. `ovl_main_80020000.c` also carries 27
`q = p +/- 8` locals, several in landed functions, so the shifted-pointer
shape is not forbidden -- but these are ~50 functions that all match today,
so it is a per-function measurement. 19 more `[-4]` sites sit in
`ovl_main_80400000.c`, which is the paused twin; leave them.

**One open typing question that this overlay settles.** `UnkNode.unk_10`'s
comment says it is "called through elsewhere, so a function pointer -- left
as `u32` until that is used". The card overlay is that use: 57 sites store a
function's address into it. Typing it as a function pointer would touch every
unit that assigns it, so it is its own change with its own container check.

## Not this thread

The six `UNK_CARD_GROUP` payload structs in `csrc/main.h` are the same gap
seen from inside a payload, and naming their pads buys nothing on its own --
every access goes through `(T *)(p + K)` arithmetic at the call site, so only
converting the call sites helps a reader, and that is the half that can move
code. It is also downstream of carving `ovl_main_80020000.c` back into its
nine original units, which that file's header comment asks for first.
