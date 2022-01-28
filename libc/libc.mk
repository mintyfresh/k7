LIBC_DIR = libc
LIBC_BUILD_DIR = build/libc
LIBC_INCLUDE = $(LIBC_DIR)/include

LIBC_SOURCES := $(wildcard $(LIBC_DIR)/**/*.c)
LIBC_OBJECTS := $(addprefix $(LIBC_BUILD_DIR)/, $(notdir $(patsubst %.c,%.o,$(LIBC_SOURCES))))
LIBC_CDEPS   := $(patsubst %.o,%.d,$(LIBC_OBJECTS))

CFLAGS := $(CFLAGS) -I $(LIBC_INCLUDE)

OBJECTS := $(OBJECTS) $(LIBC_OBJECTS)

$(LIBC_BUILD_DIR)/%.o: $(LIBC_DIR)/*/%.c
	$(CC) $(CFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$@) -c $< -o $@

-include $(LIBC_CDEPS)
