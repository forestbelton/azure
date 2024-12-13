SLUS = slus_006.14
TOWN = town.bin

AS 		= mipsel-linux-gnu-as
LD 		= mipsel-linux-gnu-ld
OBJCOPY = mipsel-linux-gnu-objcopy

ASFLAGS += -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0

SPLATFILES := $(shell find splat -type f -name '*.yaml')

BINFILES := $(shell find assets -type f -name '*.bin')
SFILES := $(shell find asm -type f -name '*.s')
OFILES := $(BINFILES:%.bin=build/%.o) $(SFILES:%.s=build/%.o)

.PHONY: all clean splat

all: build/$(SLUS) build/$(TOWN)

# TODO: Probably want to use .d files generated from `splat` here
build/$(SLUS): $(OFILES)
	$(LD) \
		--script=build/$(SLUS).ld \
		--script=splat/$(SLUS)_undefined_funcs_auto.txt \
		--script=splat/$(SLUS)_undefined_syms_auto.txt \
		--output=build/$(SLUS).elf
	$(OBJCOPY) -O binary build/$(SLUS).elf build/$(SLUS)
	@echo ""	
	ls -la build/$(SLUS) data/$(SLUS)
	@echo ""
	shasum build/$(SLUS) data/$(SLUS)

build/$(TOWN): $(OFILES)
	$(LD) \
		--script=build/$(TOWN).ld \
		--script=splat/$(TOWN)_undefined_funcs_auto.txt \
		--script=splat/$(TOWN)_undefined_syms_auto.txt \
		--output=build/$(TOWN).elf
	$(OBJCOPY) -O binary build/$(TOWN).elf build/$(TOWN)
	@echo ""	
	ls -la build/$(TOWN) data/town/$(TOWN)
	@echo ""
	shasum build/$(TOWN) data/town/$(TOWN)

build/%.o: %.s
	@mkdir -p $(shell dirname $@)
	python tools/convert_shiftjis.py $< | $(AS) $(ASFLAGS) -o $@

build/%.o: %.bin
	@mkdir -p $(shell dirname $@)
	$(LD) --relocatable --format=binary -o $@ $<

clean:
	rm -rf asm assets build

splat: $(SPLATFILES)
	@for SPLATFILE in $(SPLATFILES); do \
		echo "=== $$SPLATFILE === "; \
		splat split $$SPLATFILE; \
	done
