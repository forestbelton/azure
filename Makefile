# Fail on the first error in a pipeline. Without this, `azure shiftjis ... | as`
# reports only the assembler's status, so a crash in the first stage silently
# yields an object assembled from empty input.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

# The per-unit MASPSXFLAGS override below is a target-specific variable
# assignment, and make treats the first such target as the default goal. Pin
# the goal explicitly so that a bare `make` (and decomp-permuter's import.py,
# which runs `make --dry-run` to recover the compile command) builds
# everything rather than just build/csrc/gprel_12DD4.o.
.DEFAULT_GOAL := all

SLUS = slus_006.14
TOWN = town.bin
MAIN = main.bin
DUNGEON = dungeon.bin

# GNU binutils handles the assemble/link stage: splat emits GNU-syntax asm
# (see include/macro.inc) and a GNU ld script, neither of which ASPSX or
# psylink can consume. Debian/Ubuntu ship binutils-mipsel-linux-gnu; Fedora
# ships binutils-mips64-linux-gnu, which defaults to big-endian mips64 and so
# needs an explicit little-endian/o32 selection.
MIPS_PREFIX ?= $(shell command -v mipsel-linux-gnu-as >/dev/null 2>&1 \
                 && echo mipsel-linux-gnu- || echo mips64-linux-gnu-)
AS      = $(MIPS_PREFIX)as
LD      = $(MIPS_PREFIX)ld
OBJCOPY = $(MIPS_PREFIX)objcopy

ifeq ($(MIPS_PREFIX),mips64-linux-gnu-)
# Verified with binutils 2.46.1: as emits elf32-tradlittlemips/mips:3000 and
# ld supports the elf32ltsmip emulation.
ASFLAGS   += -EL -32
ASFLAGS_C += -EL -32
LDFLAGS   += -m elf32ltsmip
endif

AZURE = uv run azure
CHECK = python tools/check.py

# The game code was built with the cc1psx from the PSY-Q 4.1 runtime disc,
# "cygnus-2.7.2-970404 SN32.3.7.0004" (a Cygnus 2.7.2 snapshot with split
# addresses, patched by SN Systems). PSY-Q 4.0's "2.7.2.SN32.3.7" compiles the
# simple functions identically but cannot emit the hoisted-lui global access
# shape the game uses everywhere; see docs/compiler-version.md. The libraries
# are still 4.0's. aspsx 2.56 and 2.67 emulations both match so far.
# tools/cdkgcc drives a NATIVE build of that same compiler. decompals/old-gcc
# builds SN Systems' own CDK release (cdk-gcc-b18) for --target=mips-sony-psx,
# and its cc1 reports the identical version string, codegen templates and pass
# list, because it is the same compiler -- compiled for Linux instead of run as
# a Windows binary under wine. Measured 2026-11-02: all 40 units in csrc/
# compile to byte-identical assembly and all four containers stay
# byte-identical, and a full rebuild is 4.9s against wine's 8.7s.
#
# The build is reproducible, which is why this is a submodule and a Dockerfile
# rather than a downloaded binary: `make -C vendor/old-gcc VERSION=2.7.2-cdk`
# produces cc1, cpp and gcc BIT-IDENTICAL to that release's own assets
# (sha256 90db2db7.../d8005743.../7a571911...), since the image pins the host
# toolchain at ubuntu:focal.
#
# The wine path is gone entirely: there is no tools/ccpsx and no prefix pool.
# psyq/ is still needed for its headers and its libraries, and the .exes are
# kept as the provenance reference the native build was verified against --
# tools/validate-psyq.py records their hashes. See docs/compiler-version.md.
CC             = tools/cdkgcc
ASPSX_VERSION  = 2.56

CFLAGS += -O2 -Iinclude -Ipsyq/include
# decomp-permuter invokes make with PERMUTER=1. include_asm.h compiles its
# INCLUDE_ASM macros away when PERMUTER is defined, so the define has to reach
# the compiler or the permuter's build pulls in assembly it cannot handle.
ifdef PERMUTER
CFLAGS += -DPERMUTER
endif

