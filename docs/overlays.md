# Overlays: how the game's code is actually laid out

Written 2026-10-22, after `docs/where-we-are.md` had spent a while implying the
game was nearly decompiled when what is nearly decompiled is the engine.

## The PSX model, in one paragraph

There is no virtual memory and no demand paging. The R3000 has no MMU for user
code: `0x80000000` is a fixed-offset cached window onto the 2 MB of physical
RAM, so a "virtual address" here is just a physical address with the cache bit
set. An **overlay** is therefore nothing clever -- it is a blob of code
compiled and linked for one fixed address range, read off the disc into that
range at runtime, and called. Nothing is relocated when it loads: every
address inside it was baked in at link time. Two overlays that occupy the same
range are simply mutually exclusive at runtime.

## There is no "load overlay" BIOS call

The kernel's loader functions are about whole programs, not overlays:

* `Load(path, header)` reads a PS-EXE into the address named in its own header;
* `Exec(header, argc, argv)` jumps into one;
* `LoadExec(path, sp, gp)` does both and does not come back.

Some games do use `Load`/`Exec` to swap in overlays, since a PS-EXE's header
carries its own load address. But the common pattern -- and the one Azure
Dreams uses -- is for the game to do it itself: seek with libcd, read the
sectors into a buffer at the right address, and then call a function pointer or
a known absolute address. The "overlay system" is ordinary file I/O plus the
linker having been told where things go. Nothing marks an overlay as special at
runtime.

## How the toolchain supports it

PSY-Q's `psylink` has explicit overlay support: the linker command file
declares overlay groups, each with a load address, and symbols inside resolve
at link time. The linker emits each overlay as its own binary image plus a
symbol table.

The consequence that matters for us: **one address can mean different code in
different overlays.** splat, and every decompilation convention built on it,
assumes an address identifies a thing. Any attempt to map these files has to
carry an overlay identity alongside each address.

## What this project's own binary shows

The resident executable `data/slus_006.14` is loaded at `0x8002D000` and ends
at `0x8008875F`. Every function the engine calls but does not contain sits
**immediately above that**, which is the classic layout -- resident engine low,
overlay region directly on top:

```
800894A0                                  one isolated entry
800990FC 80099194 80099290 80099734       a dense run, gaps of 0x98..0x8F4
8009A028 8009A21C 8009A3D0
800A0B94 800A1618 800A1BD0 800A32A4
800A48F0 800A5720 800A9A0C 800AA36C
800AA53C 800AA888 800AC3EC 800AC4C4
800AC82C                                  ...run ends ~0x800AC82C
800D5460                                  +0x28C34 -- a different region
801768AC                                  +0xA144C -- near the top of RAM
```

The run from `0x800990FC` to `0x800AC82C` spans about 80 KB with small gaps,
which reads as **one overlay** holding many functions that the engine calls.
`0x800D5460` and `0x801768AC` are far outside it -- either further overlay
regions or separately loaded blocks. (RAM ends at `0x80200000`, so
`0x801768AC` is near the top, where a large buffer would sit.)

What the engine actually calls out to is informative: func_80042BDC's message
idiom is `func_800990FC()` / `func_80099734()` / `func_80099194()` /
`func_80099290()` / `func_800A5720()` -- allocate a message, bind an entity and
a string to it, render, release. So the text and UI layer lives in the overlay
and the engine depends on it. The engine is not a self-contained program.

## Why we can match the engine without the overlays

Because nothing is relocated. A call into an overlay is a `jal` with a 26-bit
absolute target, so the only thing the engine needs is the *number*, and
`splat/slus_006.14_undefined_funcs_auto.txt` supplies it as a linker-script
assignment. That is exactly why 162 functions can match byte-for-byte against
code that calls into a 1 MB overlay nobody has looked at.

The same mechanism is what makes the overlays hard: their code is full of
`jal`s back into the engine and references to engine globals, so they are not
independent either.

## Where the overlays actually load (answered 2026-10-26)

Three code overlays are now placed. None of the containers is loaded as a
unit -- they are 2.5/16.1/26.5 MB against 2 MB of RAM, and the disc is
addressed by absolute LBA, so a container is just a region of it.

| Container | Overlay at | Loads at | Code | Functions |
|---|---|---|---|---|
| `town.bin` | `+0xb000` | `0x80088760` | `+0xd630..+0x51ed8` | 536 |
| `dungeon.bin` | `+0xa3000` | `0x80088760` | `+0xa3e28..+0xf7594` | 353 |
| `main.bin` | `+0x25d000` | `0x80020000` | `+0x25d924..+0x264c90` | 130 |

