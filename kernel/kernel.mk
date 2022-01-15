KERNEL_DIR = kernel
KERNEL_SOURCE_DIR = $(KERNEL_DIR)/src
KERNEL_INCLUDE_DIR = $(KERNEL_DIR)/include
KERNEL_BUILD_DIR = build/kernel

KERNEL_SOURCES := $(wildcard $(KERNEL_SOURCE_DIR)/*.c)
KERNEL_OBJECTS := $(addprefix $(KERNEL_BUILD_DIR)/, $(notdir $(patsubst %.c,%.o,$(KERNEL_SOURCES))))

include kernel/arch/i686/i686.mk

CFLAGS := $(CFLAGS) -I $(KERNEL_INCLUDE_DIR)

OBJECTS := $(OBJECTS) $(ARCH_OBJECTS) $(KERNEL_OBJECTS)

$(KERNEL_BUILD_DIR)/%.o: $(KERNEL_SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
