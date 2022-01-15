.DEFAULT_GOAL := all

AS := nasm
ASFLAGS :=

CC := clang
CFLAGS := -fno-builtin -ffreestanding -nostdlib -nostdinc++

LD := clang
LDFLAGS := -ffreestanding -nostdlib -nostdinc++

include arch/i686/i686.mk
include libc/libc.mk

OBJECTS = $(ARCH_OBJECTS) $(LIBC_OBJECTS)

.PHONY: all
all: build/kernel.bin

.PHONY: qemu
qemu: build/kernel.bin
	qemu-system-i386 -kernel build/kernel.bin

.PHONY: clean
clean:
	rm -rf build
	mkdir -p build/arch build/libc

	touch build/.keep
	touch build/arch/.keep
	touch build/libc/.keep

build/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o build/kernel.bin $(OBJECTS)
