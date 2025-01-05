# azure
WIP decompilation of Azure Dreams

## Setup

### Copy CD image
Copy the CD image to project directory and name it `az.bin`.

### Install toolchain
Ubuntu has a package for the `mipsel` (cross-)compiler. Otherwise, you will
have to source the compiler yourself. This may require building the compiler
from source.

```
$ apt-get -y update
$ apt-get -y install \
    build-essential \
    g++-mipsel-linux-gnu \
    cpp-mipsel-linux-gnu \
    binutils-mipsel-linux-gnu
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
