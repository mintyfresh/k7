ARCH_DIR = arch/i686
ARCH_BUILD_DIR = build/arch
ARCH_INCLUDE = $(ARCH_DIR)/include

ARCH_ASMSOURCES := $(wildcard $(ARCH_DIR)/*.asm)
ARCH_ASMOBJECTS := $(patsubst $(ARCH_DIR)/%.asm,$(ARCH_BUILD_DIR)/%.o,$(ARCH_ASMSOURCES))

ARCH_TEMPLATES := $(wildcard $(ARCH_DIR)/*.c.sh)
ARCH_TEMPLATE_SOURCES := $(patsubst $(ARCH_DIR)/%.c.sh,$(ARCH_BUILD_DIR)/%.c,$(ARCH_TEMPLATES))

ARCH_CSOURCES := $(wildcard $(ARCH_DIR)/*.c)
ARCH_COBJECTS := $(patsubst $(ARCH_DIR)/%.c,$(ARCH_BUILD_DIR)/%.o,$(ARCH_CSOURCES)) \
			     $(patsubst $(ARCH_BUILD_DIR)/%.c,$(ARCH_BUILD_DIR)/%.o,$(ARCH_TEMPLATE_SOURCES))
ARCH_CDEPS := $(patsubst $(ARCH_BUILD_DIR)/%.o,$(ARCH_BUILD_DIR)/%.d,$(ARCH_COBJECTS))

ARCH_OBJECTS = $(ARCH_ASMOBJECTS) $(ARCH_COBJECTS)

ASFLAGS := $(ASFLAGS) -felf32
CFLAGS := $(CFLAGS) -I $(ARCH_INCLUDE) --target=i686-pc-none-elf -march=i686
LDFLAGS := $(LDFLAGS) -T $(ARCH_DIR)/linker.ld --target=i686-pc-none-elf -march=i686

$(ARCH_BUILD_DIR)/%.o: $(ARCH_DIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

$(ARCH_BUILD_DIR)/%.o: $(ARCH_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(ARCH_BUILD_DIR)/%.c: $(ARCH_DIR)/%.c.sh
	bash -c $< > $@

-include $(ARCH_CDEPS)
