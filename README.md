# azure

WIP decompilation of Azure Dreams

- [azure](#azure)
  - [Goals and Progress](#goals-and-progress)
  - [Setup](#setup)
    - [Submodules](#submodules)
    - [CD image](#cd-image)
    - [PSY-Q](#psy-q)
    - [The compiler](#the-compiler)
    - [Toolchain](#toolchain)
      - [Ubuntu](#ubuntu)
      - [Fedora](#fedora)
    - [Python](#python)
  - [Build](#build)
  - [Decompiling](#decompiling)

## Goals and Progress
The end goal of the project is **not** to simply have a source tree that compiles
byte-exact to the CD image. In some sense, that would be trivially satisfiable.

The goal is to have a source tree that reflects what someone who developed this
game would have had on their computer, sprinkled with some modern quality-of-life
improvements. This is a somewhat subjective process, but should include:

- Decompiled C source for every C function in the game
  - **NOTE**: Some C functions include assembly directives (e.g. COP2 instructions), and some functions are not C
- Descriptive names for all declarations (functions, types, data)
- Assets (textures, songs, etc.) represented in a format that can be easily viewed/edited

Ideally, the below progress table would be automated away. At the moment, it's
not, so I've opted not to include hard numbers. Looking at the splat
configurations should give a good indicator of how far along each file is.

|File|Description|
|----|-----------|
|SLUS_006.14|Code/data separated, all game C decompiled, nothing named intelligently|
|MAIN.BIN|Code/data separated, non-debug game C decompiled, some naming around memory card routines|
|TOWN.BIN|Code/data separated, all game C decompiled|
|DUNGEON.BIN|Just getting started|

## Setup

### Submodules
Several submodules are embedded into the project for tooling. In order to fetch
them, run:

```
$ git submodule init
$ git submodule update --recursive
```

### CD image

Copy the CD image to project directory and name it `az.bin`. It must have the
following SHA256:

```
2d6843c42ca595687b3780e20ab8c8873705ccead5c8bbc0b69d1fa70bc00a90
```

### PSY-Q
The only PSY-Q dependencies are the **headers** from version 4.0.

Ensure you have a `psyq/` directory in the project root with the following
contents:

```
include/
  <all psy-q headers>
```

The project includes a script to ensure that this directory has the correct contents:
```
$ python tools/validate-psyq.py
OK
```

### The compiler

The game's compiler is PSY-Q 4.1's `cc1psx`, which is SN Systems' build of a
Cygnus GCC 2.7.2 snapshot. It is built from source, natively, out of the
`vendor/old-gcc` submodule:

```
$ git submodule update --init vendor/old-gcc
$ make cdk-gcc
```

That runs the submodule's `gcc-2.7.2-cdk` Dockerfile and installs `cc1`, `cpp`
and `gcc` into `scratch/cdk-gcc/`. It needs **Docker** and takes about 20
seconds. `make` builds it on demand, so this step is optional.

The result is the same compiler the game was built with, not a substitute: its
`cc1` reports the identical version string (`cygnus-2.7.2-970404
SN32.3.7.0004`), and every unit in `csrc/` compiles to byte-identical assembly
against `psyq/cc1psx.exe` run under wine. A stock FSF GCC cannot be used --
upstream 2.7.2 has no MIPS split-address support and so cannot emit the shape
most of the game's global accesses use. See `docs/compiler-version.md`.

### Toolchain

GNU binutils handle assembly and linking. Some Linux distributions have packages
for this; otherwise, you may have to build it from source yourself.

#### Ubuntu

```
$ apt-get -y update
$ apt-get -y install \
    build-essential \
    g++-mipsel-linux-gnu \
    cpp-mipsel-linux-gnu \
    binutils-mipsel-linux-gnu
```

#### Fedora

```
$ dnf -y install gcc-mips64-linux-gnu
```

### Python

Make sure you have Python and [uv](https://docs.astral.sh/uv/) installed.

## Build

```bash
$ make extract # Extract files from CD image
$ make splat   # Extract code/data from files
$ make         # Build PSX EXE files from extracted data
```

## Decompiling

`docs/where-we-are.md` is the entry point: state, workflow, tools and queue.
`docs/matching-levers.md` is how this compiler behaves -- read it before
writing a function, and add to it when you learn something.
