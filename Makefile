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
# psyq/ is still needed for its HEADERS, which -Ipsyq/include below feeds to
# every unit. Its libraries are NOT needed: they were never a link input (the
# retail library code is already in the containers, carved as asm) and were
# only ever an analysis input for `azure libmatch`/`libscan`, whose results are
# committed -- see docs/twin-block-libcard.md. The .exes are kept as the
# provenance reference the native build was verified against, at the three
# hashes above. tools/validate-psyq.py checks the headers and nothing else.
# See docs/compiler-version.md.
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
# What -G actually changes, measured on set_card_slot (24 words): at -G8
# cc1psx records `.extern SYM,4` and a global access comes out as the
# assembler-expanded `lui $at` + `%lo` pair, giving 26 words; at -G0 cc1psx
# emits its own `%hi`/`%lo` split address into a general register -- the
# hoisted-lui shape -- giving 24. maspsx gets -G0 too, so it makes no
# small-data decisions of its own on a unit that has no small data.
#
# Worth knowing: main.bin's DEBUG TWIN at 0x80400000 is the SAME SOURCE built
# at -G8. One C source reproduces both blocks byte-for-byte (set_card_slot at
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

# csrc/main/ holds the 0x80020000 overlay's own translation units as they are
# carved out of csrc/ovl_main_80020000.c, and they do NOT match the `ovl_%`
# pattern above, so they need their own. This is the trap the comment above
# describes, one directory over: without these two lines csrc/main/c_server.c
# would silently build at the resident -G8 and the container would come out
# the wrong length.
# `-Icsrc` is for `#include "main.h"`: a quoted include resolves against the
# INCLUDING file's directory first, which is csrc/main/ here, not csrc/.
build/csrc/main/%.o: CFLAGS := $(CFLAGS_G0) -Icsrc
build/csrc/main/%.o: MASPSXFLAGS := $(MASPSXFLAGS_G0)

# THE ONE OVERLAY UNIT THAT IS -G8: main.bin's debug twin at 0x80400000. It is
# the same source as the live overlay built the other way, and -G is the whole
# difference in shape -- at -G8 cc1psx records `.extern SYM,4` and a global
# access comes out as the assembler-expanded `lui $at` + `%lo` pair, which is
# what this block uses, against the hoisted-lui split the live block uses at
# -G0. set_card_slot/func_804008A0 is the pair that shows it: 24 words at -G0,
# 26 at -G8, and the 26 match this block instruction for instruction.
#
# An exact target name beats the `ovl_%` pattern above in GNU make, which is
# the only reason this works as two lines rather than a rewrite of the pattern.
build/csrc/ovl_main_80400000.o: CFLAGS := $(CFLAGS_DEFAULT)
build/csrc/ovl_main_80400000.o: MASPSXFLAGS := $(MASPSXFLAGS_DEFAULT)

# AND IT IS ALSO THE ONE UNIT BUILT BY A DIFFERENT COMPILER. The -G8 above is
# real -- at -G0 this block loses four of its matched functions -- but it was
# never the whole story about the `lui $at` shape. That shape needs a compiler
# WITHOUT split addresses, and 4.1's has them unconditionally
# (`override_options` sets mips_split_addresses from TARGET_GAS && optimize,
# and -mno-gas leaves codegen byte-identical).
#
# PSY-Q 4.0's cc1psx is `2.7.2.SN32.3.7 Build 0001`; 4.1's is
# `cygnus-2.7.2-970404 SN32.3.7.0004`, which is cdk-gcc-b18's version.c
# exactly. So 4.0 is SN's build of FSF 2.7.2 and 4.1 is SN's build of a
# Cygnus snapshot, and every pass that separates the two blocks --
# split addresses and reload_cse_regs -- is a Cygnus/SN addition that FSF
# 2.7.2 does not have. SN never released Build 0001's source, so FSF 2.7.2
# is the closest buildable thing and tools/psx272gcc drives it.
#
# MEASURED 2026-11-11 before switching: 105 of the 106 functions landed under
# the 4.1 compiler stay byte-identical under 2.7.2, the 106th matches once a
# frame-padding DEVICE is deleted, and four such devices come out of the
# source altogether (see csrc/ovl_main_80400000.c's header).
build/csrc/ovl_main_80400000.o: CC := tools/psx272gcc

