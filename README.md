# azure
WIP decompilation of Azure Dreams

## Setup

### Install Python dependencies
First, make sure you have Python installed and `$HOME/.local/bin` is on your
`PATH`.

```
$ pip install --user splat64[mips]
```

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

### Copy ISO
1. Create a directory named `data` within the project
2. Copy the contents of Azure Dreams (NTSC-U) ISO (**not the ISO itself**) into
   `data/`

## Build

```bash
$ make splat # Extract data from ISO contents
$ make       # Build PSX EXE files from extracted data
```

`make splat` can be omitted on subsequent runs; it is only necessary to run if
the `splat` configuration has been updated.
