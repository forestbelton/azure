# azure

WIP decompilation of Azure Dreams

- [azure](#azure)
  - [Goals and Progress](#goals-and-progress)
  - [Setup](#setup)
    - [Submodules](#submodules)
    - [CD image](#cd-image)
    - [PSY-Q](#psy-q)
    - [Toolchain](#toolchain)
      - [Ubuntu](#ubuntu)
      - [Fedora](#fedora)
    - [Python](#python)
  - [Build](#build)
  - [Decompiling](#decompiling)

## Goals and Progress
The end goal of the project is **not** to simply have a source tree that compiles byte-exact to the CD image. In some sense, that would be trivially satisfiable.

The goal is to have a source tree that reflects what someone who developed this game would have had on their computer, sprinkled with some modern quality-of-life improvements. This is a subjective process, but should include:

- Decompiled C source for every C function in the game
  - **NOTE**: Some C functions include assembly directives (e.g. COP2 instructions), and some functions are not C
- Descriptive names for all declarations (functions, types, data)
- Assets (textures, songs, etc.) represented in understood formats

Ideally, the below progress table would be automated away. At the moment, it's not, so I've opted not to include hard numbers. Looking at the splat configurations should give a good indicator of how far along each file is.

|File       |Code found|Decompiled|Fully split|Labeled|
|-----------|----------|----------|-----------|-------|
|SLUS_006.14|        ✅|        ✅|         ❌|     ❌|
|MAIN.BIN   |        ✅|        🚧|         ✅|     🚧|
|TOWN.BIN   |        ✅|        ✅|         ❌|     ❌|
|DUNGEON.BIN|        🚧|        ❌|         ❌|     ❌|

## Setup

### Submodules
Several submodules are embedded into the project for tooling. In order to fetch them, run:

```
$ git submodule init
$ git submodule update --recursive
```

### CD image

Copy the CD image to project directory and name it `az.bin`. It must have the following SHA256:

```
2d6843c42ca595687b3780e20ab8c8873705ccead5c8bbc0b69d1fa70bc00a90
```

### PSY-Q
The only PSY-Q dependencies are the **headers** from version 4.0.

Ensure you have a `psyq/` directory in the project root with the following contents:

```
include/
  <all psy-q headers>
```

The project includes a script to ensure that this directory has the correct contents:
```
$ python tools/validate-psyq.py
OK
```

### Toolchain

GNU binutils handle assembly and linking. Some Linux distributions have packages for this; otherwise, you may have to build it from source yourself.

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
