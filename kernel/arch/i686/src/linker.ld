ENTRY(_kstart_low)

PHYSICAL_START = 1M;
VIRTUAL_OFFSET = 0xC0000000;

SECTIONS
{
	. = PHYSICAL_START + VIRTUAL_OFFSET;

	_kernel_start = .;

	.text ALIGN(4) : AT(ADDR(.text) - VIRTUAL_OFFSET)
	{
		*(.multiboot)
		*(.text_low)
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - VIRTUAL_OFFSET)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - VIRTUAL_OFFSET)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss) - VIRTUAL_OFFSET)
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;
	_kernel_mapped_end = . + 4M;
}