# AND IT IS THE ONE UNIT WHOSE ASSEMBLER EXPANDS DIVISION. The whole binary
# holds exactly four `div` instructions: two in the live block at 0x80023280
# and 0x800232D4, bare `div`/`mflo`, and two in func_804034D8 at 0x80403500
# and 0x80403578, each followed by the full aspsx check sequence --
# `bnez`/`break 0x7` for the zero divisor and `bne`/`lui 0x8000`/`bne`/
# `break 0x6` for 0x80000000 / -1. maspsx models that as --expand-div, and
# it is off by default because the rest of the game wants it off. The two
# blocks were therefore assembled by different aspsx settings, which is the
# same story as the compiler above: this one came out of the 4.0 SDK.
build/csrc/ovl_main_80400000.o: MASPSXFLAGS += --expand-div

# AND IT IS THE ONE UNIT WHOSE JUMP TABLE IS NOT 8-ALIGNED. gcc gives every
# jump table `.align 3` (final.c:1518, BIGGEST_ALIGNMENT 64), and for the 4.1
# units that is correct -- csrc/main/c_server.c's table is at rodata offset
# 0x60 behind four bytes of pad at 0x5C, and that pad is in the shipped image.
# This block's table is at 0x8040007C, 4 mod 8, behind 0x7C of c_server.c's
# own rodata, which an 8-aligning toolchain cannot produce. Same story as the
# compiler and --expand-div above: the 4.0 SDK did it differently. The filter
# rewrites that ONE directive and leaves every other `.align` alone; without
# it, 0x269000 needs a third C unit purely to put the table at offset 0 of a
# section. See tools/decomp/jtbl_align.py and docs/owning-data.md SS9.3.
ASFILTER ?= true
build/csrc/ovl_main_80400000.o: ASFILTER := python3 tools/decomp/jtbl_align.py

# Assembling splat's output: keep -G0 here. splat emits %gp_rel relocations
# explicitly, so the assembler must not make its own small-data decisions.
# This is independent of the -G8 used for C above.
ASFLAGS  += -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
# For C, maspsx supplies the -G value, so leave -G off here.
ASFLAGS_C += -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1

SPLATFILES := $(shell find splat -type f -name '*.yaml')

BINFILES := $(shell find assets -type f -name '*.bin' 2>/dev/null)

STRFILES := $(shell find asrc/str -type f -name '*.txt')
STRSFILES := $(STRFILES:%.txt=%.s)

# asm/nonmatchings/ holds per-function assembly that C files pull in via
# INCLUDE_ASM. Those files have no macro.inc of their own, so they must not be
# assembled standalone.
#
# asrc/ is the hand-owned half of the same picture: assembly WE maintain, as
# csrc/ is C we maintain. splat's `hrodata` subsegment type (splat/ext/
# hrodata.py) seeds a file there once and then never rewrites it, and `clean`
# does not delete the tree the way it deletes asm/. Same glob, same
# `build/%.o: %.s` rule below. The directory does not exist in a
# fresh checkout until the first such subsegment is split, which is what the
# pre-existing 2>/dev/null covers.
SFILES := $(shell find asm asrc -type f -name '*.s' \
            -not -path 'asm/nonmatchings/*' 2>/dev/null) $(STRSFILES)
CFILES := $(shell find csrc -type f -name '*.c' 2>/dev/null)
OFILES := $(BINFILES:%.bin=build/%.o) $(SFILES:%.s=build/%.o) \
          $(CFILES:%.c=build/%.o)