CFLAGS_G0 := $(CFLAGS) -G0
CFLAGS += -G8
# The DEFAULT flags under a name, so the one overlay unit that wants them --
# main.bin's debug twin at 0x80400000, which is the live overlay's source built
# the other way -- can opt back in past the `ovl_%` -G0 pattern rule below.
# This is NOT a reintroduction of the removed MASPSXFLAGS_G8 escape hatch (see
# the note further down): nothing here changes what -G the hatch changed, it
# just names the resident default so a pattern rule can be stepped around.
CFLAGS_DEFAULT := $(CFLAGS)

# -G8 is the compiler's default and the game uses small-data addressing: 245
# genuine $gp-relative loads/stores in the game's own code, against
# $gp = 0x80080994 (set by crt0 at 0x800339ac, not by the PS-EXE header, whose
# gp0 field is 0). Getting -G wrong mismatches every small-data access.

# maspsx emulates aspsx's behaviour (nop placement, div/rem expansion, the
# mflo/mfhi gap, small-.comm-to-.sbss, $gp small-data access) on top of modern
# binutils. ASPSX 2.56 is one of the versions it models explicitly.
#
# maspsx is a checkout, not a pip package: https://github.com/mkst/maspsx
MASPSX_DIR   ?= vendor/maspsx
MASPSX       ?= python3 $(MASPSX_DIR)/maspsx.py
#
# It reads the assembly on stdin, forwards any flags it does not recognise to
# GNU as, and with --run-assembler invokes as itself (reading from "-").
#   --force-stdin      fail loudly instead of silently falling back to a file
#   -G8                maspsx's own small-data limit; it emits explicit
#                      %gp_rel for symbols it knows are small
#   --use-comm-section put common symbols in .comm, the compiler's behaviour
#
# Do NOT add --dont-force-G0 here. maspsx forces -G0 on GNU as by default and
# that is what the bulk of the game wants, because at a nonzero -G gas emits
# GPREL16 for any symbol whose `.extern SYM,size` gives a size in (0, G] -- and
# cc1psx emits that `.extern` from the declared type, so every small extern
# would go $gp-relative. ASPSX instead expands lui/%hi + %lo for an import.
# Verified, back when tools/ccpsx existed, by assembling the same compiler
# output with the real ASPSX 2.56 via `ccpsx -c`: it produces the lui/%lo pair,
# and so does maspsx once gas is left at -G0. maspsx still emits %gp_rel where it belongs, so nothing is lost.
#
# (An earlier version of this comment said gas "assumes an undefined symbol is
# small data". It does not -- with no `.extern` record at all the access comes
# out absolute. The size record was the whole mechanism, and it is what made
# the per-unit -G8/-G2 escape hatches work before they were removed; see the
# note below.)
MASPSXFLAGS += --aspsx-version=$(ASPSX_VERSION) \
				--force-stdin \
                --run-assembler \
				--gnu-as-path=$(AS) \
                --use-comm-section

MASPSXFLAGS_G0 := $(MASPSXFLAGS) -G0
MASPSXFLAGS += -G8
# As with CFLAGS_DEFAULT above: the default, named, for the 0x80400000 twin.
MASPSXFLAGS_DEFAULT := $(MASPSXFLAGS)

# THERE IS NO LONGER A PER-FILE -G ESCAPE HATCH, and that is the point. Two
# variables used to live here, MASPSXFLAGS_G8 and MASPSXFLAGS_G2, which left
# gas at a nonzero -G for eleven carved `gprel_*` units so that their globals
# would come out $gp-relative. Both are gone as of 2026-11-03: a unit that
# reaches an object $gp-relatively is a unit that DEFINED that object, and a
# file-scope tentative definition says so directly -- cc1psx emits `.comm`,
# maspsx files it under `sbss_entries`, and `_uses_gp` rewrites that unit's
# accesses at the forced -G0 above. Nothing is allocated, because every one of
# these objects is already a GLOBAL dlabel in asm/data/gp_data.data.s and a
# COMMON merges with it. All eleven units converted with .text byte-identical
# and all four containers byte-identical.
#
# The one case a definition cannot cover is an ADDEND access (`SYM+N`), which
# maspsx's `_uses_gp` refuses for a symbol in `comm_symbols`. Those use a bare
# `.size` record inside `.sdata`, which populates `sdata_entries` instead;
# there are exactly two left in the tree, in csrc/unk_264E0.c and
# csrc/gprel_17B7C.c. See docs/addressing-and-translation-units.md.

