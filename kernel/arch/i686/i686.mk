ARCH_DIR = kernel/arch/i686
ARCH_SOURCE_DIR = $(ARCH_DIR)/src
ARCH_INCLUDE_DIR = $(ARCH_DIR)/include
ARCH_BUILD_DIR = build/kernel/arch

ARCH_ASMSOURCES := $(wildcard $(ARCH_SOURCE_DIR)/*.asm)
ARCH_ASMOBJECTS := $(addprefix $(ARCH_BUILD_DIR)/, $(notdir $(patsubst %.asm, %.o, $(ARCH_ASMSOURCES))))

ARCH_TEMPLATES := $(wildcard $(ARCH_SOURCE_DIR)/*.c.sh)
ARCH_TEMPLATE_SOURCES := $(addprefix $(ARCH_BUILD_DIR)/, $(notdir $(patsubst %.c.sh, %.c, $(ARCH_TEMPLATES))))

ARCH_CSOURCES := $(wildcard $(ARCH_SOURCE_DIR)/*.c)
ARCH_COBJECTS := $(addprefix $(ARCH_BUILD_DIR)/, $(notdir $(patsubst %.c, %.o, $(ARCH_CSOURCES)))) \
			     $(addprefix $(ARCH_BUILD_DIR)/, $(notdir $(patsubst %.c, %.o, $(ARCH_TEMPLATE_SOURCES))))
ARCH_CDEPS := $(patsubst %.o, %.d, $(ARCH_COBJECTS))

ARCH_OBJECTS = $(ARCH_ASMOBJECTS) $(ARCH_COBJECTS)

ASFLAGS := $(ASFLAGS) -felf32
CFLAGS := $(CFLAGS) -I $(ARCH_INCLUDE_DIR) --target=i686-pc-none-elf -march=i686
LDFLAGS := $(LDFLAGS) -T $(ARCH_SOURCE_DIR)/linker.ld --target=i686-pc-none-elf -march=i686

$(ARCH_BUILD_DIR)/%.o: $(ARCH_SOURCE_DIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

$(ARCH_BUILD_DIR)/%.o: $(ARCH_SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$@) -c $< -o $@

$(ARCH_BUILD_DIR)/%.c: $(ARCH_SOURCE_DIR)/%.c.sh
	bash -c $< > $@

-include $(ARCH_CDEPS)