`0x80088760` is the first byte after the resident engine's last
(`0x8008875F`), which is the layout this document predicted. town and dungeon
share it, so they are mutually exclusive and an address there means different
code depending on which is resident -- the identity problem described above is
real and unavoidable. `main.bin`'s overlay is different: it loads *below* the
engine, into the `0x80010000` region the engine reaches with the out-of-image
constant-base-pointer idiom.

Each overlay is rodata first and code second, which is gcc 2.7.2 at `-G8`
emitting data ahead of functions (see `include/common.h`). That is why
`town.bin+0xb000` opens on Shift-JIS text and `main.bin+0x25d000` on an
"Assertion failed" string rather than on instructions.

**The method, since it needs no emulator and generalises.** Collect every
offset in a container whose word is `addiu sp,sp,-N` (a function prologue).
Then find the single offset-to-address delta with the most agreement, by
either of two votes:

1. against the 49 overlay function addresses in
   `slus_006.14_undefined_funcs_auto.txt` -- town scored 15/49, dungeon 12/49,
   both against a noise floor of 6;
2. against the block's *own* `jal` targets, which needs no symbol list at all
   -- town 536, dungeon 353, main 130, all against a noise floor of 0. This is
   the stronger test and the only one that worked for `main.bin`, whose code
   the engine barely calls (4/28 on the first test).

Disassembling a predicted function confirms it: `town.bin+0x2ec8c` at vma
`0x800AC3EC` is a clean prologue that builds `0x80083178` (our
`D_80083178`) and makes internal `jal`s to `0x800AC37C` / `0x800AC1B0`.

All three start offsets are confirmed against live hardware behaviour. A
PCSX-Redux CD-ROM command log (`Log` window -> `Special` -> "Log CD-ROM
commands", plus `-stdout`) shows the game seeking LBA 3027 =
`main.bin+0x25d000` during boot, LBA 3099 = `town.bin+0xb000` on entering the
town, and LBA 12636 = `dungeon.bin+0xa3000` on entering a dungeon -- the last
being the most-requested address of the whole dungeon session, at 21 seeks,
which is what reloading an overlay per floor looks like.

`dungeon.bin` holds only one code overlay. Its seeks repeat at `0xa3000` plus
`0x200000` strides, as if the container were chunked in 2 MB units, but
assuming the same base at those offsets scores 0-45 against `+0xa3000`'s 353:
they are assets and low-RAM data. `+0x2a3000` holds pointers into
`0x8001d4xx`, so it belongs to the same low-RAM region `main.bin`'s overlay
occupies, and `+0x9f000` / `+0x4a3000` are texture records.

Not resolved: a second code run at `main.bin+0x269000..+0x272000` (191
prologues, so certainly code) whose base is unknown -- only 58 internal jal
targets and a best score of 6 against a floor of 5. It cannot share
`0x80020000`, which would place it across the engine's base. And
`0x801768AC`, the isolated address near the top of RAM, is in none of the
three spans. `0x800D5460` is, in dungeon's.

Do not try to trace this with PCSX-Redux Lua breakpoints: on the
`8a86b7ec` AppImage build, `PCSX.addBreakpoint` installs, reports itself
enabled, and never fires -- not even on `VSync` -- with `-debugger
-interpreter` and verified-correct addresses. The CD-ROM command log works and
costs nothing.

## If we ever map more

In rough order:

1. **Find the archive structure.** Partly done. The engine's CD queue takes
   8-byte descriptors, `{nsec << 23 | dest, LBA}`, with `dest == 0` meaning
   "the D_80081480 bump arena"; the pump is `func_8003E758` and the queue
   dispatcher `func_8003E4FC`. The 49 descriptors at `0x80081020..0x800811a8`
   are the *audio* table (KCET sequences and VAB banks, verified by the magic
   bytes at each source offset), and `D_8006E6F4` is a 2-entry table of
   descriptor pairs selecting the town or dungeon sound bank. No descriptor in
   the ROM targets a code overlay, so those are built at runtime.
2. **Recover each entry's load address.** Done for the three code overlays
   above, by prologue/jal fingerprinting rather than from any table. Still
   open for the asset entries. Either from the container's own
   table, or by taking a code entry's internal `jal` targets and finding the
   base that makes them land on its own function starts.
3. **Then** a splat spec per overlay, with its own vram base, and a way to keep
   two overlays' identical addresses apart.

Sizes, for scale: the heuristic in `docs/where-we-are.md` puts about 8.4 MB of
code-bearing blocks across the three containers against 216 KB in the resident
executable. Even discounting it heavily for packed data, the content dwarfs the
engine.
