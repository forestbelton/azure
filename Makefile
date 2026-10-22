# Fail on the first error in a pipeline. Without this, `azure shiftjis ... | as`
# reports only the assembler's status, so a crash in the first stage silently
# yields an object assembled from empty input.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

SLUS = slus_006.14
TOWN = town.bin
MAIN = main.bin

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

# Run the project's own tool with src/ on the path. `python src/azure/main.py`
# only works inside `hatch shell`: invoked directly it puts src/azure on
# sys.path rather than src, so `from azure.commands import ...` fails.
AZURE = PYTHONPATH=src python3 src/azure/main.py
CHECK = python tools/check.py

# The game was built with PSY-Q 4.0: cc1psx reports "GNU C 2.7.2.SN32.3.7"
# (SN Systems' patched GCC -- vanilla GNU 2.7.2 will NOT match) and aspsx
# reports version 2.56. tools/ccpsx drives the shipped compiler under wine.
CC             = tools/ccpsx
ASPSX_VERSION  = 2.56
CFLAGS        += -O2 -G8 -Iinclude
# -G8 is ccpsx's default and the game uses small-data addressing: there are 245
# genuine $gp-relative loads/stores in the game's own code, against
# $gp = 0x80080994 (set by crt0 at 0x800339ac, not by the PS-EXE header, whose
# gp0 field is 0). Getting -G wrong mismatches every small-data access.

# maspsx emulates aspsx's behaviour (nop placement, div/rem expansion, the
# mflo/mfhi gap, small-.comm-to-.sbss, $gp small-data access) on top of modern
# binutils. ASPSX 2.56 is one of the versions it models explicitly.
#
# maspsx is a checkout, not a pip package: https://github.com/mkst/maspsx
MASPSX_DIR   ?= $(HOME)/scratch/maspsx
MASPSX       ?= python3 $(MASPSX_DIR)/maspsx.py
#
# It reads the assembly on stdin, forwards any flags it does not recognise to
# GNU as, and with --run-assembler invokes as itself (reading from "-").
#   --force-stdin      fail loudly instead of silently falling back to a file
#   -G8                maspsx's own small-data limit; it emits explicit
#                      %gp_rel for symbols it knows are small
#   --use-comm-section put common symbols in .comm, which is ccpsx's behaviour
#
# Do NOT add --dont-force-G0. maspsx forces -G0 on GNU as by default, and that
# is required: given a nonzero -G, gas assumes an *undefined* symbol is small
# data and emits GPREL16, whereas ASPSX assumes externs are not small and
# expands lui/%hi + %lo. Verified by assembling the same ccpsx output with the
# real ASPSX 2.56 via `ccpsx -c`: it produces the lui/%lo pair, and so does
# maspsx once gas is left at -G0. maspsx still emits %gp_rel where it belongs,
# so nothing is lost.
MASPSXFLAGS  += --aspsx-version=$(ASPSX_VERSION) --force-stdin \
                --run-assembler --gnu-as-path=$(AS) \
                --use-comm-section --dont-force-G0 -G8

# Per-file escape hatch. A translation unit whose functions reach their globals
# absolutely rather than $gp-relatively needs gas left at -G0; see
# docs/addressing-and-translation-units.md. Use as:
#     build/csrc/some_unit.o: MASPSXFLAGS := $(MASPSXFLAGS_G0)
MASPSXFLAGS_G0 := --aspsx-version=$(ASPSX_VERSION) --force-stdin \
                  --run-assembler --gnu-as-path=$(AS) --use-comm-section -G8

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
SFILES := $(shell find asm -type f -name '*.s' \
            -not -path 'asm/nonmatchings/*' 2>/dev/null)
CFILES := $(shell find csrc -type f -name '*.c' 2>/dev/null)
OFILES := $(BINFILES:%.bin=build/%.o) $(SFILES:%.s=build/%.o) \
          $(CFILES:%.c=build/%.o)

# Coarse header dependency: ccpsx is too old to emit .d files conveniently, so
# rebuild every C object when any header changes. Without this, editing
# include/common.h leaves stale objects behind and the build silently keeps
# using them.
HEADERS := $(wildcard include/*.h) $(wildcard include/*.inc)
$(CFILES:%.c=build/%.o): $(HEADERS)

.PHONY: all clean splat check expected ctx ghidra-syms

all: build/$(SLUS) build/$(TOWN)

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

build/$(TOWN): $(OFILES)
	$(LD) $(LDFLAGS) \
		--script=build/$(TOWN).ld \
		--script=splat/$(TOWN)_undefined_funcs_auto.txt \
		--script=splat/$(TOWN)_undefined_syms_auto.txt \
		--output=build/$(TOWN).elf
	$(OBJCOPY) -O binary build/$(TOWN).elf build/$(TOWN)
	@echo ""
	@$(CHECK) build/$(TOWN) data/town/$(TOWN) --yaml splat/$(SLUS).yaml

build/$(MAIN): $(OFILES)
	$(LD) $(LDFLAGS) \
		--script=build/$(MAIN).ld \
		--script=splat/$(MAIN)_undefined_funcs_auto.txt \
		--script=splat/$(MAIN)_undefined_syms_auto.txt \
		--output=build/$(MAIN).elf
	$(OBJCOPY) -O binary build/$(MAIN).elf build/$(MAIN)
	@echo ""
	@$(CHECK) build/$(MAIN) data/main/$(MAIN) --yaml splat/$(SLUS).yaml

build/%.o: %.s
	@mkdir -p $(shell dirname $@)
	$(AZURE) shiftjis $< | $(AS) $(ASFLAGS) -o $@

# C goes ccpsx -S -> maspsx -> GNU as, so that aspsx's quirks are reproduced
# while still using modern binutils for the actual assembly.
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

splat: $(SPLATFILES)
	@for SPLATFILE in $(SPLATFILES); do \
		echo "=== $$SPLATFILE === "; \
		splat split $$SPLATFILE; \
	done
