LIBC_DIR = libc
LIBC_BUILD_DIR = build/libc
LIBC_INCLUDE = $(LIBC_DIR)/include

LIBC_SOURCES := $(wildcard $(LIBC_DIR)/**/*.c)
LIBC_OBJECTS := $(addprefix $(LIBC_BUILD_DIR)/, $(notdir $(patsubst %.c,%.o,$(LIBC_SOURCES))))

CFLAGS := $(CFLAGS) -I $(LIBC_INCLUDE)

$(LIBC_BUILD_DIR)/%.o: $(LIBC_DIR)/*/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
