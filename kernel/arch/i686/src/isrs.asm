%macro isr 2
global isr%1
isr%1:
    cli
    push byte 0
    push byte %1
    jmp %2
%endmacro

%macro isr_with_error 2
global isr%1
isr%1:
    cli
    push byte %1
    jmp %2
%endmacro

%macro generate_isr_machine_state 2
%1:
    pushad
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax

    mov eax, %2
    call eax

    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8

    iretd
%endmacro

isr 0, isr_common
isr 1, isr_common
isr 2, isr_common
isr 3, isr_common
isr 4, isr_common
isr 5, isr_common
isr 6, isr_common
isr 7, isr_common
isr_with_error 8, isr_common
isr 9, isr_common
isr_with_error 10, isr_common
isr_with_error 11, isr_common
isr_with_error 12, isr_common
isr_with_error 13, isr_common
isr_with_error 14, isr_common
isr 15, isr_common
isr 16, isr_common
isr_with_error 17, isr_common
isr 18, isr_common
isr 19, isr_common
isr 20, isr_common
isr_with_error 21, isr_common
isr 22, isr_common
isr 23, isr_common
isr 24, isr_common
isr 25, isr_common
isr 26, isr_common
isr 27, isr_common
isr 28, isr_common
isr_with_error 29, isr_common
isr_with_error 30, isr_common
isr 31, isr_common

isr 32, irq_common
isr 33, irq_common
isr 34, irq_common
isr 35, irq_common
isr 36, irq_common
isr 37, irq_common
isr 38, irq_common
isr 39, irq_common
isr 40, irq_common
isr 41, irq_common
isr 42, irq_common
isr 43, irq_common
isr 44, irq_common
isr 45, irq_common
isr 46, irq_common
isr 47, irq_common

extern isr_handler
extern irq_handler

generate_isr_machine_state isr_common, isr_handler
generate_isr_machine_state irq_common, irq_handler
