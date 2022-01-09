.DEFAULT_GOAL := all

include arch/i686/i686.mk

CFLAGS := -I include $(CFLAGS)

OBJECTS = $(ARCH_OBJECTS)

.PHONY: all
all: build/kernel.bin

.PHONY: qemu
qemu: build/kernel.bin
	qemu-system-i386 -kernel build/kernel.bin

.PHONY: clean
clean:
	rm -rf build
	mkdir -p build/arch

	touch build/.keep
	touch build/arch/.keep

build/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o build/kernel.bin $(OBJECTS) -lgcc