# The OVERLAYS are built at cc1psx -G0, not the -G8 the resident executable
# uses, and that is a measurement rather than a guess. There are 0 $gp-relative
# refs across ~200K instructions of split overlay code, and there could not be
# any: main.bin's overlay keeps its data at 0x80027E20 and town's and
# dungeon's live at 0x80088760+, all far outside $gp's +/-32K window around
# 0x80080994, so psylink would have rejected a GPREL16 there.
#
# What -G actually changes, measured on func_80020924 (24 words): at -G8
# cc1psx records `.extern SYM,4` and a global access comes out as the
# assembler-expanded `lui $at` + `%lo` pair, giving 26 words; at -G0 cc1psx
# emits its own `%hi`/`%lo` split address into a general register -- the
# hoisted-lui shape -- giving 24. maspsx gets -G0 too, so it makes no
# small-data decisions of its own on a unit that has no small data.
#
# Worth knowing: main.bin's DEBUG TWIN at 0x80400000 is the SAME SOURCE built
# at -G8. One C source reproduces both blocks byte-for-byte (func_80020924 at
# -G0 and its twin func_804008A0 at -G8, 0 differing words each), which gives
# every function in this overlay two independent targets. See
# docs/overlays.md.

# EVERY overlay translation unit is -G0, so this is a PATTERN rule rather than
# one pair of lines per unit. It used to be per-unit, and carving ovl_town into
# three units on 2026-11-03 showed why that is a trap: the two new units
# silently inherited the default (resident) flags, the container came out 0xC0
# bytes long, and the first symptom was a landed 27-word function compiling to
# 29 words -- with cc1psx's own output IDENTICAL in both contexts, because the
# difference was entirely in CFLAGS/MASPSXFLAGS. carve.py printed "the new unit
# needs NO Makefile change", which is true for a resident unit and wrong for an
# overlay one; it now says so.
build/csrc/ovl_%.o: CFLAGS := $(CFLAGS_G0)
build/csrc/ovl_%.o: MASPSXFLAGS := $(MASPSXFLAGS_G0)

# THE ONE OVERLAY UNIT THAT IS -G8: main.bin's debug twin at 0x80400000. It is
# the same source as the live overlay built the other way, and -G is the whole
# difference in shape -- at -G8 cc1psx records `.extern SYM,4` and a global
# access comes out as the assembler-expanded `lui $at` + `%lo` pair, which is
# what this block uses, against the hoisted-lui split the live block uses at
# -G0. func_80020924/func_804008A0 is the pair that shows it: 24 words at -G0,
# 26 at -G8, and the 26 match this block instruction for instruction.
#
# An exact target name beats the `ovl_%` pattern above in GNU make, which is
# the only reason this works as two lines rather than a rewrite of the pattern.
build/csrc/ovl_main_80400000.o: CFLAGS := $(CFLAGS_DEFAULT)
build/csrc/ovl_main_80400000.o: MASPSXFLAGS := $(MASPSXFLAGS_DEFAULT)

# Assembling splat's output: keep -G0 here. splat emits %gp_rel relocations
# explicitly, so the assembler must not make its own small-data decisions.
# This is independent of the -G8 used for C above.
ASFLAGS  += -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
# For C, maspsx supplies the -G value, so leave -G off here.
ASFLAGS_C += -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1

SPLATFILES := $(shell find splat -type f -name '*.yaml')

