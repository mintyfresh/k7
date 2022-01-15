.DEFAULT_GOAL := all

AS := nasm
ASFLAGS :=

CC := clang
CFLAGS := -fno-builtin -ffreestanding -nostdlib -nostdinc++

LD := clang
LDFLAGS := -ffreestanding -nostdlib -nostdinc++

include kernel/kernel.mk
include libc/libc.mk

.PHONY: all
all: build/kernel.bin

.PHONY: qemu
qemu: build/kernel.bin
	qemu-system-i386 -kernel build/kernel.bin

.PHONY: clean
clean:
	rm -rf build
	mkdir -p build/kernel/arch build/libc

	touch build/.keep
	touch build/kernel/.keep
	touch build/kernel/arch/.keep
	touch build/libc/.keep

build/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o build/kernel.bin $(OBJECTS)
