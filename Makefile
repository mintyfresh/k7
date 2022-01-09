OBJECTS = $(ARCH_OBJECTS)
include arch/i686/i686.mk

.PHONY: all
all: build/kernel.bin

.PHONY: clean
clean:
	rm -rf build
	mkdir -p build/arch

	touch build/.keep
	touch build/arch/.keep

build/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o build/kernel.bin $(OBJECTS) -lgcc