BINFILES := $(shell find assets -type f -name '*.bin' 2>/dev/null)
# asm/nonmatchings/ holds per-function assembly that C files pull in via
# INCLUDE_ASM. Those files have no macro.inc of their own, so they must not be
# assembled standalone.
#
# asrc/ is the hand-owned half of the same picture: assembly WE maintain, as
# csrc/ is C we maintain. splat's `hrodata` subsegment type (splat/ext/
# hrodata.py) seeds a file there once and then never rewrites it, and `clean`
# does not delete the tree the way it deletes asm/. Same glob, same
# `build/%.o: %.s` rule below -- so `azure shiftjis` runs over these files too
# and `.shiftjis "..."` is available in them. The directory does not exist in a
# fresh checkout until the first such subsegment is split, which is what the
# pre-existing 2>/dev/null covers.
SFILES := $(shell find asm asrc -type f -name '*.s' \
            -not -path 'asm/nonmatchings/*' 2>/dev/null)
CFILES := $(shell find csrc -type f -name '*.c' 2>/dev/null)
OFILES := $(BINFILES:%.bin=build/%.o) $(SFILES:%.s=build/%.o) \
          $(CFILES:%.c=build/%.o)

# Coarse header dependency: the compiler is too old to emit .d files
# conveniently, so rebuild every C object when any header changes. Without
# this, editing include/common.h leaves stale objects behind and the build
# silently keeps using them.
HEADERS := $(wildcard include/*.h) $(wildcard include/*.inc)
$(CFILES:%.c=build/%.o): $(HEADERS)

# The compiler itself, built on demand from the vendor/old-gcc submodule. It
# lives under scratch/ rather than build/ because `clean` wipes build/ and a
# Docker rebuild is 20s of churn for something that only changes when the
# submodule pin moves.
CDK_DIR := scratch/cdk-gcc
CDK_CC1 := $(CDK_DIR)/cc1
$(CFILES:%.c=build/%.o): $(CDK_CC1)

$(CDK_CC1):
	$(MAKE) -C vendor/old-gcc VERSION=2.7.2-cdk
	@mkdir -p $(CDK_DIR)
	cp vendor/old-gcc/build-gcc-2.7.2-cdk/* $(CDK_DIR)/

cdk-gcc: $(CDK_CC1)

.PHONY: all containers clean splat check expected ctx ghidra-syms extract \
        cdk-gcc

all: build/$(SLUS) build/$(TOWN) build/$(MAIN)

# Every container, not just the two in `all`. main.bin and dungeon.bin are
# 26 MB and 16 MB, so relinking them on every build is not worth it -- but
# their code overlays ARE split now, so there has to be one command that
# proves all four still come out byte-identical.
containers: build/$(SLUS) build/$(TOWN) build/$(MAIN) build/$(DUNGEON)

# TODO: Probably want to use .d files generated from `splat` here
# splat generates the linker script into build/, so it is a real prerequisite
# of the link: `rm -rf build` otherwise leaves the project unlinkable until
# `make splat` is run again.
build/%.ld: splat/%.yaml
	splat split $<

build/$(SLUS): $(OFILES) build/$(SLUS).ld
	$(LD) $(LDFLAGS) \
		--script=build/$(SLUS).ld \
		--script=splat/$(SLUS)_undefined_funcs_auto.txt \
		--script=splat/$(SLUS)_undefined_syms_auto.txt \
		-Map=build/$(SLUS).map \
		--output=build/$(SLUS).elf
	$(OBJCOPY) -O binary build/$(SLUS).elf build/$(SLUS)
	@echo ""
	@$(CHECK) build/$(SLUS) data/$(SLUS) --yaml splat/$(SLUS).yaml

# town.bin takes one extra, HAND-MAINTAINED linker script. It defines
# func_800A652C, which is an interior address of func_800A64C0 rather than a
# function of its own and so has no defining `glabel` anywhere now that those
# 113 words are landed as one C function -- see the file's own header and
# splat/town.bin_symbol_addrs.txt.
build/$(TOWN): $(OFILES) splat/$(TOWN)_extra_syms.txt
	$(LD) $(LDFLAGS) \
		--script=build/$(TOWN).ld \
		--script=splat/$(TOWN)_undefined_funcs_auto.txt \
		--script=splat/$(TOWN)_undefined_syms_auto.txt \
		--script=splat/$(TOWN)_extra_syms.txt \
		--script=splat/common_syms.txt \
		--output=build/$(TOWN).elf
	$(OBJCOPY) -O binary build/$(TOWN).elf build/$(TOWN)
	@echo ""
	@$(CHECK) build/$(TOWN) data/town/$(TOWN) --yaml splat/$(SLUS).yaml

build/$(MAIN): $(OFILES)
	$(LD) $(LDFLAGS) \
		--script=build/$(MAIN).ld \
		--script=splat/$(MAIN)_undefined_funcs_auto.txt \
		--script=splat/$(MAIN)_undefined_syms_auto.txt \
		--script=splat/common_syms.txt \
		--output=build/$(MAIN).elf
	$(OBJCOPY) -O binary build/$(MAIN).elf build/$(MAIN)
	@echo ""
	@$(CHECK) build/$(MAIN) data/main/$(MAIN) --yaml splat/$(SLUS).yaml

build/$(DUNGEON): $(OFILES)
	$(LD) $(LDFLAGS) \
		--script=build/$(DUNGEON).ld \
		--script=splat/$(DUNGEON)_undefined_funcs_auto.txt \
		--script=splat/$(DUNGEON)_undefined_syms_auto.txt \
		--output=build/$(DUNGEON).elf
	$(OBJCOPY) -O binary build/$(DUNGEON).elf build/$(DUNGEON)
	@echo ""
	@$(CHECK) build/$(DUNGEON) data/dungeon/$(DUNGEON) --yaml splat/$(SLUS).yaml

build/%.o: %.s
	@mkdir -p $(shell dirname $@)
	$(AZURE) shiftjis $< | $(AS) $(ASFLAGS) -o $@

# C goes compiler -S -> maspsx -> GNU as, so that aspsx's quirks are
# reproduced while still using modern binutils for the actual assembly.
build/%.o: %.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -S -o $(@:.o=.s) $<
	$(MASPSX) $(MASPSXFLAGS) $(ASFLAGS_C) -o $@ < $(@:.o=.s)

build/%.o: %.bin
	@mkdir -p $(shell dirname $@)
	$(LD) $(LDFLAGS) --relocatable --format=binary -o $@ $<

# Report where the build diverges from the original, and which subsegment
# owns that offset. A whole-file checksum cannot tell you that.
check: build/$(SLUS)
	@$(CHECK) build/$(SLUS) data/$(SLUS) --yaml splat/$(SLUS).yaml

# Symbol table for tools/ghidra/ImportAzureSymbols.py.
ghidra-syms:
	python3 tools/ghidra_syms.py

# Regenerate the m2c/decomp-permuter type context from include/*.h.
ctx: ctx.c
ctx.c: $(HEADERS)
	python3 tools/m2ctx.py

# asm-differ's -o mode diffs objects against a known-good copy under
# expected/. Only ever populate it from a build that `make check` passes,
# since it becomes the reference everything is judged against.
expected: check
	rm -rf expected/build
	@mkdir -p expected
	cp -r build expected/build
	@echo "expected/ populated from a byte-identical build"

clean:
	rm -rf asm assets build

# asm/nonmatchings is cleared first: splat only adds files, so shrinking or
# re-carving a `c` subsegment otherwise leaves per-function asm behind in the
# old unit's directory, and the same function ends up owned by two units.
splat: $(SPLATFILES)
	rm -rf asm/nonmatchings
	@for SPLATFILE in $(SPLATFILES); do \
		echo "=== $$SPLATFILE === "; \
		splat split $$SPLATFILE; \
	done

extract:
	$(AZURE) spec spec.yaml