# Coarse header dependency: the compiler is too old to emit .d files
# conveniently, so rebuild every C object when any header changes. Without
# this, editing include/common.h leaves stale objects behind and the build
# silently keeps using them.
HEADERS := $(wildcard include/*.h) $(wildcard include/*.inc) $(wildcard csrc/*.h)
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

# The SECOND compiler, FSF gcc 2.7.2 for mips-sony-psx, which stands in for
# PSY-Q 4.0's cc1psx for the 0x80400000 twin only -- see the CC override
# above and tools/psx272gcc's header. Same submodule, same Dockerfile
# machinery, a different VERSION.
PSX272_DIR := scratch/psx272-gcc
PSX272_CC1 := $(PSX272_DIR)/cc1
build/csrc/ovl_main_80400000.o: $(PSX272_CC1)

$(PSX272_CC1):
	$(MAKE) -C vendor/old-gcc VERSION=2.7.2-psx
	@mkdir -p $(PSX272_DIR)
	cp vendor/old-gcc/build-gcc-2.7.2-psx/* $(PSX272_DIR)/

psx272-gcc: $(PSX272_CC1)

.PHONY: all containers clean splat check expected ctx ghidra-syms extract \
        cdk-gcc psx272-gcc

all: build/$(SLUS) build/$(TOWN) build/$(MAIN)

# Every container, not just the two in `all`. main.bin and dungeon.bin are
# 26 MB and 16 MB, so relinking them on every build is not worth it -- but
# their code overlays ARE split now, so there has to be one command that
# proves all four still come out byte-identical.
containers: build/$(SLUS) build/$(TOWN) build/$(MAIN) build/$(DUNGEON)

# TODO: Probably want to use .d files generated from `splat` here
# splat generates the linker script into build/, so it is a real prerequisite
# of every link below -- all four of them. `rm -rf build` otherwise leaves the
# project unlinkable until `make splat` is run again.
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
build/$(TOWN): $(OFILES) build/$(TOWN).ld splat/$(TOWN)_extra_syms.txt
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

build/$(MAIN): $(OFILES) build/$(MAIN).ld
	$(LD) $(LDFLAGS) \
		--script=build/$(MAIN).ld \
		--script=splat/$(MAIN)_undefined_funcs_auto.txt \
		--script=splat/$(MAIN)_undefined_syms_auto.txt \
		--script=splat/common_syms.txt \
		--output=build/$(MAIN).elf
	$(OBJCOPY) -O binary build/$(MAIN).elf build/$(MAIN)
	@echo ""
	@$(CHECK) build/$(MAIN) data/main/$(MAIN) --yaml splat/$(SLUS).yaml

build/$(DUNGEON): $(OFILES) build/$(DUNGEON).ld
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
	$(AS) $(ASFLAGS) -o $@ $<

# C goes compiler -S -> maspsx -> GNU as, so that aspsx's quirks are
# reproduced while still using modern binutils for the actual assembly.
build/%.o: %.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -S -o $(@:.o=.s) $<
	$(ASFILTER) $(@:.o=.s)
	$(MASPSX) $(MASPSXFLAGS) $(ASFLAGS_C) -o $@ < $(@:.o=.s)

build/%.o: %.bin
	@mkdir -p $(shell dirname $@)
	$(LD) $(LDFLAGS) --relocatable --format=binary -o $@ $<

asrc/str/%.s: asrc/str/%.txt
	python tools/shiftjis_to_asm.py $< -o $@

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

# THE WHOLE asm/ TREE is cleared first, not just asm/nonmatchings. splat only
# ADDS files, so every subsegment that is renamed, retyped or deleted leaves
# its old output behind -- and an orphan in asm/ is worse than one in
# asm/nonmatchings, because SFILES globs asm/ (excluding nonmatchings) and the
# orphan is assembled into an object with a full set of duplicate symbol
# definitions. It is only unlinked for as long as no linker script happens to
# name it.
#
# This was not hypothetical. Converting main.bin's 0x80088760 overlay from
# `asm` to `c` on 2026-11-11 orphaned asm/ovl_main_80088760.s, 2,870 lines
# defining all 26 of that overlay's functions; it survived every `make splat`
# that day and was still being assembled hours later.
#
# Clearing asm/ entirely is safe: git tracks nothing in it, `clean` already
# deletes it outright, and splat regenerates all of it. Hand-owned assembly
# lives in asrc/, which is why `hrodata` redirects there -- see
# splat/ext/hrodata.py and the asrc/str/%.s rule below.
# build/asm goes with it, because it mirrors asm/ one-to-one and an orphaned
# OBJECT carries the same duplicate definitions as the source it came from.
# Note this is `build/asm`, never `build`: splat writes the linker scripts
# into build/, so removing the whole directory leaves the project unlinkable
# until the next `make splat` -- see the build/%.ld rule.
splat: $(SPLATFILES) $(STRSFILES)
	rm -rf asm build/asm
	@for SPLATFILE in $(SPLATFILES); do \
		echo "=== $$SPLATFILE === "; \
		splat split $$SPLATFILE; \
	done

extract:
	$(AZURE) spec spec.yaml
