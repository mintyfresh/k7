KERNEL_DIR = kernel
KERNEL_BUILD_DIR = build/kernel
KERNEL_INCLUDE = $(KERNEL_DIR)/include

KERNEL_SOURCES := $(wildcard $(KERNEL_DIR)/*.c)
KERNEL_OBJECTS := $(addprefix $(KERNEL_BUILD_DIR)/, $(notdir $(patsubst %.c,%.o,$(KERNEL_SOURCES))))

include kernel/arch/i686/i686.mk

CFLAGS := $(CFLAGS) -I $(KERNEL_INCLUDE)

OBJECTS := $(OBJECTS) $(ARCH_OBJECTS) $(KERNEL_OBJECTS)

$(KERNEL_BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
