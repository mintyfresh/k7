; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; Constants for paging kernel into higher half.
PAGE_PRESENT   equ 1 << 0
PAGE_READWRITE equ 1 << 1
PAGE_LARGE     equ 1 << 7
VIRTUAL_OFFSET equ 0xC0000000

; Multiboot header structure.
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; Initial kernel stack.
section .bss
align 16
stack_bottom:
	resb 16384 ; 16 KiB
stack_top:

; Kernel page directory.
section .data
align 4096
boot_page_directory:
	; Identity map the first 4 MiB of memory.
	dd 0 | (PAGE_PRESENT | PAGE_READWRITE | PAGE_LARGE)
	; Padding up to 3GiB mark.
	times 768 - 1 dd 0
	; Map memory at 0xC0000000 to physical memory at 0x00000000.
	dd 0 | (PAGE_PRESENT | PAGE_READWRITE | PAGE_LARGE)
	; Padding up to last directory entry.
	times 1023 - 769 dd 0
	; Map page directory to itself as final page table.
	dd (boot_page_directory - VIRTUAL_OFFSET) + (PAGE_PRESENT | PAGE_READWRITE)

section .text_low
align 4
global _kstart_low:function
_kstart_low:
	cli

	; Install kernel page directory.
	mov ecx, (boot_page_directory - VIRTUAL_OFFSET)
	mov cr3, ecx

	; Enable large-page support.
	mov ecx, cr4
	or ecx, 0x10
	mov cr4, ecx

	; Enable paging.
	mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx

	lea ecx, [_kstart]
	jmp ecx

section .text
align 4
global _kstart:function
_kstart:
	; Load kernel stack.
	mov esp, stack_top

	; Higher-half pointer to memory map.
	add ebx, VIRTUAL_OFFSET
	push ebx
	push eax

	extern kernel_main
	call kernel_main

.khang:
	; Hang if kernel exits.
	cli
	hlt
	jmp .khang

.kernel_end:
