ARCH_DIR = arch/i686
ARCH_BUILD_DIR = build/arch
ARCH_INCLUDE = $(ARCH_DIR)/include

ASM = nasm
ASMFLAGS = -felf32

CC = clang
CFLAGS = -I $(ARCH_INCLUDE) --target=i686-pc-none-elf -march=i686 -fno-builtin -ffreestanding -nostdlib -nostdinc++

LD = clang
LDFLAGS = -T $(ARCH_DIR)/linker.ld --target=i686-pc-none-elf -march=i686 -ffreestanding -nostdlib

ARCH_ASMSOURCES := $(wildcard $(ARCH_DIR)/*.asm)
ARCH_ASMOBJECTS := $(patsubst $(ARCH_DIR)/%.asm,$(ARCH_BUILD_DIR)/%.o,$(ARCH_ASMSOURCES))

ARCH_CSOURCES := $(wildcard $(ARCH_DIR)/*.c)
ARCH_COBJECTS := $(patsubst $(ARCH_DIR)/%.c,$(ARCH_BUILD_DIR)/%.o,$(ARCH_CSOURCES))
ARCH_CDEPS := $(patsubst $(ARCH_BUILD_DIR)/%.o,$(ARCH_BUILD_DIR)/%.d,$(ARCH_COBJECTS))

ARCH_OBJECTS = $(ARCH_ASMOBJECTS) $(ARCH_COBJECTS)

$(ARCH_BUILD_DIR)/%.o: $(ARCH_DIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(ARCH_BUILD_DIR)/%.o: $(ARCH_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(ARCH_CDEPS)
