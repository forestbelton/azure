# azure

WIP decompilation of Azure Dreams

- [azure](#azure)
  - [Setup](#setup)
    - [Submodules](#submodules)
    - [CD image](#cd-image)
    - [PSY-Q](#psy-q)
    - [Toolchain](#toolchain)
      - [Ubuntu](#ubuntu)
      - [Fedora](#fedora)
    - [Enter Hatch environment](#enter-hatch-environment)
  - [Build](#build)
  - [Decompiling](#decompiling)

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

The game was built using PSY-Q 4.1's `cc1psx` although its runtime libraries
are from PSY-Q 4.0. Ensure you have a `psyq/` directory in the project root
with the following contents:

```
aspsx-2.67.exe
aspsx.exe
cc1plpsx.exe
cc1psx.exe
ccpsx.exe
cpppsx.exe
lib/
    2mbyte.obj
    8mbyte.obj
    autopad.obj
    libapi.lib
    libc2.lib
    libcard.lib
    libcd.lib
    libc.lib
    libcomb.lib
    libds.lib
    libetc.lib
    libgpu.lib
    libgs.lib
    libgte.lib
    libgun.lib
    libmath.lib
    libmcrd.lib
    libpress.lib
    libsio.lib
    libsnd.lib
    libsn.lib
    libspu.lib
    libtap.lib
    noheap.obj
    none3.obj
    poweron.obj
```

The project includes a script to ensure that this directory has the correct contents:
```
$ python tools/validate-psyq.py
OK
```

### Toolchain

GNU binutils handle assembly and linking. Some Linux distributions have packages
for this; otherwise, you may have to build the compiler from source yourself.

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

### Enter Hatch environment

Make sure you have [Hatch](https://hatch.pypa.io/latest/) installed.
Then create a new shell for the project:

```
$ hatch shell
```

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
