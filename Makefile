include arch/i686/i686.mk

CDEPS := $(patsubst %.c,%.d,$(CSOURCES))
OBJECTS = $(ARCH_OBJECTS)

.PHONY: all
all: build/kernel.bin
	echo $(OBJECTS)
	echo $(ARCH_CDEPS)

.PHONY: clean
clean:
	rm -rf build
	mkdir -p build/arch

	touch build/.keep
	touch build/arch/.keep

build/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o build/kernel.bin $(OBJECTS) -lgcc

-include $(ARCH_CDEPS)
-include $(CDEPS)
