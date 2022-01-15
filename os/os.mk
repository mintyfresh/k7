OS_DIR = os
OS_BUILD_DIR = build/os
OS_INCLUDE = $(OS_DIR)/include

OS_SOURCES := $(wildcard $(OS_DIR)/*.c)
OS_OBJECTS := $(addprefix $(OS_BUILD_DIR)/, $(notdir $(patsubst %.c,%.o,$(OS_SOURCES))))

$(OS_BUILD_DIR)/%.o: $(OS_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
