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

## If we ever map one

In rough order, and none of this has been started:

1. **Find the archive structure first, not the code.** `dungeon.bin` is 26.5 MB
   against 2 MB of RAM, so it cannot be one image; it is a container of many
   entries loaded on demand. Locate its index -- offset/size/destination
   triples -- before disassembling anything. `town.bin` (16.1 MB) is the same
   shape; `main.bin` (2.5 MB) is small enough to be a handful of overlays.
2. **Recover each entry's load address.** Either from the container's own
   table, or by taking a code entry's internal `jal` targets and finding the
   base that makes them land on its own function starts.
3. **Then** a splat spec per overlay, with its own vram base, and a way to keep
   two overlays' identical addresses apart.

Sizes, for scale: the heuristic in `docs/where-we-are.md` puts about 8.4 MB of
code-bearing blocks across the three containers against 216 KB in the resident
executable. Even discounting it heavily for packed data, the content dwarfs the
engine.
