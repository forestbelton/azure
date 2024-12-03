TARGET = slus_006.14

AS 		= mipsel-linux-gnu-as
LD 		= mipsel-linux-gnu-ld
OBJCOPY = mipsel-linux-gnu-objcopy

ASFLAGS += -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0

SPLATFILES := $(shell find splat -type f -name '*.yaml')

BINFILES := $(shell find assets -type f -name '*.bin')
SFILES := $(shell find asm -type f -name '*.s')
OFILES := $(BINFILES:%.bin=build/%.o) $(SFILES:%.s=build/%.o)

.PHONY: clean splat

# TODO: Probably want to use .d files generated from `splat` here
build/$(TARGET): $(OFILES)
	$(LD) --script=build/$(TARGET).ld -o build/$(TARGET).elf
	$(OBJCOPY) -O binary build/$(TARGET).elf build/$(TARGET)
	@echo ""	
	ls -la build/$(TARGET) data/$(TARGET)
	@echo ""
	shasum build/$(TARGET) data/$(TARGET)

build/%.o: %.s
	@mkdir -p $(shell dirname $@)
	$(AS) $(ASFLAGS) -o $@ $<

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
